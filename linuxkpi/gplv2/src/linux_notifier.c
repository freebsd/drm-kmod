#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>

#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/oom.h>
#include <linux/shrinker.h>
#include <linux/rcupdate.h>
#include <linux/math64.h>
#include <linux/kernel.h>

#ifdef CONFIG_ACPI
#include <linux/acpi.h>
#include <acpi/button.h>
#endif

#ifdef DIAGNOSTIC
#define DIPRINTF printf
#else
#define DIPRINTF(...)
#endif

static struct sx shrinker_list_sx;
SX_SYSINIT(shrinker_list_lock, &shrinker_list_sx, "shrinker list lock");

static LIST_HEAD(shrinker_list);

static int
notifier_chain_register(struct notifier_block **nl,
			struct notifier_block *n)
{
	while ((*nl) != NULL) {
		if (n->priority > (*nl)->priority)
			break;
		nl = &((*nl)->next);
	}
	n->next = *nl;
	rcu_assign_pointer(*nl, n);
	return (0);
}

static int
notifier_chain_unregister(struct notifier_block **nl,
			  struct notifier_block *n)
{
	while ((*nl) != NULL) {
		if ((*nl) == n) {
			rcu_assign_pointer(*nl, n->next);
			return (0);
		}
		nl = &((*nl)->next);
	}
	return (-ENOENT);
}


static int
notifier_call_chain(struct notifier_block **nl,
		    unsigned long val, void *v,
		    int nr_to_call, int *nr_calls)
{
	int ret = NOTIFY_DONE;
	struct notifier_block *nb, *next_nb;

	nb = rcu_dereference(*nl);

	while (nb && nr_to_call) {
		next_nb = rcu_dereference(nb->next);

		ret = nb->notifier_call(nb, val, v);

		if (nr_calls)
			(*nr_calls)++;

		if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK)
			break;
		nb = next_nb;
		nr_to_call--;
	}
	return (ret);
}

int
blocking_notifier_chain_register(struct blocking_notifier_head *nh,
		struct notifier_block *n)
{
	int rc;

	down_write(&nh->rwsem);
	rc = notifier_chain_register(&nh->head, n);
	up_write(&nh->rwsem);
	return (rc);
}

int
blocking_notifier_chain_unregister(struct blocking_notifier_head *nh,
		struct notifier_block *n)
{
	int rc;

	down_write(&nh->rwsem);
	rc = notifier_chain_unregister(&nh->head, n);
	up_write(&nh->rwsem);
	return (rc);
}

static int
__blocking_notifier_call_chain(struct blocking_notifier_head *nh,
				   unsigned long val, void *v,
				   int nr_to_call, int *nr_calls)
{
	int rc = NOTIFY_DONE;

	if (rcu_access_pointer(nh->head)) {
		down_read(&nh->rwsem);
		rc = notifier_call_chain(&nh->head, val, v, nr_to_call,
					nr_calls);
		up_read(&nh->rwsem);
	}
	return (rc);
}

int
blocking_notifier_call_chain(struct blocking_notifier_head *nh,
		unsigned long val, void *v)
{
	return (__blocking_notifier_call_chain(nh, val, v, -1, NULL));
}

int
__atomic_notifier_call_chain(struct atomic_notifier_head *nh,
				 unsigned long val, void *v,
				 int nr_to_call, int *nr_calls)
{
	int ret;

	rcu_read_lock();
	ret = notifier_call_chain(&nh->head, val, v, nr_to_call, nr_calls);
	rcu_read_unlock();
	return ret;
}

int
atomic_notifier_call_chain(struct atomic_notifier_head *nh,
			       unsigned long val, void *v)
{
	return __atomic_notifier_call_chain(nh, val, v, -1, NULL);
}

int
register_oom_notifier(struct notifier_block *nb)
{

	WARN_NOT();
	return (0);
}

int
unregister_oom_notifier(struct notifier_block *nb)
{

	WARN_NOT();
	return (0);

}

#if __FreeBSD_version < 1300135
int
register_shrinker(struct shrinker *s)
{
	size_t size = sizeof(*s->nr_deferred);

	s->nr_deferred = kzalloc(size, GFP_KERNEL);
	if (s->nr_deferred == NULL)
		return (-ENOMEM);

	sx_xlock(&shrinker_list_sx);
	list_add_tail(&s->list, &shrinker_list);
	sx_xunlock(&shrinker_list_sx);
	return (0);
}

void
unregister_shrinker(struct shrinker *s)
{
	sx_xlock(&shrinker_list_sx);
	list_del(&s->list);
	sx_xunlock(&shrinker_list_sx);
}

#define SHRINK_BATCH 128

static unsigned long
do_shrink_slab(struct shrink_control *shrinkctl, struct shrinker *shrinker,
		unsigned long nr_scanned, unsigned long nr_eligible)
{
	unsigned long delta, freed = 0;

	long nr, new_nr __unused;
	long total_scan, freeable;
	int nid = shrinkctl->nid;
	long batch_size = shrinker->batch ? shrinker->batch : SHRINK_BATCH;

	DIPRINTF("shrinker invoked\n");
	freeable = shrinker->count_objects(shrinker, shrinkctl);
	if (freeable == 0) {
		DIPRINTF("nothing freeable\n");
		return (0);
	}
	nr = atomic_long_xchg(&shrinker->nr_deferred[nid], 0);

	total_scan = nr;
	delta = (4 * nr_scanned) / shrinker->seeks;
	delta *= freeable;
	do_div(delta, nr_eligible + 1);
	total_scan += delta;
	if (total_scan < 0)
		total_scan = freeable;
	if (delta < freeable / 4)
		total_scan = min(total_scan, freeable / 2);

	total_scan = min(total_scan, freeable * 2);

	while (total_scan >= batch_size ||
	       total_scan >= freeable) {
		unsigned long rc, nr_to_scan = min(batch_size, total_scan);

		shrinkctl->nr_to_scan = nr_to_scan;
		rc = shrinker->scan_objects(shrinker, shrinkctl);
		if (rc == SHRINK_STOP)
			break;
		total_scan -= nr_to_scan;
	}
	if (total_scan > 0)
		new_nr = atomic_fetchadd_long(&shrinker->nr_deferred[nid].counter, total_scan);
	else
		new_nr = READ_ONCE(shrinker->nr_deferred[nid].counter);

	return (freed);
}

/*
 * see do_try_to_free_pages -> shrink_zones -> shrink_node -> shrink_slab ->
 *     do_shrink_slab(shrinker) [shrinker handler]
 *
 */
static unsigned long
shrink_slab(gfp_t gfp_mask, int nid,
	    unsigned long nr_scanned,
	    unsigned long nr_eligible)
{
	struct shrinker *shrinker;
	unsigned long freed = 0;

	sx_slock(&shrinker_list_sx);
	list_for_each_entry(shrinker, &shrinker_list, list) {
		struct shrink_control sc = {
                        .gfp_mask = gfp_mask,
			.nid = nid,
	        };
		if (!(shrinker->flags & SHRINKER_NUMA_AWARE))
			sc.nid = 0;

		freed += do_shrink_slab(&sc, shrinker, nr_scanned, nr_eligible);
	}
	sx_sunlock(&shrinker_list_sx);

	return (freed);
}

static void
linuxkpi_vm_lowmem(void *arg __unused)
{
	int nr_scanned, nr_eligible;

	DIPRINTF("linuxkpi_vm_lowmem invoked\n");
	nr_scanned = nr_eligible = 1000;
	shrink_slab(GFP_KERNEL, 0, nr_scanned, nr_eligible);
}

static eventhandler_tag lowmem_tag;

static void
linuxkpi_register_eventhandlers(void *arg __unused)
{
	DIPRINTF("registering linuxkpi event handlers\n");

	lowmem_tag = EVENTHANDLER_REGISTER(vm_lowmem, linuxkpi_vm_lowmem, NULL, EVENTHANDLER_PRI_FIRST);
}

static void
linuxkpi_deregister_eventhandlers(void *arg __unused)
{

	EVENTHANDLER_DEREGISTER(vm_lowmem, lowmem_tag);
}

SYSINIT(linuxkpi_events, SI_SUB_DRIVERS, SI_ORDER_ANY, linuxkpi_register_eventhandlers, NULL);
SYSUNINIT(linuxkpi_events, SI_SUB_DRIVERS, SI_ORDER_ANY, linuxkpi_deregister_eventhandlers, NULL);
#endif

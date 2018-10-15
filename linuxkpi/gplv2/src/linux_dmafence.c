#include <linux/dma-fence.h>
#include <linux/dma-fence-array.h>
#include <asm/atomic64.h>

static atomic64_t dma_fence_context_counter = ATOMIC64_INIT(0);


/*
 * dma-fence.h
 */

u64
dma_fence_context_alloc(unsigned num)
{
	return (atomic64_add_return(num, &dma_fence_context_counter) - num);
}

int
dma_fence_get_status(struct dma_fence *fence)
{
	unsigned long flags;
	int status;

	spin_lock_irqsave(fence->lock, flags);
	status = dma_fence_get_status_locked(fence);
	spin_unlock_irqrestore(fence->lock, flags);

	return (status);
}


/*
 * dma-fence-array.h
 */

static const char *
dma_fence_array_get_driver_name(struct dma_fence *fence)
{
	return ("dma_fence_array");
}

static const char *
dma_fence_array_get_timeline_name(struct dma_fence *fence)
{
	return ("unbound");
}

#ifdef __linux__
static void
irq_dma_fence_array_work(struct irq_work *wrk)
{
	struct dma_fence_array *array = container_of(wrk, typeof(*array), work);

	dma_fence_signal(&array->base);
	dma_fence_put(&array->base);
}
#endif

static void
dma_fence_array_cb_func(struct dma_fence *f, struct dma_fence_cb *cb)
{
	struct dma_fence_array_cb *array_cb =
		container_of(cb, struct dma_fence_array_cb, cb);
	struct dma_fence_array *array = array_cb->array;

	if (atomic_dec_and_test(&array->num_pending)) {
#ifdef __linux__
		irq_work_queue(&array->work);
#else
		dma_fence_signal(&array->base);
		dma_fence_put(&array->base);
#endif
	} else
		dma_fence_put(&array->base);
}

static bool
dma_fence_array_enable_signaling(struct dma_fence *fence)
{
	struct dma_fence_array *array = to_dma_fence_array(fence);
	struct dma_fence_array_cb *cb = (void *)(&array[1]);
	unsigned i;

	for (i = 0; i < array->num_fences; ++i) {
		cb[i].array = array;
		/*
		 * As we may report that the fence is signaled before all
		 * callbacks are complete, we need to take an additional
		 * reference count on the array so that we do not free it too
		 * early. The core fence handling will only hold the reference
		 * until we signal the array as complete (but that is now
		 * insufficient).
		 */
		dma_fence_get(&array->base);
		if (dma_fence_add_callback(array->fences[i], &cb[i].cb,
					   dma_fence_array_cb_func)) {
			dma_fence_put(&array->base);
			if (atomic_dec_and_test(&array->num_pending))
				return false;
		}
	}

	return true;
}

static bool
dma_fence_array_signaled(struct dma_fence *fence)
{
	struct dma_fence_array *array = to_dma_fence_array(fence);

	return atomic_read(&array->num_pending) <= 0;
}

static void
dma_fence_array_release(struct dma_fence *fence)
{
	struct dma_fence_array *array = to_dma_fence_array(fence);
	unsigned i;

	for (i = 0; i < array->num_fences; ++i)
		dma_fence_put(array->fences[i]);

	kfree(array->fences);
	dma_fence_free(fence);
}

const struct dma_fence_ops dma_fence_array_ops = {
	.get_driver_name = dma_fence_array_get_driver_name,
	.get_timeline_name = dma_fence_array_get_timeline_name,
	.enable_signaling = dma_fence_array_enable_signaling,
	.signaled = dma_fence_array_signaled,
	.wait = dma_fence_default_wait,
	.release = dma_fence_array_release,
};

struct dma_fence_array *dma_fence_array_create(int num_fences, struct dma_fence **fences,
				       u64 context, unsigned seqno,
				       bool signal_on_any)
{
	struct dma_fence_array *array;
	size_t size = sizeof(*array);

	/* Allocate the callback structures behind the array. */
	size += num_fences * sizeof(struct dma_fence_array_cb);
	array = kzalloc(size, GFP_KERNEL);
	if (!array)
		return NULL;

	spin_lock_init(&array->lock);
	dma_fence_init(&array->base, &dma_fence_array_ops, &array->lock,
		       context, seqno);
#ifdef __linux__
	init_irq_work(&array->work, irq_dma_fence_array_work);
#endif
	array->num_fences = num_fences;
	atomic_set(&array->num_pending, signal_on_any ? 1 : num_fences);
	array->fences = fences;

	return array;
}

bool dma_fence_match_context(struct dma_fence *fence, u64 context)
{
	struct dma_fence_array *array = to_dma_fence_array(fence);
	unsigned i;

	if (!dma_fence_is_array(fence))
		return fence->context == context;

	for (i = 0; i < array->num_fences; i++) {
		if (array->fences[i]->context != context)
			return false;
	}

	return true;
}

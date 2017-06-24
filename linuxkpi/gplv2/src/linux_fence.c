#include <linux/fence.h>
#include <linux/fence-array.h>
#include <asm/atomic64.h>

static atomic64_t fence_context_counter = ATOMIC64_INIT(0);

static const char *
fence_array_get_driver_name(struct fence *fence)
{
	return ("fence_array");
}

static const char *
fence_array_get_timeline_name(struct fence *fence)
{
	return ("unbound");
}

static void
fence_array_cb_func(struct fence *f, struct fence_cb *cb)
{
	struct fence_array_cb *array_cb =
		container_of(cb, struct fence_array_cb, cb);
	struct fence_array *array = array_cb->array;

	if (atomic_dec_and_test(&array->num_pending))
		fence_signal(&array->base);
	fence_put(&array->base);
}

static bool
fence_array_enable_signaling(struct fence *fence)
{
	struct fence_array *array = to_fence_array(fence);
	struct fence_array_cb *cb = (void *)(&array[1]);
	unsigned i;

	for (i = 0; i < array->num_fences; ++i) {
		cb[i].array = array;
		fence_get(&array->base);
		if (fence_add_callback(array->fences[i], &cb[i].cb,
				       fence_array_cb_func)) {
			fence_put(&array->base);
			if (atomic_dec_and_test(&array->num_pending))
				return (false);
		}
	}

	return (true);
}

static bool
fence_array_signaled(struct fence *fence)
{
	struct fence_array *array = to_fence_array(fence);

	return atomic_read(&array->num_pending) <= 0;
}

static void
fence_array_release(struct fence *fence)
{
	struct fence_array *array = to_fence_array(fence);
	unsigned i;

	for (i = 0; i < array->num_fences; ++i)
		fence_put(array->fences[i]);

	kfree(array->fences);
	fence_free(fence);
}

const struct fence_ops fence_array_ops = {
	.get_driver_name = fence_array_get_driver_name,
	.get_timeline_name = fence_array_get_timeline_name,
	.enable_signaling = fence_array_enable_signaling,
	.signaled = fence_array_signaled,
	.wait = fence_default_wait,
	.release = fence_array_release,
};

u64
fence_context_alloc(unsigned num)
{
	return (atomic64_add_return(num, &fence_context_counter) - num);
}

struct fence_array *fence_array_create(int num_fences, struct fence **fences,
				       u64 context, unsigned seqno,
				       bool signal_on_any)
{
	struct fence_array *array;
	size_t size = sizeof(*array);

	size += num_fences * sizeof(struct fence_array_cb);
	array = kzalloc(size, GFP_KERNEL);
	if (!array)
		return NULL;

	spin_lock_init(&array->lock);
	fence_init(&array->base, &fence_array_ops, &array->lock,
		   context, seqno);

	array->num_fences = num_fences;
	atomic_set(&array->num_pending, signal_on_any ? 1 : num_fences);
	array->fences = fences;

	return (array);
}

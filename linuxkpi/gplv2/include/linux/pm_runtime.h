#ifndef _LINUX_PM_RUNTIME_H_
#define _LINUX_PM_RUNTIME_H_

#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/timer.h>
#include <linux/completion.h>
#include <linux/device.h>

static inline void
pm_runtime_mark_last_busy(struct device *dev)
{
	UNIMPLEMENTED_ONCE();
}


/* Runtime PM flag argument bits */
#define RPM_ASYNC		0x01	/* Request is asynchronous */
#define RPM_NOWAIT		0x02	/* Don't wait for concurrent
					    state change */
#define RPM_GET_PUT		0x04	/* Increment/decrement the
					    usage_count */
#define RPM_AUTO		0x08	/* Use autosuspend_delay */


static inline int
__pm_runtime_suspend(struct device *dev, int rpmflags)
{
	UNIMPLEMENTED_ONCE();
	return (0);
}

static inline int
pm_runtime_get_if_in_use(struct device *dev)
{
	UNIMPLEMENTED();
	return (1);
}

static inline void
__pm_runtime_use_autosuspend(struct device *dev, bool use)
{
	UNIMPLEMENTED();
}

static inline void
pm_runtime_set_autosuspend_delay(struct device *dev, int delay)
{
	UNIMPLEMENTED();
}

static inline int
__pm_runtime_resume(struct device *dev, int rpmflags)
{
	UNIMPLEMENTED_ONCE();
	return (0);
}

static inline int
__pm_runtime_idle(struct device *dev, int rpmflags)
{
	UNIMPLEMENTED();
	return (0);
}

static inline void
pm_runtime_get_noresume(struct device *dev)
{
#ifdef __linux__
	atomic_inc(&dev->power.usage_count);
#endif
}

static inline int pm_runtime_resume(struct device *dev)
{
	return __pm_runtime_resume(dev, 0);
}

static inline int
pm_runtime_get(struct device *dev)
{
	return (__pm_runtime_resume(dev, RPM_GET_PUT | RPM_ASYNC));
}

static inline int
pm_runtime_get_sync(struct device *dev)
{
	return (__pm_runtime_resume(dev, RPM_GET_PUT));
}

static inline int
pm_runtime_put(struct device *dev)
{
	return (__pm_runtime_idle(dev, RPM_GET_PUT | RPM_ASYNC));
}

static inline void pm_runtime_put_noidle(struct device *dev)
{
#ifdef __linux__
	atomic_add_unless(&dev->power.usage_count, -1, 0);
#endif
}

static inline int
pm_runtime_put_autosuspend(struct device *dev)
{
	return (__pm_runtime_suspend(dev, RPM_GET_PUT | RPM_ASYNC | RPM_AUTO));
}

static inline void
pm_runtime_use_autosuspend(struct device *dev)
{
	__pm_runtime_use_autosuspend(dev, true);
}

static inline void
pm_runtime_dont_use_autosuspend(struct device *dev)
{
	__pm_runtime_use_autosuspend(dev, false);
}

static inline int
pm_generic_runtime_suspend(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->runtime_suspend ? pm->runtime_suspend(dev) : 0);
}

static inline int
pm_generic_runtime_resume(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->runtime_resume ? pm->runtime_resume(dev) : 0);
}

static inline int
pm_generic_suspend(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->suspend ? pm->suspend(dev) : 0);
}

static inline int
pm_generic_resume(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return pm && pm->resume ? pm->resume(dev) : 0;
}

static inline int
pm_generic_restore(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->restore ? pm->restore(dev) : 0);
}

static inline int
pm_generic_freeze(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->freeze ? pm->freeze(dev) : 0);
}

static inline int
pm_generic_thaw(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->thaw ? pm->thaw(dev) : 0);
}
static inline int
pm_generic_poweroff(struct device *dev)
{
	const struct dev_pm_ops *pm = dev->driver ? dev->driver->pm : NULL;

	return (pm && pm->poweroff ? pm->poweroff(dev) : 0);
}

static inline void
pm_runtime_forbid(struct device *dev){
	UNIMPLEMENTED();
}

static inline unsigned long
pm_runtime_autosuspend(struct device *dev){
	UNIMPLEMENTED();
	return(0);
}

static inline int
pm_runtime_set_active(struct device *dev){
	UNIMPLEMENTED();
	return (0);
}

static inline void
pm_runtime_allow(struct device *dev){
	UNIMPLEMENTED();
}

static inline void pm_runtime_enable(struct device *dev) { UNIMPLEMENTED();}

static inline void __pm_runtime_disable(struct device *dev, bool c) { 	UNIMPLEMENTED(); }
static inline void pm_runtime_disable(struct device *dev)
{
	__pm_runtime_disable(dev, true);
}
#endif /*  _LINUX_PM_RUNTIME_H_ */

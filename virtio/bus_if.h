/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 *
 * Created from source file
 *   /usr/src/sys/kern/bus_if.m
 * with
 *   makeobjops.awk
 *
 * See the source file for legal information
 */

/**
 * @defgroup BUS bus - KObj methods for drivers of devices with children
 * @brief A set of methods required device drivers that support
 * child devices.
 * @{
 */

#ifndef _bus_if_h_
#define _bus_if_h_

/** @brief Unique descriptor for the BUS_PRINT_CHILD() method */
extern struct kobjop_desc bus_print_child_desc;
/** @brief A function implementing the BUS_PRINT_CHILD() method */
typedef int bus_print_child_t(device_t _dev, device_t _child);
/**
 * @brief Print a description of a child device
 *
 * This is called from system code which prints out a description of a
 * device. It should describe the attachment that the child has with
 * the parent. For instance the TurboLaser bus prints which node the
 * device is attached to. See bus_generic_print_child() for more
 * information.
 *
 * @param _dev		the device whose child is being printed
 * @param _child	the child device to describe
 *
 * @returns		the number of characters output.
 */

static __inline int BUS_PRINT_CHILD(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_print_child);
	rc = ((bus_print_child_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_PROBE_NOMATCH() method */
extern struct kobjop_desc bus_probe_nomatch_desc;
/** @brief A function implementing the BUS_PROBE_NOMATCH() method */
typedef void bus_probe_nomatch_t(device_t _dev, device_t _child);
/**
 * @brief Print a notification about an unprobed child device.
 *
 * Called for each child device that did not succeed in probing for a
 * driver.
 *
 * @param _dev		the device whose child was being probed
 * @param _child	the child device which failed to probe
 */

static __inline void BUS_PROBE_NOMATCH(device_t _dev, device_t _child)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_probe_nomatch);
	((bus_probe_nomatch_t *) _m)(_dev, _child);
}

/** @brief Unique descriptor for the BUS_READ_IVAR() method */
extern struct kobjop_desc bus_read_ivar_desc;
/** @brief A function implementing the BUS_READ_IVAR() method */
typedef int bus_read_ivar_t(device_t _dev, device_t _child, int _index,
                            uintptr_t *_result);
/**
 * @brief Read the value of a bus-specific attribute of a device
 *
 * This method, along with BUS_WRITE_IVAR() manages a bus-specific set
 * of instance variables of a child device.  The intention is that
 * each different type of bus defines a set of appropriate instance
 * variables (such as ports and irqs for ISA bus etc.)
 *
 * This information could be given to the child device as a struct but
 * that makes it hard for a bus to add or remove variables without
 * forcing an edit and recompile for all drivers which may not be
 * possible for vendor supplied binary drivers.
 *
 * This method copies the value of an instance variable to the
 * location specified by @p *_result.
 *
 * @param _dev		the device whose child was being examined
 * @param _child	the child device whose instance variable is
 *			being read
 * @param _index	the instance variable to read
 * @param _result	a location to receive the instance variable
 *			value
 *
 * @retval 0		success
 * @retval ENOENT	no such instance variable is supported by @p
 *			_dev
 */

static __inline int BUS_READ_IVAR(device_t _dev, device_t _child, int _index,
                                  uintptr_t *_result)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_read_ivar);
	rc = ((bus_read_ivar_t *) _m)(_dev, _child, _index, _result);
	return (rc);
}

/** @brief Unique descriptor for the BUS_WRITE_IVAR() method */
extern struct kobjop_desc bus_write_ivar_desc;
/** @brief A function implementing the BUS_WRITE_IVAR() method */
typedef int bus_write_ivar_t(device_t _dev, device_t _child, int _indx,
                             uintptr_t _value);
/**
 * @brief Write the value of a bus-specific attribute of a device
 *
 * This method sets the value of an instance variable to @p _value.
 *
 * @param _dev		the device whose child was being updated
 * @param _child	the child device whose instance variable is
 *			being written
 * @param _index	the instance variable to write
 * @param _value	the value to write to that instance variable
 *
 * @retval 0		success
 * @retval ENOENT	no such instance variable is supported by @p
 *			_dev
 * @retval EINVAL	the instance variable was recognised but
 *			contains a read-only value
 */

static __inline int BUS_WRITE_IVAR(device_t _dev, device_t _child, int _indx,
                                   uintptr_t _value)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_write_ivar);
	rc = ((bus_write_ivar_t *) _m)(_dev, _child, _indx, _value);
	return (rc);
}

/** @brief Unique descriptor for the BUS_CHILD_DELETED() method */
extern struct kobjop_desc bus_child_deleted_desc;
/** @brief A function implementing the BUS_CHILD_DELETED() method */
typedef void bus_child_deleted_t(device_t _dev, device_t _child);
/**
 * @brief Notify a bus that a child was deleted
 *
 * Called at the beginning of device_delete_child() to allow the parent
 * to teardown any bus-specific state for the child.
 *
 * @param _dev		the device whose child is being deleted
 * @param _child	the child device which is being deleted
 */

static __inline void BUS_CHILD_DELETED(device_t _dev, device_t _child)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_child_deleted);
	((bus_child_deleted_t *) _m)(_dev, _child);
}

/** @brief Unique descriptor for the BUS_CHILD_DETACHED() method */
extern struct kobjop_desc bus_child_detached_desc;
/** @brief A function implementing the BUS_CHILD_DETACHED() method */
typedef void bus_child_detached_t(device_t _dev, device_t _child);
/**
 * @brief Notify a bus that a child was detached
 *
 * Called after the child's DEVICE_DETACH() method to allow the parent
 * to reclaim any resources allocated on behalf of the child.
 *
 * @param _dev		the device whose child changed state
 * @param _child	the child device which changed state
 */

static __inline void BUS_CHILD_DETACHED(device_t _dev, device_t _child)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_child_detached);
	((bus_child_detached_t *) _m)(_dev, _child);
}

/** @brief Unique descriptor for the BUS_DRIVER_ADDED() method */
extern struct kobjop_desc bus_driver_added_desc;
/** @brief A function implementing the BUS_DRIVER_ADDED() method */
typedef void bus_driver_added_t(device_t _dev, driver_t *_driver);
/**
 * @brief Notify a bus that a new driver was added
 *
 * Called when a new driver is added to the devclass which owns this
 * bus. The generic implementation of this method attempts to probe and
 * attach any un-matched children of the bus.
 *
 * @param _dev		the device whose devclass had a new driver
 *			added to it
 * @param _driver	the new driver which was added
 */

static __inline void BUS_DRIVER_ADDED(device_t _dev, driver_t *_driver)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_driver_added);
	((bus_driver_added_t *) _m)(_dev, _driver);
}

/** @brief Unique descriptor for the BUS_ADD_CHILD() method */
extern struct kobjop_desc bus_add_child_desc;
/** @brief A function implementing the BUS_ADD_CHILD() method */
typedef device_t bus_add_child_t(device_t _dev, u_int _order, const char *_name,
                                 int _unit);
/**
 * @brief Create a new child device
 *
 * For buses which use use drivers supporting DEVICE_IDENTIFY() to
 * enumerate their devices, this method is used to create new
 * device instances. The new device will be added after the last
 * existing child with the same order. Implementations of bus_add_child
 * call device_add_child_ordered to add the child and often add
 * a suitable ivar to the device specific to that bus.
 *
 * @param _dev		the bus device which will be the parent of the
 *			new child device
 * @param _order	a value which is used to partially sort the
 *			children of @p _dev - devices created using
 *			lower values of @p _order appear first in @p
 *			_dev's list of children
 * @param _name		devclass name for new device or @c NULL if not
 *			specified
 * @param _unit		unit number for new device or @c -1 if not
 *			specified
 */

static __inline device_t BUS_ADD_CHILD(device_t _dev, u_int _order,
                                       const char *_name, int _unit)
{
	kobjop_t _m;
	device_t rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_add_child);
	rc = ((bus_add_child_t *) _m)(_dev, _order, _name, _unit);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESCAN() method */
extern struct kobjop_desc bus_rescan_desc;
/** @brief A function implementing the BUS_RESCAN() method */
typedef int bus_rescan_t(device_t _dev);
/**
 * @brief Rescan the bus
 *
 * This method is called by a parent bridge or devctl to trigger a bus
 * rescan.  The rescan should delete devices no longer present and
 * enumerate devices that have newly arrived.
 *
 * @param _dev		the bus device
 */

static __inline int BUS_RESCAN(device_t _dev)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_rescan);
	rc = ((bus_rescan_t *) _m)(_dev);
	return (rc);
}

/** @brief Unique descriptor for the BUS_ALLOC_RESOURCE() method */
extern struct kobjop_desc bus_alloc_resource_desc;
/** @brief A function implementing the BUS_ALLOC_RESOURCE() method */
typedef struct resource * bus_alloc_resource_t(device_t _dev, device_t _child,
                                               int _type, int *_rid,
                                               rman_res_t _start,
                                               rman_res_t _end,
                                               rman_res_t _count, u_int _flags);
/**
 * @brief Allocate a system resource
 *
 * This method is called by child devices of a bus to allocate resources.
 * The types are defined in <machine/resource.h>; the meaning of the
 * resource-ID field varies from bus to bus (but @p *rid == 0 is always
 * valid if the resource type is). If a resource was allocated and the
 * caller did not use the RF_ACTIVE to specify that it should be
 * activated immediately, the caller is responsible for calling
 * BUS_ACTIVATE_RESOURCE() when it actually uses the resource.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which is requesting an allocation
 * @param _type		the type of resource to allocate
 * @param _rid		a pointer to the resource identifier
 * @param _start	hint at the start of the resource range - pass
 *			@c 0 for any start address
 * @param _end		hint at the end of the resource range - pass
 *			@c ~0 for any end address
 * @param _count	hint at the size of range required - pass @c 1
 *			for any size
 * @param _flags	any extra flags to control the resource
 *			allocation - see @c RF_XXX flags in
 *			<sys/rman.h> for details
 *
 * @returns		the resource which was allocated or @c NULL if no
 *			resource could be allocated
 */

static __inline struct resource * BUS_ALLOC_RESOURCE(device_t _dev,
                                                     device_t _child, int _type,
                                                     int *_rid,
                                                     rman_res_t _start,
                                                     rman_res_t _end,
                                                     rman_res_t _count,
                                                     u_int _flags)
{
	kobjop_t _m;
	struct resource * rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_alloc_resource);
	rc = ((bus_alloc_resource_t *) _m)(_dev, _child, _type, _rid, _start, _end, _count, _flags);
	return (rc);
}

/** @brief Unique descriptor for the BUS_ACTIVATE_RESOURCE() method */
extern struct kobjop_desc bus_activate_resource_desc;
/** @brief A function implementing the BUS_ACTIVATE_RESOURCE() method */
typedef int bus_activate_resource_t(device_t _dev, device_t _child,
                                    struct resource *_r);
/**
 * @brief Activate a resource
 *
 * Activate a resource previously allocated with
 * BUS_ALLOC_RESOURCE().  This may enable decoding of this resource in a
 * device for instance.  It will also establish a mapping for the resource
 * unless RF_UNMAPPED was set when allocating the resource.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _r		the resource to activate
 */

static __inline int BUS_ACTIVATE_RESOURCE(device_t _dev, device_t _child,
                                          struct resource *_r)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_activate_resource);
	rc = ((bus_activate_resource_t *) _m)(_dev, _child, _r);
	return (rc);
}

/** @brief Unique descriptor for the BUS_MAP_RESOURCE() method */
extern struct kobjop_desc bus_map_resource_desc;
/** @brief A function implementing the BUS_MAP_RESOURCE() method */
typedef int bus_map_resource_t(device_t _dev, device_t _child,
                               struct resource *_r,
                               struct resource_map_request *_args,
                               struct resource_map *_map);
/**
 * @brief Map a resource
 *
 * Allocate a mapping for a range of an active resource.  The mapping
 * is described by a struct resource_map object.  This may for instance
 * map a memory region into the kernel's virtual address space.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _r		the resource to map
 * @param _args		optional attributes of the mapping
 * @param _map		the mapping
 */

static __inline int BUS_MAP_RESOURCE(device_t _dev, device_t _child,
                                     struct resource *_r,
                                     struct resource_map_request *_args,
                                     struct resource_map *_map)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_map_resource);
	rc = ((bus_map_resource_t *) _m)(_dev, _child, _r, _args, _map);
	return (rc);
}

/** @brief Unique descriptor for the BUS_UNMAP_RESOURCE() method */
extern struct kobjop_desc bus_unmap_resource_desc;
/** @brief A function implementing the BUS_UNMAP_RESOURCE() method */
typedef int bus_unmap_resource_t(device_t _dev, device_t _child,
                                 struct resource *_r,
                                 struct resource_map *_map);
/**
 * @brief Unmap a resource
 *
 * Release a mapping previously allocated with
 * BUS_MAP_RESOURCE(). This may for instance unmap a memory region
 * from the kernel's virtual address space.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _r		the resource
 * @param _map		the mapping to release
 */

static __inline int BUS_UNMAP_RESOURCE(device_t _dev, device_t _child,
                                       struct resource *_r,
                                       struct resource_map *_map)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_unmap_resource);
	rc = ((bus_unmap_resource_t *) _m)(_dev, _child, _r, _map);
	return (rc);
}

/** @brief Unique descriptor for the BUS_DEACTIVATE_RESOURCE() method */
extern struct kobjop_desc bus_deactivate_resource_desc;
/** @brief A function implementing the BUS_DEACTIVATE_RESOURCE() method */
typedef int bus_deactivate_resource_t(device_t _dev, device_t _child,
                                      struct resource *_r);
/**
 * @brief Deactivate a resource
 *
 * Deactivate a resource previously allocated with
 * BUS_ALLOC_RESOURCE().
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _r		the resource to deactivate
 */

static __inline int BUS_DEACTIVATE_RESOURCE(device_t _dev, device_t _child,
                                            struct resource *_r)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_deactivate_resource);
	rc = ((bus_deactivate_resource_t *) _m)(_dev, _child, _r);
	return (rc);
}

/** @brief Unique descriptor for the BUS_ADJUST_RESOURCE() method */
extern struct kobjop_desc bus_adjust_resource_desc;
/** @brief A function implementing the BUS_ADJUST_RESOURCE() method */
typedef int bus_adjust_resource_t(device_t _dev, device_t _child,
                                  struct resource *_res, rman_res_t _start,
                                  rman_res_t _end);
/**
 * @brief Adjust a resource
 *
 * Adjust the start and/or end of a resource allocated by
 * BUS_ALLOC_RESOURCE.  At least part of the new address range must overlap
 * with the existing address range.  If the successful, the resource's range
 * will be adjusted to [start, end] on return.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _res		the resource to adjust
 * @param _start	the new starting address of the resource range
 * @param _end		the new ending address of the resource range
 */

static __inline int BUS_ADJUST_RESOURCE(device_t _dev, device_t _child,
                                        struct resource *_res,
                                        rman_res_t _start, rman_res_t _end)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_adjust_resource);
	rc = ((bus_adjust_resource_t *) _m)(_dev, _child, _res, _start, _end);
	return (rc);
}

/** @brief Unique descriptor for the BUS_TRANSLATE_RESOURCE() method */
extern struct kobjop_desc bus_translate_resource_desc;
/** @brief A function implementing the BUS_TRANSLATE_RESOURCE() method */
typedef int bus_translate_resource_t(device_t _dev, int _type,
                                     rman_res_t _start, rman_res_t *_newstart);
/**
 * @brief translate a resource value
 *
 * Give a bus driver the opportunity to translate resource ranges.  If
 * successful, the host's view of the resource starting at @p _start is
 * returned in @p _newstart, otherwise an error is returned.
 *
 * @param _dev		the device associated with the resource
 * @param _type		the type of resource
 * @param _start	the starting address of the resource range
 * @param _newstart	the new starting address of the resource range
 */

static __inline int BUS_TRANSLATE_RESOURCE(device_t _dev, int _type,
                                           rman_res_t _start,
                                           rman_res_t *_newstart)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_translate_resource);
	rc = ((bus_translate_resource_t *) _m)(_dev, _type, _start, _newstart);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RELEASE_RESOURCE() method */
extern struct kobjop_desc bus_release_resource_desc;
/** @brief A function implementing the BUS_RELEASE_RESOURCE() method */
typedef int bus_release_resource_t(device_t _dev, device_t _child,
                                   struct resource *_res);
/**
 * @brief Release a resource
 *
 * Free a resource allocated by the BUS_ALLOC_RESOURCE.  The @p _rid
 * value must be the same as the one returned by BUS_ALLOC_RESOURCE()
 * (which is not necessarily the same as the one the client passed).
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _r		the resource to release
 */

static __inline int BUS_RELEASE_RESOURCE(device_t _dev, device_t _child,
                                         struct resource *_res)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_release_resource);
	rc = ((bus_release_resource_t *) _m)(_dev, _child, _res);
	return (rc);
}

/** @brief Unique descriptor for the BUS_SETUP_INTR() method */
extern struct kobjop_desc bus_setup_intr_desc;
/** @brief A function implementing the BUS_SETUP_INTR() method */
typedef int bus_setup_intr_t(device_t _dev, device_t _child,
                             struct resource *_irq, int _flags,
                             driver_filter_t *_filter, driver_intr_t *_intr,
                             void *_arg, void **_cookiep);
/**
 * @brief Install an interrupt handler
 *
 * This method is used to associate an interrupt handler function with
 * an irq resource. When the interrupt triggers, the function @p _intr
 * will be called with the value of @p _arg as its single
 * argument. The value returned in @p *_cookiep is used to cancel the
 * interrupt handler - the caller should save this value to use in a
 * future call to BUS_TEARDOWN_INTR().
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 * @param _flags	a set of bits from enum intr_type specifying
 *			the class of interrupt
 * @param _intr		the function to call when the interrupt
 *			triggers
 * @param _arg		a value to use as the single argument in calls
 *			to @p _intr
 * @param _cookiep	a pointer to a location to receive a cookie
 *			value that may be used to remove the interrupt
 *			handler
 */

static __inline int BUS_SETUP_INTR(device_t _dev, device_t _child,
                                   struct resource *_irq, int _flags,
                                   driver_filter_t *_filter,
                                   driver_intr_t *_intr, void *_arg,
                                   void **_cookiep)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_setup_intr);
	rc = ((bus_setup_intr_t *) _m)(_dev, _child, _irq, _flags, _filter, _intr, _arg, _cookiep);
	return (rc);
}

/** @brief Unique descriptor for the BUS_TEARDOWN_INTR() method */
extern struct kobjop_desc bus_teardown_intr_desc;
/** @brief A function implementing the BUS_TEARDOWN_INTR() method */
typedef int bus_teardown_intr_t(device_t _dev, device_t _child,
                                struct resource *_irq, void *_cookie);
/**
 * @brief Uninstall an interrupt handler
 *
 * This method is used to disassociate an interrupt handler function
 * with an irq resource. The value of @p _cookie must be the value
 * returned from a previous call to BUS_SETUP_INTR().
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 * @param _cookie	the cookie value returned when the interrupt
 *			was originally registered
 */

static __inline int BUS_TEARDOWN_INTR(device_t _dev, device_t _child,
                                      struct resource *_irq, void *_cookie)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_teardown_intr);
	rc = ((bus_teardown_intr_t *) _m)(_dev, _child, _irq, _cookie);
	return (rc);
}

/** @brief Unique descriptor for the BUS_SUSPEND_INTR() method */
extern struct kobjop_desc bus_suspend_intr_desc;
/** @brief A function implementing the BUS_SUSPEND_INTR() method */
typedef int bus_suspend_intr_t(device_t _dev, device_t _child,
                               struct resource *_irq);
/**
 * @brief Suspend an interrupt handler
 *
 * This method is used to mark a handler as suspended in the case
 * that the associated device is powered down and cannot be a source
 * for the, typically shared, interrupt.
 * The value of @p _irq must be the interrupt resource passed
 * to a previous call to BUS_SETUP_INTR().
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 */

static __inline int BUS_SUSPEND_INTR(device_t _dev, device_t _child,
                                     struct resource *_irq)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_suspend_intr);
	rc = ((bus_suspend_intr_t *) _m)(_dev, _child, _irq);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESUME_INTR() method */
extern struct kobjop_desc bus_resume_intr_desc;
/** @brief A function implementing the BUS_RESUME_INTR() method */
typedef int bus_resume_intr_t(device_t _dev, device_t _child,
                              struct resource *_irq);
/**
 * @brief Resume an interrupt handler
 *
 * This method is used to clear suspended state of a handler when
 * the associated device is powered up and can be an interrupt source
 * again.
 * The value of @p _irq must be the interrupt resource passed
 * to a previous call to BUS_SETUP_INTR().
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 */

static __inline int BUS_RESUME_INTR(device_t _dev, device_t _child,
                                    struct resource *_irq)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_resume_intr);
	rc = ((bus_resume_intr_t *) _m)(_dev, _child, _irq);
	return (rc);
}

/** @brief Unique descriptor for the BUS_SET_RESOURCE() method */
extern struct kobjop_desc bus_set_resource_desc;
/** @brief A function implementing the BUS_SET_RESOURCE() method */
typedef int bus_set_resource_t(device_t _dev, device_t _child, int _type,
                               int _rid, rman_res_t _start, rman_res_t _count);
/**
 * @brief Define a resource which can be allocated with
 * BUS_ALLOC_RESOURCE().
 *
 * This method is used by some buses (typically ISA) to allow a
 * driver to describe a resource range that it would like to
 * allocate. The resource defined by @p _type and @p _rid is defined
 * to start at @p _start and to include @p _count indices in its
 * range.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which owns the resource
 * @param _type		the type of resource
 * @param _rid		the resource identifier
 * @param _start	the start of the resource range
 * @param _count	the size of the resource range
 */

static __inline int BUS_SET_RESOURCE(device_t _dev, device_t _child, int _type,
                                     int _rid, rman_res_t _start,
                                     rman_res_t _count)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_set_resource);
	rc = ((bus_set_resource_t *) _m)(_dev, _child, _type, _rid, _start, _count);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_RESOURCE() method */
extern struct kobjop_desc bus_get_resource_desc;
/** @brief A function implementing the BUS_GET_RESOURCE() method */
typedef int bus_get_resource_t(device_t _dev, device_t _child, int _type,
                               int _rid, rman_res_t *_startp,
                               rman_res_t *_countp);
/**
 * @brief Describe a resource
 *
 * This method allows a driver to examine the range used for a given
 * resource without actually allocating it.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which owns the resource
 * @param _type		the type of resource
 * @param _rid		the resource identifier
 * @param _start	the address of a location to receive the start
 *			index of the resource range
 * @param _count	the address of a location to receive the size
 *			of the resource range
 */

static __inline int BUS_GET_RESOURCE(device_t _dev, device_t _child, int _type,
                                     int _rid, rman_res_t *_startp,
                                     rman_res_t *_countp)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_resource);
	rc = ((bus_get_resource_t *) _m)(_dev, _child, _type, _rid, _startp, _countp);
	return (rc);
}

/** @brief Unique descriptor for the BUS_DELETE_RESOURCE() method */
extern struct kobjop_desc bus_delete_resource_desc;
/** @brief A function implementing the BUS_DELETE_RESOURCE() method */
typedef void bus_delete_resource_t(device_t _dev, device_t _child, int _type,
                                   int _rid);
/**
 * @brief Delete a resource.
 *
 * Use this to delete a resource (possibly one previously added with
 * BUS_SET_RESOURCE()).
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which owns the resource
 * @param _type		the type of resource
 * @param _rid		the resource identifier
 */

static __inline void BUS_DELETE_RESOURCE(device_t _dev, device_t _child,
                                         int _type, int _rid)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_delete_resource);
	((bus_delete_resource_t *) _m)(_dev, _child, _type, _rid);
}

/** @brief Unique descriptor for the BUS_GET_RESOURCE_LIST() method */
extern struct kobjop_desc bus_get_resource_list_desc;
/** @brief A function implementing the BUS_GET_RESOURCE_LIST() method */
typedef struct resource_list * bus_get_resource_list_t(device_t _dev,
                                                       device_t _child);
/**
 * @brief Return a struct resource_list.
 *
 * Used by drivers which use bus_generic_rl_alloc_resource() etc. to
 * implement their resource handling. It should return the resource
 * list of the given child device.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which owns the resource list
 */

static __inline struct resource_list * BUS_GET_RESOURCE_LIST(device_t _dev,
                                                             device_t _child)
{
	kobjop_t _m;
	struct resource_list * rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_resource_list);
	rc = ((bus_get_resource_list_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_RMAN() method */
extern struct kobjop_desc bus_get_rman_desc;
/** @brief A function implementing the BUS_GET_RMAN() method */
typedef struct rman * bus_get_rman_t(device_t _dev, int _type, u_int _flags);
/**
 * @brief Return a struct rman.
 *
 * Used by drivers which use bus_generic_rman_alloc_resource() etc. to
 * implement their resource handling. It should return the resource
 * manager used for the given resource type.
 *
 * @param _dev		the bus device
 * @param _type		the resource type
 * @param _flags	resource flags (@c RF_XXX flags in
 *			<sys/rman.h>)
 */

static __inline struct rman * BUS_GET_RMAN(device_t _dev, int _type,
                                           u_int _flags)
{
	kobjop_t _m;
	struct rman * rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_rman);
	rc = ((bus_get_rman_t *) _m)(_dev, _type, _flags);
	return (rc);
}

/** @brief Unique descriptor for the BUS_CHILD_PRESENT() method */
extern struct kobjop_desc bus_child_present_desc;
/** @brief A function implementing the BUS_CHILD_PRESENT() method */
typedef int bus_child_present_t(device_t _dev, device_t _child);
/**
 * @brief Is the hardware described by @p _child still attached to the
 * system?
 *
 * This method should return 0 if the device is not present.  It
 * should return -1 if it is present.  Any errors in determining
 * should be returned as a normal errno value.  Client drivers are to
 * assume that the device is present, even if there is an error
 * determining if it is there.  Buses are to try to avoid returning
 * errors, but newcard will return an error if the device fails to
 * implement this method.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which is being examined
 */

static __inline int BUS_CHILD_PRESENT(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_child_present);
	rc = ((bus_child_present_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_CHILD_PNPINFO() method */
extern struct kobjop_desc bus_child_pnpinfo_desc;
/** @brief A function implementing the BUS_CHILD_PNPINFO() method */
typedef int bus_child_pnpinfo_t(device_t _dev, device_t _child,
                                struct sbuf *_sb);
/**
 * @brief Returns the pnp info for this device.
 *
 * Return it as a string, appended to @p _sb
 *
 * The string must be formatted as a space-separated list of
 * name=value pairs.  Names may only contain alphanumeric characters,
 * underscores ('_') and hyphens ('-').  Values can contain any
 * non-whitespace characters.  Values containing whitespace can be
 * quoted with double quotes ('"').  Double quotes and backslashes in
 * quoted values can be escaped with backslashes ('\').
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which is being examined
 * @param _sb		sbuf for results string
 */

static __inline int BUS_CHILD_PNPINFO(device_t _dev, device_t _child,
                                      struct sbuf *_sb)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_child_pnpinfo);
	rc = ((bus_child_pnpinfo_t *) _m)(_dev, _child, _sb);
	return (rc);
}

/** @brief Unique descriptor for the BUS_CHILD_LOCATION() method */
extern struct kobjop_desc bus_child_location_desc;
/** @brief A function implementing the BUS_CHILD_LOCATION() method */
typedef int bus_child_location_t(device_t _dev, device_t _child,
                                 struct sbuf *_sb);
/**
 * @brief Returns the location for this device.
 *
 * Return it as a string, appended to @p _sb
 *
 * The string must be formatted as a space-separated list of
 * name=value pairs.  Names may only contain alphanumeric characters,
 * underscores ('_') and hyphens ('-').  Values can contain any
 * non-whitespace characters.  Values containing whitespace can be
 * quoted with double quotes ('"').  Double quotes and backslashes in
 * quoted values can be escaped with backslashes ('\').
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which is being examined
 * @param _sb		sbuf for results string
 */

static __inline int BUS_CHILD_LOCATION(device_t _dev, device_t _child,
                                       struct sbuf *_sb)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_child_location);
	rc = ((bus_child_location_t *) _m)(_dev, _child, _sb);
	return (rc);
}

/** @brief Unique descriptor for the BUS_BIND_INTR() method */
extern struct kobjop_desc bus_bind_intr_desc;
/** @brief A function implementing the BUS_BIND_INTR() method */
typedef int bus_bind_intr_t(device_t _dev, device_t _child,
                            struct resource *_irq, int _cpu);
/**
 * @brief Allow drivers to request that an interrupt be bound to a specific
 * CPU.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 * @param _cpu		the CPU to bind the interrupt to
 */

static __inline int BUS_BIND_INTR(device_t _dev, device_t _child,
                                  struct resource *_irq, int _cpu)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_bind_intr);
	rc = ((bus_bind_intr_t *) _m)(_dev, _child, _irq, _cpu);
	return (rc);
}

/** @brief Unique descriptor for the BUS_CONFIG_INTR() method */
extern struct kobjop_desc bus_config_intr_desc;
/** @brief A function implementing the BUS_CONFIG_INTR() method */
typedef int bus_config_intr_t(device_t _dev, int _irq, enum intr_trigger _trig,
                              enum intr_polarity _pol);
/**
 * @brief Allow (bus) drivers to specify the trigger mode and polarity
 * of the specified interrupt.
 *
 * @param _dev		the bus device
 * @param _irq		the interrupt number to modify
 * @param _trig		the trigger mode required
 * @param _pol		the interrupt polarity required
 */

static __inline int BUS_CONFIG_INTR(device_t _dev, int _irq,
                                    enum intr_trigger _trig,
                                    enum intr_polarity _pol)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_config_intr);
	rc = ((bus_config_intr_t *) _m)(_dev, _irq, _trig, _pol);
	return (rc);
}

/** @brief Unique descriptor for the BUS_DESCRIBE_INTR() method */
extern struct kobjop_desc bus_describe_intr_desc;
/** @brief A function implementing the BUS_DESCRIBE_INTR() method */
typedef int bus_describe_intr_t(device_t _dev, device_t _child,
                                struct resource *_irq, void *_cookie,
                                const char *_descr);
/**
 * @brief Allow drivers to associate a description with an active
 * interrupt handler.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device which allocated the resource
 * @param _irq		the resource representing the interrupt
 * @param _cookie	the cookie value returned when the interrupt
 *			was originally registered
 * @param _descr	the description to associate with the interrupt
 */

static __inline int BUS_DESCRIBE_INTR(device_t _dev, device_t _child,
                                      struct resource *_irq, void *_cookie,
                                      const char *_descr)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_describe_intr);
	rc = ((bus_describe_intr_t *) _m)(_dev, _child, _irq, _cookie, _descr);
	return (rc);
}

/** @brief Unique descriptor for the BUS_HINTED_CHILD() method */
extern struct kobjop_desc bus_hinted_child_desc;
/** @brief A function implementing the BUS_HINTED_CHILD() method */
typedef void bus_hinted_child_t(device_t _dev, const char *_dname, int _dunit);
/**
 * @brief Notify a (bus) driver about a child that the hints mechanism
 * believes it has discovered.
 *
 * The bus is responsible for then adding the child in the right order
 * and discovering other things about the child.  The bus driver is
 * free to ignore this hint, to do special things, etc.  It is all up
 * to the bus driver to interpret.
 *
 * This method is only called in response to the parent bus asking for
 * hinted devices to be enumerated.
 *
 * @param _dev		the bus device
 * @param _dname	the name of the device w/o unit numbers
 * @param _dunit	the unit number of the device
 */

static __inline void BUS_HINTED_CHILD(device_t _dev, const char *_dname,
                                      int _dunit)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_hinted_child);
	((bus_hinted_child_t *) _m)(_dev, _dname, _dunit);
}

/** @brief Unique descriptor for the BUS_GET_DMA_TAG() method */
extern struct kobjop_desc bus_get_dma_tag_desc;
/** @brief A function implementing the BUS_GET_DMA_TAG() method */
typedef bus_dma_tag_t bus_get_dma_tag_t(device_t _dev, device_t _child);
/**
 * @brief Returns bus_dma_tag_t for use w/ devices on the bus.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device to which the tag will belong
 */

static __inline bus_dma_tag_t BUS_GET_DMA_TAG(device_t _dev, device_t _child)
{
	kobjop_t _m;
	bus_dma_tag_t rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_dma_tag);
	rc = ((bus_get_dma_tag_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_BUS_TAG() method */
extern struct kobjop_desc bus_get_bus_tag_desc;
/** @brief A function implementing the BUS_GET_BUS_TAG() method */
typedef bus_space_tag_t bus_get_bus_tag_t(device_t _dev, device_t _child);
/**
 * @brief Returns bus_space_tag_t for use w/ devices on the bus.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device to which the tag will belong
 */

static __inline bus_space_tag_t BUS_GET_BUS_TAG(device_t _dev, device_t _child)
{
	kobjop_t _m;
	bus_space_tag_t rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_bus_tag);
	rc = ((bus_get_bus_tag_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_HINT_DEVICE_UNIT() method */
extern struct kobjop_desc bus_hint_device_unit_desc;
/** @brief A function implementing the BUS_HINT_DEVICE_UNIT() method */
typedef void bus_hint_device_unit_t(device_t _dev, device_t _child,
                                    const char *_name, int *_unitp);
/**
 * @brief Allow the bus to determine the unit number of a device.
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device whose unit is to be wired
 * @param _name		the name of the device's new devclass
 * @param _unitp	a pointer to the device's new unit value
 */

static __inline void BUS_HINT_DEVICE_UNIT(device_t _dev, device_t _child,
                                          const char *_name, int *_unitp)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_hint_device_unit);
	((bus_hint_device_unit_t *) _m)(_dev, _child, _name, _unitp);
}

/** @brief Unique descriptor for the BUS_NEW_PASS() method */
extern struct kobjop_desc bus_new_pass_desc;
/** @brief A function implementing the BUS_NEW_PASS() method */
typedef void bus_new_pass_t(device_t _dev);
/**
 * @brief Notify a bus that the bus pass level has been changed
 *
 * @param _dev		the bus device
 */

static __inline void BUS_NEW_PASS(device_t _dev)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_new_pass);
	((bus_new_pass_t *) _m)(_dev);
}

/** @brief Unique descriptor for the BUS_REMAP_INTR() method */
extern struct kobjop_desc bus_remap_intr_desc;
/** @brief A function implementing the BUS_REMAP_INTR() method */
typedef int bus_remap_intr_t(device_t _dev, device_t _child, u_int _irq);
/**
 * @brief Notify a bus that specified child's IRQ should be remapped.
 *
 * @param _dev		the bus device
 * @param _child	the child device
 * @param _irq		the irq number
 */

static __inline int BUS_REMAP_INTR(device_t _dev, device_t _child, u_int _irq)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_remap_intr);
	rc = ((bus_remap_intr_t *) _m)(_dev, _child, _irq);
	return (rc);
}

/** @brief Unique descriptor for the BUS_SUSPEND_CHILD() method */
extern struct kobjop_desc bus_suspend_child_desc;
/** @brief A function implementing the BUS_SUSPEND_CHILD() method */
typedef int bus_suspend_child_t(device_t _dev, device_t _child);
/**
 * @brief Suspend a given child
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device to suspend
 */

static __inline int BUS_SUSPEND_CHILD(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_suspend_child);
	rc = ((bus_suspend_child_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESUME_CHILD() method */
extern struct kobjop_desc bus_resume_child_desc;
/** @brief A function implementing the BUS_RESUME_CHILD() method */
typedef int bus_resume_child_t(device_t _dev, device_t _child);
/**
 * @brief Resume a given child
 *
 * @param _dev		the parent device of @p _child
 * @param _child	the device to resume
 */

static __inline int BUS_RESUME_CHILD(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_resume_child);
	rc = ((bus_resume_child_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_DOMAIN() method */
extern struct kobjop_desc bus_get_domain_desc;
/** @brief A function implementing the BUS_GET_DOMAIN() method */
typedef int bus_get_domain_t(device_t _dev, device_t _child, int *_domain);
/**
 * @brief Get the VM domain handle for the given bus and child.
 *
 * @param _dev		the bus device
 * @param _child	the child device
 * @param _domain	a pointer to the bus's domain handle identifier
 */

static __inline int BUS_GET_DOMAIN(device_t _dev, device_t _child, int *_domain)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_domain);
	rc = ((bus_get_domain_t *) _m)(_dev, _child, _domain);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_CPUS() method */
extern struct kobjop_desc bus_get_cpus_desc;
/** @brief A function implementing the BUS_GET_CPUS() method */
typedef int bus_get_cpus_t(device_t _dev, device_t _child, enum cpu_sets _op,
                           size_t _setsize, struct _cpuset *_cpuset);
/**
 * @brief Request a set of CPUs
 *
 * @param _dev		the bus device
 * @param _child	the child device
 * @param _op		type of CPUs to request
 * @param _setsize	the size of the set passed in _cpuset
 * @param _cpuset	a pointer to a cpuset to receive the requested
 *			set of CPUs
 */

static __inline int BUS_GET_CPUS(device_t _dev, device_t _child,
                                 enum cpu_sets _op, size_t _setsize,
                                 struct _cpuset *_cpuset)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_cpus);
	rc = ((bus_get_cpus_t *) _m)(_dev, _child, _op, _setsize, _cpuset);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESET_PREPARE() method */
extern struct kobjop_desc bus_reset_prepare_desc;
/** @brief A function implementing the BUS_RESET_PREPARE() method */
typedef int bus_reset_prepare_t(device_t _dev, device_t _child);
/**
 * @brief Prepares the given child of the bus for reset
 *
 * Typically bus detaches or suspends children' drivers, and then
 * calls this method to save bus-specific information, for instance,
 * PCI config space, which is damaged by reset.
 *
 * The bus_helper_reset_prepare() helper is provided to ease
 * implementing bus reset methods.
 *
 * @param _dev		the bus device
 * @param _child	the child device
 */

static __inline int BUS_RESET_PREPARE(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_reset_prepare);
	rc = ((bus_reset_prepare_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESET_POST() method */
extern struct kobjop_desc bus_reset_post_desc;
/** @brief A function implementing the BUS_RESET_POST() method */
typedef int bus_reset_post_t(device_t _dev, device_t _child);
/**
 * @brief Restores the child operations after the reset
 *
 * The bus_helper_reset_post() helper is provided to ease
 * implementing bus reset methods.
 *
 * @param _dev		the bus device
 * @param _child	the child device
 */

static __inline int BUS_RESET_POST(device_t _dev, device_t _child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_reset_post);
	rc = ((bus_reset_post_t *) _m)(_dev, _child);
	return (rc);
}

/** @brief Unique descriptor for the BUS_RESET_CHILD() method */
extern struct kobjop_desc bus_reset_child_desc;
/** @brief A function implementing the BUS_RESET_CHILD() method */
typedef int bus_reset_child_t(device_t _dev, device_t _child, int _flags);
/**
 * @brief Performs reset of the child
 *
 * @param _dev		the bus device
 * @param _child	the child device
 * @param _flags	DEVF_RESET_ flags
 */

static __inline int BUS_RESET_CHILD(device_t _dev, device_t _child, int _flags)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_reset_child);
	rc = ((bus_reset_child_t *) _m)(_dev, _child, _flags);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_PROPERTY() method */
extern struct kobjop_desc bus_get_property_desc;
/** @brief A function implementing the BUS_GET_PROPERTY() method */
typedef ssize_t bus_get_property_t(device_t _dev, device_t _child,
                                   const char *_propname, void *_propvalue,
                                   size_t _size, device_property_type_t type);
/**
 * @brief Gets child's specific property
 *
 * The bus_get_property can be used to access device
 * specific properties stored on the bus. If _propvalue
 * is NULL or _size is 0, then method only returns size
 * of the property.
 *
 * @param _dev			the bus device
 * @param _child		the child device
 * @param _propname		property name
 * @param _propvalue	property value destination
 * @param _size			property value size
 *
 * @returns size of property if successful otherwise -1
 */

static __inline ssize_t BUS_GET_PROPERTY(device_t _dev, device_t _child,
                                         const char *_propname,
                                         void *_propvalue, size_t _size,
                                         device_property_type_t type)
{
	kobjop_t _m;
	ssize_t rc;
	KOBJOPLOOKUP(((kobj_t)_dev)->ops,bus_get_property);
	rc = ((bus_get_property_t *) _m)(_dev, _child, _propname, _propvalue, _size, type);
	return (rc);
}

/** @brief Unique descriptor for the BUS_GET_DEVICE_PATH() method */
extern struct kobjop_desc bus_get_device_path_desc;
/** @brief A function implementing the BUS_GET_DEVICE_PATH() method */
typedef int bus_get_device_path_t(device_t _bus, device_t _child,
                                  const char *_locator, struct sbuf *_sb);
/**
 * @brief Gets a child's full path to the device
 *
 * The get_device_path method retrieves a device's
 * full path to the device using one of several
 * locators present in the system.
 *
 * @param _bus			the bus device
 * @param _child		the child device
 * @param _locator		locator name
 * @param _sb			buffer loaction string
 */

static __inline int BUS_GET_DEVICE_PATH(device_t _bus, device_t _child,
                                        const char *_locator, struct sbuf *_sb)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)_bus)->ops,bus_get_device_path);
	rc = ((bus_get_device_path_t *) _m)(_bus, _child, _locator, _sb);
	return (rc);
}

#endif /* _bus_if_h_ */

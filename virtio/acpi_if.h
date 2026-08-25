/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 *
 * Created from source file
 *   /usr/src/sys/dev/acpica/acpi_if.m
 * with
 *   makeobjops.awk
 *
 * See the source file for legal information
 */


#ifndef _acpi_if_h_
#define _acpi_if_h_


typedef ACPI_STATUS (*acpi_scan_cb_t)(ACPI_HANDLE h, device_t *dev,
    int level, void *arg);

struct acpi_bix;
struct acpi_bif;
struct acpi_bst;

/** @brief Unique descriptor for the ACPI_ID_PROBE() method */
extern struct kobjop_desc acpi_id_probe_desc;
/** @brief A function implementing the ACPI_ID_PROBE() method */
typedef int acpi_id_probe_t(device_t bus, device_t dev, char **ids,
                            char **match);

static __inline int ACPI_ID_PROBE(device_t bus, device_t dev, char **ids,
                                  char **match)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,acpi_id_probe);
	rc = ((acpi_id_probe_t *) _m)(bus, dev, ids, match);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_EVALUATE_OBJECT() method */
extern struct kobjop_desc acpi_evaluate_object_desc;
/** @brief A function implementing the ACPI_EVALUATE_OBJECT() method */
typedef ACPI_STATUS acpi_evaluate_object_t(device_t bus, device_t dev,
                                           ACPI_STRING pathname,
                                           ACPI_OBJECT_LIST *parameters,
                                           ACPI_BUFFER *ret);

static __inline ACPI_STATUS ACPI_EVALUATE_OBJECT(device_t bus, device_t dev,
                                                 ACPI_STRING pathname,
                                                 ACPI_OBJECT_LIST *parameters,
                                                 ACPI_BUFFER *ret)
{
	kobjop_t _m;
	ACPI_STATUS rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,acpi_evaluate_object);
	rc = ((acpi_evaluate_object_t *) _m)(bus, dev, pathname, parameters, ret);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_GET_PROPERTY() method */
extern struct kobjop_desc acpi_get_property_desc;
/** @brief A function implementing the ACPI_GET_PROPERTY() method */
typedef ACPI_STATUS acpi_get_property_t(device_t bus, device_t dev,
                                        ACPI_STRING propname,
                                        const ACPI_OBJECT **value);

static __inline ACPI_STATUS ACPI_GET_PROPERTY(device_t bus, device_t dev,
                                              ACPI_STRING propname,
                                              const ACPI_OBJECT **value)
{
	kobjop_t _m;
	ACPI_STATUS rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,acpi_get_property);
	rc = ((acpi_get_property_t *) _m)(bus, dev, propname, value);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_PWR_FOR_SLEEP() method */
extern struct kobjop_desc acpi_pwr_for_sleep_desc;
/** @brief A function implementing the ACPI_PWR_FOR_SLEEP() method */
typedef int acpi_pwr_for_sleep_t(device_t bus, device_t dev, int *dstate);

static __inline int ACPI_PWR_FOR_SLEEP(device_t bus, device_t dev, int *dstate)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,acpi_pwr_for_sleep);
	rc = ((acpi_pwr_for_sleep_t *) _m)(bus, dev, dstate);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_SCAN_CHILDREN() method */
extern struct kobjop_desc acpi_scan_children_desc;
/** @brief A function implementing the ACPI_SCAN_CHILDREN() method */
typedef ACPI_STATUS acpi_scan_children_t(device_t bus, device_t dev,
                                         int max_depth, acpi_scan_cb_t user_fn,
                                         void *arg);

static __inline ACPI_STATUS ACPI_SCAN_CHILDREN(device_t bus, device_t dev,
                                               int max_depth,
                                               acpi_scan_cb_t user_fn,
                                               void *arg)
{
	kobjop_t _m;
	ACPI_STATUS rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,acpi_scan_children);
	rc = ((acpi_scan_children_t *) _m)(bus, dev, max_depth, user_fn, arg);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_GET_FEATURES() method */
extern struct kobjop_desc acpi_get_features_desc;
/** @brief A function implementing the ACPI_GET_FEATURES() method */
typedef int acpi_get_features_t(driver_t *driver, u_int *features);

static __inline int ACPI_GET_FEATURES(driver_t *driver, u_int *features)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(driver->ops,acpi_get_features);
	rc = ((acpi_get_features_t *) _m)(driver, features);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_EC_READ() method */
extern struct kobjop_desc acpi_ec_read_desc;
/** @brief A function implementing the ACPI_EC_READ() method */
typedef int acpi_ec_read_t(device_t dev, u_int addr, UINT64 *val, int width);

static __inline int ACPI_EC_READ(device_t dev, u_int addr, UINT64 *val,
                                 int width)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,acpi_ec_read);
	rc = ((acpi_ec_read_t *) _m)(dev, addr, val, width);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_EC_WRITE() method */
extern struct kobjop_desc acpi_ec_write_desc;
/** @brief A function implementing the ACPI_EC_WRITE() method */
typedef int acpi_ec_write_t(device_t dev, u_int addr, UINT64 val, int width);

static __inline int ACPI_EC_WRITE(device_t dev, u_int addr, UINT64 val,
                                  int width)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,acpi_ec_write);
	rc = ((acpi_ec_write_t *) _m)(dev, addr, val, width);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_BATT_GET_INFO() method */
extern struct kobjop_desc acpi_batt_get_info_desc;
/** @brief A function implementing the ACPI_BATT_GET_INFO() method */
typedef int acpi_batt_get_info_t(device_t dev, void *bix, size_t len);

static __inline int ACPI_BATT_GET_INFO(device_t dev, void *bix, size_t len)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,acpi_batt_get_info);
	rc = ((acpi_batt_get_info_t *) _m)(dev, bix, len);
	return (rc);
}

/** @brief Unique descriptor for the ACPI_BATT_GET_STATUS() method */
extern struct kobjop_desc acpi_batt_get_status_desc;
/** @brief A function implementing the ACPI_BATT_GET_STATUS() method */
typedef int acpi_batt_get_status_t(device_t dev, struct acpi_bst *bst);

static __inline int ACPI_BATT_GET_STATUS(device_t dev, struct acpi_bst *bst)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,acpi_batt_get_status);
	rc = ((acpi_batt_get_status_t *) _m)(dev, bst);
	return (rc);
}

#endif /* _acpi_if_h_ */

/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 *
 * Created from source file
 *   /usr/src/sys/dev/virtio/virtio_if.m
 * with
 *   makeobjops.awk
 *
 * See the source file for legal information
 */


#ifndef _virtio_if_h_
#define _virtio_if_h_

/** @brief Unique descriptor for the VIRTIO_ATTACH_COMPLETED() method */
extern struct kobjop_desc virtio_attach_completed_desc;
/** @brief A function implementing the VIRTIO_ATTACH_COMPLETED() method */
typedef int virtio_attach_completed_t(device_t dev);

static __inline int VIRTIO_ATTACH_COMPLETED(device_t dev)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_attach_completed);
	rc = ((virtio_attach_completed_t *) _m)(dev);
	return (rc);
}

/** @brief Unique descriptor for the VIRTIO_CONFIG_CHANGE() method */
extern struct kobjop_desc virtio_config_change_desc;
/** @brief A function implementing the VIRTIO_CONFIG_CHANGE() method */
typedef int virtio_config_change_t(device_t dev);

static __inline int VIRTIO_CONFIG_CHANGE(device_t dev)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_config_change);
	rc = ((virtio_config_change_t *) _m)(dev);
	return (rc);
}

#endif /* _virtio_if_h_ */

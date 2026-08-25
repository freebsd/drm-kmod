/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 *
 * Created from source file
 *   /usr/src/sys/dev/virtio/pci/virtio_pci_if.m
 * with
 *   makeobjops.awk
 *
 * See the source file for legal information
 */


#ifndef _virtio_pci_if_h_
#define _virtio_pci_if_h_


struct virtqueue;
struct vtpci_interrupt;

/** @brief Unique descriptor for the VIRTIO_PCI_READ_ISR() method */
extern struct kobjop_desc virtio_pci_read_isr_desc;
/** @brief A function implementing the VIRTIO_PCI_READ_ISR() method */
typedef uint8_t virtio_pci_read_isr_t(device_t dev);

static __inline uint8_t VIRTIO_PCI_READ_ISR(device_t dev)
{
	kobjop_t _m;
	uint8_t rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_read_isr);
	rc = ((virtio_pci_read_isr_t *) _m)(dev);
	return (rc);
}

/** @brief Unique descriptor for the VIRTIO_PCI_GET_VQ_SIZE() method */
extern struct kobjop_desc virtio_pci_get_vq_size_desc;
/** @brief A function implementing the VIRTIO_PCI_GET_VQ_SIZE() method */
typedef uint16_t virtio_pci_get_vq_size_t(device_t dev, int idx);

static __inline uint16_t VIRTIO_PCI_GET_VQ_SIZE(device_t dev, int idx)
{
	kobjop_t _m;
	uint16_t rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_get_vq_size);
	rc = ((virtio_pci_get_vq_size_t *) _m)(dev, idx);
	return (rc);
}

/** @brief Unique descriptor for the VIRTIO_PCI_GET_VQ_NOTIFY_OFF() method */
extern struct kobjop_desc virtio_pci_get_vq_notify_off_desc;
/** @brief A function implementing the VIRTIO_PCI_GET_VQ_NOTIFY_OFF() method */
typedef bus_size_t virtio_pci_get_vq_notify_off_t(device_t dev, int idx);

static __inline bus_size_t VIRTIO_PCI_GET_VQ_NOTIFY_OFF(device_t dev, int idx)
{
	kobjop_t _m;
	bus_size_t rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_get_vq_notify_off);
	rc = ((virtio_pci_get_vq_notify_off_t *) _m)(dev, idx);
	return (rc);
}

/** @brief Unique descriptor for the VIRTIO_PCI_SET_VQ() method */
extern struct kobjop_desc virtio_pci_set_vq_desc;
/** @brief A function implementing the VIRTIO_PCI_SET_VQ() method */
typedef void virtio_pci_set_vq_t(device_t dev, struct virtqueue *vq);

static __inline void VIRTIO_PCI_SET_VQ(device_t dev, struct virtqueue *vq)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_set_vq);
	((virtio_pci_set_vq_t *) _m)(dev, vq);
}

/** @brief Unique descriptor for the VIRTIO_PCI_DISABLE_VQ() method */
extern struct kobjop_desc virtio_pci_disable_vq_desc;
/** @brief A function implementing the VIRTIO_PCI_DISABLE_VQ() method */
typedef void virtio_pci_disable_vq_t(device_t dev, int idx);

static __inline void VIRTIO_PCI_DISABLE_VQ(device_t dev, int idx)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_disable_vq);
	((virtio_pci_disable_vq_t *) _m)(dev, idx);
}

/** @brief Unique descriptor for the VIRTIO_PCI_REGISTER_CFG_MSIX() method */
extern struct kobjop_desc virtio_pci_register_cfg_msix_desc;
/** @brief A function implementing the VIRTIO_PCI_REGISTER_CFG_MSIX() method */
typedef int virtio_pci_register_cfg_msix_t(device_t dev,
                                           struct vtpci_interrupt *intr);

static __inline int VIRTIO_PCI_REGISTER_CFG_MSIX(device_t dev,
                                                 struct vtpci_interrupt *intr)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_register_cfg_msix);
	rc = ((virtio_pci_register_cfg_msix_t *) _m)(dev, intr);
	return (rc);
}

/** @brief Unique descriptor for the VIRTIO_PCI_REGISTER_VQ_MSIX() method */
extern struct kobjop_desc virtio_pci_register_vq_msix_desc;
/** @brief A function implementing the VIRTIO_PCI_REGISTER_VQ_MSIX() method */
typedef int virtio_pci_register_vq_msix_t(device_t dev, int idx,
                                          struct vtpci_interrupt *intr);

static __inline int VIRTIO_PCI_REGISTER_VQ_MSIX(device_t dev, int idx,
                                                struct vtpci_interrupt *intr)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,virtio_pci_register_vq_msix);
	rc = ((virtio_pci_register_vq_msix_t *) _m)(dev, idx, intr);
	return (rc);
}

#endif /* _virtio_pci_if_h_ */

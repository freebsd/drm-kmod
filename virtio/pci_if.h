/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 *
 * Created from source file
 *   /usr/src/sys/dev/pci/pci_if.m
 * with
 *   makeobjops.awk
 *
 * See the source file for legal information
 */


#ifndef _pci_if_h_
#define _pci_if_h_


struct nvlist;

enum pci_id_type {
    PCI_ID_RID,
    PCI_ID_MSI,
    PCI_ID_OFW_IOMMU,
};

enum pci_feature {
    PCI_FEATURE_HP,		/* Hot Plug feature */
    PCI_FEATURE_AER,		/* Advanced Error Reporting */
};

/** @brief Unique descriptor for the PCI_READ_CONFIG() method */
extern struct kobjop_desc pci_read_config_desc;
/** @brief A function implementing the PCI_READ_CONFIG() method */
typedef u_int32_t pci_read_config_t(device_t dev, device_t child, int reg,
                                    int width);

static __inline u_int32_t PCI_READ_CONFIG(device_t dev, device_t child, int reg,
                                          int width)
{
	kobjop_t _m;
	u_int32_t rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_read_config);
	rc = ((pci_read_config_t *) _m)(dev, child, reg, width);
	return (rc);
}

/** @brief Unique descriptor for the PCI_WRITE_CONFIG() method */
extern struct kobjop_desc pci_write_config_desc;
/** @brief A function implementing the PCI_WRITE_CONFIG() method */
typedef void pci_write_config_t(device_t dev, device_t child, int reg,
                                u_int32_t val, int width);

static __inline void PCI_WRITE_CONFIG(device_t dev, device_t child, int reg,
                                      u_int32_t val, int width)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_write_config);
	((pci_write_config_t *) _m)(dev, child, reg, val, width);
}

/** @brief Unique descriptor for the PCI_GET_POWERSTATE() method */
extern struct kobjop_desc pci_get_powerstate_desc;
/** @brief A function implementing the PCI_GET_POWERSTATE() method */
typedef int pci_get_powerstate_t(device_t dev, device_t child);

static __inline int PCI_GET_POWERSTATE(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_get_powerstate);
	rc = ((pci_get_powerstate_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_SET_POWERSTATE() method */
extern struct kobjop_desc pci_set_powerstate_desc;
/** @brief A function implementing the PCI_SET_POWERSTATE() method */
typedef int pci_set_powerstate_t(device_t dev, device_t child, int state);

static __inline int PCI_SET_POWERSTATE(device_t dev, device_t child, int state)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_set_powerstate);
	rc = ((pci_set_powerstate_t *) _m)(dev, child, state);
	return (rc);
}

/** @brief Unique descriptor for the PCI_GET_VPD_IDENT() method */
extern struct kobjop_desc pci_get_vpd_ident_desc;
/** @brief A function implementing the PCI_GET_VPD_IDENT() method */
typedef int pci_get_vpd_ident_t(device_t dev, device_t child,
                                const char **identptr);

static __inline int PCI_GET_VPD_IDENT(device_t dev, device_t child,
                                      const char **identptr)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_get_vpd_ident);
	rc = ((pci_get_vpd_ident_t *) _m)(dev, child, identptr);
	return (rc);
}

/** @brief Unique descriptor for the PCI_GET_VPD_READONLY() method */
extern struct kobjop_desc pci_get_vpd_readonly_desc;
/** @brief A function implementing the PCI_GET_VPD_READONLY() method */
typedef int pci_get_vpd_readonly_t(device_t dev, device_t child, const char *kw,
                                   const char **vptr);

static __inline int PCI_GET_VPD_READONLY(device_t dev, device_t child,
                                         const char *kw, const char **vptr)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_get_vpd_readonly);
	rc = ((pci_get_vpd_readonly_t *) _m)(dev, child, kw, vptr);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ENABLE_BUSMASTER() method */
extern struct kobjop_desc pci_enable_busmaster_desc;
/** @brief A function implementing the PCI_ENABLE_BUSMASTER() method */
typedef int pci_enable_busmaster_t(device_t dev, device_t child);

static __inline int PCI_ENABLE_BUSMASTER(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_enable_busmaster);
	rc = ((pci_enable_busmaster_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_DISABLE_BUSMASTER() method */
extern struct kobjop_desc pci_disable_busmaster_desc;
/** @brief A function implementing the PCI_DISABLE_BUSMASTER() method */
typedef int pci_disable_busmaster_t(device_t dev, device_t child);

static __inline int PCI_DISABLE_BUSMASTER(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_disable_busmaster);
	rc = ((pci_disable_busmaster_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ENABLE_IO() method */
extern struct kobjop_desc pci_enable_io_desc;
/** @brief A function implementing the PCI_ENABLE_IO() method */
typedef int pci_enable_io_t(device_t dev, device_t child, int space);

static __inline int PCI_ENABLE_IO(device_t dev, device_t child, int space)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_enable_io);
	rc = ((pci_enable_io_t *) _m)(dev, child, space);
	return (rc);
}

/** @brief Unique descriptor for the PCI_DISABLE_IO() method */
extern struct kobjop_desc pci_disable_io_desc;
/** @brief A function implementing the PCI_DISABLE_IO() method */
typedef int pci_disable_io_t(device_t dev, device_t child, int space);

static __inline int PCI_DISABLE_IO(device_t dev, device_t child, int space)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_disable_io);
	rc = ((pci_disable_io_t *) _m)(dev, child, space);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ASSIGN_INTERRUPT() method */
extern struct kobjop_desc pci_assign_interrupt_desc;
/** @brief A function implementing the PCI_ASSIGN_INTERRUPT() method */
typedef int pci_assign_interrupt_t(device_t dev, device_t child);

static __inline int PCI_ASSIGN_INTERRUPT(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_assign_interrupt);
	rc = ((pci_assign_interrupt_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_CAP() method */
extern struct kobjop_desc pci_find_cap_desc;
/** @brief A function implementing the PCI_FIND_CAP() method */
typedef int pci_find_cap_t(device_t dev, device_t child, int capability,
                           int *capreg);

static __inline int PCI_FIND_CAP(device_t dev, device_t child, int capability,
                                 int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_cap);
	rc = ((pci_find_cap_t *) _m)(dev, child, capability, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_NEXT_CAP() method */
extern struct kobjop_desc pci_find_next_cap_desc;
/** @brief A function implementing the PCI_FIND_NEXT_CAP() method */
typedef int pci_find_next_cap_t(device_t dev, device_t child, int capability,
                                int start, int *capreg);

static __inline int PCI_FIND_NEXT_CAP(device_t dev, device_t child,
                                      int capability, int start, int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_next_cap);
	rc = ((pci_find_next_cap_t *) _m)(dev, child, capability, start, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_EXTCAP() method */
extern struct kobjop_desc pci_find_extcap_desc;
/** @brief A function implementing the PCI_FIND_EXTCAP() method */
typedef int pci_find_extcap_t(device_t dev, device_t child, int capability,
                              int *capreg);

static __inline int PCI_FIND_EXTCAP(device_t dev, device_t child,
                                    int capability, int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_extcap);
	rc = ((pci_find_extcap_t *) _m)(dev, child, capability, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_NEXT_EXTCAP() method */
extern struct kobjop_desc pci_find_next_extcap_desc;
/** @brief A function implementing the PCI_FIND_NEXT_EXTCAP() method */
typedef int pci_find_next_extcap_t(device_t dev, device_t child, int capability,
                                   int start, int *capreg);

static __inline int PCI_FIND_NEXT_EXTCAP(device_t dev, device_t child,
                                         int capability, int start, int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_next_extcap);
	rc = ((pci_find_next_extcap_t *) _m)(dev, child, capability, start, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_HTCAP() method */
extern struct kobjop_desc pci_find_htcap_desc;
/** @brief A function implementing the PCI_FIND_HTCAP() method */
typedef int pci_find_htcap_t(device_t dev, device_t child, int capability,
                             int *capreg);

static __inline int PCI_FIND_HTCAP(device_t dev, device_t child, int capability,
                                   int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_htcap);
	rc = ((pci_find_htcap_t *) _m)(dev, child, capability, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_FIND_NEXT_HTCAP() method */
extern struct kobjop_desc pci_find_next_htcap_desc;
/** @brief A function implementing the PCI_FIND_NEXT_HTCAP() method */
typedef int pci_find_next_htcap_t(device_t dev, device_t child, int capability,
                                  int start, int *capreg);

static __inline int PCI_FIND_NEXT_HTCAP(device_t dev, device_t child,
                                        int capability, int start, int *capreg)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_find_next_htcap);
	rc = ((pci_find_next_htcap_t *) _m)(dev, child, capability, start, capreg);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ALLOC_MSI() method */
extern struct kobjop_desc pci_alloc_msi_desc;
/** @brief A function implementing the PCI_ALLOC_MSI() method */
typedef int pci_alloc_msi_t(device_t dev, device_t child, int *count);

static __inline int PCI_ALLOC_MSI(device_t dev, device_t child, int *count)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_alloc_msi);
	rc = ((pci_alloc_msi_t *) _m)(dev, child, count);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ALLOC_MSIX() method */
extern struct kobjop_desc pci_alloc_msix_desc;
/** @brief A function implementing the PCI_ALLOC_MSIX() method */
typedef int pci_alloc_msix_t(device_t dev, device_t child, int *count);

static __inline int PCI_ALLOC_MSIX(device_t dev, device_t child, int *count)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_alloc_msix);
	rc = ((pci_alloc_msix_t *) _m)(dev, child, count);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ENABLE_MSI() method */
extern struct kobjop_desc pci_enable_msi_desc;
/** @brief A function implementing the PCI_ENABLE_MSI() method */
typedef void pci_enable_msi_t(device_t dev, device_t child, uint64_t address,
                              uint16_t data);

static __inline void PCI_ENABLE_MSI(device_t dev, device_t child,
                                    uint64_t address, uint16_t data)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_enable_msi);
	((pci_enable_msi_t *) _m)(dev, child, address, data);
}

/** @brief Unique descriptor for the PCI_ENABLE_MSIX() method */
extern struct kobjop_desc pci_enable_msix_desc;
/** @brief A function implementing the PCI_ENABLE_MSIX() method */
typedef void pci_enable_msix_t(device_t dev, device_t child, u_int index,
                               uint64_t address, uint32_t data);

static __inline void PCI_ENABLE_MSIX(device_t dev, device_t child, u_int index,
                                     uint64_t address, uint32_t data)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_enable_msix);
	((pci_enable_msix_t *) _m)(dev, child, index, address, data);
}

/** @brief Unique descriptor for the PCI_DISABLE_MSI() method */
extern struct kobjop_desc pci_disable_msi_desc;
/** @brief A function implementing the PCI_DISABLE_MSI() method */
typedef void pci_disable_msi_t(device_t dev, device_t child);

static __inline void PCI_DISABLE_MSI(device_t dev, device_t child)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_disable_msi);
	((pci_disable_msi_t *) _m)(dev, child);
}

/** @brief Unique descriptor for the PCI_REMAP_MSIX() method */
extern struct kobjop_desc pci_remap_msix_desc;
/** @brief A function implementing the PCI_REMAP_MSIX() method */
typedef int pci_remap_msix_t(device_t dev, device_t child, int count,
                             const u_int *vectors);

static __inline int PCI_REMAP_MSIX(device_t dev, device_t child, int count,
                                   const u_int *vectors)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_remap_msix);
	rc = ((pci_remap_msix_t *) _m)(dev, child, count, vectors);
	return (rc);
}

/** @brief Unique descriptor for the PCI_RELEASE_MSI() method */
extern struct kobjop_desc pci_release_msi_desc;
/** @brief A function implementing the PCI_RELEASE_MSI() method */
typedef int pci_release_msi_t(device_t dev, device_t child);

static __inline int PCI_RELEASE_MSI(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_release_msi);
	rc = ((pci_release_msi_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_MSI_COUNT() method */
extern struct kobjop_desc pci_msi_count_desc;
/** @brief A function implementing the PCI_MSI_COUNT() method */
typedef int pci_msi_count_t(device_t dev, device_t child);

static __inline int PCI_MSI_COUNT(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_msi_count);
	rc = ((pci_msi_count_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_MSIX_COUNT() method */
extern struct kobjop_desc pci_msix_count_desc;
/** @brief A function implementing the PCI_MSIX_COUNT() method */
typedef int pci_msix_count_t(device_t dev, device_t child);

static __inline int PCI_MSIX_COUNT(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_msix_count);
	rc = ((pci_msix_count_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_MSIX_PBA_BAR() method */
extern struct kobjop_desc pci_msix_pba_bar_desc;
/** @brief A function implementing the PCI_MSIX_PBA_BAR() method */
typedef int pci_msix_pba_bar_t(device_t dev, device_t child);

static __inline int PCI_MSIX_PBA_BAR(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_msix_pba_bar);
	rc = ((pci_msix_pba_bar_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_MSIX_TABLE_BAR() method */
extern struct kobjop_desc pci_msix_table_bar_desc;
/** @brief A function implementing the PCI_MSIX_TABLE_BAR() method */
typedef int pci_msix_table_bar_t(device_t dev, device_t child);

static __inline int PCI_MSIX_TABLE_BAR(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_msix_table_bar);
	rc = ((pci_msix_table_bar_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_GET_ID() method */
extern struct kobjop_desc pci_get_id_desc;
/** @brief A function implementing the PCI_GET_ID() method */
typedef int pci_get_id_t(device_t dev, device_t child, enum pci_id_type type,
                         uintptr_t *id);

static __inline int PCI_GET_ID(device_t dev, device_t child,
                               enum pci_id_type type, uintptr_t *id)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_get_id);
	rc = ((pci_get_id_t *) _m)(dev, child, type, id);
	return (rc);
}

/** @brief Unique descriptor for the PCI_ALLOC_DEVINFO() method */
extern struct kobjop_desc pci_alloc_devinfo_desc;
/** @brief A function implementing the PCI_ALLOC_DEVINFO() method */
typedef struct pci_devinfo * pci_alloc_devinfo_t(device_t dev);

static __inline struct pci_devinfo * PCI_ALLOC_DEVINFO(device_t dev)
{
	kobjop_t _m;
	struct pci_devinfo * rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_alloc_devinfo);
	rc = ((pci_alloc_devinfo_t *) _m)(dev);
	return (rc);
}

/** @brief Unique descriptor for the PCI_CHILD_ADDED() method */
extern struct kobjop_desc pci_child_added_desc;
/** @brief A function implementing the PCI_CHILD_ADDED() method */
typedef void pci_child_added_t(device_t dev, device_t child);

static __inline void PCI_CHILD_ADDED(device_t dev, device_t child)
{
	kobjop_t _m;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_child_added);
	((pci_child_added_t *) _m)(dev, child);
}

/** @brief Unique descriptor for the PCI_IOV_ATTACH() method */
extern struct kobjop_desc pci_iov_attach_desc;
/** @brief A function implementing the PCI_IOV_ATTACH() method */
typedef int pci_iov_attach_t(device_t dev, device_t child,
                             struct nvlist *pf_schema, struct nvlist *vf_schema,
                             const char *name);

static __inline int PCI_IOV_ATTACH(device_t dev, device_t child,
                                   struct nvlist *pf_schema,
                                   struct nvlist *vf_schema, const char *name)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_iov_attach);
	rc = ((pci_iov_attach_t *) _m)(dev, child, pf_schema, vf_schema, name);
	return (rc);
}

/** @brief Unique descriptor for the PCI_IOV_DETACH() method */
extern struct kobjop_desc pci_iov_detach_desc;
/** @brief A function implementing the PCI_IOV_DETACH() method */
typedef int pci_iov_detach_t(device_t dev, device_t child);

static __inline int PCI_IOV_DETACH(device_t dev, device_t child)
{
	kobjop_t _m;
	int rc;
	KOBJOPLOOKUP(((kobj_t)dev)->ops,pci_iov_detach);
	rc = ((pci_iov_detach_t *) _m)(dev, child);
	return (rc);
}

/** @brief Unique descriptor for the PCI_CREATE_IOV_CHILD() method */
extern struct kobjop_desc pci_create_iov_child_desc;
/** @brief A function implementing the PCI_CREATE_IOV_CHILD() method */
typedef device_t pci_create_iov_child_t(device_t bus, device_t pf, uint16_t rid,
                                        uint16_t vid, uint16_t did);

static __inline device_t PCI_CREATE_IOV_CHILD(device_t bus, device_t pf,
                                              uint16_t rid, uint16_t vid,
                                              uint16_t did)
{
	kobjop_t _m;
	device_t rc;
	KOBJOPLOOKUP(((kobj_t)bus)->ops,pci_create_iov_child);
	rc = ((pci_create_iov_child_t *) _m)(bus, pf, rid, vid, did);
	return (rc);
}

#endif /* _pci_if_h_ */

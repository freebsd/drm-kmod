/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * cec-notifier.h - notify CEC drivers of physical address changes
 *
 * Copyright 2016 Russell King <rmk+kernel@arm.linux.org.uk>
 * Copyright 2016-2017 Cisco Systems, Inc. and/or its affiliates. All rights reserved.
 */

#ifndef LINUX_GPLV2_CEC_NOTIFIER_H
#define LINUX_GPLV2_CEC_NOTIFIER_H

#include <linux/types.h>

#define	CEC_LOG_ADDR_INVALID		0xff
#define	CEC_PHYS_ADDR_INVALID		0xffff

struct device;
struct edid;
struct cec_adapter;
struct cec_notifier;

static inline struct cec_notifier *cec_notifier_get_conn(struct device *dev,
    const char *conn)
{
	/* A non-NULL pointer is expected on success */
	return (struct cec_notifier *)0xdeadfeed;
}

static inline void cec_notifier_put(struct cec_notifier *n)
{
}

static inline void cec_notifier_set_phys_addr(struct cec_notifier *n, u16 pa)
{
}

static inline void cec_notifier_set_phys_addr_from_edid(struct cec_notifier *n,
    const struct edid *edid)
{
}

static inline void cec_notifier_register(struct cec_notifier *n,
    struct cec_adapter *adap,
    void (*callback)(struct cec_adapter *adap, u16 pa))
{
}

static inline void cec_notifier_unregister(struct cec_notifier *n)
{
}

static inline void cec_register_cec_notifier(struct cec_adapter *adap,
    struct cec_notifier *notifier)
{
}

/**
 * cec_notifier_get - find or create a new cec_notifier for the given device.
 * @dev: device that sends the events.
 *
 * If a notifier for device @dev already exists, then increase the refcount
 * and return that notifier.
 *
 * If it doesn't exist, then allocate a new notifier struct and return a
 * pointer to that new struct.
 *
 * Return NULL if the memory could not be allocated.
 */
static inline struct cec_notifier *cec_notifier_get(struct device *dev)
{
	return cec_notifier_get_conn(dev, NULL);
}

/**
 * cec_notifier_phys_addr_invalidate() - set the physical address to INVALID
 *
 * @n: the CEC notifier
 *
 * This is a simple helper function to invalidate the physical
 * address. Does nothing if @n == NULL.
 */
static inline void cec_notifier_phys_addr_invalidate(struct cec_notifier *n)
{
	cec_notifier_set_phys_addr(n, CEC_PHYS_ADDR_INVALID);
}

#endif

/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * strncpy_from_user(), which base LinuxKPI lacks.  Only reachable via
 * the 3D context-init ioctl path, rejected at runtime by this port.
 */

#ifndef _VIRTGPU_SHIM_LINUX_UACCESS_H
#define	_VIRTGPU_SHIM_LINUX_UACCESS_H

#include_next <linux/uaccess.h>

/*
 * LinuxKPI has memdup_user (kmalloc-backed) but not the kvmalloc-backed
 * variant; the difference is irrelevant for this driver's usage.
 */
#define	vmemdup_user(src, len)	memdup_user((src), (len))

static inline long
strncpy_from_user(char *dst, const char __user *src, long count)
{
	size_t done;
	int error;

	if (count <= 0)
		return (0);
	error = copyinstr(src, dst, count, &done);
	if (error == 0)
		return ((long)done - 1);	/* done includes the NUL */
	if (error == ENAMETOOLONG)
		return (count);			/* truncated, as Linux */
	return (-EFAULT);
}

#endif /* _VIRTGPU_SHIM_LINUX_UACCESS_H */

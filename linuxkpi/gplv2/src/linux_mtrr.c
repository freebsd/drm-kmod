/*-
 * Copyright (c) 2016 Matt Macy <mmacy@nextbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/memrange.h>

#include <asm/mtrr.h>

#include <linux/kernel.h>
#include <linux/idr.h>

/*
 * Check that there is a SYSUNINIT for this
 */
DEFINE_IDR(mtrr_idr);
struct mtrr_info {
	unsigned long base;
	unsigned long size;
};

static MALLOC_DEFINE(M_LKMTRR, "idr", "Linux MTRR compat");

static int
mtrr_add(unsigned long offset, unsigned long size, int flags)
{
	int act, bsdflags, rc;
	struct mem_range_desc mrdesc;

	bsdflags = 0;
	/* assert that we receive only flags we know about */
	MPASS((flags & ~MTRR_TYPE_WRCOMB) == 0);

	if (flags & MTRR_TYPE_WRCOMB)
		bsdflags |= MDF_WRITECOMBINE;

	mrdesc.mr_base = offset;
	mrdesc.mr_len = size;
	mrdesc.mr_flags = bsdflags;
	act = MEMRANGE_SET_UPDATE;
	strlcpy(mrdesc.mr_owner, "drm", sizeof(mrdesc.mr_owner));
	rc = mem_range_attr_set(&mrdesc, &act);

	/* there's no way to get the actual register without churning the interface */
	return (rc ? -rc : 0);
}

static int
mtrr_del(unsigned long offset, unsigned long size)
{
	int act;
	struct mem_range_desc mrdesc;

	mrdesc.mr_base = offset;
	mrdesc.mr_len = size;
	mrdesc.mr_flags = 0;
	act = MEMRANGE_SET_REMOVE;
	strlcpy(mrdesc.mr_owner, "drm", sizeof(mrdesc.mr_owner));
	return (-mem_range_attr_set(&mrdesc, &act));
}

int
arch_phys_wc_add(unsigned long base, unsigned long size)
{
	int rc, rc2 __unused, id;
	struct mtrr_info *mi;

	mi = malloc(sizeof(*mi), M_LKMTRR, M_WAITOK);
	mi->base = base;
	mi->size = size;
	rc = mtrr_add(base, size, MTRR_TYPE_WRCOMB);
	if (rc >= 0) {
		pr_warn("Successfully added WC MTRR for [%p-%p]: %d; ",
		    (void *)base, (void *)(base + size - 1), rc);
		rc2 = idr_get_new(&mtrr_idr, mi, &id);
		MPASS(idr_find(&mtrr_idr, id) == mi);
		MPASS(rc2 == 0);
	} else {
		free(mi, M_LKMTRR);
		pr_warn(
		    "Failed to add WC MTRR for [%p-%p]: %d; "
		    "performance may suffer",
		    (void *)base, (void *)(base + size - 1), rc);
	}

	return (rc != 0 ? rc : id);
}

void
arch_phys_wc_del(int reg)
{
	struct mtrr_info *mi;

	if (reg < 1) {
		/* arch_phys_wc_add() failed. */
		return;
	}

	mi = idr_find(&mtrr_idr, reg);
	MPASS(mi != NULL);
	idr_remove(&mtrr_idr, reg);
	mtrr_del(mi->base, mi->size);
	free(mi, M_LKMTRR);
}

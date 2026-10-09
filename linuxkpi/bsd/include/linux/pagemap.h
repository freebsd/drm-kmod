/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _BSD_LKPI_LINUX_PAGEMAP_H_
#define	_BSD_LKPI_LINUX_PAGEMAP_H_

#include_next <linux/pagemap.h>

/*
 * Missing from the base system's LinuxKPI.  Its shmem objects take no
 * allocation flags and keep their pages wired, and a folio is one page.
 */
static inline gfp_t
mapping_gfp_mask(vm_object_t mapping)
{
	return (0);
}

static inline gfp_t
mapping_gfp_constraint(vm_object_t mapping, gfp_t gfp_mask)
{
	return (mapping_gfp_mask(mapping) & gfp_mask);
}

static inline void
mapping_set_unevictable(vm_object_t mapping)
{
}

static inline struct page *
folio_file_page(struct folio *folio, pgoff_t index)
{
	return (&folio->page);
}

#endif /* _BSD_LKPI_LINUX_PAGEMAP_H_ */

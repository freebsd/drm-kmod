/*-
 * Copyright (c) 2019 Johannes Lundberg <johalun@FreeBSD.org>
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

#include <linux/module.h>

#include "dummygfx_drv.h"

static int __init dummygfx_init(void)
{
	int ret;

	dummygfx_debugfs_init();
	ret = 0;
	return ret;
}

static void __exit dummygfx_exit(void)
{

	dummygfx_debugfs_exit();
}

LKPI_DRIVER_MODULE(dummygfx, dummygfx_init, dummygfx_exit);
/* LKPI_PNP_INFO(pci, dummygfx, dummy_pci_id_list); */
MODULE_DEPEND(dummygfx, drmn, 2, 2, 2);
MODULE_DEPEND(dummygfx, ttm, 1, 1, 1);
MODULE_DEPEND(dummygfx, agp, 1, 1, 1);
MODULE_DEPEND(dummygfx, linuxkpi, 1, 1, 1);
#ifdef CONFIG_DEBUG_FS
MODULE_DEPEND(dummygfx, lindebugfs, 1, 1, 1);
#endif

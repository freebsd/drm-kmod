/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2013 The FreeBSD Foundation
 * Copyright (c) 2023 Jean-Sébastien Pédron <dumbbell@FreeBSD.org>
 *
 * This initial software `sys/dev/vt/hw/vt_fb.h` was developed by Aleksandr
 * Rybalko under sponsorship from the FreeBSD Foundation.
 * This file is a copy of the initial file and is modified by Jean-Sébastien
 * Pédron.
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
 *
 * $FreeBSD$
 */

#ifndef _DEV_VT_HW_FB_VT_DRMFB_H_
#define	_DEV_VT_HW_FB_VT_DRMFB_H_
/* Generic framebuffer interface call vt_drmfb_attach to init VT(9) */
int vt_drmfb_attach(struct fb_info *info);
void vt_drmfb_resume(struct vt_device *vd);
void vt_drmfb_suspend(struct vt_device *vd);
int vt_drmfb_detach(struct fb_info *info);

#endif /* _DEV_VT_HW_FB_VT_DRMFB_H_ */

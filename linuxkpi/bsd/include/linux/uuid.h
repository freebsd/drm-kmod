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

/*
 * Interim: uuid_t and its helpers are under review for base LinuxKPI, see
 * https://reviews.freebsd.org/D60199; drop this file once it lands.
 */

#ifndef _BSD_LKPI_LINUX_UUID_H_
#define	_BSD_LKPI_LINUX_UUID_H_

#include_next <linux/uuid.h>

/* Renamed to stay clear of the other uuid_t types in the tree. */
typedef struct {
	uint8_t	b[UUID_SIZE];
} linux_uuid_t;
#define	uuid_t	linux_uuid_t

static inline bool
uuid_equal(const uuid_t *u1, const uuid_t *u2)
{
	return (memcmp(u1, u2, sizeof(uuid_t)) == 0);
}

static inline void
uuid_copy(uuid_t *dst, const uuid_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline void
import_uuid(uuid_t *dst, const uint8_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline void
export_uuid(uint8_t *dst, const uuid_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

#endif /* _BSD_LKPI_LINUX_UUID_H_ */

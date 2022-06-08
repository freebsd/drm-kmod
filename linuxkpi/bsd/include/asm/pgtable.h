/*-
 * Copyright (c) 2022 Beckhoff Automation GmbH & Co. KG
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
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _BSD_LKPI_ASM_PGTABLE_H_
#define	_BSD_LKPI_ASM_PGTABLE_H_

#include_next <asm/pgtable.h>

#ifndef _PAGE_BIT_PRESENT
#define	_PAGE_BIT_PRESENT	0
#endif
#ifndef _PAGE_BIT_RW
#define	_PAGE_BIT_RW		1
#endif
#ifndef _PAGE_BIT_USER
#define	_PAGE_BIT_USER		2
#endif
#ifndef _PAGE_BIT_PWT
#define	_PAGE_BIT_PWT		3
#endif
#ifndef _PAGE_BIT_PCD
#define	_PAGE_BIT_PCD		4
#endif
#ifndef _PAGE_BIT_PAT
#define	_PAGE_BIT_PAT		7
#endif

#ifndef _PAGE_PRESENT
#define	_PAGE_PRESENT	(((pteval_t) 1) << _PAGE_BIT_PRESENT)
#endif
#ifndef _PAGE_RW
#define	_PAGE_RW	(((pteval_t) 1) << _PAGE_BIT_RW)
#endif
#ifndef _PAGE_PWT
#define	_PAGE_PWT	(((pteval_t) 1) << _PAGE_BIT_PWT)
#endif
#ifndef _PAGE_PCD
#define	_PAGE_PCD	(((pteval_t) 1) << _PAGE_BIT_PCD)
#endif
#ifndef _PAGE_PAT
#define	_PAGE_PAT	(((pteval_t) 1) << _PAGE_BIT_PAT)
#endif

#endif	/* _BSD_LKPI_ASM_PGTABLE_H_ */

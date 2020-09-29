/*-
 * Copyright (c) 2016 Matthew Macy
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
 *
 * $FreeBSD$
 */

#ifndef	_LINUX_GPLV2_SCATTERLIST_H_
#define	_LINUX_GPLV2_SCATTERLIST_H_

#include_next <linux/scatterlist.h>

#if !defined(LINUXKPI_COOKIE) || (LINUXKPI_COOKIE < 1600256818)
static inline size_t
sg_pcopy_from_buffer(struct scatterlist *sgl, unsigned int nents,
    const void *buf, size_t buflen, off_t offset)
{
	struct sg_page_iter iter;
	struct scatterlist *sg;
	struct page *page;
	void *vaddr;
	size_t total = 0;
	size_t len;

	for_each_sg_page(sgl, &iter, nents, 0) {
		sg = iter.sg;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		}
		len = min(buflen, sg->length - offset);
		if (len == 0)
			break;

		page = sg_page_iter_page(&iter);
		vaddr = ((caddr_t)kmap(page)) + sg->offset + offset;
		memcpy(vaddr, buf, len);
		kunmap(page);

		/* start at beginning of next page */
		offset = 0;

		/* advance buffer */
		buf = (const char *)buf + len;
		buflen -= len;
		total += len;
	}
	return (total);
}
#endif

static inline size_t
sg_copy_from_buffer(struct scatterlist *sgl, unsigned int nents,
    const void *buf, size_t buflen)
{
	return (sg_pcopy_from_buffer(sgl, nents, buf, buflen, 0));
}

static inline size_t
sg_pcopy_to_buffer(struct scatterlist *sgl, unsigned int nents,
    void *buf, size_t buflen, off_t offset)
{
	struct sg_page_iter iter;
	struct scatterlist *sg;
	struct page *page;
	void *vaddr;
	size_t total = 0;
	size_t len;

	for_each_sg_page(sgl, &iter, nents, 0) {
		sg = iter.sg;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		}
		len = min(buflen, sg->length - offset);
		if (len == 0)
			break;

		page = sg_page_iter_page(&iter);
		vaddr = ((caddr_t)kmap(page)) + sg->offset + offset;
		memcpy(buf, vaddr, len);
		kunmap(page);

		/* start at beginning of next page */
		offset = 0;

		/* advance buffer */
		buf = (char *)buf + len;
		buflen -= len;
		total += len;
	}
	return (total);
}

#endif					/* _LINUX_GPLV2_SCATTERLIST_H_ */

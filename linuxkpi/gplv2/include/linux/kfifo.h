/*
 * A generic kernel FIFO implementation
 *
 * Copyright (C) 2013 Stefani Seibold <stefani@seibold.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _LINUX_KFIFO_H
#define _LINUX_KFIFO_H
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/scatterlist.h>

#include <asm/processor.h>

struct __kfifo {
	unsigned int	in;
	unsigned int	out;
	unsigned int	mask;
	unsigned int	esize;
	void		*data;
};

#define __STRUCT_KFIFO_COMMON(datatype, recsize, ptrtype) \
	union { \
		struct __kfifo	kfifo; \
		datatype	*type; \
		const datatype	*const_type; \
		char		(*rectype)[recsize]; \
		ptrtype		*ptr; \
		ptrtype const	*ptr_const; \
	}

#define __STRUCT_KFIFO(type, size, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[((size < 2) || (size & (size - 1))) ? -1 : size]; \
}

#define STRUCT_KFIFO(type, size) \
	struct __STRUCT_KFIFO(type, size, 0, type)

#define __STRUCT_KFIFO_PTR(type, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[0]; \
}

#define STRUCT_KFIFO_PTR(type) \
	struct __STRUCT_KFIFO_PTR(type, 0, type)

/*
 * define compatibility "struct kfifo" for dynamic allocated fifos
 */
struct kfifo __STRUCT_KFIFO_PTR(unsigned char, 0, void);

#define STRUCT_KFIFO_REC_1(size) \
	struct __STRUCT_KFIFO(unsigned char, size, 1, void)

#define STRUCT_KFIFO_REC_2(size) \
	struct __STRUCT_KFIFO(unsigned char, size, 2, void)

struct kfifo_rec_ptr_1 __STRUCT_KFIFO_PTR(unsigned char, 1, void);
struct kfifo_rec_ptr_2 __STRUCT_KFIFO_PTR(unsigned char, 2, void);

#define	__is_kfifo_ptr(fifo)	(sizeof(*fifo) == sizeof(struct __kfifo))
#define DECLARE_KFIFO_PTR(fifo, type)	STRUCT_KFIFO_PTR(type) fifo
#define DECLARE_KFIFO(fifo, type, size)	STRUCT_KFIFO(type, size) fifo

#define INIT_KFIFO(fifo) \
(void)({ \
	typeof(&(fifo)) __tmp = &(fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__kfifo->in = 0; \
	__kfifo->out = 0; \
	__kfifo->mask = __is_kfifo_ptr(__tmp) ? 0 : ARRAY_SIZE(__tmp->buf) - 1;\
	__kfifo->esize = sizeof(*__tmp->buf); \
	__kfifo->data = __is_kfifo_ptr(__tmp) ?  NULL : __tmp->buf; \
})
#define DEFINE_KFIFO(fifo, type, size) \
	DECLARE_KFIFO(fifo, type, size) = \
	(typeof(fifo)) { \
		{ \
			{ \
			.in	= 0, \
			.out	= 0, \
			.mask	= __is_kfifo_ptr(&(fifo)) ? \
				  0 : \
				  ARRAY_SIZE((fifo).buf) - 1, \
			.esize	= sizeof(*(fifo).buf), \
			.data	= __is_kfifo_ptr(&(fifo)) ? \
				NULL : \
				(fifo).buf, \
			} \
		} \
	}


static inline unsigned int __must_check
__kfifo_uint_must_check_helper(unsigned int val)
{
	return val;
}

static inline int __must_check
__kfifo_int_must_check_helper(int val)
{
	return val;
}

#define kfifo_initialized(fifo) ((fifo)->kfifo.mask)
#define kfifo_esize(fifo)	((fifo)->kfifo.esize)
#define kfifo_recsize(fifo)	(sizeof(*(fifo)->rectype))
#define kfifo_size(fifo)	((fifo)->kfifo.mask + 1)

#define kfifo_reset(fifo) \
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	__tmp->kfifo.in = __tmp->kfifo.out = 0; \
})

#define kfifo_reset_out(fifo)	\
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	__tmp->kfifo.out = __tmp->kfifo.in; \
})

#define kfifo_len(fifo) \
({ \
	typeof((fifo) + 1) __tmpl = (fifo); \
	__tmpl->kfifo.in - __tmpl->kfifo.out; \
})

#define	kfifo_is_empty(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	__tmpq->kfifo.in == __tmpq->kfifo.out; \
})

#define	kfifo_is_full(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	kfifo_len(__tmpq) > __tmpq->kfifo.mask; \
})

#define	kfifo_avail(fifo) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	const size_t __recsize = sizeof(*__tmpq->rectype); \
	unsigned int __avail = kfifo_size(__tmpq) - kfifo_len(__tmpq); \
	(__recsize) ? ((__avail <= __recsize) ? 0 : \
	__kfifo_max_r(__avail - __recsize, __recsize)) : \
	__avail; \
}) \
)


#define kfifo_alloc(fifo, size, gfp_mask) \
__kfifo_int_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__is_kfifo_ptr(__tmp) ? \
	linux_kfifo_alloc(__kfifo, size, sizeof(*__tmp->type), gfp_mask) : \
	-EINVAL; \
}) \
)

#define kfifo_free(fifo) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__is_kfifo_ptr(__tmp)) \
		linux_kfifo_free(__kfifo); \
})

#define kfifo_init(fifo, buffer, size) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__is_kfifo_ptr(__tmp) ? \
	__kfifo_init(__kfifo, buffer, size, sizeof(*__tmp->type)) : \
	-EINVAL; \
})

#define	kfifo_put(fifo, val)			\
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(*__tmp->const_type) __val = (val); \
	unsigned int __ret; \
	size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_in_r(__kfifo, &__val, sizeof(__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_full(__tmp); \
		if (__ret) { \
			(__is_kfifo_ptr(__tmp) ? \
			((typeof(__tmp->type))__kfifo->data) : \
			(__tmp->buf) \
			)[__kfifo->in & __tmp->kfifo.mask] = \
				*(typeof(__tmp->type))&__val; \
			smp_wmb(); \
			__kfifo->in++; \
		} \
	} \
	__ret; \
})


#define	kfifo_get(fifo, val) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __val = (val); \
	unsigned int __ret; \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_out_r(__kfifo, __val, sizeof(*__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_empty(__tmp); \
		if (__ret) { \
			*(typeof(__tmp->type))__val = \
				(__is_kfifo_ptr(__tmp) ? \
				((typeof(__tmp->type))__kfifo->data) : \
				(__tmp->buf) \
				)[__kfifo->out & __tmp->kfifo.mask]; \
			smp_wmb(); \
			__kfifo->out++; \
		} \
	} \
	__ret; \
}) \
)

#define	kfifo_peek(fifo, val) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __val = (val); \
	unsigned int __ret; \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_out_peek_r(__kfifo, __val, sizeof(*__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_empty(__tmp); \
		if (__ret) { \
			*(typeof(__tmp->type))__val = \
				(__is_kfifo_ptr(__tmp) ? \
				((typeof(__tmp->type))__kfifo->data) : \
				(__tmp->buf) \
				)[__kfifo->out & __tmp->kfifo.mask]; \
			smp_wmb(); \
		} \
	} \
	__ret; \
}) \
)

#define	kfifo_in(fifo, buf, n)			\
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr_const) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ?\
	__kfifo_in_r(__kfifo, __buf, __n, __recsize) : \
	linux_kfifo_in(__kfifo, __buf, __n); \
})

#define	kfifo_in_spinlocked(fifo, buf, n, lock) \
({ \
	unsigned long __flags; \
	unsigned int __ret; \
	spin_lock_irqsave(lock, __flags); \
	__ret = kfifo_in(fifo, buf, n); \
	spin_unlock_irqrestore(lock, __flags); \
	__ret; \
})

#define kfifo_in_locked(fifo, buf, n, lock) \
		kfifo_in_spinlocked(fifo, buf, n, lock)

#define	kfifo_out(fifo, buf, n) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ?\
	__kfifo_out_r(__kfifo, __buf, __n, __recsize) : \
	linux_kfifo_out(__kfifo, __buf, __n); \
}) \
)

#define	kfifo_out_spinlocked(fifo, buf, n, lock) \
__kfifo_uint_must_check_helper( \
({ \
	unsigned long __flags; \
	unsigned int __ret; \
	spin_lock_irqsave(lock, __flags); \
	__ret = kfifo_out(fifo, buf, n); \
	spin_unlock_irqrestore(lock, __flags); \
	__ret; \
}) \
)

#define kfifo_out_locked(fifo, buf, n, lock) \
		kfifo_out_spinlocked(fifo, buf, n, lock)

#define	kfifo_from_user(fifo, from, len, copied) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	const void __user *__from = (from); \
	unsigned int __len = (len); \
	unsigned int *__copied = (copied); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_from_user_r(__kfifo, __from, __len,  __copied, __recsize) : \
	__kfifo_from_user(__kfifo, __from, __len, __copied); \
}) \
)

#define	kfifo_out_peek(fifo, buf, n) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_out_peek_r(__kfifo, __buf, __n, __recsize) : \
	linux_kfifo_out_peek(__kfifo, __buf, __n); \
}) \
)


static inline unsigned int
kfifo_unused(struct __kfifo *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

static inline int
linux_kfifo_alloc(struct __kfifo *fifo, unsigned int size,
		size_t esize, gfp_t gfp_mask)
{
	/*
	 * round down to the next power of 2, since our 'let the indices
	 * wrap' technique works only in this case.
	 */
	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2) {
		fifo->data = NULL;
		fifo->mask = 0;
		return -EINVAL;
	}

	fifo->data = kmalloc(size * esize, gfp_mask);

	if (!fifo->data) {
		fifo->mask = 0;
		return -ENOMEM;
	}
	fifo->mask = size - 1;

	return 0;
}

static inline void
linux_kfifo_free(struct __kfifo *fifo)
{
	kfree(fifo->data);
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = 0;
	fifo->data = NULL;
	fifo->mask = 0;
}

static inline void
kfifo_copy_in(struct __kfifo *fifo, const void *src,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(fifo->data + off, src, l);
	memcpy(fifo->data, src + l, len - l);
	/*
	 * make sure that the data in the fifo is up to date before
	 * incrementing the fifo->in index counter
	 */
	smp_wmb();
}

static inline unsigned int
linux_kfifo_in(struct __kfifo *fifo,
		const void *buf, unsigned int len)
{
	unsigned int l;

	l = kfifo_unused(fifo);
	if (len > l)
		len = l;

	kfifo_copy_in(fifo, buf, len, fifo->in);
	fifo->in += len;
	return len;
}

static inline void
kfifo_copy_out(struct __kfifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(dst, fifo->data + off, l);
	memcpy(dst + l, fifo->data, len - l);
	/*
	 * make sure that the data is copied before
	 * incrementing the fifo->out index counter
	 */
	smp_wmb();
}

static inline unsigned int
linux_kfifo_out_peek(struct __kfifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	kfifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}

static inline unsigned int
linux_kfifo_out(struct __kfifo *fifo,
		void *buf, unsigned int len)
{
	len = linux_kfifo_out_peek(fifo, buf, len);
	fifo->out += len;
	return len;
}

extern int __kfifo_init(struct __kfifo *fifo, void *buffer,
	unsigned int size, size_t esize);

extern int __kfifo_from_user(struct __kfifo *fifo,
	const void __user *from, unsigned long len, unsigned int *copied);

extern int __kfifo_to_user(struct __kfifo *fifo,
	void __user *to, unsigned long len, unsigned int *copied);

extern unsigned int __kfifo_dma_in_prepare(struct __kfifo *fifo,
	struct scatterlist *sgl, int nents, unsigned int len);

extern unsigned int __kfifo_dma_out_prepare(struct __kfifo *fifo,
	struct scatterlist *sgl, int nents, unsigned int len);


extern unsigned int __kfifo_in_r(struct __kfifo *fifo,
	const void *buf, unsigned int len, size_t recsize);

extern unsigned int __kfifo_out_r(struct __kfifo *fifo,
	void *buf, unsigned int len, size_t recsize);

extern int __kfifo_from_user_r(struct __kfifo *fifo,
	const void __user *from, unsigned long len, unsigned int *copied,
	size_t recsize);

extern int __kfifo_to_user_r(struct __kfifo *fifo, void __user *to,
	unsigned long len, unsigned int *copied, size_t recsize);

extern unsigned int __kfifo_dma_in_prepare_r(struct __kfifo *fifo,
	struct scatterlist *sgl, int nents, unsigned int len, size_t recsize);

extern void __kfifo_dma_in_finish_r(struct __kfifo *fifo,
	unsigned int len, size_t recsize);

extern unsigned int __kfifo_dma_out_prepare_r(struct __kfifo *fifo,
	struct scatterlist *sgl, int nents, unsigned int len, size_t recsize);

extern void __kfifo_dma_out_finish_r(struct __kfifo *fifo, size_t recsize);

extern unsigned int __kfifo_len_r(struct __kfifo *fifo, size_t recsize);

extern void __kfifo_skip_r(struct __kfifo *fifo, size_t recsize);

extern unsigned int __kfifo_out_peek_r(struct __kfifo *fifo,
	void *buf, unsigned int len, size_t recsize);

extern unsigned int __kfifo_max_r(unsigned int len, size_t recsize);

#endif

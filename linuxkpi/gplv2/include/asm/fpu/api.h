#ifndef _ASM_X86_FPU_API_H
#define _ASM_X86_FPU_API_H

#if defined(__i386__)
#include <machine/npx.h>
#else
#include <machine/fpu.h>
#endif

static struct fpu_kern_ctx *__fpu_ctx;
static unsigned int __fpu_ctx_level = 0;

#define	kernel_fpu_begin()				\
	if (__fpu_ctx_level++ == 0) {			\
		__fpu_ctx = fpu_kern_alloc_ctx(0);	\
		fpu_kern_enter(curthread, __fpu_ctx,	\
		    FPU_KERN_NORMAL);			\
	}

#define	kernel_fpu_end()				\
	if (--__fpu_ctx_level == 0) {			\
		fpu_kern_leave(curthread, __fpu_ctx);	\
		fpu_kern_free_ctx(__fpu_ctx);		\
	}

#endif /* _ASM_X86_FPU_API_H */

#ifndef _ASM_X86_FPU_API_H
#define _ASM_X86_FPU_API_H

#if defined(__i386__)

/*
 * Allow build on i386. Use of these functions by i915
 * is disabled since CONFIG_AS_MOVNTDQA is amd64 only.
 * Other users are amdgpu, assume no one is using amdgpu
 * driver on 32bit hardware...
 */
#define	kernel_fpu_begin()
#define	kernel_fpu_end()

#else

#include <machine/fpu.h>

#define	kernel_fpu_begin()			\
	struct fpu_kern_ctx *__fpu_ctx;		\
	__fpu_ctx = fpu_kern_alloc_ctx(0);	\
	fpu_kern_enter(curthread, __fpu_ctx,	\
	    FPU_KERN_NORMAL);

#define	kernel_fpu_end()			\
	fpu_kern_leave(curthread, __fpu_ctx);	\
	fpu_kern_free_ctx(__fpu_ctx);
#endif

#endif /* _ASM_X86_FPU_API_H */

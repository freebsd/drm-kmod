# $FreeBSD$

SUBDIR=	linuxkpi	\
	ttm		\
	drm		\
	dummygfx	\
	${_vmwgfx}	\
	${_i915}	\
	amd		\
	radeon

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386"
_i915 =		i915 
_vmwgfx =	vmwgfx
.endif

.include <bsd.subdir.mk>

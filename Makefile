# $FreeBSD$

SUBDIR=	linuxkpi	\
	lindebugfs	\
	drm		\
	${_vmwgfx}	\
	${_i915}	\
	amd		\
	radeon		\
	${_staging}	

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386"
_i915 =		i915 
_vmwgfx =	vmwgfx
.endif

.if defined(STAGING)
_staging=	staging
.endif


.include <bsd.subdir.mk>

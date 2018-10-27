# $FreeBSD$

SUBDIR=	linuxkpi	\
	lindebugfs	\
	drm		\
	vmwgfx		\
	i915		\
	amd		\
	radeon		\
	${_staging}	


.if defined(STAGING)
_staging=	staging
.endif

.include <bsd.subdir.mk>

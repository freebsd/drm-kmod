# $FreeBSD$

SUBDIR=	${_vc4}		\
	${_vboxvideo}


.if ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_CPUARCH} == "arm"
_vc4=	vc4
.endif

.if 1 # Valid targets/archs?
_vboxvideo= vboxvideo
.endif

.include <bsd.subdir.mk>

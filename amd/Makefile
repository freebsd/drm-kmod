# $FreeBSD$

.if ${MACHINE_CPUARCH} == "amd64"
_amdgpu=	amdgpu
_amdkfd=	amdkfd
.endif

SUBDIR=	${_amdgpu} \
	${_amdkfd}

.include <bsd.subdir.mk>

# $FreeBSD$

.if ${MACHINE_CPUARCH} == "amd64"
_amdgpu=	amdgpu
_amdkfd=	amdkfd
.endif

.if ${MACHINE_ARCH} == "powerpc64"
_amdgpu=	amdgpu
.endif

SUBDIR=	${_amdgpu} \
	${_amdkfd}

.include <bsd.subdir.mk>

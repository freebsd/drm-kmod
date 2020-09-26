# $FreeBSD$

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_ARCH} == "powerpc64" || ${MACHINE_ARCH} == "powerpc64le"
_amdgpu=	amdgpu
.endif

.if ${MACHINE_CPUARCH} == "amd64"
# _amdkfd=	amdkfd
.endif

SUBDIR=	${_amdgpu} \
	${_amdkfd}

.include <bsd.subdir.mk>

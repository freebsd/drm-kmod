# $FreeBSD$

AMDGPU_ARCH=	amd64 \
		aarch64 \
		powerpc64 \
		powerpc64le \
		riscv64 \
		riscv64sf

.if !empty(AMDGPU_ARCH:M${MACHINE_ARCH})
_amdgpu=	amdgpu
.endif

.if ${MACHINE_CPUARCH} == "amd64"
# _amdkfd=	amdkfd
.endif

SUBDIR=	${_amdgpu} \
	${_amdkfd}

.include <bsd.subdir.mk>

# $FreeBSD$

SYSDIR?=/usr/src/sys
.include "${SYSDIR}/conf/kern.opts.mk"

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_ARCH} == "powerpc64"

SUBDIR=	linuxkpi	\
	ttm		\
	drm		\
	${_dummygfx}	\
	${_vboxvideo}	\
	${_vmwgfx}	\
	${_i915}	\
	amd		\
	radeon

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386"
_i915 =		i915 
_vmwgfx =	vmwgfx
_vboxvideo =	vboxvideo
.endif

.if defined(DUMMYGFX)
_dummygfx = dummygfx
.endif

# Calling kldxref(8) for each module is expensive.
.if !defined(NO_XREF)
.MAKEFLAGS+=	-DNO_XREF
afterinstall: .PHONY
	@if type kldxref >/dev/null 2>&1; then \
		${ECHO} ${KLDXREF_CMD} ${DESTDIR}${KMODDIR}; \
		${KLDXREF_CMD} ${DESTDIR}${KMODDIR}; \
	fi
.endif

.include <bsd.subdir.mk>

.else
dummy:
	echo "Unsupported architecture"
.endif

# $FreeBSD$

SYSDIR?=${SRCTOP}/sys
.include "${SYSDIR}/conf/kern.opts.mk"

.if ${MACHINE_CPUARCH} == "amd64"
_i915kms=	i915
_radeonkms=	radeonkms
. if ${MK_SOURCELESS_UCODE} != "no"
_amdgpukmsfw=	amdgpukmsfw
_radeonkmsfw=	radeonkmsfw
. endif
.endif

.if ${MACHINE_CPUARCH} == "i386"
. if ${MACHINE} != "pc98"
_i915kms=	i915
_radeonkms=	radeonkms
.  if ${MK_SOURCELESS_UCODE} != "no"
_amdgpukmsfw=	amdgpukmsfw
_radeonkmsfw=	radeonkmsfw
.  endif
. endif
.endif

.if ${MACHINE_CPUARCH} == "powerpc"
_radeonkms=	radeonkms
. if ${MK_SOURCELESS_UCODE} != "no"
_radeonkmsfw=	radeonkmsfw
. endif
.endif

SUBDIR = \
	drm \
	amd \
	${_i915kms} \
	${_amdgpukmsfw} \
	${_radeonkmsfw} \
	${_radeonkms}

# Special target that causes drm2 code to printf error returns.  Useful for
# determining the root cause of errors.  Depends on drmP.h macro DRM_ERR_RET.
# After running this target, set the ENABLE_DRM_ERR_RET make variable.
# A few files don't support this mechanism, so just exclude them.
#
# WARNING: THIS WILL CHANGE THE SOURCE TREE!  ONLY USE IF THERE ARE NO OTHER
#          CHANGES THAT NEED TO BE RECOVERED!
#
DRM_DEBUG_EXCLUDES=	drm_mipi_dsi|drm_panel|linux_hdmi|ttm/ttm_object
drm2-debug-source-changes:
	find ${SYSDIR}/dev/drm -name '*.[ch]' | \
		egrep -v '(${DRM_DEBUG_EXCLUDES}).c' | \
		xargs -t perl -pi -e 's,return (-E[A-Z]*);,DRM_ERR_RET(\1);,g'

.include <bsd.subdir.mk>

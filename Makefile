# $FreeBSD$

SUBDIR=	linuxkpi	\
	lindebugfs	\
	ttm		\
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


# Special target that causes drm2 code to printf error returns.  Useful for
# determining the root cause of errors.  Depends on drmP.h macro DRM_ERR_RET.
# After running this target, set the ENABLE_DRM_ERR_RET make variable.
# A few files don't support this mechanism, so just exclude them.
#
# WARNING: THIS WILL CHANGE THE SOURCE TREE!  ONLY USE IF THERE ARE NO OTHER
#          CHANGES THAT NEED TO BE RECOVERED!
#
.if make(drm2-debug-source-changes)
SYSDIR?=	/usr/src/sys
DRM_DEBUG_EXCLUDES=	drm_mipi_dsi|drm_panel|linux_hdmi|ttm/ttm_object
.endif
drm2-debug-source-changes:
	find ${SYSDIR}/dev/drm -name '*.[ch]' | \
		egrep -v '(${DRM_DEBUG_EXCLUDES}).c' | \
		xargs -t perl -pi -e 's,return (-E[A-Z]*);,DRM_ERR_RET(\1);,g'

.include <bsd.subdir.mk>

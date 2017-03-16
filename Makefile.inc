# $FreeBSD$

# See Makefile:drm2-debug-source-changes target for details.
.if defined(ENABLE_DRM_ERR_RET)
CFLAGS+=	-DENABLE_DRM_ERR_RET
.endif

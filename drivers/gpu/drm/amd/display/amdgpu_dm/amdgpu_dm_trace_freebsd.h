#ifndef _AMDGPU_DM_TRACE_FREEBSD_H_
#define _AMDGPU_DM_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>
#include <drm/drm_connector.h>
#include <drm/drm_crtc.h>
#include <drm/drm_plane.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_encoder.h>
#include <drm/drm_atomic.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_os_freebsd.h>	/* KTR_DRM */
#include "dcn10/dcn10_optc.h"

#include "dc/inc/core_types.h"

/* TRACE_EVENT(amdgpu_dc_rreg, */
/* 	TP_PROTO(unsigned long *read_count, uint32_t reg, uint32_t value), */
static inline void
trace_amdgpu_dc_rreg(unsigned long *read_count, uint32_t reg, uint32_t value)
{
	CTR3(KTR_DRM, "amdgpu_dc_rreg %p %d %d", read_count, reg, value);
}

/* TRACE_EVENT(amdgpu_dc_wreg, */
	/* TP_PROTO(unsigned long *write_count, uint32_t reg, uint32_t value), */
static inline void
trace_amdgpu_dc_wreg(unsigned long *write_count, uint32_t reg, uint32_t value)
{
	CTR3(KTR_DRM, "amdgpu_dc_wreg %p %d %d", write_count, reg, value);
}

/* TRACE_EVENT(amdgpu_dc_performance, */
/* 	TP_PROTO(unsigned long read_count, unsigned long write_count, */
/* 		unsigned long *last_read, unsigned long *last_write, */
/* 		const char *func, unsigned int line), */

static inline void
trace_amdgpu_dc_performance(unsigned long read_count, unsigned long write_count,
    unsigned long *last_read, unsigned long *last_write, const char *func, unsigned int line)
{
	CTR6(KTR_DRM, "amdgpu_dc_performance %u %u %p %p %s %u", read_count,
	    write_count, last_read, last_write, func, line);
}

/* TRACE_EVENT(amdgpu_dm_connector_atomic_check, */
/* 	    TP_PROTO(const struct drm_connector_state *state), */

static inline void
trace_amdgpu_dm_connector_atomic_check(const struct drm_connector_state *state)
{
#ifdef KTR
	uint32_t conn_id;
	const struct drm_connector_state * conn_state;
	const struct drm_atomic_state * state_state;
	const struct drm_crtc_commit * commit;
	uint32_t crtc_id;
	uint32_t best_encoder_id;
	enum drm_link_status link_status;
	bool self_refresh_aware;
	enum hdmi_picture_aspect picture_aspect_ratio;
	unsigned int content_type;
	unsigned int hdcp_content_type;
	unsigned int content_protection;
	unsigned int scaling_mode;
	u32 colorspace;
	u8 max_requested_bpc;
	u8 max_bpc;

	conn_id = state->connector->base.id;
	conn_state = state;
	state_state = state->state;
	commit = state->commit;
	crtc_id = state->crtc ? state->crtc->base.id : 0;
	best_encoder_id = state->best_encoder ?
	    state->best_encoder->base.id : 0;
	link_status = state->link_status;
	self_refresh_aware = state->self_refresh_aware;
	picture_aspect_ratio = state->picture_aspect_ratio;
	content_type = state->content_type;
	hdcp_content_type = state->hdcp_content_type;
	content_protection = state->content_protection;
	scaling_mode = state->scaling_mode;
	colorspace = state->colorspace;
	max_requested_bpc = state->max_requested_bpc;
	max_bpc = state->max_bpc;
#endif

	CTR5(KTR_DRM,
	    "amdgpu_dm_connector_atomic_check [1/3] "
	    "conn_id=%u conn_state=%p state=%p commit=%p crtc_id=%u",
	    conn_id, conn_state, state_state, commit, crtc_id);
	CTR5(KTR_DRM,
	    "amdgpu_dm_connector_atomic_check [2/3] "
	    "best_encoder_id=%u link_status=%d self_refresh_aware=%d "
	    "picture_aspect_ratio=%d content_type=%u", best_encoder_id,
	    link_status, self_refresh_aware,
	    picture_aspect_ratio, content_type);
	CTR6(KTR_DRM,
	    "amdgpu_dm_connector_atomic_check [3/3] "
	    "hdcp_content_type=%u content_protection=%u scaling_mode=%u "
	    "colorspace=%u max_requested_bpc=%u max_bpc=%u",
	    hdcp_content_type, content_protection,
	    scaling_mode, colorspace,
	    max_requested_bpc, max_bpc);
}

/* TRACE_EVENT(amdgpu_dm_crtc_atomic_check, */
/* 	    TP_PROTO(const struct drm_crtc_state *state), */

static inline void
trace_amdgpu_dm_crtc_atomic_check(const struct drm_crtc_state *state)
{
#ifdef KTR
	const struct drm_atomic_state * state_state;
	const struct drm_crtc_state * crtc_state;
	const struct drm_crtc_commit * commit;
	uint32_t crtc_id;
	bool enable;
	bool active;
	bool planes_changed;
	bool mode_changed;
	bool active_changed;
	bool connectors_changed;
	bool zpos_changed;
	bool color_mgmt_changed;
	bool no_vblank;
	bool async_flip;
	bool vrr_enabled;
	bool self_refresh_active;
	u32 plane_mask;
	u32 connector_mask;
	u32 encoder_mask;

	state_state = state->state;
	crtc_state = state;
	crtc_id = state->crtc->base.id;
	commit = state->commit;
	enable = state->enable;
	active = state->active;
	planes_changed = state->planes_changed;
	mode_changed = state->mode_changed;
	active_changed = state->active_changed;
	connectors_changed = state->connectors_changed;
	zpos_changed = state->zpos_changed;
	color_mgmt_changed = state->color_mgmt_changed;
	no_vblank = state->no_vblank;
	async_flip = state->async_flip;
	vrr_enabled = state->vrr_enabled;
	self_refresh_active = state->self_refresh_active;
	plane_mask = state->plane_mask;
	connector_mask = state->connector_mask;
	encoder_mask = state->encoder_mask;
#endif

	CTR4(KTR_DRM,
	    "amdgpu_dm_crtc_atomic_check[1/4] "
	    "crtc_id=%u crtc_state=%p state=%p commit=%p",
	    crtc_id, crtc_state, state_state, commit);
	CTR6(KTR_DRM,
	    "amdgpu_dm_crtc_atomic_check[2/4] "
	    "changed("
	    "planes=%d mode=%d active=%d conn=%d zpos=%d color_mgmt=%d)",
	    planes_changed, mode_changed, active_changed,
	    connectors_changed, zpos_changed, color_mgmt_changed);
	CTR6(KTR_DRM,
	    "amdgpu_dm_crtc_atomic_check[3/4] "
	    "state(enable=%d active=%d async_flip=%d vrr_enabled=%d "
	    "self_refresh_active=%d no_vblank=%d)",
	    enable, active, async_flip, vrr_enabled, self_refresh_active,
	    no_vblank);
	CTR3(KTR_DRM,
	    "amdgpu_dm_crtc_atomic_check[4/4] "
	    "mask(plane=%x conn=%x enc=%x)",
	    plane_mask, connector_mask, encoder_mask);
}

/* DECLARE_EVENT_CLASS(amdgpu_dm_plane_state_template, */
/* 	    TP_PROTO(const struct drm_plane_state *state), */

#ifdef KTR
#define trace_amdgpu_dm_plane_state_template(name) \
static inline void trace_ ## name(const struct drm_plane_state *state) \
{ \
	uint32_t plane_id; \
	enum drm_plane_type plane_type; \
	const struct drm_plane_state * plane_state; \
	const struct drm_atomic_state * state_state; \
	uint32_t crtc_id; \
	uint32_t fb_id; \
	uint32_t fb_format; \
	uint8_t fb_planes; \
	uint64_t fb_modifier; \
	const struct dma_fence * fence; \
	int32_t crtc_x; \
	int32_t crtc_y; \
	uint32_t crtc_w; \
	uint32_t crtc_h; \
	uint32_t src_x; \
	uint32_t src_y; \
	uint32_t src_w; \
	uint32_t src_h; \
	u32 alpha; \
	uint32_t pixel_blend_mode; \
	unsigned int rotation; \
	unsigned int zpos; \
	unsigned int normalized_zpos; \
	enum drm_color_encoding color_encoding; \
	enum drm_color_range color_range; \
	bool visible; \
\
	plane_id = state->plane->base.id; \
	plane_type = state->plane->type; \
	plane_state = state; \
	state_state = state->state; \
	crtc_id = state->crtc ? state->crtc->base.id : 0; \
	fb_id = state->fb ? state->fb->base.id : 0; \
	fb_format = state->fb ? state->fb->format->format : 0; \
	fb_planes = state->fb ? state->fb->format->num_planes : 0; \
	fb_modifier = state->fb ? state->fb->modifier : 0; \
	fence = state->fence; \
	crtc_x = state->crtc_x; \
	crtc_y = state->crtc_y; \
	crtc_w = state->crtc_w; \
	crtc_h = state->crtc_h; \
	src_x = state->src_x >> 16; \
	src_y = state->src_y >> 16; \
	src_w = state->src_w >> 16; \
	src_h = state->src_h >> 16; \
	alpha = state->alpha; \
	pixel_blend_mode = state->pixel_blend_mode; \
	rotation = state->rotation; \
	zpos = state->zpos; \
	normalized_zpos = state->normalized_zpos; \
	color_encoding = state->color_encoding; \
	color_range = state->color_range; \
	visible = state->visible; \
\
	CTR5(KTR_DRM, \
	    #name "[1/6] " \
	    "plane_id=%u plane_type=%d plane_state=%p state=%p " \
	    "crtc_id=%u", \
	    plane_id, plane_type, plane_state, state_state, crtc_id); \
	CTR5(KTR_DRM, \
	    #name "[2/6] " \
	    "fb(id=%u fmt=%c%c%c%c)", \
	    fb_id, \
	    (fb_format & 0xff) ? (fb_format & 0xff) : 'N', \
	    ((fb_format >> 8) & 0xff) ? ((fb_format >> 8) & 0xff) : 'O', \
	    ((fb_format >> 16) & 0xff) ? ((fb_format >> 16) & 0xff) : 'N', \
	    ((fb_format >> 24) & 0x7f) ? ((fb_format >> 24) & 0x7f) : 'E'); \
	CTR2(KTR_DRM, \
	    #name "[3/6] " \
	    "fb(planes=%u mod=%llu)", \
	    fb_planes, fb_modifier); \
	CTR5(KTR_DRM, \
	    #name "[4/6] " \
	    "fence=%p crtc_x=%d crtc_y=%d crtc_w=%u crtc_h=%u", \
	    fence, crtc_x, crtc_y, crtc_w, crtc_h); \
	CTR6(KTR_DRM, \
	    #name "[5/6] " \
	    "src_x=%u src_y=%u src_w=%u src_h=%u alpha=%u " \
	    "pixel_blend_mode=%u", \
	    src_x, src_y, src_w, src_h, alpha, pixel_blend_mode); \
	CTR6(KTR_DRM, \
	    #name "[6/6] " \
	    "rotation=%u zpos=%u " \
	    "normalized_zpos=%u color_encoding=%d color_range=%d " \
	    "visible=%d", \
	    rotation, zpos, normalized_zpos, color_encoding, color_range, \
	    visible); \
}
#else
#define trace_amdgpu_dm_plane_state_template(name) \
static inline void trace_ ## name(const struct drm_plane_state *state) \
{ \
}
#endif

/* DEFINE_EVENT(amdgpu_dm_plane_state_template, amdgpu_dm_plane_atomic_check, */
/* 	     TP_PROTO(const struct drm_plane_state *state), */

trace_amdgpu_dm_plane_state_template(amdgpu_dm_plane_atomic_check);

/* DEFINE_EVENT(amdgpu_dm_plane_state_template, amdgpu_dm_atomic_update_cursor, */
/* 	     TP_PROTO(const struct drm_plane_state *state), */

trace_amdgpu_dm_plane_state_template(amdgpu_dm_atomic_update_cursor);

/* TRACE_EVENT(amdgpu_dm_atomic_state_template, */
/* 	    TP_PROTO(const struct drm_atomic_state *state), */

#ifdef KTR
#define trace_amdgpu_dm_atomic_state_template(name) \
static inline void trace_ ## name(const struct drm_atomic_state *state) \
{ \
	const struct drm_atomic_state * state_state; \
	bool allow_modeset; \
	bool legacy_cursor_update; \
	bool async_update; \
	bool duplicated; \
	int num_connector; \
	int num_private_objs; \
 \
	state_state = state; \
	allow_modeset = state->allow_modeset; \
	legacy_cursor_update = state->legacy_cursor_update; \
	async_update = state->async_update; \
	duplicated = state->duplicated; \
	num_connector = state->num_connector; \
	num_private_objs = state->num_private_objs; \
 \
	CTR3(KTR_DRM, \
	    #name "[1/2] " \
	    "state=%p allow_modeset=%d legacy_cursor_update=%d", \
	    state_state, allow_modeset, legacy_cursor_update); \
	CTR4(KTR_DRM, \
	    #name "[2/2] " \
	    "async_update=%d duplicated=%d num_connector=%d " \
	    "num_private_objs=%d", \
	    async_update, duplicated, num_connector, num_private_objs); \
}
#else
#define trace_amdgpu_dm_atomic_state_template(name) \
static inline void trace_ ## name(const struct drm_atomic_state *state) \
{ \
}
#endif

/* DEFINE_EVENT(amdgpu_dm_atomic_state_template, amdgpu_dm_atomic_commit_tail_begin, */
/* 	     TP_PROTO(const struct drm_atomic_state *state), */

trace_amdgpu_dm_atomic_state_template(amdgpu_dm_atomic_commit_tail_begin);

/* DEFINE_EVENT(amdgpu_dm_atomic_state_template, amdgpu_dm_atomic_commit_tail_finish, */
/* 	     TP_PROTO(const struct drm_atomic_state *state), */

trace_amdgpu_dm_atomic_state_template(amdgpu_dm_atomic_commit_tail_finish);

/* DEFINE_EVENT(amdgpu_dm_atomic_state_template, amdgpu_dm_atomic_check_begin, */
/* 	     TP_PROTO(const struct drm_atomic_state *state), */

trace_amdgpu_dm_atomic_state_template(amdgpu_dm_atomic_check_begin);

/* TRACE_EVENT(amdgpu_dm_atomic_check_finish, */
/* 	    TP_PROTO(const struct drm_atomic_state *state, int res), */

static inline void
trace_amdgpu_dm_atomic_check_finish(
    const struct drm_atomic_state *state, int res)
{
#ifdef KTR
	const struct drm_atomic_state * state_state;
	bool async_update;
	bool allow_modeset;

	state_state = state;
	async_update = state->async_update;
	allow_modeset = state->allow_modeset;
#endif

	CTR4(KTR_DRM, \
	    "amdgpu_dm_atomic_check_finish " \
	    "state=%p res=%d async_update=%d allow_modeset=%d",
	    state_state, res, async_update, allow_modeset);
}

/* TRACE_EVENT(amdgpu_dm_dc_pipe_state, */
/* 	    TP_PROTO(int pipe_idx, const struct dc_plane_state *plane_state, */
/*		     const struct dc_stream_state *stream, */
/*		     const struct plane_resource *plane_res, */
/*		     int update_flags), */

static inline void
trace_amdgpu_dm_dc_pipe_state(
    int pipe_idx, const struct dc_plane_state *plane_state,
    const struct dc_stream_state *stream,
    const struct plane_resource *plane_res,
    int update_flags)
{
#ifdef KTR
	int stream_w;
	int stream_h;
	int dst_x;
	int dst_y;
	int dst_w;
	int dst_h;
	int src_x;
	int src_y;
	int src_w;
	int src_h;
	int clip_x;
	int clip_y;
	int clip_w;
	int clip_h;
	int recout_x;
	int recout_y;
	int recout_w;
	int recout_h;
	int viewport_x;
	int viewport_y;
	int viewport_w;
	int viewport_h;
	int flip_immediate;
	int surface_pitch;
	int format;
	int swizzle;

	stream_w = stream->timing.h_addressable;
	stream_h = stream->timing.v_addressable;
	dst_x = plane_state->dst_rect.x;
	dst_y = plane_state->dst_rect.y;
	dst_w = plane_state->dst_rect.width;
	dst_h = plane_state->dst_rect.height;
	src_x = plane_state->src_rect.x;
	src_y = plane_state->src_rect.y;
	src_w = plane_state->src_rect.width;
	src_h = plane_state->src_rect.height;
	clip_x = plane_state->clip_rect.x;
	clip_y = plane_state->clip_rect.y;
	clip_w = plane_state->clip_rect.width;
	clip_h = plane_state->clip_rect.height;
	recout_x = plane_res->scl_data.recout.x;
	recout_y = plane_res->scl_data.recout.y;
	recout_w = plane_res->scl_data.recout.width;
	recout_h = plane_res->scl_data.recout.height;
	viewport_x = plane_res->scl_data.viewport.x;
	viewport_y = plane_res->scl_data.viewport.y;
	viewport_w = plane_res->scl_data.viewport.width;
	viewport_h = plane_res->scl_data.viewport.height;
	flip_immediate = plane_state->flip_immediate;
	surface_pitch = plane_state->plane_size.surface_pitch;
	format = plane_state->format;
	swizzle = plane_state->tiling_info.gfx9.swizzle;
#endif

	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[1/7] "
	    "pipe_idx=%d stream=%p rct(%d,%d)",
	    pipe_idx, stream, stream_w, stream_h);
	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[2/7] "
	    "dst=(%d,%d,%d,%d)",
	    dst_x, dst_y, dst_w, dst_h);
	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[3/7] "
	    "src=(%d,%d,%d,%d)",
	    src_x, src_y, src_w, src_h);
	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[4/7] "
	    "clip=(%d,%d,%d,%d)",
	    clip_x, clip_y, clip_w, clip_h);
	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[5/7] "
	    "recout=(%d,%d,%d,%d)",
	    recout_x, recout_y, recout_w, recout_h);
	CTR4(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[6/7] "
	    "viewport=(%d,%d,%d,%d)",
	    viewport_x, viewport_y, viewport_w, viewport_h);
	CTR5(KTR_DRM,
	    "amdgpu_dm_dc_pipe_state[7/7] "
	    "flip_immediate=%d pitch=%d "
	    "format=%d swizzle=%d update_flags=%x",
	    flip_immediate, surface_pitch, format, swizzle, update_flags);
}

/* TRACE_EVENT(amdgpu_dm_dc_clocks_state, */
/* 	    TP_PROTO(const struct dc_clocks *clk), */

static inline void
trace_amdgpu_dm_dc_clocks_state(const struct dc_clocks *clk)
{
#ifdef KTR
	int dispclk_khz;
	int dppclk_khz;
	int disp_dpp_voltage_level_khz;
	int dcfclk_khz;
	int socclk_khz;
	int dcfclk_deep_sleep_khz;
	int fclk_khz;
	int phyclk_khz;
	int dramclk_khz;
	int p_state_change_support;
	int prev_p_state_change_support;
	int pwr_state;
	int dtm_level;
	int max_supported_dppclk_khz;
	int max_supported_dispclk_khz;
	int bw_dppclk_khz;
	int bw_dispclk_khz;

	dispclk_khz = clk->dispclk_khz;
	dppclk_khz = clk->dppclk_khz;
	disp_dpp_voltage_level_khz = -1;
	dcfclk_khz = clk->dcfclk_khz;
	socclk_khz = clk->socclk_khz;
	dcfclk_deep_sleep_khz = clk->dcfclk_deep_sleep_khz;
	fclk_khz = clk->fclk_khz;
	phyclk_khz = clk->phyclk_khz;
	dramclk_khz = clk->dramclk_khz;
	p_state_change_support = clk->p_state_change_support;
	prev_p_state_change_support = clk->prev_p_state_change_support;
	pwr_state = clk->pwr_state;
	prev_p_state_change_support = clk->prev_p_state_change_support;
	dtm_level = clk->dtm_level;
	max_supported_dppclk_khz = clk->max_supported_dppclk_khz;
	max_supported_dispclk_khz = clk->max_supported_dispclk_khz;
	bw_dppclk_khz = clk->bw_dppclk_khz;
	bw_dispclk_khz = clk->bw_dispclk_khz;
#endif

	CTR5(KTR_DRM,
	    "amdgpu_dm_dc_clocks_state[1/4] "
	    "dispclk_khz=%d dppclk_khz=%d disp_dpp_voltage_level_khz=%d "
	    "dcfclk_khz=%d socclk_khz=%d",
	    dispclk_khz, dppclk_khz, disp_dpp_voltage_level_khz, dcfclk_khz,
	    socclk_khz);
	CTR5(KTR_DRM,
	    "amdgpu_dm_dc_clocks_state[2/4] "
	    "dcfclk_deep_sleep_khz=%d fclk_khz=%d phyclk_khz=%d "
	    "dramclk_khz=%d p_state_change_support=%d",
	    dcfclk_deep_sleep_khz, fclk_khz, phyclk_khz, dramclk_khz,
	    p_state_change_support);
	CTR6(KTR_DRM,
	    "amdgpu_dm_dc_clocks_state[3/4] "
	    "prev_p_state_change_support=%d pwr_state=%d prev_p_state_change_support=%d "
	    "dtm_level=%d max_supported_dppclk_khz=%d max_supported_dispclk_khz=%d",
	    prev_p_state_change_support, pwr_state,
	    prev_p_state_change_support, dtm_level, max_supported_dppclk_khz,
	    max_supported_dispclk_khz);
	CTR2(KTR_DRM,
	    "amdgpu_dm_dc_clocks_state[4/4] "
	    "bw_dppclk_khz=%d bw_dispclk_khz=%d",
	    bw_dppclk_khz, bw_dispclk_khz);
}

/* TRACE_EVENT(amdgpu_dm_dce_clocks_state, */
/* 	    TP_PROTO(const struct dce_bw_output *clk), */

static inline void
trace_amdgpu_dm_dce_clocks_state(const struct dce_bw_output *clk)
{
#ifdef KTR
	bool cpuc_state_change_enable;
	bool cpup_state_change_enable;
	bool stutter_mode_enable;
	bool nbp_state_change_enable;
	bool all_displays_in_sync;
	int sclk_khz;
	int sclk_deep_sleep_khz;
	int yclk_khz;
	int dispclk_khz;
	int blackout_recovery_time_us;

	cpuc_state_change_enable = clk->cpuc_state_change_enable;
	cpup_state_change_enable = clk->cpup_state_change_enable;
	stutter_mode_enable = clk->stutter_mode_enable;
	nbp_state_change_enable = clk->nbp_state_change_enable;
	all_displays_in_sync = clk->all_displays_in_sync;
	sclk_khz = clk->sclk_khz;
	sclk_deep_sleep_khz = clk->sclk_deep_sleep_khz;
	yclk_khz = clk->yclk_khz;
	dispclk_khz = clk->dispclk_khz;
	blackout_recovery_time_us = clk->blackout_recovery_time_us;
#endif

	CTR5(KTR_DRM,
	    "amdgpu_dm_dce_clocks_state[1/2] "
	    "cpuc_state_change_enable=%d cpup_state_change_enable=%d stutter_mode_enable=%d "
	    "nbp_state_change_enable=%d all_displays_in_sync=%d",
	    cpuc_state_change_enable, cpup_state_change_enable,
	    stutter_mode_enable, nbp_state_change_enable,
	    all_displays_in_sync);
	CTR5(KTR_DRM,
	    "amdgpu_dm_dce_clocks_state[2/2] "
	    "sclk_khz=%d sclk_deep_sleep_khz=%d "
	    "yclk_khz=%d dispclk_khz=%d blackout_recovery_time_us=%d",
	    sclk_khz, sclk_deep_sleep_khz, yclk_khz, dispclk_khz,
	    blackout_recovery_time_us);
}

/* TRACE_EVENT(amdgpu_dmub_trace_high_irq, */
/*          TP_PROTO(uint32_t trace_code, uint32_t tick_count, uint32_t param0, */
/*                   uint32_t param1), */

static inline void
trace_amdgpu_dmub_trace_high_irq(uint32_t trace_code, uint32_t tick_count,
    uint32_t param0, uint32_t param1)
{
	CTR4(KTR_DRM,
	    "amdgpu_dmub_trace_high_irq "
	    "trace_code=%u tick_count=%u param0=%u param1=%u",
	    trace_code, tick_count,
	    param0, param1);
}

/* TRACE_EVENT(amdgpu_refresh_rate_track, */
/*          TP_PROTO(int crtc_index, ktime_t refresh_rate_ns, uint32_t refresh_rate_hz), */

static inline void
trace_amdgpu_refresh_rate_track(int crtc_index, ktime_t refresh_rate_ns,
    uint32_t refresh_rate_hz)
{
	CTR3(KTR_DRM,
	    "amdgpu_refresh_rate_track "
	    "crtc_index=%d refresh_rate=%dHz (%lld)",
	    crtc_index, refresh_rate_hz, refresh_rate_ns);
}

/* TRACE_EVENT(dcn_fpu, */
/*	    TP_PROTO(bool begin, const char *function, const int line), */

static inline void
trace_dcn_fpu(bool begin, const char *function, const int line, const int recursion_depth)
{
	CTR4(KTR_DRM,
	    "dcn_fpu "
	    "%s: recursion_depth: %d: %s()+%d:",
	    begin ? "begin" : "end", recursion_depth, function, line);
}

/* TRACE_EVENT(dcn_optc_lock_unlock_state, */
/*            TP_PROTO(const struct optc *optc_state, int instance, bool lock, const char *function, const int line), */

static inline void
trace_dcn_optc_lock_unlock_state(const struct optc *optc_state, int instance,
    bool lock, const char *function, const int line)
{
#ifdef KTR
	int opp_count;
	int max_h_total;
	int max_v_total;
	int min_h_blank;
	int min_h_sync_width;
	int min_v_sync_width;
	int min_v_blank;
	int min_v_blank_interlace;
	int vstartup_start;
	int vupdate_offset;
	int vupdate_width;
	int vready_offset;

	opp_count = optc_state->opp_count;
	max_h_total = optc_state->max_h_total;
	max_v_total = optc_state->max_v_total;
	min_h_blank = optc_state->min_h_blank;
	min_h_sync_width = optc_state->min_h_sync_width;
	min_v_sync_width = optc_state->min_v_sync_width;
	min_v_blank = optc_state->min_v_blank;
	min_v_blank_interlace = optc_state->min_v_blank_interlace;
	vstartup_start = optc_state->vstartup_start;
	vupdate_offset = optc_state->vupdate_offset;
	vupdate_width = optc_state->vupdate_width;
	vready_offset = optc_state->vupdate_offset;
#endif
  
	CTR5(KTR_DRM,
	    "dcn_optc_lock_unlock_state[1/4] "
	    "%s: %s()+%d: optc_instance=%d opp_count=%d",
	    lock ? "Lock" : "Unlock", function, line, instance, opp_count);
	CTR5(KTR_DRM,
	    "dcn_optc_lock_unlock_state[2/4] "
	    "max_h_total=%d max_v_total=%d min_h_blank=%d min_h_sync_width=%d "
	    "min_v_sync_width=%d",
	    max_h_total, max_v_total, min_h_blank, min_h_sync_width,
	    min_v_sync_width);
	CTR5(KTR_DRM,
	    "dcn_optc_lock_unlock_state[3/4] "
	    "min_v_blank=%d min_v_blank_interlace=%d vstartup_start=%d "
	    "vupdate_offset=%d vupdate_width=%d",
	    min_v_blank, min_v_blank_interlace, vstartup_start, vupdate_offset,
	    vupdate_width);
	CTR1(KTR_DRM,
	    "dcn_optc_lock_unlock_state[4/4] "
	    "vready_offset=%d", vready_offset);
}

#endif /* _AMDGPU_DM_TRACE_FREEBSD_H_ */

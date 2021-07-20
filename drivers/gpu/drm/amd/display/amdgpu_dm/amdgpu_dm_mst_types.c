/*
 * Copyright 2012-15 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#include <linux/version.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_dp_mst_helper.h>
#include "dm_services.h"
#include "amdgpu.h"
#include "amdgpu_dm.h"
#include "amdgpu_dm_mst_types.h"

#include "dc.h"
#include "dm_helpers.h"

#include "dc_link_ddc.h"

#include "i2caux_interface.h"
#if defined(CONFIG_DEBUG_FS)
#include "amdgpu_dm_debugfs.h"
#endif
/* #define TRACE_DPCD */

#ifdef TRACE_DPCD
#define SIDE_BAND_MSG(address) (address >= DP_SIDEBAND_MSG_DOWN_REQ_BASE && address < DP_SINK_COUNT_ESI)

static inline char *side_band_msg_type_to_str(uint32_t address)
{
	static char str[10] = {0};

	if (address < DP_SIDEBAND_MSG_UP_REP_BASE)
		strcpy(str, "DOWN_REQ");
	else if (address < DP_SIDEBAND_MSG_DOWN_REP_BASE)
		strcpy(str, "UP_REP");
	else if (address < DP_SIDEBAND_MSG_UP_REQ_BASE)
		strcpy(str, "DOWN_REP");
	else
		strcpy(str, "UP_REQ");

	return str;
}

static void log_dpcd(uint8_t type,
		     uint32_t address,
		     uint8_t *data,
		     uint32_t size,
		     bool res)
{
	DRM_DEBUG_KMS("Op: %s, addr: %04x, SideBand Msg: %s, Op res: %s\n",
			(type == DP_AUX_NATIVE_READ) ||
			(type == DP_AUX_I2C_READ) ?
					"Read" : "Write",
			address,
			SIDE_BAND_MSG(address) ?
					side_band_msg_type_to_str(address) : "Nop",
			res ? "OK" : "Fail");

	if (res) {
		print_hex_dump(KERN_INFO, "Body: ", DUMP_PREFIX_NONE, 16, 1, data, size, false);
	}
}
#endif

static ssize_t dm_dp_aux_transfer(struct drm_dp_aux *aux,
				  struct drm_dp_aux_msg *msg)
{
	ssize_t result = 0;
	struct aux_payload payload;
	enum aux_channel_operation_result operation_result;

	if (WARN_ON(msg->size > 16))
		return -E2BIG;

	payload.address = msg->address;
	payload.data = msg->buffer;
	payload.length = msg->size;
	payload.reply = &msg->reply;
	payload.i2c_over_aux = (msg->request & DP_AUX_NATIVE_WRITE) == 0;
	payload.write = (msg->request & DP_AUX_I2C_READ) == 0;
	payload.mot = (msg->request & DP_AUX_I2C_MOT) != 0;
	payload.defer_delay = 0;

	result = dc_link_aux_transfer_raw(TO_DM_AUX(aux)->ddc_service, &payload,
				      &operation_result);

	if (payload.write)
		result = msg->size;

	if (result < 0)
		switch (operation_result) {
		case AUX_CHANNEL_OPERATION_SUCCEEDED:
			break;
		case AUX_CHANNEL_OPERATION_FAILED_HPD_DISCON:
		case AUX_CHANNEL_OPERATION_FAILED_REASON_UNKNOWN:
			result = -EIO;
			break;
		case AUX_CHANNEL_OPERATION_FAILED_INVALID_REPLY:
		case AUX_CHANNEL_OPERATION_FAILED_ENGINE_ACQUIRE:
			result = -EBUSY;
			break;
		case AUX_CHANNEL_OPERATION_FAILED_TIMEOUT:
			result = -ETIMEDOUT;
			break;
		}

	return result;
}

static void
dm_dp_mst_connector_destroy(struct drm_connector *connector)
{
	struct amdgpu_dm_connector *amdgpu_dm_connector = to_amdgpu_dm_connector(connector);
	struct amdgpu_encoder *amdgpu_encoder = amdgpu_dm_connector->mst_encoder;

	kfree(amdgpu_dm_connector->edid);
	amdgpu_dm_connector->edid = NULL;

	drm_encoder_cleanup(&amdgpu_encoder->base);
	kfree(amdgpu_encoder);
	drm_connector_cleanup(connector);
	drm_dp_mst_put_port_malloc(amdgpu_dm_connector->port);
	kfree(amdgpu_dm_connector);
}

static int
amdgpu_dm_mst_connector_late_register(struct drm_connector *connector)
{
	struct amdgpu_dm_connector *amdgpu_dm_connector =
		to_amdgpu_dm_connector(connector);
	struct drm_dp_mst_port *port = amdgpu_dm_connector->port;

#if defined(CONFIG_DEBUG_FS)
	connector_debugfs_init(amdgpu_dm_connector);
	amdgpu_dm_connector->debugfs_dpcd_address = 0;
	amdgpu_dm_connector->debugfs_dpcd_size = 0;
#endif

	return drm_dp_mst_connector_late_register(connector, port);
}

static void
amdgpu_dm_mst_connector_early_unregister(struct drm_connector *connector)
{
	struct amdgpu_dm_connector *amdgpu_dm_connector =
		to_amdgpu_dm_connector(connector);
	struct drm_dp_mst_port *port = amdgpu_dm_connector->port;

	drm_dp_mst_connector_early_unregister(connector, port);
}

static const struct drm_connector_funcs dm_dp_mst_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = dm_dp_mst_connector_destroy,
	.reset = amdgpu_dm_connector_funcs_reset,
	.atomic_duplicate_state = amdgpu_dm_connector_atomic_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
	.atomic_set_property = amdgpu_dm_connector_atomic_set_property,
	.atomic_get_property = amdgpu_dm_connector_atomic_get_property,
	.late_register = amdgpu_dm_mst_connector_late_register,
	.early_unregister = amdgpu_dm_mst_connector_early_unregister,
};

static bool validate_dsc_caps_on_connector(struct amdgpu_dm_connector *aconnector)
{
	struct dc_sink *dc_sink = aconnector->dc_sink;
	struct drm_dp_mst_port *port = aconnector->port;
	u8 dsc_caps[16] = { 0 };

	aconnector->dsc_aux = drm_dp_mst_dsc_aux_for_port(port);

	if (!aconnector->dsc_aux)
		return false;

	if (drm_dp_dpcd_read(aconnector->dsc_aux, DP_DSC_SUPPORT, dsc_caps, 16) < 0)
		return false;

	if (!dc_dsc_parse_dsc_dpcd(aconnector->dc_link->ctx->dc,
				   dsc_caps, NULL,
				   &dc_sink->sink_dsc_caps.dsc_dec_caps))
		return false;

	return true;
}

static int dm_dp_mst_get_modes(struct drm_connector *connector)
{
	struct amdgpu_dm_connector *aconnector = to_amdgpu_dm_connector(connector);
	int ret = 0;

	if (!aconnector)
		return drm_add_edid_modes(connector, NULL);

	if (!aconnector->edid) {
		struct edid *edid;
		edid = drm_dp_mst_get_edid(connector, &aconnector->mst_port->mst_mgr, aconnector->port);

		if (!edid) {
			drm_connector_update_edid_property(
				&aconnector->base,
				NULL);
			return ret;
		}

		aconnector->edid = edid;
	}

	if (aconnector->dc_sink && aconnector->dc_sink->sink_signal == SIGNAL_TYPE_VIRTUAL) {
		dc_sink_release(aconnector->dc_sink);
		aconnector->dc_sink = NULL;
	}

	if (!aconnector->dc_sink) {
		struct dc_sink *dc_sink;
		struct dc_sink_init_data init_params = {
				.link = aconnector->dc_link,
				.sink_signal = SIGNAL_TYPE_DISPLAY_PORT_MST };
		dc_sink = dc_link_add_remote_sink(
			aconnector->dc_link,
			(uint8_t *)aconnector->edid,
			(aconnector->edid->extensions + 1) * EDID_LENGTH,
			&init_params);

		dc_sink->priv = aconnector;
		/* dc_link_add_remote_sink returns a new reference */
		aconnector->dc_sink = dc_sink;

		if (aconnector->dc_sink) {
			amdgpu_dm_update_freesync_caps(
					connector, aconnector->edid);

			if (!validate_dsc_caps_on_connector(aconnector))
				memset(&aconnector->dc_sink->sink_dsc_caps,
				       0, sizeof(aconnector->dc_sink->sink_dsc_caps));
		}
	}

	drm_connector_update_edid_property(
					&aconnector->base, aconnector->edid);

	ret = drm_add_edid_modes(connector, aconnector->edid);

	return ret;
}

static struct drm_encoder *
dm_mst_atomic_best_encoder(struct drm_connector *connector,
			   struct drm_connector_state *connector_state)
{
	return &to_amdgpu_dm_connector(connector)->mst_encoder->base;
}

static int
dm_dp_mst_detect(struct drm_connector *connector,
		 struct drm_modeset_acquire_ctx *ctx, bool force)
{
	struct amdgpu_dm_connector *aconnector = to_amdgpu_dm_connector(connector);
	struct amdgpu_dm_connector *master = aconnector->mst_port;

	return drm_dp_mst_detect_port(connector, ctx, &master->mst_mgr,
				      aconnector->port);
}

static int dm_dp_mst_atomic_check(struct drm_connector *connector,
				struct drm_atomic_state *state)
{
	struct drm_connector_state *new_conn_state =
			drm_atomic_get_new_connector_state(state, connector);
	struct drm_connector_state *old_conn_state =
			drm_atomic_get_old_connector_state(state, connector);
	struct amdgpu_dm_connector *aconnector = to_amdgpu_dm_connector(connector);
	struct drm_crtc_state *new_crtc_state;
	struct drm_dp_mst_topology_mgr *mst_mgr;
	struct drm_dp_mst_port *mst_port;

	mst_port = aconnector->port;
	mst_mgr = &aconnector->mst_port->mst_mgr;

	if (!old_conn_state->crtc)
		return 0;

	if (new_conn_state->crtc) {
		new_crtc_state = drm_atomic_get_new_crtc_state(state, new_conn_state->crtc);
		if (!new_crtc_state ||
		    !drm_atomic_crtc_needs_modeset(new_crtc_state) ||
		    new_crtc_state->enable)
			return 0;
		}

	return drm_dp_atomic_release_vcpi_slots(state,
						mst_mgr,
						mst_port);
}

static const struct drm_connector_helper_funcs dm_dp_mst_connector_helper_funcs = {
	.get_modes = dm_dp_mst_get_modes,
	.mode_valid = amdgpu_dm_connector_mode_valid,
	.atomic_best_encoder = dm_mst_atomic_best_encoder,
	.detect_ctx = dm_dp_mst_detect,
	.atomic_check = dm_dp_mst_atomic_check,
};

static void amdgpu_dm_encoder_destroy(struct drm_encoder *encoder)
{
	drm_encoder_cleanup(encoder);
	kfree(encoder);
}

static const struct drm_encoder_funcs amdgpu_dm_encoder_funcs = {
	.destroy = amdgpu_dm_encoder_destroy,
};

static struct amdgpu_encoder *
dm_dp_create_fake_mst_encoder(struct amdgpu_dm_connector *connector)
{
	struct drm_device *dev = connector->base.dev;
	struct amdgpu_device *adev = dev->dev_private;
	struct amdgpu_encoder *amdgpu_encoder;
	struct drm_encoder *encoder;

	amdgpu_encoder = kzalloc(sizeof(*amdgpu_encoder), GFP_KERNEL);
	if (!amdgpu_encoder)
		return NULL;

	encoder = &amdgpu_encoder->base;
	encoder->possible_crtcs = amdgpu_dm_get_encoder_crtc_mask(adev);

	drm_encoder_init(
		dev,
		&amdgpu_encoder->base,
		&amdgpu_dm_encoder_funcs,
		DRM_MODE_ENCODER_DPMST,
		NULL);

	drm_encoder_helper_add(encoder, &amdgpu_dm_encoder_helper_funcs);

	return amdgpu_encoder;
}

static struct drm_connector *
dm_dp_add_mst_connector(struct drm_dp_mst_topology_mgr *mgr,
			struct drm_dp_mst_port *port,
			const char *pathprop)
{
	struct amdgpu_dm_connector *master = container_of(mgr, struct amdgpu_dm_connector, mst_mgr);
	struct drm_device *dev = master->base.dev;
	struct amdgpu_device *adev = dev->dev_private;
	struct amdgpu_dm_connector *aconnector;
	struct drm_connector *connector;

	aconnector = kzalloc(sizeof(*aconnector), GFP_KERNEL);
	if (!aconnector)
		return NULL;

	connector = &aconnector->base;
	aconnector->port = port;
	aconnector->mst_port = master;

	if (drm_connector_init(
		dev,
		connector,
		&dm_dp_mst_connector_funcs,
		DRM_MODE_CONNECTOR_DisplayPort)) {
		kfree(aconnector);
		return NULL;
	}
	drm_connector_helper_add(connector, &dm_dp_mst_connector_helper_funcs);

	amdgpu_dm_connector_init_helper(
		&adev->dm,
		aconnector,
		DRM_MODE_CONNECTOR_DisplayPort,
		master->dc_link,
		master->connector_id);

	aconnector->mst_encoder = dm_dp_create_fake_mst_encoder(master);
	drm_connector_attach_encoder(&aconnector->base,
				     &aconnector->mst_encoder->base);

	drm_object_attach_property(
		&connector->base,
		dev->mode_config.path_property,
		0);
	drm_object_attach_property(
		&connector->base,
		dev->mode_config.tile_property,
		0);

	drm_connector_set_path_property(connector, pathprop);

	/*
	 * Initialize connector state before adding the connectror to drm and
	 * framebuffer lists
	 */
	amdgpu_dm_connector_funcs_reset(connector);

	DRM_INFO("DM_MST: added connector: %p [id: %d] [master: %p]\n",
		 aconnector, connector->base.id, aconnector->mst_port);

	drm_dp_mst_get_port_malloc(port);

	DRM_DEBUG_KMS(":%d\n", connector->base.id);

	return connector;
}

static void dm_dp_destroy_mst_connector(struct drm_dp_mst_topology_mgr *mgr,
					struct drm_connector *connector)
{
	struct amdgpu_dm_connector *master = container_of(mgr, struct amdgpu_dm_connector, mst_mgr);
	struct drm_device *dev = master->base.dev;
	struct amdgpu_device *adev = dev->dev_private;
	struct amdgpu_dm_connector *aconnector = to_amdgpu_dm_connector(connector);

	DRM_INFO("DM_MST: Disabling connector: %p [id: %d] [master: %p]\n",
		 aconnector, connector->base.id, aconnector->mst_port);

	if (aconnector->dc_sink) {
		amdgpu_dm_update_freesync_caps(connector, NULL);
		dc_link_remove_remote_sink(aconnector->dc_link,
					   aconnector->dc_sink);
		dc_sink_release(aconnector->dc_sink);
		aconnector->dc_sink = NULL;
	}

	drm_connector_unregister(connector);
	if (adev->mode_info.rfbdev)
		drm_fb_helper_remove_one_connector(&adev->mode_info.rfbdev->helper, connector);
	drm_connector_put(connector);
}

static void dm_dp_mst_register_connector(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct amdgpu_device *adev = dev->dev_private;

	if (adev->mode_info.rfbdev)
		drm_fb_helper_add_one_connector(&adev->mode_info.rfbdev->helper, connector);
	else
		DRM_ERROR("adev->mode_info.rfbdev is NULL\n");

	drm_connector_register(connector);
}

static const struct drm_dp_mst_topology_cbs dm_mst_cbs = {
	.add_connector = dm_dp_add_mst_connector,
	.destroy_connector = dm_dp_destroy_mst_connector,
	.register_connector = dm_dp_mst_register_connector
};

void amdgpu_dm_initialize_dp_connector(struct amdgpu_display_manager *dm,
				       struct amdgpu_dm_connector *aconnector)
{
	aconnector->dm_dp_aux.aux.name = "dmdc";
	aconnector->dm_dp_aux.aux.dev = aconnector->base.kdev;
	aconnector->dm_dp_aux.aux.transfer = dm_dp_aux_transfer;
	aconnector->dm_dp_aux.ddc_service = aconnector->dc_link->ddc;

	drm_dp_aux_register(&aconnector->dm_dp_aux.aux);
#ifdef __linux__
	drm_dp_cec_register_connector(&aconnector->dm_dp_aux.aux,
				      aconnector->base.name, dm->adev->dev);
#endif

	if (aconnector->base.connector_type == DRM_MODE_CONNECTOR_eDP)
		return;

	aconnector->mst_mgr.cbs = &dm_mst_cbs;
	drm_dp_mst_topology_mgr_init(
		&aconnector->mst_mgr,
		dm->adev->ddev,
		&aconnector->dm_dp_aux.aux,
		16,
		4,
		aconnector->connector_id);
}


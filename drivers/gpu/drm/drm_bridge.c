/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <linux/err.h>
#include <linux/module.h>
#include <linux/mutex.h>

#include <drm/drm_atomic_state_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_encoder.h>

#include "drm_crtc_internal.h"

/**
 * DOC: overview
 *
 * &struct drm_bridge represents a device that hangs on to an encoder. These are
 * handy when a regular &drm_encoder entity isn't enough to represent the entire
 * encoder chain.
 *
 * A bridge is always attached to a single &drm_encoder at a time, but can be
 * either connected to it directly, or through an intermediate bridge::
 *
 *     encoder ---> bridge B ---> bridge A
 *
 * Here, the output of the encoder feeds to bridge B, and that furthers feeds to
 * bridge A.
 *
 * The driver using the bridge is responsible to make the associations between
 * the encoder and bridges. Once these links are made, the bridges will
 * participate along with encoder functions to perform mode_set/enable/disable
 * through the ops provided in &drm_bridge_funcs.
 *
 * drm_bridge, like drm_panel, aren't drm_mode_object entities like planes,
 * CRTCs, encoders or connectors and hence are not visible to userspace. They
 * just provide additional hooks to get the desired output at the end of the
 * encoder chain.
 *
 * Bridges can also be chained up using the &drm_bridge.chain_node field.
 *
 * Both legacy CRTC helpers and the new atomic modeset helpers support bridges.
 */

static DEFINE_MUTEX(bridge_lock);
static LIST_HEAD(bridge_list);

/**
 * drm_bridge_add - add the given bridge to the global bridge list
 *
 * @bridge: bridge control structure
 */
void drm_bridge_add(struct drm_bridge *bridge)
{
	mutex_lock(&bridge_lock);
	list_add_tail(&bridge->list, &bridge_list);
	mutex_unlock(&bridge_lock);
}
EXPORT_SYMBOL(drm_bridge_add);

/**
 * drm_bridge_remove - remove the given bridge from the global bridge list
 *
 * @bridge: bridge control structure
 */
void drm_bridge_remove(struct drm_bridge *bridge)
{
	mutex_lock(&bridge_lock);
	list_del_init(&bridge->list);
	mutex_unlock(&bridge_lock);
}
EXPORT_SYMBOL(drm_bridge_remove);

static struct drm_private_state *
drm_bridge_atomic_duplicate_priv_state(struct drm_private_obj *obj)
{
	struct drm_bridge *bridge = drm_priv_to_bridge(obj);
	struct drm_bridge_state *state;

	state = bridge->funcs->atomic_duplicate_state(bridge);
	return state ? &state->base : NULL;
}

static void
drm_bridge_atomic_destroy_priv_state(struct drm_private_obj *obj,
				     struct drm_private_state *s)
{
	struct drm_bridge_state *state = drm_priv_to_bridge_state(s);
	struct drm_bridge *bridge = drm_priv_to_bridge(obj);

	bridge->funcs->atomic_destroy_state(bridge, state);
}

static const struct drm_private_state_funcs drm_bridge_priv_state_funcs = {
	.atomic_duplicate_state = drm_bridge_atomic_duplicate_priv_state,
	.atomic_destroy_state = drm_bridge_atomic_destroy_priv_state,
};

/**
 * drm_bridge_attach - attach the bridge to an encoder's chain
 *
 * @encoder: DRM encoder
 * @bridge: bridge to attach
 * @previous: previous bridge in the chain (optional)
 *
 * Called by a kms driver to link the bridge to an encoder's chain. The previous
 * argument specifies the previous bridge in the chain. If NULL, the bridge is
 * linked directly at the encoder's output. Otherwise it is linked at the
 * previous bridge's output.
 *
 * If non-NULL the previous bridge must be already attached by a call to this
 * function.
 *
 * Note that bridges attached to encoders are auto-detached during encoder
 * cleanup in drm_encoder_cleanup(), so drm_bridge_attach() should generally
 * *not* be balanced with a drm_bridge_detach() in driver code.
 *
 * RETURNS:
 * Zero on success, error code on failure
 */
int drm_bridge_attach(struct drm_encoder *encoder, struct drm_bridge *bridge,
		      struct drm_bridge *previous)
{
	int ret;

	if (!encoder || !bridge)
		return -EINVAL;

	if (previous && (!previous->dev || previous->encoder != encoder))
		return -EINVAL;

	if (bridge->dev)
		return -EBUSY;

	bridge->dev = encoder->dev;
	bridge->encoder = encoder;

	if (previous)
		list_add(&bridge->chain_node, &previous->chain_node);
	else
		list_add(&bridge->chain_node, &encoder->bridge_chain);

	if (bridge->funcs->attach) {
		ret = bridge->funcs->attach(bridge);
		if (ret < 0)
			goto err_reset_bridge;
	}

	if (bridge->funcs->atomic_reset) {
		struct drm_bridge_state *state;

		state = bridge->funcs->atomic_reset(bridge);
		if (IS_ERR(state)) {
			ret = PTR_ERR(state);
			goto err_detach_bridge;
		}

		drm_atomic_private_obj_init(bridge->dev, &bridge->base,
					    &state->base,
					    &drm_bridge_priv_state_funcs);
	}

	return 0;

err_detach_bridge:
	if (bridge->funcs->detach)
		bridge->funcs->detach(bridge);

err_reset_bridge:
	bridge->dev = NULL;
	bridge->encoder = NULL;
	list_del(&bridge->chain_node);
	return ret;
}
EXPORT_SYMBOL(drm_bridge_attach);

void drm_bridge_detach(struct drm_bridge *bridge)
{
	if (WARN_ON(!bridge))
		return;

	if (WARN_ON(!bridge->dev))
		return;

	if (bridge->funcs->atomic_reset)
		drm_atomic_private_obj_fini(&bridge->base);

	if (bridge->funcs->detach)
		bridge->funcs->detach(bridge);

	list_del(&bridge->chain_node);
	bridge->dev = NULL;
}

/**
 * DOC: bridge callbacks
 *
 * The &drm_bridge_funcs ops are populated by the bridge driver. The DRM
 * internals (atomic and CRTC helpers) use the helpers defined in drm_bridge.c
 * These helpers call a specific &drm_bridge_funcs op for all the bridges
 * during encoder configuration.
 *
 * For detailed specification of the bridge callbacks see &drm_bridge_funcs.
 */

/**
 * drm_bridge_chain_mode_fixup - fixup proposed mode for all bridges in the
 *				 encoder chain
 * @bridge: bridge control structure
 * @mode: desired mode to be set for the bridge
 * @adjusted_mode: updated mode that works for this bridge
 *
 * Calls &drm_bridge_funcs.mode_fixup for all the bridges in the
 * encoder chain, starting from the first bridge to the last.
 *
 * Note: the bridge passed should be the one closest to the encoder
 *
 * RETURNS:
 * true on success, false on failure
 */
bool drm_bridge_chain_mode_fixup(struct drm_bridge *bridge,
				 const struct drm_display_mode *mode,
				 struct drm_display_mode *adjusted_mode)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return true;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (!bridge->funcs->mode_fixup)
			continue;

		if (!bridge->funcs->mode_fixup(bridge, mode, adjusted_mode))
			return false;
	}

	return true;
}
EXPORT_SYMBOL(drm_bridge_chain_mode_fixup);

/**
 * drm_bridge_chain_mode_valid - validate the mode against all bridges in the
 *				 encoder chain.
 * @bridge: bridge control structure
 * @mode: desired mode to be validated
 *
 * Calls &drm_bridge_funcs.mode_valid for all the bridges in the encoder
 * chain, starting from the first bridge to the last. If at least one bridge
 * does not accept the mode the function returns the error code.
 *
 * Note: the bridge passed should be the one closest to the encoder.
 *
 * RETURNS:
 * MODE_OK on success, drm_mode_status Enum error code on failure
 */
enum drm_mode_status
drm_bridge_chain_mode_valid(struct drm_bridge *bridge,
			    const struct drm_display_mode *mode)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return MODE_OK;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		enum drm_mode_status ret;

		if (!bridge->funcs->mode_valid)
			continue;

		ret = bridge->funcs->mode_valid(bridge, mode);
		if (ret != MODE_OK)
			return ret;
	}

	return MODE_OK;
}
EXPORT_SYMBOL(drm_bridge_chain_mode_valid);

/**
 * drm_bridge_chain_disable - disables all bridges in the encoder chain
 * @bridge: bridge control structure
 *
 * Calls &drm_bridge_funcs.disable op for all the bridges in the encoder
 * chain, starting from the last bridge to the first. These are called before
 * calling the encoder's prepare op.
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_bridge_chain_disable(struct drm_bridge *bridge)
{
	struct drm_encoder *encoder;
	struct drm_bridge *iter;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_reverse(iter, &encoder->bridge_chain, chain_node) {
		if (iter->funcs->disable)
			iter->funcs->disable(iter);

		if (iter == bridge)
			break;
	}
}
EXPORT_SYMBOL(drm_bridge_chain_disable);

/**
 * drm_bridge_chain_post_disable - cleans up after disabling all bridges in the
 *				   encoder chain
 * @bridge: bridge control structure
 *
 * Calls &drm_bridge_funcs.post_disable op for all the bridges in the
 * encoder chain, starting from the first bridge to the last. These are called
 * after completing the encoder's prepare op.
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_bridge_chain_post_disable(struct drm_bridge *bridge)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (bridge->funcs->post_disable)
			bridge->funcs->post_disable(bridge);
	}
}
EXPORT_SYMBOL(drm_bridge_chain_post_disable);

/**
 * drm_bridge_chain_mode_set - set proposed mode for all bridges in the
 *			       encoder chain
 * @bridge: bridge control structure
 * @mode: desired mode to be set for the encoder chain
 * @adjusted_mode: updated mode that works for this encoder chain
 *
 * Calls &drm_bridge_funcs.mode_set op for all the bridges in the
 * encoder chain, starting from the first bridge to the last.
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_bridge_chain_mode_set(struct drm_bridge *bridge,
			       const struct drm_display_mode *mode,
			       const struct drm_display_mode *adjusted_mode)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (bridge->funcs->mode_set)
			bridge->funcs->mode_set(bridge, mode, adjusted_mode);
	}
}
EXPORT_SYMBOL(drm_bridge_chain_mode_set);

/**
 * drm_bridge_chain_pre_enable - prepares for enabling all bridges in the
 *				 encoder chain
 * @bridge: bridge control structure
 *
 * Calls &drm_bridge_funcs.pre_enable op for all the bridges in the encoder
 * chain, starting from the last bridge to the first. These are called
 * before calling the encoder's commit op.
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_bridge_chain_pre_enable(struct drm_bridge *bridge)
{
	struct drm_encoder *encoder;
	struct drm_bridge *iter;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_reverse(iter, &encoder->bridge_chain, chain_node) {
		if (iter->funcs->pre_enable)
			iter->funcs->pre_enable(iter);
	}
}
EXPORT_SYMBOL(drm_bridge_chain_pre_enable);

/**
 * drm_bridge_chain_enable - enables all bridges in the encoder chain
 * @bridge: bridge control structure
 *
 * Calls &drm_bridge_funcs.enable op for all the bridges in the encoder
 * chain, starting from the first bridge to the last. These are called
 * after completing the encoder's commit op.
 *
 * Note that the bridge passed should be the one closest to the encoder
 */
void drm_bridge_chain_enable(struct drm_bridge *bridge)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (bridge->funcs->enable)
			bridge->funcs->enable(bridge);
	}
}
EXPORT_SYMBOL(drm_bridge_chain_enable);

/**
 * drm_atomic_bridge_chain_disable - disables all bridges in the encoder chain
 * @bridge: bridge control structure
 * @old_state: old atomic state
 *
 * Calls &drm_bridge_funcs.atomic_disable (falls back on
 * &drm_bridge_funcs.disable) op for all the bridges in the encoder chain,
 * starting from the last bridge to the first. These are called before calling
 * &drm_encoder_helper_funcs.atomic_disable
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_atomic_bridge_chain_disable(struct drm_bridge *bridge,
				     struct drm_atomic_state *old_state)
{
	struct drm_encoder *encoder;
	struct drm_bridge *iter;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_reverse(iter, &encoder->bridge_chain, chain_node) {
		if (iter->funcs->atomic_disable) {
			struct drm_bridge_state *old_bridge_state;

			old_bridge_state =
				drm_atomic_get_old_bridge_state(old_state,
								iter);
			if (WARN_ON(!old_bridge_state))
				return;

			iter->funcs->atomic_disable(iter, old_bridge_state);
		} else if (iter->funcs->disable) {
			iter->funcs->disable(iter);
		}

		if (iter == bridge)
			break;
	}
}
EXPORT_SYMBOL(drm_atomic_bridge_chain_disable);

/**
 * drm_atomic_bridge_chain_post_disable - cleans up after disabling all bridges
 *					  in the encoder chain
 * @bridge: bridge control structure
 * @old_state: old atomic state
 *
 * Calls &drm_bridge_funcs.atomic_post_disable (falls back on
 * &drm_bridge_funcs.post_disable) op for all the bridges in the encoder chain,
 * starting from the first bridge to the last. These are called after completing
 * &drm_encoder_helper_funcs.atomic_disable
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_atomic_bridge_chain_post_disable(struct drm_bridge *bridge,
					  struct drm_atomic_state *old_state)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (bridge->funcs->atomic_post_disable) {
			struct drm_bridge_state *old_bridge_state;

			old_bridge_state =
				drm_atomic_get_old_bridge_state(old_state,
								bridge);
			if (WARN_ON(!old_bridge_state))
				return;

			bridge->funcs->atomic_post_disable(bridge,
							   old_bridge_state);
		} else if (bridge->funcs->post_disable) {
			bridge->funcs->post_disable(bridge);
		}
	}
}
EXPORT_SYMBOL(drm_atomic_bridge_chain_post_disable);

/**
 * drm_atomic_bridge_chain_pre_enable - prepares for enabling all bridges in
 *					the encoder chain
 * @bridge: bridge control structure
 * @old_state: old atomic state
 *
 * Calls &drm_bridge_funcs.atomic_pre_enable (falls back on
 * &drm_bridge_funcs.pre_enable) op for all the bridges in the encoder chain,
 * starting from the last bridge to the first. These are called before calling
 * &drm_encoder_helper_funcs.atomic_enable
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_atomic_bridge_chain_pre_enable(struct drm_bridge *bridge,
					struct drm_atomic_state *old_state)
{
	struct drm_encoder *encoder;
	struct drm_bridge *iter;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_reverse(iter, &encoder->bridge_chain, chain_node) {
		if (iter->funcs->atomic_pre_enable) {
			struct drm_bridge_state *old_bridge_state;

			old_bridge_state =
				drm_atomic_get_old_bridge_state(old_state,
								iter);
			if (WARN_ON(!old_bridge_state))
				return;

			iter->funcs->atomic_pre_enable(iter, old_bridge_state);
		} else if (iter->funcs->pre_enable) {
			iter->funcs->pre_enable(iter);
		}

		if (iter == bridge)
			break;
	}
}
EXPORT_SYMBOL(drm_atomic_bridge_chain_pre_enable);

/**
 * drm_atomic_bridge_chain_enable - enables all bridges in the encoder chain
 * @bridge: bridge control structure
 * @old_state: old atomic state
 *
 * Calls &drm_bridge_funcs.atomic_enable (falls back on
 * &drm_bridge_funcs.enable) op for all the bridges in the encoder chain,
 * starting from the first bridge to the last. These are called after completing
 * &drm_encoder_helper_funcs.atomic_enable
 *
 * Note: the bridge passed should be the one closest to the encoder
 */
void drm_atomic_bridge_chain_enable(struct drm_bridge *bridge,
				    struct drm_atomic_state *old_state)
{
	struct drm_encoder *encoder;

	if (!bridge)
		return;

	encoder = bridge->encoder;
	list_for_each_entry_from(bridge, &encoder->bridge_chain, chain_node) {
		if (bridge->funcs->atomic_enable) {
			struct drm_bridge_state *old_bridge_state;

			old_bridge_state =
				drm_atomic_get_old_bridge_state(old_state,
								bridge);
			if (WARN_ON(!old_bridge_state))
				return;

			bridge->funcs->atomic_enable(bridge, old_bridge_state);
		} else if (bridge->funcs->enable) {
			bridge->funcs->enable(bridge);
		}
	}
}
EXPORT_SYMBOL(drm_atomic_bridge_chain_enable);

static int drm_atomic_bridge_check(struct drm_bridge *bridge,
				   struct drm_crtc_state *crtc_state,
				   struct drm_connector_state *conn_state)
{
	if (bridge->funcs->atomic_check) {
		struct drm_bridge_state *bridge_state;
		int ret;

		bridge_state = drm_atomic_get_new_bridge_state(crtc_state->state,
							       bridge);
		if (WARN_ON(!bridge_state))
			return -EINVAL;

		ret = bridge->funcs->atomic_check(bridge, bridge_state,
						  crtc_state, conn_state);
		if (ret)
			return ret;
	} else if (bridge->funcs->mode_fixup) {
		if (!bridge->funcs->mode_fixup(bridge, &crtc_state->mode,
					       &crtc_state->adjusted_mode))
			return -EINVAL;
	}

	return 0;
}

static int select_bus_fmt_recursive(struct drm_bridge *first_bridge,
				    struct drm_bridge *cur_bridge,
				    struct drm_crtc_state *crtc_state,
				    struct drm_connector_state *conn_state,
				    u32 out_bus_fmt)
{
	struct drm_bridge_state *cur_state;
	unsigned int num_in_bus_fmts, i;
	struct drm_bridge *prev_bridge;
	u32 *in_bus_fmts;
	int ret;

	prev_bridge = drm_bridge_get_prev_bridge(cur_bridge);
	cur_state = drm_atomic_get_new_bridge_state(crtc_state->state,
						    cur_bridge);

	/*
	 * If bus format negotiation is not supported by this bridge, let's
	 * pass MEDIA_BUS_FMT_FIXED to the previous bridge in the chain and
	 * hope that it can handle this situation gracefully (by providing
	 * appropriate default values).
	 */
	if (!cur_bridge->funcs->atomic_get_input_bus_fmts) {
		if (cur_bridge != first_bridge) {
			ret = select_bus_fmt_recursive(first_bridge,
						       prev_bridge, crtc_state,
						       conn_state,
						       MEDIA_BUS_FMT_FIXED);
			if (ret)
				return ret;
		}

		/*
		 * Driver does not implement the atomic state hooks, but that's
		 * fine, as long as it does not access the bridge state.
		 */
		if (cur_state) {
			cur_state->input_bus_cfg.format = MEDIA_BUS_FMT_FIXED;
			cur_state->output_bus_cfg.format = out_bus_fmt;
		}

		return 0;
	}

	/*
	 * If the driver implements ->atomic_get_input_bus_fmts() it
	 * should also implement the atomic state hooks.
	 */
	if (WARN_ON(!cur_state))
		return -EINVAL;

	in_bus_fmts = cur_bridge->funcs->atomic_get_input_bus_fmts(cur_bridge,
							cur_state,
							crtc_state,
							conn_state,
							out_bus_fmt,
							&num_in_bus_fmts);
	if (!num_in_bus_fmts)
		return -ENOTSUPP;
	else if (!in_bus_fmts)
		return -ENOMEM;

	if (first_bridge == cur_bridge) {
		cur_state->input_bus_cfg.format = in_bus_fmts[0];
		cur_state->output_bus_cfg.format = out_bus_fmt;
		kfree(in_bus_fmts);
		return 0;
	}

	for (i = 0; i < num_in_bus_fmts; i++) {
		ret = select_bus_fmt_recursive(first_bridge, prev_bridge,
					       crtc_state, conn_state,
					       in_bus_fmts[i]);
		if (ret != -ENOTSUPP)
			break;
	}

	if (!ret) {
		cur_state->input_bus_cfg.format = in_bus_fmts[i];
		cur_state->output_bus_cfg.format = out_bus_fmt;
	}

	kfree(in_bus_fmts);
	return ret;
}

/*
 * This function is called by &drm_atomic_bridge_chain_check() just before
 * calling &drm_bridge_funcs.atomic_check() on all elements of the chain.
 * It performs bus format negotiation between bridge elements. The negotiation
 * happens in reverse order, starting from the last element in the chain up to
 * @bridge.
 *
 * Negotiation starts by retrieving supported output bus formats on the last
 * bridge element and testing them one by one. The test is recursive, meaning
 * that for each tested output format, the whole chain will be walked backward,
 * and each element will have to choose an input bus format that can be
 * transcoded to the requested output format. When a bridge element does not
 * support transcoding into a specific output format -ENOTSUPP is returned and
 * the next bridge element will have to try a different format. If none of the
 * combinations worked, -ENOTSUPP is returned and the atomic modeset will fail.
 *
 * This implementation is relying on
 * &drm_bridge_funcs.atomic_get_output_bus_fmts() and
 * &drm_bridge_funcs.atomic_get_input_bus_fmts() to gather supported
 * input/output formats.
 *
 * When &drm_bridge_funcs.atomic_get_output_bus_fmts() is not implemented by
 * the last element of the chain, &drm_atomic_bridge_chain_select_bus_fmts()
 * tries a single format: &drm_connector.display_info.bus_formats[0] if
 * available, MEDIA_BUS_FMT_FIXED otherwise.
 *
 * When &drm_bridge_funcs.atomic_get_input_bus_fmts() is not implemented,
 * &drm_atomic_bridge_chain_select_bus_fmts() skips the negotiation on the
 * bridge element that lacks this hook and asks the previous element in the
 * chain to try MEDIA_BUS_FMT_FIXED. It's up to bridge drivers to decide what
 * to do in that case (fail if they want to enforce bus format negotiation, or
 * provide a reasonable default if they need to support pipelines where not
 * all elements support bus format negotiation).
 */
static int
drm_atomic_bridge_chain_select_bus_fmts(struct drm_bridge *bridge,
					struct drm_crtc_state *crtc_state,
					struct drm_connector_state *conn_state)
{
	struct drm_connector *conn = conn_state->connector;
	struct drm_encoder *encoder = bridge->encoder;
	struct drm_bridge_state *last_bridge_state;
	unsigned int i, num_out_bus_fmts;
	struct drm_bridge *last_bridge;
	u32 *out_bus_fmts;
	int ret = 0;

	last_bridge = list_last_entry(&encoder->bridge_chain,
				      struct drm_bridge, chain_node);
	last_bridge_state = drm_atomic_get_new_bridge_state(crtc_state->state,
							    last_bridge);

	if (last_bridge->funcs->atomic_get_output_bus_fmts) {
		const struct drm_bridge_funcs *funcs = last_bridge->funcs;

		/*
		 * If the driver implements ->atomic_get_output_bus_fmts() it
		 * should also implement the atomic state hooks.
		 */
		if (WARN_ON(!last_bridge_state))
			return -EINVAL;

		out_bus_fmts = funcs->atomic_get_output_bus_fmts(last_bridge,
							last_bridge_state,
							crtc_state,
							conn_state,
							&num_out_bus_fmts);
		if (!num_out_bus_fmts)
			return -ENOTSUPP;
		else if (!out_bus_fmts)
			return -ENOMEM;
	} else {
		num_out_bus_fmts = 1;
		out_bus_fmts = kmalloc(sizeof(*out_bus_fmts), GFP_KERNEL);
		if (!out_bus_fmts)
			return -ENOMEM;

		if (conn->display_info.num_bus_formats &&
		    conn->display_info.bus_formats)
			out_bus_fmts[0] = conn->display_info.bus_formats[0];
		else
			out_bus_fmts[0] = MEDIA_BUS_FMT_FIXED;
	}

	for (i = 0; i < num_out_bus_fmts; i++) {
		ret = select_bus_fmt_recursive(bridge, last_bridge, crtc_state,
					       conn_state, out_bus_fmts[i]);
		if (ret != -ENOTSUPP)
			break;
	}

	kfree(out_bus_fmts);

	return ret;
}

static void
drm_atomic_bridge_propagate_bus_flags(struct drm_bridge *bridge,
				      struct drm_connector *conn,
				      struct drm_atomic_state *state)
{
	struct drm_bridge_state *bridge_state, *next_bridge_state;
	struct drm_bridge *next_bridge;
	u32 output_flags = 0;

	bridge_state = drm_atomic_get_new_bridge_state(state, bridge);

	/* No bridge state attached to this bridge => nothing to propagate. */
	if (!bridge_state)
		return;

	next_bridge = drm_bridge_get_next_bridge(bridge);

	/*
	 * Let's try to apply the most common case here, that is, propagate
	 * display_info flags for the last bridge, and propagate the input
	 * flags of the next bridge element to the output end of the current
	 * bridge when the bridge is not the last one.
	 * There are exceptions to this rule, like when signal inversion is
	 * happening at the board level, but that's something drivers can deal
	 * with from their &drm_bridge_funcs.atomic_check() implementation by
	 * simply overriding the flags value we've set here.
	 */
	if (!next_bridge) {
		output_flags = conn->display_info.bus_flags;
	} else {
		next_bridge_state = drm_atomic_get_new_bridge_state(state,
								next_bridge);
		/*
		 * No bridge state attached to the next bridge, just leave the
		 * flags to 0.
		 */
		if (next_bridge_state)
			output_flags = next_bridge_state->input_bus_cfg.flags;
	}

	bridge_state->output_bus_cfg.flags = output_flags;

	/*
	 * Propage the output flags to the input end of the bridge. Again, it's
	 * not necessarily what all bridges want, but that's what most of them
	 * do, and by doing that by default we avoid forcing drivers to
	 * duplicate the "dummy propagation" logic.
	 */
	bridge_state->input_bus_cfg.flags = output_flags;
}

/**
 * drm_atomic_bridge_chain_check() - Do an atomic check on the bridge chain
 * @bridge: bridge control structure
 * @crtc_state: new CRTC state
 * @conn_state: new connector state
 *
 * First trigger a bus format negotiation before calling
 * &drm_bridge_funcs.atomic_check() (falls back on
 * &drm_bridge_funcs.mode_fixup()) op for all the bridges in the encoder chain,
 * starting from the last bridge to the first. These are called before calling
 * &drm_encoder_helper_funcs.atomic_check()
 *
 * RETURNS:
 * 0 on success, a negative error code on failure
 */
int drm_atomic_bridge_chain_check(struct drm_bridge *bridge,
				  struct drm_crtc_state *crtc_state,
				  struct drm_connector_state *conn_state)
{
	struct drm_connector *conn = conn_state->connector;
	struct drm_encoder *encoder;
	struct drm_bridge *iter;
	int ret;

	if (!bridge)
		return 0;

	ret = drm_atomic_bridge_chain_select_bus_fmts(bridge, crtc_state,
						      conn_state);
	if (ret)
		return ret;

	encoder = bridge->encoder;
	list_for_each_entry_reverse(iter, &encoder->bridge_chain, chain_node) {
		int ret;

		/*
		 * Bus flags are propagated by default. If a bridge needs to
		 * tweak the input bus flags for any reason, it should happen
		 * in its &drm_bridge_funcs.atomic_check() implementation such
		 * that preceding bridges in the chain can propagate the new
		 * bus flags.
		 */
		drm_atomic_bridge_propagate_bus_flags(iter, conn,
						      crtc_state->state);

		ret = drm_atomic_bridge_check(iter, crtc_state, conn_state);
		if (ret)
			return ret;

		if (iter == bridge)
			break;
	}

	return 0;
}
EXPORT_SYMBOL(drm_atomic_bridge_chain_check);

#ifdef CONFIG_OF
/**
 * of_drm_find_bridge - find the bridge corresponding to the device node in
 *			the global bridge list
 *
 * @np: device node
 *
 * RETURNS:
 * drm_bridge control struct on success, NULL on failure
 */
struct drm_bridge *of_drm_find_bridge(struct device_node *np)
{
	struct drm_bridge *bridge;

	mutex_lock(&bridge_lock);

	list_for_each_entry(bridge, &bridge_list, list) {
		if (bridge->of_node == np) {
			mutex_unlock(&bridge_lock);
			return bridge;
		}
	}

	mutex_unlock(&bridge_lock);
	return NULL;
}
EXPORT_SYMBOL(of_drm_find_bridge);
#endif

MODULE_AUTHOR("Ajay Kumar <ajaykumar.rs@samsung.com>");
MODULE_DESCRIPTION("DRM bridge infrastructure");
MODULE_LICENSE("GPL and additional rights");

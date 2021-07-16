/*
 * Copyright (c) 2016 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef __DRM_BRIDGE_H__
#define __DRM_BRIDGE_H__

#include <linux/list.h>
#include <linux/ctype.h>

#include <drm/drm_atomic.h>
#include <drm/drm_encoder.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_modes.h>

struct drm_bridge;
struct drm_bridge_timings;
struct drm_panel;

/**
 * struct drm_bridge_state - Atomic bridge state object
 * @base: inherit from &drm_private_state
 * @bridge: the bridge this state refers to
 */
struct drm_bridge_state {
	struct drm_private_state base;

	struct drm_bridge *bridge;
};

static inline struct drm_bridge_state *
drm_priv_to_bridge_state(struct drm_private_state *priv)
{
	return container_of(priv, struct drm_bridge_state, base);
}

/**
 * struct drm_bridge_funcs - drm_bridge control functions
 */
struct drm_bridge_funcs {
	/**
	 * @attach:
	 *
	 * This callback is invoked whenever our bridge is being attached to a
	 * &drm_encoder.
	 *
	 * The @attach callback is optional.
	 *
	 * RETURNS:
	 *
	 * Zero on success, error code on failure.
	 */
	int (*attach)(struct drm_bridge *bridge);

	/**
	 * @detach:
	 *
	 * This callback is invoked whenever our bridge is being detached from a
	 * &drm_encoder.
	 *
	 * The @detach callback is optional.
	 */
	void (*detach)(struct drm_bridge *bridge);

	/**
	 * @mode_valid:
	 *
	 * This callback is used to check if a specific mode is valid in this
	 * bridge. This should be implemented if the bridge has some sort of
	 * restriction in the modes it can display. For example, a given bridge
	 * may be responsible to set a clock value. If the clock can not
	 * produce all the values for the available modes then this callback
	 * can be used to restrict the number of modes to only the ones that
	 * can be displayed.
	 *
	 * This hook is used by the probe helpers to filter the mode list in
	 * drm_helper_probe_single_connector_modes(), and it is used by the
	 * atomic helpers to validate modes supplied by userspace in
	 * drm_atomic_helper_check_modeset().
	 *
	 * The @mode_valid callback is optional.
	 *
	 * NOTE:
	 *
	 * Since this function is both called from the check phase of an atomic
	 * commit, and the mode validation in the probe paths it is not allowed
	 * to look at anything else but the passed-in mode, and validate it
	 * against configuration-invariant hardward constraints. Any further
	 * limits which depend upon the configuration can only be checked in
	 * @mode_fixup.
	 *
	 * RETURNS:
	 *
	 * drm_mode_status Enum
	 */
	enum drm_mode_status (*mode_valid)(struct drm_bridge *bridge,
					   const struct drm_display_mode *mode);

	/**
	 * @mode_fixup:
	 *
	 * This callback is used to validate and adjust a mode. The parameter
	 * mode is the display mode that should be fed to the next element in
	 * the display chain, either the final &drm_connector or the next
	 * &drm_bridge. The parameter adjusted_mode is the input mode the bridge
	 * requires. It can be modified by this callback and does not need to
	 * match mode. See also &drm_crtc_state.adjusted_mode for more details.
	 *
	 * This is the only hook that allows a bridge to reject a modeset. If
	 * this function passes all other callbacks must succeed for this
	 * configuration.
	 *
	 * The mode_fixup callback is optional. &drm_bridge_funcs.mode_fixup()
	 * is not called when &drm_bridge_funcs.atomic_check() is implemented,
	 * so only one of them should be provided.
	 *
	 * NOTE:
	 *
	 * This function is called in the check phase of atomic modesets, which
	 * can be aborted for any reason (including on userspace's request to
	 * just check whether a configuration would be possible). Drivers MUST
	 * NOT touch any persistent state (hardware or software) or data
	 * structures except the passed in @state parameter.
	 *
	 * Also beware that userspace can request its own custom modes, neither
	 * core nor helpers filter modes to the list of probe modes reported by
	 * the GETCONNECTOR IOCTL and stored in &drm_connector.modes. To ensure
	 * that modes are filtered consistently put any bridge constraints and
	 * limits checks into @mode_valid.
	 *
	 * RETURNS:
	 *
	 * True if an acceptable configuration is possible, false if the modeset
	 * operation should be rejected.
	 */
	bool (*mode_fixup)(struct drm_bridge *bridge,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	/**
	 * @disable:
	 *
	 * This callback should disable the bridge. It is called right before
	 * the preceding element in the display pipe is disabled. If the
	 * preceding element is a bridge this means it's called before that
	 * bridge's @disable vfunc. If the preceding element is a &drm_encoder
	 * it's called right before the &drm_encoder_helper_funcs.disable,
	 * &drm_encoder_helper_funcs.prepare or &drm_encoder_helper_funcs.dpms
	 * hook.
	 *
	 * The bridge can assume that the display pipe (i.e. clocks and timing
	 * signals) feeding it is still running when this callback is called.
	 *
	 * The @disable callback is optional.
	 */
	void (*disable)(struct drm_bridge *bridge);

	/**
	 * @post_disable:
	 *
	 * This callback should disable the bridge. It is called right after the
	 * preceding element in the display pipe is disabled. If the preceding
	 * element is a bridge this means it's called after that bridge's
	 * @post_disable function. If the preceding element is a &drm_encoder
	 * it's called right after the encoder's
	 * &drm_encoder_helper_funcs.disable, &drm_encoder_helper_funcs.prepare
	 * or &drm_encoder_helper_funcs.dpms hook.
	 *
	 * The bridge must assume that the display pipe (i.e. clocks and timing
	 * singals) feeding it is no longer running when this callback is
	 * called.
	 *
	 * The @post_disable callback is optional.
	 */
	void (*post_disable)(struct drm_bridge *bridge);

	/**
	 * @mode_set:
	 *
	 * This callback should set the given mode on the bridge. It is called
	 * after the @mode_set callback for the preceding element in the display
	 * pipeline has been called already. If the bridge is the first element
	 * then this would be &drm_encoder_helper_funcs.mode_set. The display
	 * pipe (i.e.  clocks and timing signals) is off when this function is
	 * called.
	 *
	 * The adjusted_mode parameter is the mode output by the CRTC for the
	 * first bridge in the chain. It can be different from the mode
	 * parameter that contains the desired mode for the connector at the end
	 * of the bridges chain, for instance when the first bridge in the chain
	 * performs scaling. The adjusted mode is mostly useful for the first
	 * bridge in the chain and is likely irrelevant for the other bridges.
	 *
	 * For atomic drivers the adjusted_mode is the mode stored in
	 * &drm_crtc_state.adjusted_mode.
	 *
	 * NOTE:
	 *
	 * If a need arises to store and access modes adjusted for other
	 * locations than the connection between the CRTC and the first bridge,
	 * the DRM framework will have to be extended with DRM bridge states.
	 */
	void (*mode_set)(struct drm_bridge *bridge,
			 const struct drm_display_mode *mode,
			 const struct drm_display_mode *adjusted_mode);
	/**
	 * @pre_enable:
	 *
	 * This callback should enable the bridge. It is called right before
	 * the preceding element in the display pipe is enabled. If the
	 * preceding element is a bridge this means it's called before that
	 * bridge's @pre_enable function. If the preceding element is a
	 * &drm_encoder it's called right before the encoder's
	 * &drm_encoder_helper_funcs.enable, &drm_encoder_helper_funcs.commit or
	 * &drm_encoder_helper_funcs.dpms hook.
	 *
	 * The display pipe (i.e. clocks and timing signals) feeding this bridge
	 * will not yet be running when this callback is called. The bridge must
	 * not enable the display link feeding the next bridge in the chain (if
	 * there is one) when this callback is called.
	 *
	 * The @pre_enable callback is optional.
	 */
	void (*pre_enable)(struct drm_bridge *bridge);

	/**
	 * @enable:
	 *
	 * This callback should enable the bridge. It is called right after
	 * the preceding element in the display pipe is enabled. If the
	 * preceding element is a bridge this means it's called after that
	 * bridge's @enable function. If the preceding element is a
	 * &drm_encoder it's called right after the encoder's
	 * &drm_encoder_helper_funcs.enable, &drm_encoder_helper_funcs.commit or
	 * &drm_encoder_helper_funcs.dpms hook.
	 *
	 * The bridge can assume that the display pipe (i.e. clocks and timing
	 * signals) feeding it is running when this callback is called. This
	 * callback must enable the display link feeding the next bridge in the
	 * chain if there is one.
	 *
	 * The @enable callback is optional.
	 */
	void (*enable)(struct drm_bridge *bridge);

	/**
	 * @atomic_pre_enable:
	 *
	 * This callback should enable the bridge. It is called right before
	 * the preceding element in the display pipe is enabled. If the
	 * preceding element is a bridge this means it's called before that
	 * bridge's @atomic_pre_enable or @pre_enable function. If the preceding
	 * element is a &drm_encoder it's called right before the encoder's
	 * &drm_encoder_helper_funcs.atomic_enable hook.
	 *
	 * The display pipe (i.e. clocks and timing signals) feeding this bridge
	 * will not yet be running when this callback is called. The bridge must
	 * not enable the display link feeding the next bridge in the chain (if
	 * there is one) when this callback is called.
	 *
	 * Note that this function will only be invoked in the context of an
	 * atomic commit. It will not be invoked from
	 * &drm_bridge_chain_pre_enable. It would be prudent to also provide an
	 * implementation of @pre_enable if you are expecting driver calls into
	 * &drm_bridge_chain_pre_enable.
	 *
	 * The @atomic_pre_enable callback is optional.
	 */
	void (*atomic_pre_enable)(struct drm_bridge *bridge,
				  struct drm_bridge_state *old_bridge_state);

	/**
	 * @atomic_enable:
	 *
	 * This callback should enable the bridge. It is called right after
	 * the preceding element in the display pipe is enabled. If the
	 * preceding element is a bridge this means it's called after that
	 * bridge's @atomic_enable or @enable function. If the preceding element
	 * is a &drm_encoder it's called right after the encoder's
	 * &drm_encoder_helper_funcs.atomic_enable hook.
	 *
	 * The bridge can assume that the display pipe (i.e. clocks and timing
	 * signals) feeding it is running when this callback is called. This
	 * callback must enable the display link feeding the next bridge in the
	 * chain if there is one.
	 *
	 * Note that this function will only be invoked in the context of an
	 * atomic commit. It will not be invoked from &drm_bridge_chain_enable.
	 * It would be prudent to also provide an implementation of @enable if
	 * you are expecting driver calls into &drm_bridge_chain_enable.
	 *
	 * The @atomic_enable callback is optional.
	 */
	void (*atomic_enable)(struct drm_bridge *bridge,
			      struct drm_bridge_state *old_bridge_state);
	/**
	 * @atomic_disable:
	 *
	 * This callback should disable the bridge. It is called right before
	 * the preceding element in the display pipe is disabled. If the
	 * preceding element is a bridge this means it's called before that
	 * bridge's @atomic_disable or @disable vfunc. If the preceding element
	 * is a &drm_encoder it's called right before the
	 * &drm_encoder_helper_funcs.atomic_disable hook.
	 *
	 * The bridge can assume that the display pipe (i.e. clocks and timing
	 * signals) feeding it is still running when this callback is called.
	 *
	 * Note that this function will only be invoked in the context of an
	 * atomic commit. It will not be invoked from
	 * &drm_bridge_chain_disable. It would be prudent to also provide an
	 * implementation of @disable if you are expecting driver calls into
	 * &drm_bridge_chain_disable.
	 *
	 * The @atomic_disable callback is optional.
	 */
	void (*atomic_disable)(struct drm_bridge *bridge,
			       struct drm_bridge_state *old_bridge_state);

	/**
	 * @atomic_post_disable:
	 *
	 * This callback should disable the bridge. It is called right after the
	 * preceding element in the display pipe is disabled. If the preceding
	 * element is a bridge this means it's called after that bridge's
	 * @atomic_post_disable or @post_disable function. If the preceding
	 * element is a &drm_encoder it's called right after the encoder's
	 * &drm_encoder_helper_funcs.atomic_disable hook.
	 *
	 * The bridge must assume that the display pipe (i.e. clocks and timing
	 * signals) feeding it is no longer running when this callback is
	 * called.
	 *
	 * Note that this function will only be invoked in the context of an
	 * atomic commit. It will not be invoked from
	 * &drm_bridge_chain_post_disable.
	 * It would be prudent to also provide an implementation of
	 * @post_disable if you are expecting driver calls into
	 * &drm_bridge_chain_post_disable.
	 *
	 * The @atomic_post_disable callback is optional.
	 */
	void (*atomic_post_disable)(struct drm_bridge *bridge,
				    struct drm_bridge_state *old_bridge_state);

	/**
	 * @atomic_duplicate_state:
	 *
	 * Duplicate the current bridge state object (which is guaranteed to be
	 * non-NULL).
	 *
	 * The atomic_duplicate_state() is optional. When not implemented the
	 * core allocates a drm_bridge_state object and calls
	 * &__drm_atomic_helper_bridge_duplicate_state() to initialize it.
	 *
	 * RETURNS:
	 * A valid drm_bridge_state object or NULL if the allocation fails.
	 */
	struct drm_bridge_state *(*atomic_duplicate_state)(struct drm_bridge *bridge);

	/**
	 * @atomic_destroy_state:
	 *
	 * Destroy a bridge state object previously allocated by
	 * &drm_bridge_funcs.atomic_duplicate_state().
	 *
	 * The atomic_destroy_state hook is optional. When not implemented the
	 * core calls kfree() on the state.
	 */
	void (*atomic_destroy_state)(struct drm_bridge *bridge,
				     struct drm_bridge_state *state);

	/**
	 * @atomic_check:
	 *
	 * This method is responsible for checking bridge state correctness.
	 * It can also check the state of the surrounding components in chain
	 * to make sure the whole pipeline can work properly.
	 *
	 * &drm_bridge_funcs.atomic_check() hooks are called in reverse
	 * order (from the last to the first bridge).
	 *
	 * This method is optional. &drm_bridge_funcs.mode_fixup() is not
	 * called when &drm_bridge_funcs.atomic_check() is implemented, so only
	 * one of them should be provided.
	 *
	 * RETURNS:
	 * zero if the check passed, a negative error code otherwise.
	 */
	int (*atomic_check)(struct drm_bridge *bridge,
			    struct drm_bridge_state *bridge_state,
			    struct drm_crtc_state *crtc_state,
			    struct drm_connector_state *conn_state);

	/**
	 * @atomic_reset:
	 *
	 * Reset the bridge to a predefined state (or retrieve its current
	 * state) and return a &drm_bridge_state object matching this state.
	 * This function is called at attach time.
	 *
	 * The atomic_reset hook is optional. When not implemented the core
	 * allocates a new state and calls &__drm_atomic_helper_bridge_reset().
	 *
	 * RETURNS:
	 * A valid drm_bridge_state object in case of success, an ERR_PTR()
	 * giving the reason of the failure otherwise.
	 */
	struct drm_bridge_state *(*atomic_reset)(struct drm_bridge *bridge);
};

/**
 * struct drm_bridge_timings - timing information for the bridge
 */
struct drm_bridge_timings {
	/**
	 * @input_bus_flags:
	 *
	 * Tells what additional settings for the pixel data on the bus
	 * this bridge requires (like pixel signal polarity). See also
	 * &drm_display_info->bus_flags.
	 */
	u32 input_bus_flags;
	/**
	 * @setup_time_ps:
	 *
	 * Defines the time in picoseconds the input data lines must be
	 * stable before the clock edge.
	 */
	u32 setup_time_ps;
	/**
	 * @hold_time_ps:
	 *
	 * Defines the time in picoseconds taken for the bridge to sample the
	 * input signal after the clock edge.
	 */
	u32 hold_time_ps;
	/**
	 * @dual_link:
	 *
	 * True if the bus operates in dual-link mode. The exact meaning is
	 * dependent on the bus type. For LVDS buses, this indicates that even-
	 * and odd-numbered pixels are received on separate links.
	 */
	bool dual_link;
};

/**
 * struct drm_bridge - central DRM bridge control structure
 */
struct drm_bridge {
	/** @base: inherit from &drm_private_object */
	struct drm_private_obj base;
	/** @dev: DRM device this bridge belongs to */
	struct drm_device *dev;
	/** @encoder: encoder to which this bridge is connected */
	struct drm_encoder *encoder;
	/** @chain_node: used to form a bridge chain */
	struct list_head chain_node;
#ifdef CONFIG_OF
	/** @of_node: device node pointer to the bridge */
	struct device_node *of_node;
#endif
	/** @list: to keep track of all added bridges */
	struct list_head list;
	/**
	 * @timings:
	 *
	 * the timing specification for the bridge, if any (may be NULL)
	 */
	const struct drm_bridge_timings *timings;
	/** @funcs: control functions */
	const struct drm_bridge_funcs *funcs;
	/** @driver_private: pointer to the bridge driver's internal context */
	void *driver_private;
};

static inline struct drm_bridge *
drm_priv_to_bridge(struct drm_private_obj *priv)
{
	return container_of(priv, struct drm_bridge, base);
}

void drm_bridge_add(struct drm_bridge *bridge);
void drm_bridge_remove(struct drm_bridge *bridge);
#ifdef __linux__
struct drm_bridge *of_drm_find_bridge(struct device_node *np);
#endif
int drm_bridge_attach(struct drm_encoder *encoder, struct drm_bridge *bridge,
		      struct drm_bridge *previous);

/**
 * drm_bridge_get_next_bridge() - Get the next bridge in the chain
 * @bridge: bridge object
 *
 * RETURNS:
 * the next bridge in the chain after @bridge, or NULL if @bridge is the last.
 */
static inline struct drm_bridge *
drm_bridge_get_next_bridge(struct drm_bridge *bridge)
{
	if (list_is_last(&bridge->chain_node, &bridge->encoder->bridge_chain))
		return NULL;

	return list_next_entry(bridge, chain_node);
}

/**
 * drm_bridge_get_prev_bridge() - Get the previous bridge in the chain
 * @bridge: bridge object
 *
 * RETURNS:
 * the previous bridge in the chain, or NULL if @bridge is the first.
 */
static inline struct drm_bridge *
drm_bridge_get_prev_bridge(struct drm_bridge *bridge)
{
	if (list_is_first(&bridge->chain_node, &bridge->encoder->bridge_chain))
		return NULL;

	return list_prev_entry(bridge, chain_node);
}

/**
 * drm_bridge_chain_get_first_bridge() - Get the first bridge in the chain
 * @encoder: encoder object
 *
 * RETURNS:
 * the first bridge in the chain, or NULL if @encoder has no bridge attached
 * to it.
 */
static inline struct drm_bridge *
drm_bridge_chain_get_first_bridge(struct drm_encoder *encoder)
{
	return list_first_entry_or_null(&encoder->bridge_chain,
					struct drm_bridge, chain_node);
}

/**
 * drm_for_each_bridge_in_chain() - Iterate over all bridges present in a chain
 * @encoder: the encoder to iterate bridges on
 * @bridge: a bridge pointer updated to point to the current bridge at each
 *	    iteration
 *
 * Iterate over all bridges present in the bridge chain attached to @encoder.
 */
#define drm_for_each_bridge_in_chain(encoder, bridge)			\
	list_for_each_entry(bridge, &(encoder)->bridge_chain, chain_node)

bool drm_bridge_chain_mode_fixup(struct drm_bridge *bridge,
				 const struct drm_display_mode *mode,
				 struct drm_display_mode *adjusted_mode);
enum drm_mode_status
drm_bridge_chain_mode_valid(struct drm_bridge *bridge,
			    const struct drm_display_mode *mode);
void drm_bridge_chain_disable(struct drm_bridge *bridge);
void drm_bridge_chain_post_disable(struct drm_bridge *bridge);
void drm_bridge_chain_mode_set(struct drm_bridge *bridge,
			       const struct drm_display_mode *mode,
			       const struct drm_display_mode *adjusted_mode);
void drm_bridge_chain_pre_enable(struct drm_bridge *bridge);
void drm_bridge_chain_enable(struct drm_bridge *bridge);

int drm_atomic_bridge_chain_check(struct drm_bridge *bridge,
				  struct drm_crtc_state *crtc_state,
				  struct drm_connector_state *conn_state);
void drm_atomic_bridge_chain_disable(struct drm_bridge *bridge,
				     struct drm_atomic_state *state);
void drm_atomic_bridge_chain_post_disable(struct drm_bridge *bridge,
					  struct drm_atomic_state *state);
void drm_atomic_bridge_chain_pre_enable(struct drm_bridge *bridge,
					struct drm_atomic_state *state);
void drm_atomic_bridge_chain_enable(struct drm_bridge *bridge,
				    struct drm_atomic_state *state);

void __drm_atomic_helper_bridge_reset(struct drm_bridge *bridge,
				      struct drm_bridge_state *state);
void __drm_atomic_helper_bridge_duplicate_state(struct drm_bridge *bridge,
						struct drm_bridge_state *new);

static inline struct drm_bridge_state *
drm_atomic_get_bridge_state(struct drm_atomic_state *state,
			    struct drm_bridge *bridge)
{
	struct drm_private_state *obj_state;

	obj_state = drm_atomic_get_private_obj_state(state, &bridge->base);
	if (IS_ERR(obj_state))
		return ERR_CAST(obj_state);

	return drm_priv_to_bridge_state(obj_state);
}

static inline struct drm_bridge_state *
drm_atomic_get_old_bridge_state(struct drm_atomic_state *state,
				struct drm_bridge *bridge)
{
	struct drm_private_state *obj_state;

	obj_state = drm_atomic_get_old_private_obj_state(state, &bridge->base);
	if (!obj_state)
		return NULL;

	return drm_priv_to_bridge_state(obj_state);
}

static inline struct drm_bridge_state *
drm_atomic_get_new_bridge_state(struct drm_atomic_state *state,
				struct drm_bridge *bridge)
{
	struct drm_private_state *obj_state;

	obj_state = drm_atomic_get_new_private_obj_state(state, &bridge->base);
	if (!obj_state)
		return NULL;

	return drm_priv_to_bridge_state(obj_state);
}

#ifdef CONFIG_DRM_PANEL_BRIDGE
struct drm_bridge *drm_panel_bridge_add(struct drm_panel *panel);
struct drm_bridge *drm_panel_bridge_add_typed(struct drm_panel *panel,
					      u32 connector_type);
void drm_panel_bridge_remove(struct drm_bridge *bridge);
struct drm_bridge *devm_drm_panel_bridge_add(struct device *dev,
					     struct drm_panel *panel);
struct drm_bridge *devm_drm_panel_bridge_add_typed(struct device *dev,
						   struct drm_panel *panel,
						   u32 connector_type);
struct drm_connector *drm_panel_bridge_connector(struct drm_bridge *bridge);
#endif

#endif

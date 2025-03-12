# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm

.PATH:	${SRCDIR} ${SRCDIR}/display ${SRCDIR}/scheduler

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	drm
SRCS=	drm_atomic.c \
	drm_atomic_helper.c \
	drm_atomic_state_helper.c \
	drm_atomic_uapi.c \
	drm_aperture.c \
	drm_auth.c \
	drm_buddy.c \
	drm_blend.c \
	drm_bridge.c \
	drm_cache.c \
	drm_client.c \
	drm_client_modeset.c \
	drm_color_mgmt.c \
	drm_connector.c \
	drm_crtc.c \
	drm_crtc_helper.c \
	drm_damage_helper.c \
	drm_drv.c \
	drm_dumb_buffers.c \
	drm_edid.c \
	drm_displayid.c \
	drm_encoder.c \
	drm_file.c \
	drm_flip_work.c \
	drm_fourcc.c \
	drm_framebuffer.c \
	drm_gem.c \
	drm_gem_atomic_helper.c \
	drm_gem_framebuffer_helper.c \
	drm_hashtab.c \
	drm_ioctl.c \
	drm_lease.c \
	drm_managed.c \
	drm_mm.c \
	drm_modes.c \
	drm_mode_config.c \
	drm_mode_object.c \
	drm_modeset_lock.c \
	drm_modeset_helper.c \
	drm_os_freebsd.c \
	drm_panel.c \
	drm_pci.c \
	drm_plane.c \
	drm_plane_helper.c \
	drm_print.c \
	drm_prime.c \
	drm_probe_helper.c \
	drm_property.c \
	drm_rect.c \
	drm_self_refresh_helper.c \
	drm_suballoc.c \
	drm_syncobj.c \
	drm_sysctl_freebsd.c \
	drm_sysfs.c \
	drm_vblank.c \
	drm_vblank_work.c \
	drm_vma_manager.c \
	linux_fb.c \
	vt_drmfb.c

.if !empty(KCONFIG:MDRM_FBDEV_EMULATION)
SRCS+=	drm_fb_helper.c \
	drm_fbdev_generic.c
.endif

.if !empty(KCONFIG:MDRM_LEGACY)
SRCS+=	drm_agpsupport.c \
	drm_irq.c \
	drm_memory.c
.endif

.if !empty(KCONFIG:MCOMPAT)
SRCS+=	drm_ioc32.c
.endif

.if !empty(KCONFIG:MDEBUG_FS)
SRCS+=	\
	drm_debugfs.c \
	drm_debugfs_crc.c
.endif

.if !empty(KCONFIG:MDRM_MIPI_DSI)
SRCS+=	drm_mipi_dsi.c
.endif

.if !empty(KCONFIG:MDRM_PANEL_ORIENTATION_QUIRKS)
SRCS+=	drm_panel_orientation_quirks.c
.endif

.if !empty(KCONFIG:MDRM_PRIVACY_SCREEN)
SRCS+=	drm_privacy_screen.c \
	drm_privacy_screen_x86.c
.endif

# drm scheduler (moved from amdgpu)
SRCS+=	sched_main.c \
	sched_fence.c \
	sched_entity.c

# drm display (moved from amdgpu)
SRCS+=	drm_scdc_helper.c \
	drm_display_helper_mod.c \
	drm_dp_dual_mode_helper.c \
	drm_dp_mst_topology.c \
	drm_dp_helper.c \
	drm_dsc_helper.c \
	drm_hdcp_helper.c \
	drm_hdmi_helper.c

.if !empty(KCONFIG:MDRM_DP_AUX_CHARDEV)
SRCS+=	drm_dp_aux_dev.c
.endif

.if !empty(KCONFIG:MDRM_EXEC)
SRCS+=	drm_exec.c
.endif

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include
CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=drm_' -DDRM_SYSCTL_PARAM_PREFIX=_dri
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS+=	device_if.h \
	vnode_if.h \
	bus_if.h \
	pci_if.h \
	pci_iov_if.h \
	device_if.h \
	iicbus_if.h \
	opt_teken.h \
	opt_splash.h \
	opt_syscons.h \
	opt_drm.h \
	opt_vm.h \
	opt_syscons.h

.if ${MACHINE_CPUARCH} == "powerpc"
CWARNFLAGS+=-Wno-cast-qual
.endif
CWARNFLAGS+= -Wno-pointer-sign -Wno-format

CWARNFLAGS.drm_ioc32.c+=	-Wno-address-of-packed-member

# drivers/gpu/drm/display/drm_dp_helper.c
EXPORT_SYMS+=	drm_dp_128b132b_cds_interlane_align_done
EXPORT_SYMS+=	drm_dp_128b132b_eq_interlane_align_done
EXPORT_SYMS+=	drm_dp_128b132b_lane_channel_eq_done
EXPORT_SYMS+=	drm_dp_128b132b_lane_symbol_locked
EXPORT_SYMS+=	drm_dp_128b132b_link_training_failed
EXPORT_SYMS+=	drm_dp_128b132b_read_aux_rd_interval
EXPORT_SYMS+=	drm_dp_aux_init
EXPORT_SYMS+=	drm_dp_aux_register
EXPORT_SYMS+=	drm_dp_aux_unregister
EXPORT_SYMS+=	drm_dp_bw_code_to_link_rate
EXPORT_SYMS+=	drm_dp_channel_eq_ok
EXPORT_SYMS+=	drm_dp_clock_recovery_ok
EXPORT_SYMS+=	drm_dp_downstream_420_passthrough
EXPORT_SYMS+=	drm_dp_downstream_444_to_420_conversion
EXPORT_SYMS+=	drm_dp_downstream_debug
EXPORT_SYMS+=	drm_dp_downstream_id
EXPORT_SYMS+=	drm_dp_downstream_is_tmds
EXPORT_SYMS+=	drm_dp_downstream_is_type
EXPORT_SYMS+=	drm_dp_downstream_max_bpc
EXPORT_SYMS+=	drm_dp_downstream_max_dotclock
EXPORT_SYMS+=	drm_dp_downstream_max_tmds_clock
EXPORT_SYMS+=	drm_dp_downstream_min_tmds_clock
EXPORT_SYMS+=	drm_dp_downstream_mode
EXPORT_SYMS+=	drm_dp_downstream_rgb_to_ycbcr_conversion
EXPORT_SYMS+=	drm_dp_dpcd_probe
EXPORT_SYMS+=	drm_dp_dpcd_read
EXPORT_SYMS+=	drm_dp_dpcd_read_link_status
EXPORT_SYMS+=	drm_dp_dpcd_read_phy_link_status
EXPORT_SYMS+=	drm_dp_dpcd_write
EXPORT_SYMS+=	drm_dp_dsc_sink_line_buf_depth
EXPORT_SYMS+=	drm_dp_dsc_sink_max_slice_count
EXPORT_SYMS+=	drm_dp_dsc_sink_supported_input_bpcs
EXPORT_SYMS+=	drm_dp_get_adjust_request_pre_emphasis
EXPORT_SYMS+=	drm_dp_get_adjust_request_voltage
EXPORT_SYMS+=	drm_dp_get_adjust_tx_ffe_preset
EXPORT_SYMS+=	drm_dp_get_pcon_max_frl_bw
EXPORT_SYMS+=	drm_dp_get_phy_test_pattern
EXPORT_SYMS+=	drm_dp_link_rate_to_bw_code
EXPORT_SYMS+=	drm_dp_link_train_channel_eq_delay
EXPORT_SYMS+=	drm_dp_link_train_clock_recovery_delay
EXPORT_SYMS+=	drm_dp_lttpr_count
EXPORT_SYMS+=	drm_dp_lttpr_link_train_channel_eq_delay
EXPORT_SYMS+=	drm_dp_lttpr_link_train_clock_recovery_delay
EXPORT_SYMS+=	drm_dp_lttpr_max_lane_count
EXPORT_SYMS+=	drm_dp_lttpr_max_link_rate
EXPORT_SYMS+=	drm_dp_lttpr_pre_emphasis_level_3_supported
EXPORT_SYMS+=	drm_dp_lttpr_voltage_swing_level_3_supported
EXPORT_SYMS+=	drm_dp_pcon_convert_rgb_to_ycbcr
EXPORT_SYMS+=	drm_dp_pcon_dsc_bpp_incr
EXPORT_SYMS+=	drm_dp_pcon_dsc_max_slice_width
EXPORT_SYMS+=	drm_dp_pcon_dsc_max_slices
EXPORT_SYMS+=	drm_dp_pcon_enc_is_dsc_1_2
EXPORT_SYMS+=	drm_dp_pcon_frl_configure_1
EXPORT_SYMS+=	drm_dp_pcon_frl_configure_2
EXPORT_SYMS+=	drm_dp_pcon_frl_enable
EXPORT_SYMS+=	drm_dp_pcon_frl_prepare
EXPORT_SYMS+=	drm_dp_pcon_hdmi_frl_link_error_count
EXPORT_SYMS+=	drm_dp_pcon_hdmi_link_active
EXPORT_SYMS+=	drm_dp_pcon_hdmi_link_mode
EXPORT_SYMS+=	drm_dp_pcon_is_frl_ready
EXPORT_SYMS+=	drm_dp_pcon_pps_default
EXPORT_SYMS+=	drm_dp_pcon_pps_override_buf
EXPORT_SYMS+=	drm_dp_pcon_pps_override_param
EXPORT_SYMS+=	drm_dp_pcon_reset_frl_config
EXPORT_SYMS+=	drm_dp_phy_name
EXPORT_SYMS+=	drm_dp_psr_setup_time
EXPORT_SYMS+=	drm_dp_read_channel_eq_delay
EXPORT_SYMS+=	drm_dp_read_clock_recovery_delay
EXPORT_SYMS+=	drm_dp_read_desc
EXPORT_SYMS+=	drm_dp_read_downstream_info
EXPORT_SYMS+=	drm_dp_read_dpcd_caps
EXPORT_SYMS+=	drm_dp_read_lttpr_common_caps
EXPORT_SYMS+=	drm_dp_read_lttpr_phy_caps
EXPORT_SYMS+=	drm_dp_read_sink_count
EXPORT_SYMS+=	drm_dp_read_sink_count_cap
EXPORT_SYMS+=	drm_dp_remote_aux_init
EXPORT_SYMS+=	drm_dp_send_real_edid_checksum
EXPORT_SYMS+=	drm_dp_set_phy_test_pattern
EXPORT_SYMS+=	drm_dp_set_subconnector_property
EXPORT_SYMS+=	drm_dp_start_crc
EXPORT_SYMS+=	drm_dp_stop_crc
EXPORT_SYMS+=	drm_dp_subconnector_type
EXPORT_SYMS+=	drm_dp_vsc_sdp_log
EXPORT_SYMS+=	drm_edp_backlight_disable
EXPORT_SYMS+=	drm_edp_backlight_enable
EXPORT_SYMS+=	drm_edp_backlight_init
EXPORT_SYMS+=	drm_edp_backlight_set_level
EXPORT_SYMS+=	drm_panel_dp_aux_backlight

# drivers/gpu/drm/display/drm_dp_dual_mode_helper.c
EXPORT_SYMS+=	drm_dp_dual_mode_detect
EXPORT_SYMS+=	drm_dp_dual_mode_get_tmds_output
EXPORT_SYMS+=	drm_dp_dual_mode_max_tmds_clock
EXPORT_SYMS+=	drm_dp_dual_mode_read
EXPORT_SYMS+=	drm_dp_dual_mode_set_tmds_output
EXPORT_SYMS+=	drm_dp_dual_mode_write
EXPORT_SYMS+=	drm_dp_get_dual_mode_type_name
EXPORT_SYMS+=	drm_lspcon_get_mode
EXPORT_SYMS+=	drm_lspcon_set_mode

# drivers/gpu/drm/display/drm_dp_mst_topology.c
EXPORT_SYMS+=	drm_atomic_get_mst_payload_state
EXPORT_SYMS+=	drm_atomic_get_mst_topology_state
EXPORT_SYMS+=	drm_atomic_get_new_mst_topology_state
EXPORT_SYMS+=	drm_atomic_get_old_mst_topology_state
EXPORT_SYMS+=	drm_dp_add_payload_part1
EXPORT_SYMS+=	drm_dp_add_payload_part2
EXPORT_SYMS+=	drm_dp_atomic_find_time_slots
EXPORT_SYMS+=	drm_dp_atomic_release_time_slots
EXPORT_SYMS+=	drm_dp_calc_pbn_mode
EXPORT_SYMS+=	drm_dp_check_act_status
EXPORT_SYMS+=	drm_dp_decode_sideband_req
EXPORT_SYMS+=	drm_dp_dump_sideband_msg_req_body
EXPORT_SYMS+=	drm_dp_encode_sideband_req
EXPORT_SYMS+=	drm_dp_get_vc_payload_bw
EXPORT_SYMS+=	drm_dp_mst_add_affected_dsc_crtcs
EXPORT_SYMS+=	drm_dp_mst_atomic_check
EXPORT_SYMS+=	drm_dp_mst_atomic_enable_dsc
EXPORT_SYMS+=	drm_dp_mst_atomic_setup_commit
EXPORT_SYMS+=	drm_dp_mst_atomic_wait_for_dependencies
EXPORT_SYMS+=	drm_dp_mst_connector_early_unregister
EXPORT_SYMS+=	drm_dp_mst_connector_late_register
EXPORT_SYMS+=	drm_dp_mst_detect_port
EXPORT_SYMS+=	drm_dp_mst_dsc_aux_for_port
EXPORT_SYMS+=	drm_dp_mst_dump_topology
EXPORT_SYMS+=	drm_dp_mst_edid_read
EXPORT_SYMS+=	drm_dp_mst_get_edid
EXPORT_SYMS+=	drm_dp_mst_get_port_malloc
EXPORT_SYMS+=	drm_dp_mst_hpd_irq_handle_event
EXPORT_SYMS+=	drm_dp_mst_hpd_irq_send_new_request
EXPORT_SYMS+=	drm_dp_mst_put_port_malloc
EXPORT_SYMS+=	drm_dp_mst_root_conn_atomic_check
EXPORT_SYMS+=	drm_dp_mst_topology_mgr_destroy
EXPORT_SYMS+=	drm_dp_mst_topology_mgr_init
EXPORT_SYMS+=	drm_dp_mst_topology_mgr_resume
EXPORT_SYMS+=	drm_dp_mst_topology_mgr_set_mst
EXPORT_SYMS+=	drm_dp_mst_topology_mgr_suspend
EXPORT_SYMS+=	drm_dp_mst_topology_state_funcs
EXPORT_SYMS+=	drm_dp_mst_update_slots
EXPORT_SYMS+=	drm_dp_read_mst_cap
EXPORT_SYMS+=	drm_dp_remove_payload
EXPORT_SYMS+=	drm_dp_send_power_updown_phy
EXPORT_SYMS+=	drm_dp_send_query_stream_enc_status

# drivers/gpu/drm/display/drm_dsc_helper.c
EXPORT_SYMS+=	drm_dsc_compute_rc_parameters
EXPORT_SYMS+=	drm_dsc_dp_pps_header_init
EXPORT_SYMS+=	drm_dsc_dp_rc_buffer_size
EXPORT_SYMS+=	drm_dsc_flatness_det_thresh
EXPORT_SYMS+=	drm_dsc_get_bpp_int
EXPORT_SYMS+=	drm_dsc_initial_scale_value
EXPORT_SYMS+=	drm_dsc_pps_payload_pack
EXPORT_SYMS+=	drm_dsc_set_const_params
EXPORT_SYMS+=	drm_dsc_set_rc_buf_thresh
EXPORT_SYMS+=	drm_dsc_setup_rc_params

# drivers/gpu/drm/display/drm_hdcp_helper.c
EXPORT_SYMS+=	drm_connector_attach_content_protection_property
EXPORT_SYMS+=	drm_hdcp_check_ksvs_revoked
EXPORT_SYMS+=	drm_hdcp_update_content_protection

# drivers/gpu/drm/display/drm_hdmi_helper.c
EXPORT_SYMS+=	drm_hdmi_avi_infoframe_bars
EXPORT_SYMS+=	drm_hdmi_avi_infoframe_colorimetry
EXPORT_SYMS+=	drm_hdmi_avi_infoframe_content_type
EXPORT_SYMS+=	drm_hdmi_infoframe_set_hdr_metadata

# drivers/gpu/drm/display/drm_scdc_helper.c
EXPORT_SYMS+=	drm_scdc_get_scrambling_status
EXPORT_SYMS+=	drm_scdc_read
EXPORT_SYMS+=	drm_scdc_set_high_tmds_clock_ratio
EXPORT_SYMS+=	drm_scdc_set_scrambling
EXPORT_SYMS+=	drm_scdc_write

# drivers/gpu/drm/drm_agpsupport.c
EXPORT_SYMS+=	drm_legacy_agp_acquire
EXPORT_SYMS+=	drm_legacy_agp_alloc
EXPORT_SYMS+=	drm_legacy_agp_bind
EXPORT_SYMS+=	drm_legacy_agp_enable
EXPORT_SYMS+=	drm_legacy_agp_free
EXPORT_SYMS+=	drm_legacy_agp_info
EXPORT_SYMS+=	drm_legacy_agp_init
EXPORT_SYMS+=	drm_legacy_agp_release
EXPORT_SYMS+=	drm_legacy_agp_unbind

# drivers/gpu/drm/drm_aperture.c
EXPORT_SYMS+=	devm_aperture_acquire_from_firmware
EXPORT_SYMS+=	drm_aperture_remove_conflicting_framebuffers
EXPORT_SYMS+=	drm_aperture_remove_conflicting_pci_framebuffers

# drivers/gpu/drm/drm_atomic.c
EXPORT_SYMS+=	__drm_atomic_helper_disable_plane
EXPORT_SYMS+=	__drm_atomic_helper_set_config
EXPORT_SYMS+=	__drm_atomic_state_free
EXPORT_SYMS+=	__drm_crtc_commit_free
EXPORT_SYMS+=	drm_atomic_add_affected_connectors
EXPORT_SYMS+=	drm_atomic_add_affected_planes
EXPORT_SYMS+=	drm_atomic_add_encoder_bridges
EXPORT_SYMS+=	drm_atomic_check_only
EXPORT_SYMS+=	drm_atomic_commit
EXPORT_SYMS+=	drm_atomic_get_bridge_state
EXPORT_SYMS+=	drm_atomic_get_connector_state
EXPORT_SYMS+=	drm_atomic_get_crtc_state
EXPORT_SYMS+=	drm_atomic_get_new_bridge_state
EXPORT_SYMS+=	drm_atomic_get_new_connector_for_encoder
EXPORT_SYMS+=	drm_atomic_get_new_crtc_for_encoder
EXPORT_SYMS+=	drm_atomic_get_new_private_obj_state
EXPORT_SYMS+=	drm_atomic_get_old_bridge_state
EXPORT_SYMS+=	drm_atomic_get_old_connector_for_encoder
EXPORT_SYMS+=	drm_atomic_get_old_crtc_for_encoder
EXPORT_SYMS+=	drm_atomic_get_old_private_obj_state
EXPORT_SYMS+=	drm_atomic_get_plane_state
EXPORT_SYMS+=	drm_atomic_get_private_obj_state
EXPORT_SYMS+=	drm_atomic_nonblocking_commit
EXPORT_SYMS+=	drm_atomic_print_new_state
EXPORT_SYMS+=	drm_atomic_private_obj_fini
EXPORT_SYMS+=	drm_atomic_private_obj_init
EXPORT_SYMS+=	drm_atomic_state_alloc
EXPORT_SYMS+=	drm_atomic_state_clear
EXPORT_SYMS+=	drm_atomic_state_default_clear
EXPORT_SYMS+=	drm_atomic_state_default_release
EXPORT_SYMS+=	drm_atomic_state_init
EXPORT_SYMS+=	drm_crtc_commit_wait
EXPORT_SYMS+=	drm_state_dump

# drivers/gpu/drm/drm_atomic_helper.c
EXPORT_SYMS+=	drm_atomic_helper_async_check
EXPORT_SYMS+=	drm_atomic_helper_async_commit
EXPORT_SYMS+=	drm_atomic_helper_bridge_propagate_bus_fmt
EXPORT_SYMS+=	drm_atomic_helper_calc_timestamping_constants
EXPORT_SYMS+=	drm_atomic_helper_check
EXPORT_SYMS+=	drm_atomic_helper_check_crtc_primary_plane
EXPORT_SYMS+=	drm_atomic_helper_check_modeset
EXPORT_SYMS+=	drm_atomic_helper_check_plane_state
EXPORT_SYMS+=	drm_atomic_helper_check_planes
EXPORT_SYMS+=	drm_atomic_helper_check_wb_encoder_state
EXPORT_SYMS+=	drm_atomic_helper_cleanup_planes
EXPORT_SYMS+=	drm_atomic_helper_commit
EXPORT_SYMS+=	drm_atomic_helper_commit_cleanup_done
EXPORT_SYMS+=	drm_atomic_helper_commit_duplicated_state
EXPORT_SYMS+=	drm_atomic_helper_commit_hw_done
EXPORT_SYMS+=	drm_atomic_helper_commit_modeset_disables
EXPORT_SYMS+=	drm_atomic_helper_commit_modeset_enables
EXPORT_SYMS+=	drm_atomic_helper_commit_planes
EXPORT_SYMS+=	drm_atomic_helper_commit_planes_on_crtc
EXPORT_SYMS+=	drm_atomic_helper_commit_tail
EXPORT_SYMS+=	drm_atomic_helper_commit_tail_rpm
EXPORT_SYMS+=	drm_atomic_helper_disable_all
EXPORT_SYMS+=	drm_atomic_helper_disable_plane
EXPORT_SYMS+=	drm_atomic_helper_disable_planes_on_crtc
EXPORT_SYMS+=	drm_atomic_helper_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_fake_vblank
EXPORT_SYMS+=	drm_atomic_helper_page_flip
EXPORT_SYMS+=	drm_atomic_helper_page_flip_target
EXPORT_SYMS+=	drm_atomic_helper_prepare_planes
EXPORT_SYMS+=	drm_atomic_helper_resume
EXPORT_SYMS+=	drm_atomic_helper_set_config
EXPORT_SYMS+=	drm_atomic_helper_setup_commit
EXPORT_SYMS+=	drm_atomic_helper_shutdown
EXPORT_SYMS+=	drm_atomic_helper_suspend
EXPORT_SYMS+=	drm_atomic_helper_swap_state
EXPORT_SYMS+=	drm_atomic_helper_update_legacy_modeset_state
EXPORT_SYMS+=	drm_atomic_helper_update_plane
EXPORT_SYMS+=	drm_atomic_helper_wait_for_dependencies
EXPORT_SYMS+=	drm_atomic_helper_wait_for_fences
EXPORT_SYMS+=	drm_atomic_helper_wait_for_flip_done
EXPORT_SYMS+=	drm_atomic_helper_wait_for_vblanks

# drivers/gpu/drm/drm_atomic_state_helper.c
EXPORT_SYMS+=	__drm_atomic_helper_bridge_duplicate_state
EXPORT_SYMS+=	__drm_atomic_helper_bridge_reset
EXPORT_SYMS+=	__drm_atomic_helper_connector_destroy_state
EXPORT_SYMS+=	__drm_atomic_helper_connector_duplicate_state
EXPORT_SYMS+=	__drm_atomic_helper_connector_reset
EXPORT_SYMS+=	__drm_atomic_helper_connector_state_reset
EXPORT_SYMS+=	__drm_atomic_helper_crtc_destroy_state
EXPORT_SYMS+=	__drm_atomic_helper_crtc_duplicate_state
EXPORT_SYMS+=	__drm_atomic_helper_crtc_reset
EXPORT_SYMS+=	__drm_atomic_helper_crtc_state_reset
EXPORT_SYMS+=	__drm_atomic_helper_plane_destroy_state
EXPORT_SYMS+=	__drm_atomic_helper_plane_duplicate_state
EXPORT_SYMS+=	__drm_atomic_helper_plane_reset
EXPORT_SYMS+=	__drm_atomic_helper_plane_state_reset
EXPORT_SYMS+=	__drm_atomic_helper_private_obj_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_bridge_destroy_state
EXPORT_SYMS+=	drm_atomic_helper_bridge_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_bridge_reset
EXPORT_SYMS+=	drm_atomic_helper_connector_destroy_state
EXPORT_SYMS+=	drm_atomic_helper_connector_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_connector_reset
EXPORT_SYMS+=	drm_atomic_helper_connector_tv_check
EXPORT_SYMS+=	drm_atomic_helper_connector_tv_margins_reset
EXPORT_SYMS+=	drm_atomic_helper_connector_tv_reset
EXPORT_SYMS+=	drm_atomic_helper_crtc_destroy_state
EXPORT_SYMS+=	drm_atomic_helper_crtc_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_crtc_reset
EXPORT_SYMS+=	drm_atomic_helper_plane_destroy_state
EXPORT_SYMS+=	drm_atomic_helper_plane_duplicate_state
EXPORT_SYMS+=	drm_atomic_helper_plane_reset

# drivers/gpu/drm/drm_atomic_uapi.c
EXPORT_SYMS+=	drm_atomic_set_crtc_for_connector
EXPORT_SYMS+=	drm_atomic_set_crtc_for_plane
EXPORT_SYMS+=	drm_atomic_set_fb_for_plane
EXPORT_SYMS+=	drm_atomic_set_mode_for_crtc
EXPORT_SYMS+=	drm_atomic_set_mode_prop_for_crtc

# drivers/gpu/drm/drm_auth.c
EXPORT_SYMS+=	drm_file_get_master
EXPORT_SYMS+=	drm_is_current_master
EXPORT_SYMS+=	drm_master_get
EXPORT_SYMS+=	drm_master_internal_acquire
EXPORT_SYMS+=	drm_master_internal_release
EXPORT_SYMS+=	drm_master_put

# drivers/gpu/drm/drm_blend.c
EXPORT_SYMS+=	drm_atomic_normalize_zpos
EXPORT_SYMS+=	drm_plane_create_alpha_property
EXPORT_SYMS+=	drm_plane_create_blend_mode_property
EXPORT_SYMS+=	drm_plane_create_rotation_property
EXPORT_SYMS+=	drm_plane_create_zpos_immutable_property
EXPORT_SYMS+=	drm_plane_create_zpos_property
EXPORT_SYMS+=	drm_rotation_simplify

# drivers/gpu/drm/drm_bridge.c
EXPORT_SYMS+=	devm_drm_bridge_add
EXPORT_SYMS+=	drm_atomic_bridge_chain_check
EXPORT_SYMS+=	drm_atomic_bridge_chain_disable
EXPORT_SYMS+=	drm_atomic_bridge_chain_enable
EXPORT_SYMS+=	drm_atomic_bridge_chain_post_disable
EXPORT_SYMS+=	drm_atomic_bridge_chain_pre_enable
EXPORT_SYMS+=	drm_bridge_add
EXPORT_SYMS+=	drm_bridge_attach
EXPORT_SYMS+=	drm_bridge_chain_mode_fixup
EXPORT_SYMS+=	drm_bridge_chain_mode_set
EXPORT_SYMS+=	drm_bridge_chain_mode_valid
EXPORT_SYMS+=	drm_bridge_detect
EXPORT_SYMS+=	drm_bridge_get_edid
EXPORT_SYMS+=	drm_bridge_get_modes
EXPORT_SYMS+=	drm_bridge_hpd_disable
EXPORT_SYMS+=	drm_bridge_hpd_enable
EXPORT_SYMS+=	drm_bridge_hpd_notify
EXPORT_SYMS+=	drm_bridge_remove
EXPORT_SYMS+=	of_drm_find_bridge

# drivers/gpu/drm/drm_buddy.c
EXPORT_SYMS+=	drm_buddy_alloc_blocks
EXPORT_SYMS+=	drm_buddy_block_print
EXPORT_SYMS+=	drm_buddy_block_trim
EXPORT_SYMS+=	drm_buddy_fini
EXPORT_SYMS+=	drm_buddy_free_block
EXPORT_SYMS+=	drm_buddy_free_list
EXPORT_SYMS+=	drm_buddy_init
EXPORT_SYMS+=	drm_buddy_print
EXPORT_SYMS+=	drm_get_buddy

# drivers/gpu/drm/drm_cache.c
EXPORT_SYMS+=	drm_clflush_pages
EXPORT_SYMS+=	drm_clflush_sg
EXPORT_SYMS+=	drm_clflush_virt_range
EXPORT_SYMS+=	drm_memcpy_from_wc
EXPORT_SYMS+=	drm_memcpy_from_wc
EXPORT_SYMS+=	drm_need_swiotlb

# drivers/gpu/drm/drm_client.c
EXPORT_SYMS+=	drm_client_buffer_vmap
EXPORT_SYMS+=	drm_client_buffer_vunmap
EXPORT_SYMS+=	drm_client_dev_hotplug
EXPORT_SYMS+=	drm_client_framebuffer_create
EXPORT_SYMS+=	drm_client_framebuffer_delete
EXPORT_SYMS+=	drm_client_framebuffer_flush
EXPORT_SYMS+=	drm_client_init
EXPORT_SYMS+=	drm_client_register
EXPORT_SYMS+=	drm_client_release

# drivers/gpu/drm/drm_client_modeset.c
EXPORT_SYMS+=	drm_client_modeset_check
EXPORT_SYMS+=	drm_client_modeset_commit
EXPORT_SYMS+=	drm_client_modeset_commit_locked
EXPORT_SYMS+=	drm_client_modeset_dpms
EXPORT_SYMS+=	drm_client_modeset_probe
EXPORT_SYMS+=	drm_client_rotation

# drivers/gpu/drm/drm_color_mgmt.c
EXPORT_SYMS+=	drm_color_ctm_s31_32_to_qm_n
EXPORT_SYMS+=	drm_color_lut_check
EXPORT_SYMS+=	drm_crtc_enable_color_mgmt
EXPORT_SYMS+=	drm_mode_crtc_set_gamma_size
EXPORT_SYMS+=	drm_plane_create_color_properties

# drivers/gpu/drm/drm_connector.c
EXPORT_SYMS+=	drm_connector_atomic_hdr_metadata_equal
EXPORT_SYMS+=	drm_connector_attach_colorspace_property
EXPORT_SYMS+=	drm_connector_attach_content_type_property
EXPORT_SYMS+=	drm_connector_attach_dp_subconnector_property
EXPORT_SYMS+=	drm_connector_attach_edid_property
EXPORT_SYMS+=	drm_connector_attach_encoder
EXPORT_SYMS+=	drm_connector_attach_hdr_output_metadata_property
EXPORT_SYMS+=	drm_connector_attach_max_bpc_property
EXPORT_SYMS+=	drm_connector_attach_privacy_screen_properties
EXPORT_SYMS+=	drm_connector_attach_privacy_screen_provider
EXPORT_SYMS+=	drm_connector_attach_scaling_mode_property
EXPORT_SYMS+=	drm_connector_attach_tv_margin_properties
EXPORT_SYMS+=	drm_connector_attach_vrr_capable_property
EXPORT_SYMS+=	drm_connector_cleanup
EXPORT_SYMS+=	drm_connector_create_privacy_screen_properties
EXPORT_SYMS+=	drm_connector_has_possible_encoder
EXPORT_SYMS+=	drm_connector_init
EXPORT_SYMS+=	drm_connector_init_with_ddc
EXPORT_SYMS+=	drm_connector_list_iter_begin
EXPORT_SYMS+=	drm_connector_list_iter_end
EXPORT_SYMS+=	drm_connector_list_iter_next
EXPORT_SYMS+=	drm_connector_oob_hotplug_event
EXPORT_SYMS+=	drm_connector_register
EXPORT_SYMS+=	drm_connector_set_link_status_property
EXPORT_SYMS+=	drm_connector_set_orientation_from_panel
EXPORT_SYMS+=	drm_connector_set_panel_orientation
EXPORT_SYMS+=	drm_connector_set_panel_orientation_with_quirk
EXPORT_SYMS+=	drm_connector_set_path_property
EXPORT_SYMS+=	drm_connector_set_tile_property
EXPORT_SYMS+=	drm_connector_set_vrr_capable_property
EXPORT_SYMS+=	drm_connector_unregister
EXPORT_SYMS+=	drm_connector_update_privacy_screen
EXPORT_SYMS+=	drm_display_info_set_bus_formats
EXPORT_SYMS+=	drm_get_connector_status_name
EXPORT_SYMS+=	drm_get_connector_type_name
EXPORT_SYMS+=	drm_get_subpixel_order_name
EXPORT_SYMS+=	drm_get_tv_mode_from_name
EXPORT_SYMS+=	drm_mode_create_aspect_ratio_property
EXPORT_SYMS+=	drm_mode_create_content_type_property
EXPORT_SYMS+=	drm_mode_create_dp_colorspace_property
EXPORT_SYMS+=	drm_mode_create_dvi_i_properties
EXPORT_SYMS+=	drm_mode_create_hdmi_colorspace_property
EXPORT_SYMS+=	drm_mode_create_scaling_mode_property
EXPORT_SYMS+=	drm_mode_create_suggested_offset_properties
EXPORT_SYMS+=	drm_mode_create_tile_group
EXPORT_SYMS+=	drm_mode_create_tv_margin_properties
EXPORT_SYMS+=	drm_mode_create_tv_properties
EXPORT_SYMS+=	drm_mode_create_tv_properties_legacy
EXPORT_SYMS+=	drm_mode_get_tile_group
EXPORT_SYMS+=	drm_mode_put_tile_group
EXPORT_SYMS+=	drmm_connector_init

# drivers/gpu/drm/drm_crtc.c
EXPORT_SYMS+=	__drmm_crtc_alloc_with_planes
EXPORT_SYMS+=	drm_crtc_check_viewport
EXPORT_SYMS+=	drm_crtc_cleanup
EXPORT_SYMS+=	drm_crtc_create_scaling_filter_property
EXPORT_SYMS+=	drm_crtc_from_index
EXPORT_SYMS+=	drm_crtc_init_with_planes
EXPORT_SYMS+=	drm_mode_set_config_internal
EXPORT_SYMS+=	drmm_crtc_init_with_planes

# drivers/gpu/drm/drm_crtc_helper.c
EXPORT_SYMS+=	drm_crtc_helper_atomic_check
EXPORT_SYMS+=	drm_crtc_helper_set_config
EXPORT_SYMS+=	drm_crtc_helper_set_mode
EXPORT_SYMS+=	drm_helper_connector_dpms
EXPORT_SYMS+=	drm_helper_crtc_in_use
EXPORT_SYMS+=	drm_helper_disable_unused_functions
EXPORT_SYMS+=	drm_helper_encoder_in_use
EXPORT_SYMS+=	drm_helper_force_disable_all
EXPORT_SYMS+=	drm_helper_resume_force_mode

# drivers/gpu/drm/drm_damage_helper.c
EXPORT_SYMS+=	drm_atomic_helper_check_plane_damage
EXPORT_SYMS+=	drm_atomic_helper_damage_iter_init
EXPORT_SYMS+=	drm_atomic_helper_damage_iter_next
EXPORT_SYMS+=	drm_atomic_helper_damage_merged
EXPORT_SYMS+=	drm_atomic_helper_dirtyfb

# drivers/gpu/drm/drm_debugfs.c
EXPORT_SYMS+=	drm_debugfs_add_file
EXPORT_SYMS+=	drm_debugfs_add_files
EXPORT_SYMS+=	drm_debugfs_create_files
EXPORT_SYMS+=	drm_debugfs_gpuva_info
EXPORT_SYMS+=	drm_debugfs_remove_files

# drivers/gpu/drm/drm_debugfs_crc.c
EXPORT_SYMS+=	drm_crtc_add_crc_entry

# drivers/gpu/drm/drm_drv.c
EXPORT_SYMS+=	__devm_drm_dev_alloc
EXPORT_SYMS+=	drm_dev_alloc
EXPORT_SYMS+=	drm_dev_enter
EXPORT_SYMS+=	drm_dev_exit
EXPORT_SYMS+=	drm_dev_get
EXPORT_SYMS+=	drm_dev_put
EXPORT_SYMS+=	drm_dev_register
EXPORT_SYMS+=	drm_dev_unplug
EXPORT_SYMS+=	drm_dev_unregister
EXPORT_SYMS+=	drm_put_dev

# drivers/gpu/drm/drm_edid.c
EXPORT_SYMS+=	drm_add_edid_modes
EXPORT_SYMS+=	drm_add_modes_noedid
EXPORT_SYMS+=	drm_av_sync_delay
EXPORT_SYMS+=	drm_connector_update_edid_property
EXPORT_SYMS+=	drm_default_rgb_quant_range
EXPORT_SYMS+=	drm_detect_hdmi_monitor
EXPORT_SYMS+=	drm_detect_monitor_audio
EXPORT_SYMS+=	drm_display_mode_from_cea_vic
EXPORT_SYMS+=	drm_do_get_edid
EXPORT_SYMS+=	drm_edid_alloc
EXPORT_SYMS+=	drm_edid_are_equal
EXPORT_SYMS+=	drm_edid_block_valid
EXPORT_SYMS+=	drm_edid_connector_add_modes
EXPORT_SYMS+=	drm_edid_connector_update
EXPORT_SYMS+=	drm_edid_dup
EXPORT_SYMS+=	drm_edid_duplicate
EXPORT_SYMS+=	drm_edid_free
EXPORT_SYMS+=	drm_edid_get_monitor_name
EXPORT_SYMS+=	drm_edid_get_panel_id
EXPORT_SYMS+=	drm_edid_header_is_valid
EXPORT_SYMS+=	drm_edid_is_valid
EXPORT_SYMS+=	drm_edid_override_connector_update
EXPORT_SYMS+=	drm_edid_raw
EXPORT_SYMS+=	drm_edid_read
EXPORT_SYMS+=	drm_edid_read_custom
EXPORT_SYMS+=	drm_edid_read_ddc
EXPORT_SYMS+=	drm_edid_read_switcheroo
EXPORT_SYMS+=	drm_edid_to_sad
EXPORT_SYMS+=	drm_edid_to_speaker_allocation
EXPORT_SYMS+=	drm_edid_valid
EXPORT_SYMS+=	drm_get_edid
EXPORT_SYMS+=	drm_get_edid_switcheroo
EXPORT_SYMS+=	drm_hdmi_avi_infoframe_from_display_mode
EXPORT_SYMS+=	drm_hdmi_avi_infoframe_quant_range
EXPORT_SYMS+=	drm_hdmi_vendor_infoframe_from_display_mode
EXPORT_SYMS+=	drm_match_cea_mode
EXPORT_SYMS+=	drm_mode_find_dmt
EXPORT_SYMS+=	drm_probe_ddc
EXPORT_SYMS+=	drm_set_preferred_mode

# drivers/gpu/drm/drm_encoder.c
EXPORT_SYMS+=	__drmm_encoder_alloc
EXPORT_SYMS+=	drm_encoder_cleanup
EXPORT_SYMS+=	drm_encoder_init
EXPORT_SYMS+=	drmm_encoder_init

# drivers/gpu/drm/drm_exec.c
EXPORT_SYMS+=	drm_exec_cleanup
EXPORT_SYMS+=	drm_exec_fini
EXPORT_SYMS+=	drm_exec_init
EXPORT_SYMS+=	drm_exec_lock_obj
EXPORT_SYMS+=	drm_exec_prepare_array
EXPORT_SYMS+=	drm_exec_prepare_obj
EXPORT_SYMS+=	drm_exec_unlock_obj

# drivers/gpu/drm/drm_fb_helper.c
EXPORT_SYMS+=	drm_fb_helper_alloc_info
EXPORT_SYMS+=	drm_fb_helper_blank
EXPORT_SYMS+=	drm_fb_helper_check_var
EXPORT_SYMS+=	drm_fb_helper_damage_area
EXPORT_SYMS+=	drm_fb_helper_damage_range
EXPORT_SYMS+=	drm_fb_helper_debug_enter
EXPORT_SYMS+=	drm_fb_helper_debug_leave
EXPORT_SYMS+=	drm_fb_helper_deferred_io
EXPORT_SYMS+=	drm_fb_helper_fill_info
EXPORT_SYMS+=	drm_fb_helper_fini
EXPORT_SYMS+=	drm_fb_helper_hotplug_event
EXPORT_SYMS+=	drm_fb_helper_init
EXPORT_SYMS+=	drm_fb_helper_initial_config
EXPORT_SYMS+=	drm_fb_helper_ioctl
EXPORT_SYMS+=	drm_fb_helper_lastclose
EXPORT_SYMS+=	drm_fb_helper_output_poll_changed
EXPORT_SYMS+=	drm_fb_helper_pan_display
EXPORT_SYMS+=	drm_fb_helper_prepare
EXPORT_SYMS+=	drm_fb_helper_release_info
EXPORT_SYMS+=	drm_fb_helper_restore_fbdev_mode_unlocked
EXPORT_SYMS+=	drm_fb_helper_set_par
EXPORT_SYMS+=	drm_fb_helper_set_suspend
EXPORT_SYMS+=	drm_fb_helper_set_suspend_unlocked
EXPORT_SYMS+=	drm_fb_helper_setcmap
EXPORT_SYMS+=	drm_fb_helper_unprepare
EXPORT_SYMS+=	drm_fb_helper_unregister_info

# drivers/gpu/drm/drm_fbdev_generic.c
EXPORT_SYMS+=	drm_fbdev_generic_setup

# drivers/gpu/drm/drm_file.c
EXPORT_SYMS+=	drm_event_cancel_free
EXPORT_SYMS+=	drm_event_reserve_init
EXPORT_SYMS+=	drm_event_reserve_init_locked
EXPORT_SYMS+=	drm_open
EXPORT_SYMS+=	drm_poll
EXPORT_SYMS+=	drm_print_memory_stats
EXPORT_SYMS+=	drm_read
EXPORT_SYMS+=	drm_release
EXPORT_SYMS+=	drm_release_noglobal
EXPORT_SYMS+=	drm_send_event
EXPORT_SYMS+=	drm_send_event_locked
EXPORT_SYMS+=	drm_send_event_timestamp_locked
EXPORT_SYMS+=	drm_show_fdinfo
EXPORT_SYMS+=	drm_show_memory_stats
EXPORT_SYMS+=	mock_drm_getfile

# drivers/gpu/drm/drm_flip_work.c
EXPORT_SYMS+=	drm_flip_work_allocate_task
EXPORT_SYMS+=	drm_flip_work_cleanup
EXPORT_SYMS+=	drm_flip_work_commit
EXPORT_SYMS+=	drm_flip_work_init
EXPORT_SYMS+=	drm_flip_work_queue
EXPORT_SYMS+=	drm_flip_work_queue_task

# drivers/gpu/drm/drm_fourcc.c
EXPORT_SYMS+=	drm_driver_legacy_fb_format
EXPORT_SYMS+=	drm_format_info
EXPORT_SYMS+=	drm_format_info_block_height
EXPORT_SYMS+=	drm_format_info_block_width
EXPORT_SYMS+=	drm_format_info_bpp
EXPORT_SYMS+=	drm_format_info_min_pitch
EXPORT_SYMS+=	drm_get_format_info
EXPORT_SYMS+=	drm_mode_legacy_fb_format

# drivers/gpu/drm/drm_framebuffer.c
EXPORT_SYMS+=	drm_framebuffer_cleanup
EXPORT_SYMS+=	drm_framebuffer_init
EXPORT_SYMS+=	drm_framebuffer_lookup
EXPORT_SYMS+=	drm_framebuffer_plane_height
EXPORT_SYMS+=	drm_framebuffer_plane_width
EXPORT_SYMS+=	drm_framebuffer_remove
EXPORT_SYMS+=	drm_framebuffer_unregister_private
EXPORT_SYMS+=	drm_internal_framebuffer_create

# drivers/gpu/drm/drm_gem.c
EXPORT_SYMS+=	drm_gem_create_mmap_offset
EXPORT_SYMS+=	drm_gem_create_mmap_offset_size
EXPORT_SYMS+=	drm_gem_dma_resv_wait
EXPORT_SYMS+=	drm_gem_dumb_map_offset
EXPORT_SYMS+=	drm_gem_evict
EXPORT_SYMS+=	drm_gem_free_mmap_offset
EXPORT_SYMS+=	drm_gem_get_pages
EXPORT_SYMS+=	drm_gem_handle_create
EXPORT_SYMS+=	drm_gem_handle_delete
EXPORT_SYMS+=	drm_gem_lock_reservations
EXPORT_SYMS+=	drm_gem_lru_init
EXPORT_SYMS+=	drm_gem_lru_move_tail
EXPORT_SYMS+=	drm_gem_lru_move_tail_locked
EXPORT_SYMS+=	drm_gem_lru_remove
EXPORT_SYMS+=	drm_gem_lru_scan
EXPORT_SYMS+=	drm_gem_mmap
EXPORT_SYMS+=	drm_gem_mmap_obj
EXPORT_SYMS+=	drm_gem_object_free
EXPORT_SYMS+=	drm_gem_object_init
EXPORT_SYMS+=	drm_gem_object_lookup
EXPORT_SYMS+=	drm_gem_object_release
EXPORT_SYMS+=	drm_gem_objects_lookup
EXPORT_SYMS+=	drm_gem_private_object_fini
EXPORT_SYMS+=	drm_gem_private_object_init
EXPORT_SYMS+=	drm_gem_put_pages
EXPORT_SYMS+=	drm_gem_unlock_reservations
EXPORT_SYMS+=	drm_gem_vm_close
EXPORT_SYMS+=	drm_gem_vm_open
EXPORT_SYMS+=	drm_gem_vmap
EXPORT_SYMS+=	drm_gem_vmap_unlocked
EXPORT_SYMS+=	drm_gem_vunmap
EXPORT_SYMS+=	drm_gem_vunmap_unlocked

# drivers/gpu/drm/drm_ioc32.c
EXPORT_SYMS+=	drm_compat_ioctl

# drivers/gpu/drm/drm_ioctl.c
EXPORT_SYMS+=	drm_invalid_op
EXPORT_SYMS+=	drm_ioctl
EXPORT_SYMS+=	drm_ioctl_flags
EXPORT_SYMS+=	drm_ioctl_kernel
EXPORT_SYMS+=	drm_noop

# drivers/gpu/drm/drm_irq.c
EXPORT_SYMS+=	drm_legacy_irq_uninstall

# drivers/gpu/drm/drm_memory.c
EXPORT_SYMS+=	drm_legacy_ioremap
EXPORT_SYMS+=	drm_legacy_ioremap_wc
EXPORT_SYMS+=	drm_legacy_ioremapfree

# drivers/gpu/drm/drm_mipi_dsi.c
EXPORT_SYMS+=	devm_mipi_dsi_attach
EXPORT_SYMS+=	devm_mipi_dsi_device_register_full
EXPORT_SYMS+=	mipi_dsi_attach
EXPORT_SYMS+=	mipi_dsi_compression_mode
EXPORT_SYMS+=	mipi_dsi_create_packet
EXPORT_SYMS+=	mipi_dsi_dcs_enter_sleep_mode
EXPORT_SYMS+=	mipi_dsi_dcs_exit_sleep_mode
EXPORT_SYMS+=	mipi_dsi_dcs_get_display_brightness
EXPORT_SYMS+=	mipi_dsi_dcs_get_display_brightness_large
EXPORT_SYMS+=	mipi_dsi_dcs_get_pixel_format
EXPORT_SYMS+=	mipi_dsi_dcs_get_power_mode
EXPORT_SYMS+=	mipi_dsi_dcs_nop
EXPORT_SYMS+=	mipi_dsi_dcs_read
EXPORT_SYMS+=	mipi_dsi_dcs_set_column_address
EXPORT_SYMS+=	mipi_dsi_dcs_set_display_brightness
EXPORT_SYMS+=	mipi_dsi_dcs_set_display_brightness_large
EXPORT_SYMS+=	mipi_dsi_dcs_set_display_off
EXPORT_SYMS+=	mipi_dsi_dcs_set_display_on
EXPORT_SYMS+=	mipi_dsi_dcs_set_page_address
EXPORT_SYMS+=	mipi_dsi_dcs_set_pixel_format
EXPORT_SYMS+=	mipi_dsi_dcs_set_tear_off
EXPORT_SYMS+=	mipi_dsi_dcs_set_tear_on
EXPORT_SYMS+=	mipi_dsi_dcs_set_tear_scanline
EXPORT_SYMS+=	mipi_dsi_dcs_soft_reset
EXPORT_SYMS+=	mipi_dsi_dcs_write
EXPORT_SYMS+=	mipi_dsi_dcs_write_buffer
EXPORT_SYMS+=	mipi_dsi_detach
EXPORT_SYMS+=	mipi_dsi_device_register_full
EXPORT_SYMS+=	mipi_dsi_device_unregister
EXPORT_SYMS+=	mipi_dsi_driver_register_full
EXPORT_SYMS+=	mipi_dsi_driver_unregister
EXPORT_SYMS+=	mipi_dsi_generic_read
EXPORT_SYMS+=	mipi_dsi_generic_write
EXPORT_SYMS+=	mipi_dsi_host_register
EXPORT_SYMS+=	mipi_dsi_host_unregister
EXPORT_SYMS+=	mipi_dsi_packet_format_is_long
EXPORT_SYMS+=	mipi_dsi_packet_format_is_short
EXPORT_SYMS+=	mipi_dsi_picture_parameter_set
EXPORT_SYMS+=	mipi_dsi_set_maximum_return_packet_size
EXPORT_SYMS+=	mipi_dsi_shutdown_peripheral
EXPORT_SYMS+=	mipi_dsi_turn_on_peripheral
EXPORT_SYMS+=	of_find_mipi_dsi_device_by_node
EXPORT_SYMS+=	of_find_mipi_dsi_host_by_node

# drivers/gpu/drm/drm_mm.c
EXPORT_SYMS+=	__drm_mm_interval_first
EXPORT_SYMS+=	drm_mm_init
EXPORT_SYMS+=	drm_mm_insert_node_in_range
EXPORT_SYMS+=	drm_mm_print
EXPORT_SYMS+=	drm_mm_remove_node
EXPORT_SYMS+=	drm_mm_replace_node
EXPORT_SYMS+=	drm_mm_reserve_node
EXPORT_SYMS+=	drm_mm_scan_add_block
EXPORT_SYMS+=	drm_mm_scan_color_evict
EXPORT_SYMS+=	drm_mm_scan_init_with_range
EXPORT_SYMS+=	drm_mm_scan_remove_block
EXPORT_SYMS+=	drm_mm_takedown

# drivers/gpu/drm/drm_mode_config.c
EXPORT_SYMS+=	drm_mode_config_cleanup
EXPORT_SYMS+=	drm_mode_config_reset
EXPORT_SYMS+=	drmm_mode_config_init

# drivers/gpu/drm/drm_mode_object.c
EXPORT_SYMS+=	drm_mode_object_find
EXPORT_SYMS+=	drm_mode_object_get
EXPORT_SYMS+=	drm_mode_object_put
EXPORT_SYMS+=	drm_object_attach_property
EXPORT_SYMS+=	drm_object_property_get_default_value
EXPORT_SYMS+=	drm_object_property_get_value
EXPORT_SYMS+=	drm_object_property_set_value

# drivers/gpu/drm/drm_modes.c
EXPORT_SYMS+=	drm_analog_tv_mode
EXPORT_SYMS+=	drm_bus_flags_from_videomode
EXPORT_SYMS+=	drm_connector_list_update
EXPORT_SYMS+=	drm_cvt_mode
EXPORT_SYMS+=	drm_display_mode_from_videomode
EXPORT_SYMS+=	drm_display_mode_to_videomode
EXPORT_SYMS+=	drm_gtf_mode
EXPORT_SYMS+=	drm_gtf_mode_complex
EXPORT_SYMS+=	drm_mode_copy
EXPORT_SYMS+=	drm_mode_create
EXPORT_SYMS+=	drm_mode_create_from_cmdline_mode
EXPORT_SYMS+=	drm_mode_debug_printmodeline
EXPORT_SYMS+=	drm_mode_destroy
EXPORT_SYMS+=	drm_mode_duplicate
EXPORT_SYMS+=	drm_mode_equal
EXPORT_SYMS+=	drm_mode_equal_no_clocks
EXPORT_SYMS+=	drm_mode_equal_no_clocks_no_stereo
EXPORT_SYMS+=	drm_mode_get_hv_timing
EXPORT_SYMS+=	drm_mode_init
EXPORT_SYMS+=	drm_mode_is_420
EXPORT_SYMS+=	drm_mode_is_420_also
EXPORT_SYMS+=	drm_mode_is_420_only
EXPORT_SYMS+=	drm_mode_match
EXPORT_SYMS+=	drm_mode_parse_command_line_for_connector
EXPORT_SYMS+=	drm_mode_probed_add
EXPORT_SYMS+=	drm_mode_prune_invalid
EXPORT_SYMS+=	drm_mode_set_crtcinfo
EXPORT_SYMS+=	drm_mode_set_name
EXPORT_SYMS+=	drm_mode_sort
EXPORT_SYMS+=	drm_mode_validate_driver
EXPORT_SYMS+=	drm_mode_validate_size
EXPORT_SYMS+=	drm_mode_validate_ycbcr420
EXPORT_SYMS+=	drm_mode_vrefresh
EXPORT_SYMS+=	of_get_drm_display_mode
EXPORT_SYMS+=	of_get_drm_panel_display_mode

# drivers/gpu/drm/drm_modeset_helper.c
EXPORT_SYMS+=	drm_crtc_init
EXPORT_SYMS+=	drm_helper_mode_fill_fb_struct
EXPORT_SYMS+=	drm_helper_move_panel_connectors_to_head
EXPORT_SYMS+=	drm_mode_config_helper_resume
EXPORT_SYMS+=	drm_mode_config_helper_suspend

# drivers/gpu/drm/drm_modeset_lock.c
EXPORT_SYMS+=	drm_modeset_acquire_fini
EXPORT_SYMS+=	drm_modeset_acquire_init
EXPORT_SYMS+=	drm_modeset_backoff
EXPORT_SYMS+=	drm_modeset_drop_locks
EXPORT_SYMS+=	drm_modeset_lock
EXPORT_SYMS+=	drm_modeset_lock_all
EXPORT_SYMS+=	drm_modeset_lock_all_ctx
EXPORT_SYMS+=	drm_modeset_lock_init
EXPORT_SYMS+=	drm_modeset_lock_single_interruptible
EXPORT_SYMS+=	drm_modeset_unlock
EXPORT_SYMS+=	drm_modeset_unlock_all
EXPORT_SYMS+=	drm_warn_on_modeset_not_all_locked

# drivers/gpu/drm/drm_panel.c
EXPORT_SYMS+=	devm_drm_panel_add_follower
EXPORT_SYMS+=	drm_is_panel_follower
EXPORT_SYMS+=	drm_panel_add
EXPORT_SYMS+=	drm_panel_add_follower
EXPORT_SYMS+=	drm_panel_disable
EXPORT_SYMS+=	drm_panel_enable
EXPORT_SYMS+=	drm_panel_get_modes
EXPORT_SYMS+=	drm_panel_init
EXPORT_SYMS+=	drm_panel_of_backlight
EXPORT_SYMS+=	drm_panel_prepare
EXPORT_SYMS+=	drm_panel_remove
EXPORT_SYMS+=	drm_panel_remove_follower
EXPORT_SYMS+=	drm_panel_unprepare
EXPORT_SYMS+=	of_drm_find_panel
EXPORT_SYMS+=	of_drm_get_panel_orientation

# drivers/gpu/drm/drm_panel_orientation_quirks.c
EXPORT_SYMS+=	drm_get_panel_orientation_quirk
EXPORT_SYMS+=	drm_get_panel_orientation_quirk

# drivers/gpu/drm/drm_pci.c
EXPORT_SYMS+=	drm_legacy_pci_exit
EXPORT_SYMS+=	drm_legacy_pci_init

# drivers/gpu/drm/drm_plane.c
EXPORT_SYMS+=	__drm_universal_plane_alloc
EXPORT_SYMS+=	__drmm_universal_plane_alloc
EXPORT_SYMS+=	drm_any_plane_has_format
EXPORT_SYMS+=	drm_mode_plane_set_obj_prop
EXPORT_SYMS+=	drm_plane_cleanup
EXPORT_SYMS+=	drm_plane_create_scaling_filter_property
EXPORT_SYMS+=	drm_plane_enable_fb_damage_clips
EXPORT_SYMS+=	drm_plane_force_disable
EXPORT_SYMS+=	drm_plane_from_index
EXPORT_SYMS+=	drm_plane_get_damage_clips
EXPORT_SYMS+=	drm_plane_get_damage_clips_count
EXPORT_SYMS+=	drm_universal_plane_init

# drivers/gpu/drm/drm_plane_helper.c
EXPORT_SYMS+=	drm_plane_helper_atomic_check
EXPORT_SYMS+=	drm_plane_helper_destroy
EXPORT_SYMS+=	drm_plane_helper_disable_primary
EXPORT_SYMS+=	drm_plane_helper_update_primary

# drivers/gpu/drm/drm_prime.c
EXPORT_SYMS+=	drm_gem_dmabuf_export
EXPORT_SYMS+=	drm_gem_dmabuf_mmap
EXPORT_SYMS+=	drm_gem_dmabuf_release
EXPORT_SYMS+=	drm_gem_dmabuf_vmap
EXPORT_SYMS+=	drm_gem_dmabuf_vunmap
EXPORT_SYMS+=	drm_gem_map_attach
EXPORT_SYMS+=	drm_gem_map_detach
EXPORT_SYMS+=	drm_gem_map_dma_buf
EXPORT_SYMS+=	drm_gem_prime_export
EXPORT_SYMS+=	drm_gem_prime_import
EXPORT_SYMS+=	drm_gem_prime_import_dev
EXPORT_SYMS+=	drm_gem_prime_mmap
EXPORT_SYMS+=	drm_gem_unmap_dma_buf
EXPORT_SYMS+=	drm_prime_gem_destroy
EXPORT_SYMS+=	drm_prime_get_contiguous_size
EXPORT_SYMS+=	drm_prime_pages_to_sg
EXPORT_SYMS+=	drm_prime_sg_to_dma_addr_array
EXPORT_SYMS+=	drm_prime_sg_to_page_array

# drivers/gpu/drm/drm_print.c
EXPORT_SYMS+=	___drm_dbg
EXPORT_SYMS+=	___drm_dbg
EXPORT_SYMS+=	__drm_debug
EXPORT_SYMS+=	__drm_dev_dbg
EXPORT_SYMS+=	__drm_err
EXPORT_SYMS+=	__drm_err
EXPORT_SYMS+=	__drm_printfn_coredump
EXPORT_SYMS+=	__drm_printfn_debug
EXPORT_SYMS+=	__drm_printfn_err
EXPORT_SYMS+=	__drm_printfn_info
EXPORT_SYMS+=	__drm_printfn_seq_file
EXPORT_SYMS+=	__drm_puts_coredump
EXPORT_SYMS+=	__drm_puts_seq_file
EXPORT_SYMS+=	drm_dev_printk
EXPORT_SYMS+=	drm_print_bits
EXPORT_SYMS+=	drm_print_regset32
EXPORT_SYMS+=	drm_printf
EXPORT_SYMS+=	drm_puts

# drivers/gpu/drm/drm_privacy_screen.c
EXPORT_SYMS+=	drm_privacy_screen_call_notifier_chain
EXPORT_SYMS+=	drm_privacy_screen_get
EXPORT_SYMS+=	drm_privacy_screen_get_state
EXPORT_SYMS+=	drm_privacy_screen_lookup_add
EXPORT_SYMS+=	drm_privacy_screen_lookup_remove
EXPORT_SYMS+=	drm_privacy_screen_put
EXPORT_SYMS+=	drm_privacy_screen_register
EXPORT_SYMS+=	drm_privacy_screen_register_notifier
EXPORT_SYMS+=	drm_privacy_screen_set_sw_state
EXPORT_SYMS+=	drm_privacy_screen_unregister
EXPORT_SYMS+=	drm_privacy_screen_unregister_notifier

# drivers/gpu/drm/drm_probe_helper.c
EXPORT_SYMS+=	drm_connector_helper_get_modes
EXPORT_SYMS+=	drm_connector_helper_get_modes_fixed
EXPORT_SYMS+=	drm_connector_helper_get_modes_from_ddc
EXPORT_SYMS+=	drm_connector_helper_hpd_irq_event
EXPORT_SYMS+=	drm_connector_helper_tv_get_modes
EXPORT_SYMS+=	drm_crtc_helper_mode_valid_fixed
EXPORT_SYMS+=	drm_helper_hpd_irq_event
EXPORT_SYMS+=	drm_helper_probe_detect
EXPORT_SYMS+=	drm_helper_probe_single_connector_modes
EXPORT_SYMS+=	drm_kms_helper_connector_hotplug_event
EXPORT_SYMS+=	drm_kms_helper_hotplug_event
EXPORT_SYMS+=	drm_kms_helper_is_poll_worker
EXPORT_SYMS+=	drm_kms_helper_poll_disable
EXPORT_SYMS+=	drm_kms_helper_poll_enable
EXPORT_SYMS+=	drm_kms_helper_poll_fini
EXPORT_SYMS+=	drm_kms_helper_poll_init
EXPORT_SYMS+=	drm_kms_helper_poll_reschedule

# drivers/gpu/drm/drm_property.c
EXPORT_SYMS+=	drm_property_add_enum
EXPORT_SYMS+=	drm_property_blob_get
EXPORT_SYMS+=	drm_property_blob_put
EXPORT_SYMS+=	drm_property_create
EXPORT_SYMS+=	drm_property_create_bitmask
EXPORT_SYMS+=	drm_property_create_blob
EXPORT_SYMS+=	drm_property_create_bool
EXPORT_SYMS+=	drm_property_create_enum
EXPORT_SYMS+=	drm_property_create_object
EXPORT_SYMS+=	drm_property_create_range
EXPORT_SYMS+=	drm_property_create_signed_range
EXPORT_SYMS+=	drm_property_destroy
EXPORT_SYMS+=	drm_property_lookup_blob
EXPORT_SYMS+=	drm_property_replace_blob
EXPORT_SYMS+=	drm_property_replace_global_blob

# drivers/gpu/drm/drm_rect.c
EXPORT_SYMS+=	drm_rect_calc_hscale
EXPORT_SYMS+=	drm_rect_calc_vscale
EXPORT_SYMS+=	drm_rect_clip_scaled
EXPORT_SYMS+=	drm_rect_debug_print
EXPORT_SYMS+=	drm_rect_intersect
EXPORT_SYMS+=	drm_rect_rotate
EXPORT_SYMS+=	drm_rect_rotate_inv

# drivers/gpu/drm/drm_self_refresh_helper.c
EXPORT_SYMS+=	drm_self_refresh_helper_alter_state
EXPORT_SYMS+=	drm_self_refresh_helper_cleanup
EXPORT_SYMS+=	drm_self_refresh_helper_init
EXPORT_SYMS+=	drm_self_refresh_helper_update_avg_times

# drivers/gpu/drm/drm_suballoc.c
EXPORT_SYMS+=	drm_suballoc_dump_debug_info
EXPORT_SYMS+=	drm_suballoc_free
EXPORT_SYMS+=	drm_suballoc_manager_fini
EXPORT_SYMS+=	drm_suballoc_manager_init
EXPORT_SYMS+=	drm_suballoc_new

# drivers/gpu/drm/drm_syncobj.c
EXPORT_SYMS+=	drm_syncobj_add_point
EXPORT_SYMS+=	drm_syncobj_create
EXPORT_SYMS+=	drm_syncobj_find
EXPORT_SYMS+=	drm_syncobj_find_fence
EXPORT_SYMS+=	drm_syncobj_free
EXPORT_SYMS+=	drm_syncobj_get_fd
EXPORT_SYMS+=	drm_syncobj_get_handle
EXPORT_SYMS+=	drm_syncobj_replace_fence
EXPORT_SYMS+=	drm_timeout_abs_to_jiffies

# drivers/gpu/drm/drm_vblank.c
EXPORT_SYMS+=	drm_calc_timestamping_constants
EXPORT_SYMS+=	drm_crtc_accurate_vblank_count
EXPORT_SYMS+=	drm_crtc_arm_vblank_event
EXPORT_SYMS+=	drm_crtc_handle_vblank
EXPORT_SYMS+=	drm_crtc_next_vblank_start
EXPORT_SYMS+=	drm_crtc_send_vblank_event
EXPORT_SYMS+=	drm_crtc_set_max_vblank_count
EXPORT_SYMS+=	drm_crtc_vblank_count
EXPORT_SYMS+=	drm_crtc_vblank_count_and_time
EXPORT_SYMS+=	drm_crtc_vblank_get
EXPORT_SYMS+=	drm_crtc_vblank_helper_get_vblank_timestamp
EXPORT_SYMS+=	drm_crtc_vblank_helper_get_vblank_timestamp_internal
EXPORT_SYMS+=	drm_crtc_vblank_off
EXPORT_SYMS+=	drm_crtc_vblank_on
EXPORT_SYMS+=	drm_crtc_vblank_put
EXPORT_SYMS+=	drm_crtc_vblank_reset
EXPORT_SYMS+=	drm_crtc_vblank_restore
EXPORT_SYMS+=	drm_crtc_vblank_waitqueue
EXPORT_SYMS+=	drm_crtc_wait_one_vblank
EXPORT_SYMS+=	drm_dev_has_vblank
EXPORT_SYMS+=	drm_handle_vblank
EXPORT_SYMS+=	drm_vblank_init
EXPORT_SYMS+=	drm_wait_one_vblank

# drivers/gpu/drm/drm_vblank_work.c
EXPORT_SYMS+=	drm_vblank_work_cancel_sync
EXPORT_SYMS+=	drm_vblank_work_flush
EXPORT_SYMS+=	drm_vblank_work_init
EXPORT_SYMS+=	drm_vblank_work_schedule

# drivers/gpu/drm/drm_vma_manager.c
EXPORT_SYMS+=	drm_vma_node_allow
EXPORT_SYMS+=	drm_vma_node_allow_once
EXPORT_SYMS+=	drm_vma_node_is_allowed
EXPORT_SYMS+=	drm_vma_node_revoke
EXPORT_SYMS+=	drm_vma_offset_add
EXPORT_SYMS+=	drm_vma_offset_lookup_locked
EXPORT_SYMS+=	drm_vma_offset_manager_destroy
EXPORT_SYMS+=	drm_vma_offset_manager_init
EXPORT_SYMS+=	drm_vma_offset_remove

# drivers/gpu/drm/scheduler/sched_entity.c
EXPORT_SYMS+=	drm_sched_entity_destroy
EXPORT_SYMS+=	drm_sched_entity_error
EXPORT_SYMS+=	drm_sched_entity_fini
EXPORT_SYMS+=	drm_sched_entity_flush
EXPORT_SYMS+=	drm_sched_entity_init
EXPORT_SYMS+=	drm_sched_entity_modify_sched
EXPORT_SYMS+=	drm_sched_entity_push_job
EXPORT_SYMS+=	drm_sched_entity_set_priority

# drivers/gpu/drm/scheduler/sched_fence.c
EXPORT_SYMS+=	to_drm_sched_fence

### drivers/gpu/drm/scheduler/sched_main.c
EXPORT_SYMS+=	drm_sched_fault
EXPORT_SYMS+=	drm_sched_fini
EXPORT_SYMS+=	drm_sched_increase_karma
EXPORT_SYMS+=	drm_sched_init
EXPORT_SYMS+=	drm_sched_job_add_dependency
EXPORT_SYMS+=	drm_sched_job_add_implicit_dependencies
EXPORT_SYMS+=	drm_sched_job_add_resv_dependencies
EXPORT_SYMS+=	drm_sched_job_add_syncobj_dependency
EXPORT_SYMS+=	drm_sched_job_arm
EXPORT_SYMS+=	drm_sched_job_cleanup
EXPORT_SYMS+=	drm_sched_job_init
EXPORT_SYMS+=	drm_sched_pick_best
EXPORT_SYMS+=	drm_sched_resubmit_jobs
EXPORT_SYMS+=	drm_sched_resume_timeout
EXPORT_SYMS+=	drm_sched_start
EXPORT_SYMS+=	drm_sched_stop
EXPORT_SYMS+=	drm_sched_suspend_timeout

EXPORT_SYMS+=	drmm_add_action
EXPORT_SYMS+=	drmm_add_action_or_reset
EXPORT_SYMS+=	drmm_kzalloc
#EXPORT_SYMS+=	drmm_kmalloc
#EXPORT_SYMS+=	drmm_kstrdup
#EXPORT_SYMS+=	drmm_kfree
EXPORT_SYMS+=	register_fictitious_range
EXPORT_SYMS+=	unregister_fictitious_range
EXPORT_SYMS+=	fb_io_read
EXPORT_SYMS+=	fb_io_write
EXPORT_SYMS+=	cfb_fillrect
EXPORT_SYMS+=	cfb_copyarea
EXPORT_SYMS+=	cfb_imageblit
EXPORT_SYMS+=	drm_gem_fb_get_obj

.include <bsd.kmod.mk>

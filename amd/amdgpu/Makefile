# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/amd
DRM= ${.CURDIR:H:H}/drivers/gpu/drm

KMOD=	amdgpu

#
# Work around ctfmerge(1) segfaults with recent amdgpu.ko
#
# https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=276828
#
MK_CTF=	no

.include "../../kconfig.mk"
.include "../../linuxkpi_version.mk"

.if !empty(KCONFIG:MDRM_AMD_DC_FP)
_dml=	${SRCDIR}/display/dc/dml \
	${SRCDIR}/display/dc/dml/dcn10 \
	${SRCDIR}/display/dc/dml/dcn20 \
	${SRCDIR}/display/dc/dml/dcn21 \
	${SRCDIR}/display/dc/dml/dcn30 \
	${SRCDIR}/display/dc/dml/dcn301 \
	${SRCDIR}/display/dc/dml/dcn302 \
	${SRCDIR}/display/dc/dml/dcn303 \
	${SRCDIR}/display/dc/dml/dcn31 \
	${SRCDIR}/display/dc/dml/dcn314 \
	${SRCDIR}/display/dc/dml/dcn32 \
	${SRCDIR}/display/dc/dml/dcn321 \
	${SRCDIR}/display/dc/dml/dcn35 \
	${SRCDIR}/display/dc/dml/dsc \
	${SRCDIR}/display/dc/dml2
.endif

.PATH:	${SRCDIR}/acp \
	${SRCDIR}/amdgpu \
	${SRCDIR}/amdxcp \
	${SRCDIR}/display \
	${SRCDIR}/display/amdgpu_dm \
	${SRCDIR}/display/dc \
	${SRCDIR}/display/dc/basics \
	${SRCDIR}/display/dc/bios \
	${SRCDIR}/display/dc/bios/dce110 \
	${SRCDIR}/display/dc/bios/dce112 \
	${SRCDIR}/display/dc/bios/dce60 \
	${SRCDIR}/display/dc/bios/dce80 \
	${SRCDIR}/display/dc/core \
	${SRCDIR}/display/dc/clk_mgr \
	${SRCDIR}/display/dc/clk_mgr/dce100 \
	${SRCDIR}/display/dc/clk_mgr/dce110 \
	${SRCDIR}/display/dc/clk_mgr/dce112 \
	${SRCDIR}/display/dc/clk_mgr/dce120 \
	${SRCDIR}/display/dc/clk_mgr/dce60 \
	${SRCDIR}/display/dc/clk_mgr/dcn10 \
	${SRCDIR}/display/dc/clk_mgr/dcn20 \
	${SRCDIR}/display/dc/clk_mgr/dcn201 \
	${SRCDIR}/display/dc/clk_mgr/dcn21 \
	${SRCDIR}/display/dc/clk_mgr/dcn30 \
	${SRCDIR}/display/dc/clk_mgr/dcn301 \
	${SRCDIR}/display/dc/clk_mgr/dcn31 \
	${SRCDIR}/display/dc/clk_mgr/dcn314 \
	${SRCDIR}/display/dc/clk_mgr/dcn315 \
	${SRCDIR}/display/dc/clk_mgr/dcn316 \
	${SRCDIR}/display/dc/clk_mgr/dcn32 \
	${SRCDIR}/display/dc/clk_mgr/dcn35 \
	${SRCDIR}/display/dc/dce \
	${SRCDIR}/display/dc/dce100 \
	${SRCDIR}/display/dc/dce110 \
	${SRCDIR}/display/dc/dce112 \
	${SRCDIR}/display/dc/dce120 \
	${SRCDIR}/display/dc/dce60 \
	${SRCDIR}/display/dc/dce80 \
	${SRCDIR}/display/dc/dcn10 \
	${SRCDIR}/display/dc/dcn20 \
	${SRCDIR}/display/dc/dcn201 \
	${SRCDIR}/display/dc/dcn21 \
	${SRCDIR}/display/dc/dcn30 \
	${SRCDIR}/display/dc/dcn301 \
	${SRCDIR}/display/dc/dcn302 \
	${SRCDIR}/display/dc/dcn303 \
	${SRCDIR}/display/dc/dcn31 \
	${SRCDIR}/display/dc/dcn314 \
	${SRCDIR}/display/dc/dcn315 \
	${SRCDIR}/display/dc/dcn316 \
	${SRCDIR}/display/dc/dcn32 \
	${SRCDIR}/display/dc/dcn321 \
	${SRCDIR}/display/dc/dcn35 \
	${SRCDIR}/display/dc/hwss/dce \
	${SRCDIR}/display/dc/hwss/dce100 \
	${SRCDIR}/display/dc/hwss/dce110 \
	${SRCDIR}/display/dc/hwss/dce112 \
	${SRCDIR}/display/dc/hwss/dce120 \
	${SRCDIR}/display/dc/hwss/dce60 \
	${SRCDIR}/display/dc/hwss/dce80 \
	${SRCDIR}/display/dc/hwss/dcn10 \
	${SRCDIR}/display/dc/hwss/dcn20 \
	${SRCDIR}/display/dc/hwss/dcn201 \
	${SRCDIR}/display/dc/hwss/dcn21 \
	${SRCDIR}/display/dc/hwss/dcn30 \
	${SRCDIR}/display/dc/hwss/dcn301 \
	${SRCDIR}/display/dc/hwss/dcn302 \
	${SRCDIR}/display/dc/hwss/dcn303 \
	${SRCDIR}/display/dc/hwss/dcn31 \
	${SRCDIR}/display/dc/hwss/dcn314 \
	${SRCDIR}/display/dc/hwss/dcn315 \
	${SRCDIR}/display/dc/hwss/dcn316 \
	${SRCDIR}/display/dc/hwss/dcn32 \
	${SRCDIR}/display/dc/hwss/dcn321 \
	${SRCDIR}/display/dc/hwss/dcn35 \
	${SRCDIR}/display/dc/hwss/dcn351 \
	${SRCDIR}/display/dc/dml/calcs \
	${SRCDIR}/display/dc/dsc \
	${SRCDIR}/display/dc/dsc/dcn20 \
	${SRCDIR}/display/dc/dsc/dcn35 \
	${_dml} \
	${SRCDIR}/display/dc/gpio \
	${SRCDIR}/display/dc/gpio/dce110 \
	${SRCDIR}/display/dc/gpio/dce120 \
	${SRCDIR}/display/dc/gpio/dce60 \
	${SRCDIR}/display/dc/gpio/dce80 \
	${SRCDIR}/display/dc/gpio/dcn10 \
	${SRCDIR}/display/dc/gpio/dcn20 \
	${SRCDIR}/display/dc/gpio/dcn21 \
	${SRCDIR}/display/dc/gpio/dcn30 \
	${SRCDIR}/display/dc/gpio/dcn315 \
	${SRCDIR}/display/dc/gpio/dcn32 \
	${SRCDIR}/display/dc/hdcp \
	${SRCDIR}/display/dc/i2caux \
	${SRCDIR}/display/dc/i2caux/dce100 \
	${SRCDIR}/display/dc/i2caux/dce110 \
	${SRCDIR}/display/dc/i2caux/dce112 \
	${SRCDIR}/display/dc/i2caux/dce120 \
	${SRCDIR}/display/dc/i2caux/dce80 \
	${SRCDIR}/display/dc/i2caux/dcn10 \
	${SRCDIR}/display/dc/i2caux/diagnostics \
	${SRCDIR}/display/dc/inc \
	${SRCDIR}/display/dc/inc/hw \
	${SRCDIR}/display/dc/irq \
	${SRCDIR}/display/dc/irq/dce110 \
	${SRCDIR}/display/dc/irq/dce120 \
	${SRCDIR}/display/dc/irq/dce60 \
	${SRCDIR}/display/dc/irq/dce80 \
	${SRCDIR}/display/dc/irq/dcn10 \
	${SRCDIR}/display/dc/irq/dcn20 \
	${SRCDIR}/display/dc/irq/dcn201 \
	${SRCDIR}/display/dc/irq/dcn21 \
	${SRCDIR}/display/dc/irq/dcn30 \
	${SRCDIR}/display/dc/irq/dcn302 \
	${SRCDIR}/display/dc/irq/dcn303 \
	${SRCDIR}/display/dc/irq/dcn31 \
	${SRCDIR}/display/dc/irq/dcn314 \
	${SRCDIR}/display/dc/irq/dcn315 \
	${SRCDIR}/display/dc/irq/dcn32 \
	${SRCDIR}/display/dc/irq/dcn35 \
	${SRCDIR}/display/dc/link \
	${SRCDIR}/display/dc/link/accessories \
	${SRCDIR}/display/dc/link/hwss \
	${SRCDIR}/display/dc/link/protocols \
	${SRCDIR}/display/dc/optc/dcn10 \
	${SRCDIR}/display/dc/optc/dcn20 \
	${SRCDIR}/display/dc/optc/dcn201 \
	${SRCDIR}/display/dc/optc/dcn30 \
	${SRCDIR}/display/dc/optc/dcn301 \
	${SRCDIR}/display/dc/optc/dcn31 \
	${SRCDIR}/display/dc/optc/dcn314 \
	${SRCDIR}/display/dc/optc/dcn32 \
	${SRCDIR}/display/dc/optc/dcn35 \
	${SRCDIR}/display/dc/resource/dce100 \
	${SRCDIR}/display/dc/resource/dce110 \
	${SRCDIR}/display/dc/resource/dce112 \
	${SRCDIR}/display/dc/resource/dce120 \
	${SRCDIR}/display/dc/resource/dce60 \
	${SRCDIR}/display/dc/resource/dce80 \
	${SRCDIR}/display/dc/resource/dcn10 \
	${SRCDIR}/display/dc/resource/dcn20 \
	${SRCDIR}/display/dc/resource/dcn201 \
	${SRCDIR}/display/dc/resource/dcn21 \
	${SRCDIR}/display/dc/resource/dcn30 \
	${SRCDIR}/display/dc/resource/dcn301 \
	${SRCDIR}/display/dc/resource/dcn302 \
	${SRCDIR}/display/dc/resource/dcn303 \
	${SRCDIR}/display/dc/resource/dcn31 \
	${SRCDIR}/display/dc/resource/dcn314 \
	${SRCDIR}/display/dc/resource/dcn315 \
	${SRCDIR}/display/dc/resource/dcn316 \
	${SRCDIR}/display/dc/resource/dcn32 \
	${SRCDIR}/display/dc/resource/dcn321 \
	${SRCDIR}/display/dc/resource/dcn35 \
	${SRCDIR}/display/dc/virtual \
	${SRCDIR}/display/dmub/src \
	${SRCDIR}/display/modules \
	${SRCDIR}/display/modules/color \
	${SRCDIR}/display/modules/freesync \
	${SRCDIR}/display/modules/hdcp \
	${SRCDIR}/display/modules/inc \
	${SRCDIR}/display/modules/info_packet \
	${SRCDIR}/display/modules/power \
	${SRCDIR}/lib \
	${SRCDIR}/pm \
	${SRCDIR}/pm/inc \
	${SRCDIR}/pm/legacy-dpm \
	${SRCDIR}/pm/powerplay \
	${SRCDIR}/pm/powerplay/hwmgr \
	${SRCDIR}/pm/powerplay/inc \
	${SRCDIR}/pm/powerplay/smumgr \
	${SRCDIR}/pm/swsmu \
	${SRCDIR}/pm/swsmu/inc \
	${SRCDIR}/pm/swsmu/inc/pmfw_if \
	${SRCDIR}/pm/swsmu/smu11 \
	${SRCDIR}/pm/swsmu/smu12 \
	${SRCDIR}/pm/swsmu/smu13 \
	${SRCDIR}/pm/swsmu/smu14 \
	${DRM}/scheduler

# acp
SRCS=	acp_hw.c

# amdgpu
# amdgpu_acp.c \ # not yet - need i2s
# amdgpu_hmm.c \  # disabled

SRCS+=	amdgpu_afmt.c \
	amdgpu_amdkfd.c \
	amdgpu_atombios.c \
	amdgpu_atomfirmware.c \
	amdgpu_benchmark.c \
	amdgpu_bios.c \
	amdgpu_bo_list.c \
	amdgpu_cgs.c \
	amdgpu_connectors.c \
	amdgpu_cs.c \
	amdgpu_csa.c \
	amdgpu_ctx.c \
	amdgpu_debugfs.c \
	amdgpu_device.c \
	amdgpu_discovery.c \
	amdgpu_display.c \
	amdgpu_doorbell_mgr.c \
	amdgpu_dpm.c \
	amdgpu_dpm_internal.c \
	amdgpu_drv.c \
	amdgpu_eeprom.c \
	amdgpu_encoders.c \
	amdgpu_fdinfo.c \
	amdgpu_fence.c \
	amdgpu_freebsd.c \
	amdgpu_fru_eeprom.c \
	amdgpu_fw_attestation.c \
	amdgpu_securedisplay.c \
	amdgpu_gart.c \
	amdgpu_gem.c \
	amdgpu_gfx.c \
	amdgpu_gmc.c \
	amdgpu_gtt_mgr.c \
	amdgpu_preempt_mgr.c \
	amdgpu_hdp.c \
	amdgpu_i2c.c \
	amdgpu_ib.c \
	amdgpu_ids.c \
	amdgpu_ih.c \
	amdgpu_ioc32.c \
	amdgpu_irq.c \
	amdgpu_job.c \
	amdgpu_jpeg.c \
	amdgpu_kms.c \
	amdgpu_lsdma.c \
	amdgpu_mca.c \
	amdgpu_mes.c \
	amdgpu_mmhub.c \
	amdgpu_nbio.c \
	amdgpu_object.c \
	amdgpu_pll.c \
	amdgpu_pm.c \
	amdgpu_dma_buf.c \
	amdgpu_psp.c \
	amdgpu_psp_ta.c \
	amdgpu_ring.c \
	amdgpu_rap.c \
	amdgpu_ras.c \
	amdgpu_ras_eeprom.c \
	amdgpu_reset.c \
	amdgpu_ring_mux.c \
	amdgpu_rlc.c \
	amdgpu_sa.c \
	amdgpu_sched.c \
	amdgpu_sdma.c \
	amdgpu_umsch_mm.c \
	amdgpu_vpe.c \
	sienna_cichlid.c \
	amdgpu_seq64.c \
	amdgpu_sync.c \
	amdgpu_trace_points.c \
	amdgpu_ttm.c \
	amdgpu_ucode.c \
	amdgpu_umc.c \
	amdgpu_uvd.c \
	amdgpu_vce.c \
	amdgpu_vcn.c \
	amdgpu_vf_error.c \
	amdgpu_virt.c \
	amdgpu_vm.c \
	amdgpu_vkms.c \
	amdgpu_vm_cpu.c \
	amdgpu_vm_pt.c \
	amdgpu_vm_sdma.c \
	amdgpu_vram_mgr.c \
	amdgpu_xcp.c \
	amdgpu_xgmi.c \
	arct_reg_init.c \
	athub_v1_0.c \
	athub_v2_0.c \
	athub_v2_1.c \
	athub_v3_0.c \
	atom.c \
	atombios_crtc.c \
	atombios_dp.c \
	atombios_encoders.c \
	atombios_i2c.c \
	aqua_vanjaram.c \
	cik_ih.c \
	cik_sdma.c \
	cik.c \
	cz_ih.c \
	dce_v10_0.c \
	dce_v11_0.c \
	dce_v6_0.c \
	dce_v8_0.c \
	dimgrey_cavefish_reg_init.c \
	df_v1_7.c \
	df_v3_6.c \
	df_v4_3.c \
	df_v4_6_2.c \
	emu_soc.c \
	gfx_v6_0.c \
	gfx_v7_0.c \
	gfx_v8_0.c \
	gfx_v9_0.c \
	gfx_v9_4.c \
	gfx_v9_4_2.c \
	gfx_v9_4_3.c \
	gfx_v10_0.c \
	gfx_v11_0.c \
	gfx_v11_0_3.c \
	gfxhub_v1_0.c \
	gfxhub_v1_1.c \
	gfxhub_v1_2.c \
	gfxhub_v2_0.c \
	gfxhub_v2_1.c \
	gfxhub_v3_0.c \
	gfxhub_v3_0_3.c \
	gfxhub_v11_5_0.c \
	gmc_v6_0.c \
	gmc_v7_0.c \
	gmc_v8_0.c \
	gmc_v9_0.c \
	gmc_v10_0.c \
	gmc_v11_0.c \
	hdp_v4_0.c \
	hdp_v5_0.c \
	hdp_v5_2.c \
	hdp_v6_0.c \
	aldebaran_reg_init.c \
	aldebaran.c \
	iceland_ih.c \
	ih_v6_0.c \
	ih_v6_1.c \
	imu_v11_0.c \
	imu_v11_0_3.c \
	jpeg_v1_0.c \
	jpeg_v2_0.c \
	jpeg_v2_5.c \
	jpeg_v3_0.c \
	jpeg_v4_0.c \
	jpeg_v4_0_3.c \
	jpeg_v4_0_5.c \
	kv_dpm.c \
	kv_smc.c \
	legacy_dpm.c \
	lsdma_v6_0.c \
	mca_v3_0.c \
	mes_v10_1.c \
	mes_v11_0.c \
	mmhub_v1_0.c \
	mmhub_v1_7.c \
	mmhub_v1_8.c \
	mmhub_v2_0.c \
	mmhub_v2_3.c \
	mmhub_v3_0.c \
	mmhub_v3_0_1.c \
	mmhub_v3_0_2.c \
	mmhub_v3_3.c \
	mmhub_v9_4.c \
	mxgpu_ai.c \
	mxgpu_nv.c \
	mxgpu_vi.c \
	nbio_v2_3.c \
	nbio_v4_3.c \
	nbio_v6_1.c \
	nbio_v7_0.c \
	nbio_v7_2.c \
	nbio_v7_4.c \
	nbio_v7_7.c \
	nbio_v7_9.c \
	nbio_v7_11.c \
	navi10_ih.c \
	nv.c \
	psp_v3_1.c \
	psp_v10_0.c \
	psp_v11_0.c \
	psp_v11_0_8.c \
	psp_v12_0.c \
	psp_v13_0.c \
	psp_v13_0_4.c \
	sdma_v2_4.c \
	sdma_v3_0.c \
	sdma_v4_0.c \
	sdma_v4_4.c \
	sdma_v4_4_2.c \
	sdma_v5_0.c \
	sdma_v5_2.c \
	sdma_v6_0.c \
	si_dma.c \
	si_dpm.c \
	si_ih.c \
	si_smc.c \
	si.c \
	soc15.c \
	soc21.c \
	smu_v13_0_10.c \
	smuio_v9_0.c \
	smuio_v11_0.c \
	smuio_v11_0_6.c \
	smuio_v13_0.c \
	smuio_v13_0_3.c \
	smuio_v13_0_6.c \
	tonga_ih.c \
	umc_v6_0.c \
	umc_v6_1.c \
	umc_v6_7.c \
	umc_v8_10.c \
	umc_v8_7.c \
	umc_v12_0.c \
	umsch_mm_v4_0.c \
	uvd_v3_1.c \
	uvd_v4_2.c \
	uvd_v5_0.c \
	uvd_v6_0.c \
	uvd_v7_0.c \
	vce_v2_0.c \
	vce_v3_0.c \
	vce_v4_0.c \
	vcn_sw_ring.c \
	vcn_v1_0.c \
	vcn_v2_0.c \
	vcn_v2_5.c \
	vcn_v3_0.c \
	vcn_v4_0.c \
	vcn_v4_0_3.c \
	vcn_v4_0_5.c \
	vega10_ih.c \
	vega10_reg_init.c \
	vega20_ih.c \
	vega20_reg_init.c \
	vi.c \
	vpe_v6_1.c

# amdxcp
SRCS+=	amdgpu_xcp_drv.c

# display/amdgpu_dm
SRCS+=	amdgpu_dm_color.c \
	amdgpu_dm_crc.c \
	amdgpu_dm_crtc.c \
	amdgpu_dm_debugfs.c \
	amdgpu_dm_hdcp.c \
	amdgpu_dm_helpers.c \
	amdgpu_dm_irq.c \
	amdgpu_dm_mst_types.c \
	amdgpu_dm_plane.c \
	amdgpu_dm_pp_smu.c \
	amdgpu_dm_psr.c \
	amdgpu_dm_replay.c \
	amdgpu_dm_services.c \
	amdgpu_dm_wb.c \
	amdgpu_dm.c \
	dc_fpu.c

# display/dc/basics
SRCS+=	bw_fixed.c \
	conversion.c \
	custom_float.c \
	dc_common.c \
	dce_calcs.c \
	fixpt31_32.c \
	vector.c

# display/dc/bios
SRCS+=	bios_parser_common.c \
	bios_parser_helper.c \
	bios_parser_interface.c \
	bios_parser.c \
	bios_parser2.c \
	command_table_helper.c \
	command_table_helper2.c \
	command_table.c \
	command_table2.c \
	command_table_helper2_dce112.c \
	command_table_helper_dce110.c \
	command_table_helper_dce112.c \
	command_table_helper_dce60.c \
	command_table_helper_dce80.c

# display/dc/core
SRCS+=	dc_debug.c \
	dc_hw_sequencer.c \
	dc_link_exports.c \
	dc_resource.c \
	dc_sink.c \
	dc_state.c \
	dc_stream.c \
	dc_link_enc_cfg.c \
	dc_surface.c \
	dc.c \
	dc_stat.c

# display/dc/dce
SRCS+=	dce_abm.c \
	dce_audio.c \
	dce_aux.c \
	dce_clock_source.c \
	dce_dmcu.c \
	dce_hwseq.c \
	dce_i2c_hw.c \
	dce_i2c_sw.c \
	dce_i2c.c \
	dce_ipp.c \
	dce_link_encoder.c \
	dce_mem_input.c \
	dce_opp.c \
	dce_panel_cntl.c \
	dce_scl_filters.c \
	dce_stream_encoder.c \
	dce_transform.c \
	dmub_abm.c \
	dmub_abm_lcd.c \
	dmub_psr.c \
	dmub_replay.c \
	dmub_hw_lock_mgr.c \
	dmub_outbox.c

# display/dc/dce100
SRCS+=	dce100_hwseq.c \
	dce100_resource.c

# display/dc/dce110
SRCS+=	dce110_compressor.c \
	dce110_hwseq.c \
	dce110_mem_input_v.c \
	dce110_opp_csc_v.c \
	dce110_opp_regamma_v.c \
	dce110_opp_v.c \
	dce110_resource.c \
	dce110_timing_generator.c \
	dce110_timing_generator_v.c \
	dce110_transform_v.c

# display/dc/dce112
SRCS+=	dce112_compressor.c \
	dce112_hwseq.c \
	dce112_resource.c

# display/dc/dce120
SRCS+=	dce120_hwseq.c \
	dce120_resource.c \
	dce120_timing_generator.c

# display/dc/dce60
SRCS+=	dce60_hw_sequencer.c \
	dce60_resource.c \
	dce60_timing_generator.c

# display/dc/dce80
SRCS+=	dce80_hwseq.c \
	dce80_resource.c \
	dce80_timing_generator.c

# display/dc/clk_mgr
SRCS+=	clk_mgr.c \
	dce_clk_mgr.c \
	dce110_clk_mgr.c \
	dce112_clk_mgr.c \
	dce120_clk_mgr.c \
	dce60_clk_mgr.c \
	rv1_clk_mgr.c \
	rv1_clk_mgr_vbios_smu.c \
	rv2_clk_mgr.c

# display/dc/dcn10
.if !empty(KCONFIG:MDRM_AMD_DC_FP)
SRCS+=	dcn10_cm_common.c \
	dcn10_dpp.c \
	dcn10_dpp_cm.c \
	dcn10_dpp_dscl.c \
	dcn10_hubbub.c \
	dcn10_hubp.c \
	dcn10_hwseq.c \
	dcn10_hw_sequencer_debug.c \
	dcn10_init.c \
	dcn10_ipp.c \
	dcn10_link_encoder.c \
	dcn10_mpc.c \
	dcn10_opp.c \
	dcn10_optc.c \
	dcn10_resource.c \
	dcn10_stream_encoder.c

# display/dc/dcn20
SRCS+=	dcn20_clk_mgr.c \
	dcn20_dccg.c \
	dcn20_dpp_cm.c \
	dcn20_dpp.c \
	dcn20_dsc.c \
	dcn20_dwb_scl.c \
	dcn20_dwb.c \
	dcn20_fpu.c \
	dcn20_hubbub.c \
	dcn20_hubp.c \
	dcn20_hwseq.c \
	dcn20_init.c \
	dcn20_link_encoder.c \
	dcn20_mmhubbub.c \
	dcn20_mpc.c \
	dcn20_opp.c \
	dcn20_optc.c \
	dcn20_resource.c \
	dcn20_stream_encoder.c \
	dcn20_vmid.c \
	display_rq_dlg_calc_20v2.c \
	display_mode_vba_20v2.c

# display/dc/dcn201
SRCS+=	dcn201_clk_mgr.c \
	dcn201_dccg.c \
	dcn201_dpp.c \
	dcn201_hubbub.c \
	dcn201_hubp.c \
	dcn201_hwseq.c \
	dcn201_init.c \
	dcn201_link_encoder.c \
	dcn201_mpc.c \
	dcn201_opp.c \
	dcn201_optc.c \
	dcn201_resource.c \
	irq_service_dcn201.c

# display/dc/dcn21
SRCS+=	dcn21_dccg.c \
	dcn21_hubbub.c \
	dcn21_hubp.c \
	dcn21_hwseq.c \
	dcn21_init.c \
	dcn21_link_encoder.c \
	dcn21_resource.c

# display/dc/dcn30
.if !empty(KCONFIG:MDRM_AMD_DC_FP)
SRCS+=	dcn30_dio_link_encoder.c \
	dcn30_clk_mgr.c \
	dcn30_clk_mgr_smu_msg.c \
	dcn30_dccg.c \
	dcn30_optc.c \
	dcn30_mpc.c \
	dcn30_cm_common.c \
	dcn30_dpp.c \
	dcn30_dpp_cm.c \
	dcn30_hubbub.c \
	dcn30_hubp.c \
	dcn30_mmhubbub.c \
	dcn30_dwb.c \
	dcn30_dwb_cm.c \
	dcn30_dio_stream_encoder.c \
	dcn30_hwseq.c \
	dcn30_resource.c \
	dcn30_afmt.c \
	dcn30_vpg.c \
	dcn30_init.c \
	irq_service_dcn30.c \
	hw_factory_dcn30.c \
	hw_translate_dcn30.c \
	dmub_dcn30.c \
	display_mode_vba_30.c \
	display_rq_dlg_calc_30.c
.endif

# display/dc/dcn301
SRCS+=	dcn301_init.c \
	dcn301_resource.c \
	dcn301_dccg.c \
	dcn301_dio_link_encoder.c \
	dcn301_fpu.c \
	dcn301_hwseq.c \
	dcn301_optc.c \
	dcn301_panel_cntl.c \
	dcn301_hubbub.c \
	vg_clk_mgr.c \
	dcn301_smu.c \
	dmub_dcn301.c

# display/dc/dcn302
SRCS+=	dcn302_init.c \
	dcn302_hwseq.c \
	dcn302_resource.c \
	irq_service_dcn302.c \
	dmub_dcn302.c

# display/dc/dcn303
SRCS+=	dcn303_hwseq.c \
	dcn303_init.c \
	dcn303_resource.c \
	irq_service_dcn303.c \
	dmub_dcn303.c

# display/dc/dcn314
SRCS+=	dcn314_dccg.c \
	dcn314_dio_stream_encoder.c \
	dcn314_hwseq.c \
	dcn314_init.c \
	dcn314_optc.c \
	dcn314_resource.c

# display/dc/dcn315
SRCS+=	dcn315_resource.c

# display/dc/dcn316
SRCS+=	dcn316_resource.c

# display/dc/dcn32
SRCS+=	dcn32_dccg.c \
	dcn32_dio_link_encoder.c \
	dcn32_dio_stream_encoder.c \
	dcn32_dpp.c \
	dcn32_hpo_dp_link_encoder.c \
	dcn32_hubbub.c \
	dcn32_hubp.c \
	dcn32_hwseq.c \
	dcn32_init.c \
	dcn32_mmhubbub.c \
	dcn32_mpc.c \
	dcn32_optc.c \
	dcn32_resource.c \
	dcn32_resource_helpers.c

# display/dc/dcn321
SRCS+=	dcn321_dio_link_encoder.c \
	dcn321_resource.c

# display/dc/dcn35
SRCS+=	dcn35_clk_mgr.c \
	dcn35_dccg.c \
	dcn35_dio_link_encoder.c \
	dcn35_dio_stream_encoder.c \
	dcn35_dpp.c \
	dcn35_dsc.c \
	dcn35_dwb.c \
	dcn35_fpu.c \
	dcn35_hubbub.c \
	dcn35_hubp.c \
	dcn35_hwseq.c \
	dcn35_init.c \
	dcn35_mmhubbub.c \
	dcn35_opp.c \
	dcn35_optc.c \
	dcn35_pg_cntl.c \
	dcn35_resource.c \
	dcn35_smu.c \
	dmub_dcn35.c \
	irq_service_dcn35.c

# display/dc/clk_mgr/dcn31
SRCS+=	dcn31_afmt.c \
	dcn31_apg.c \
	dcn31_smu.c \
	dcn31_clk_mgr.c \
	dcn31_dccg.c \
	dcn31_dio_link_encoder.c \
	dcn31_hpo_dp_link_encoder.c \
	dcn31_hpo_dp_stream_encoder.c \
	dcn31_hubp.c \
	dcn31_hubbub.c \
	dcn31_hwseq.c \
	dcn31_init.c \
	dcn31_optc.c \
	dcn31_panel_cntl.c \
	dcn31_resource.c \
	dcn31_vpg.c \
	display_mode_vba_31.c \
	display_rq_dlg_calc_31.c \
	dmub_dcn31.c \
	irq_service_dcn31.c

# display/dc/clk_mgr/dcn314
SRCS+=	dcn314_clk_mgr.c \
	dcn314_smu.c

# display/dc/clk_mgr/dcn316
SRCS+=	dcn315_clk_mgr.c \
	dcn315_smu.c

# display/dc/clk_mgr/dcn316
SRCS+=	dcn316_clk_mgr.c \
	dcn316_smu.c

# display/dc/clk_mgr/dcn32
SRCS+=	dcn32_clk_mgr.c \
	dcn32_clk_mgr_smu_msg.c

# display/dc/core
SRCS+=	dc_vm_helper.c

# display/dc/clk_mgr
SRCS+=	rn_clk_mgr.c \
	rn_clk_mgr_vbios_smu.c

# display/dc/dsc
SRCS+=	rc_calc.c \
	rc_calc_dpi.c \
	rc_calc_fpu.c \
	dc_dsc.c

.endif

# display/dc/dml
.if !empty(KCONFIG:MDRM_AMD_DC_FP)
SRCS+=	display_mode_lib.c \
	display_mode_vba.c \
	display_mode_vba_20.c \
	display_mode_vba_21.c \
	display_rq_dlg_helpers.c \
	display_rq_dlg_calc_20.c \
	display_rq_dlg_calc_21.c \
	dml1_display_rq_dlg_calc.c
.endif

# display/dc/dml/dcn10
SRCS+=	dcn10_fpu.c

# display/dc/dml/dcn30
SRCS+=	dcn30_fpu.c

# display/dc/dml/dcn302
SRCS+=	dcn302_fpu.c

# display/dc/dml/dcn303
SRCS+=	dcn303_fpu.c

# display/dc/dml/dcn31
SRCS+=	dcn31_fpu.c

# display/dc/dml/dcn314
SRCS+=	dcn314_fpu.c \
	display_mode_vba_314.c \
	display_rq_dlg_calc_314.c

# display/dc/dml/dcn32
SRCS+=	dcn32_fpu.c \
	display_mode_vba_32.c \
	display_mode_vba_util_32.c \
	display_rq_dlg_calc_32.c

# display/dc/dml/dcn321
SRCS+=	dcn321_fpu.c

# display/dc/dml2
SRCS+=	display_mode_core.c \
	display_mode_util.c \
	dml_display_rq_dlg_calc.c \
	dml2_dc_resource_mgmt.c \
	dml2_mall_phantom.c \
	dml2_policy.c \
	dml2_translation_helper.c \
	dml2_utils.c \
	dml2_wrapper.c

# display/dc/gpio/*
SRCS+=	gpio_base.c \
	gpio_service.c \
	hw_ddc.c \
	hw_factory.c \
	hw_generic.c \
	hw_gpio.c \
	hw_hpd.c \
	hw_translate.c \
	hw_factory_dce110.c \
	hw_factory_dce120.c \
	hw_factory_dce60.c \
	hw_factory_dce80.c \
	hw_factory_dcn10.c \
	hw_factory_dcn20.c \
	hw_factory_dcn21.c \
	hw_factory_dcn315.c \
	hw_factory_dcn32.c \
	hw_translate_dce110.c \
	hw_translate_dce120.c \
	hw_translate_dce60.c \
	hw_translate_dce80.c \
	hw_translate_dcn10.c \
	hw_translate_dcn20.c \
	hw_translate_dcn21.c \
	hw_translate_dcn315.c \
	hw_translate_dcn32.c

# display/dc/hdcp
SRCS+=	hdcp_msg.c \

# display/dc/irq/*
SRCS+=	irq_service.c \
	irq_service_dce110.c \
	irq_service_dce120.c \
	irq_service_dce60.c \
	irq_service_dce80.c \
	irq_service_dcn10.c \
	irq_service_dcn20.c \
	irq_service_dcn21.c \
	irq_service_dcn314.c \
	irq_service_dcn315.c \
	irq_service_dcn32.c

# display/dc/link/*
SRCS+=	link_ddc.c \
	link_detection.c \
	link_dp_capability.c \
	link_dp_cts.c \
	link_dp_dpia.c \
	link_dp_dpia_bw.c \
	link_dp_irq_handler.c \
	link_dp_phy.c \
	link_dp_trace.c \
	link_dp_training.c \
	link_dp_training_128b_132b.c \
	link_dp_training_8b_10b.c \
	link_dp_training_auxless.c \
	link_dp_training_dpia.c \
	link_dp_training_fixed_vs_pe_retimer.c \
	link_dpcd.c \
	link_dpms.c \
	link_edp_panel_control.c \
	link_factory.c \
	link_hpd.c \
	link_hwss_dio.c \
	link_hwss_dio_fixed_vs_pe_retimer.c \
	link_hwss_dpia.c \
	link_hwss_hpo_dp.c \
	link_hwss_hpo_fixed_vs_pe_retimer_dp.c \
	link_resource.c \
	link_validation.c

# display/dc/virtual
SRCS+=	virtual_link_encoder.c \
	virtual_link_hwss.c \
	virtual_stream_encoder.c

# display/modules/*
SRCS+=	color_gamma.c \
	color_table.c \
	freesync.c \
	hdcp.c \
	hdcp_ddc.c \
	hdcp_log.c \
	hdcp_psp.c \
	hdcp1_execution.c \
	hdcp1_transition.c \
	hdcp2_execution.c \
	hdcp2_transition.c \
	info_packet.c \
	power_helpers.c

# display/dc
SRCS+=	dc_helper.c \
	dc_dmub_srv.c \
	dc_edid_parser.c

# display/dmub/src
SRCS+=	dmub_dcn20.c \
	dmub_dcn21.c \
	dmub_dcn314.c \
	dmub_dcn315.c \
	dmub_dcn316.c \
	dmub_dcn32.c \
	dmub_reg.c \
	dmub_srv.c \
	dmub_srv_stat.c

# Debug files, not handled for now
SRCS_DEBUG=	amdgpu_dm_crc.c \
		amdgpu_dm_debugfs.c

.if !empty(KCONFIG:MACPI*)
SRCS+=	amdgpu_acpi.c
.else
.endif

.if !empty(KCONFIG:MDRM_AMD_DC_FP)
SRCS+=	dcn_calc_auto.c \
	dcn_calc_math.c \
	dcn_calcs.c
.else
.endif

# powerplay
SRCS+=	amd_powerplay.c \
	amdgpu_smu.c \
	aldebaran_ppt.c \
	yellow_carp_ppt.c \
	arcturus_ppt.c \
	common_baco.c \
	ci_smumgr.c \
	ci_baco.c \
	cyan_skillfish_ppt.c \
	fiji_smumgr.c \
	fiji_baco.c \
	hardwaremanager.c \
	hwmgr.c \
	iceland_smumgr.c \
	navi10_ppt.c \
	renoir_ppt.c \
	polaris10_smumgr.c \
	polaris_baco.c \
	pp_overdriver.c \
	pp_psm.c \
	ppatomctrl.c \
	ppatomfwctrl.c \
	pppcielanes.c \
	process_pptables_v1_0.c \
	processpptables.c \
	sienna_cichlid_ppt.c \
	smu7_baco.c \
	smu7_clockpowergating.c \
	smu7_hwmgr.c \
	smu7_powertune.c \
	smu7_smumgr.c \
	smu7_thermal.c \
	smu8_hwmgr.c \
	smu8_smumgr.c \
	smu9_baco.c \
	smu9_smumgr.c \
	smu10_hwmgr.c \
	smu10_smumgr.c \
	smu_cmn.c \
	smu_v11_0.c \
	smu_v11_0_i2c.c \
	smu_v12_0.c \
	smu_v13_0.c \
	smu_v13_0_0_ppt.c \
	smu_v13_0_4_ppt.c \
	smu_v13_0_5_ppt.c \
	smu_v13_0_6_ppt.c \
	smu_v13_0_7_ppt.c \
	smu_v14_0.c \
	smu_v14_0_0_ppt.c \
	smu_helper.c \
	smumgr.c \
	tonga_baco.c \
	tonga_smumgr.c \
	vangogh_ppt.c \
	vega10_baco.c \
	vega10_hwmgr.c \
	vega10_powertune.c \
	vega10_processpptables.c \
	vega10_smumgr.c \
	vega10_thermal.c \
	vega12_baco.c \
	vega12_hwmgr.c \
	vega12_processpptables.c \
	vega12_smumgr.c \
	vega12_thermal.c \
	vega20_baco.c \
	vega20_hwmgr.c \
	vega20_powertune.c \
	vega20_processpptables.c \
	vega20_smumgr.c \
	vega20_thermal.c \
	vegam_smumgr.c

.ifdef (CONFIG_DEBUG_FS)
SRCS+=	amdgpu_pmu.c
.endif

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H:H}/include
CFLAGS+= -I${.CURDIR:H:H}/include/drm
CFLAGS+= -I${.CURDIR:H:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${SRCDIR}/acp/include
CFLAGS+= -I${SRCDIR}/amdgpu
CFLAGS+= -I${SRCDIR}/amdkfd
CFLAGS+= -I${SRCDIR}/display
CFLAGS+= -I${SRCDIR}/display/amdgpu_dm
CFLAGS+= -I${SRCDIR}/display/dc
CFLAGS+= -I${SRCDIR}/display/dc/clk_mgr/
CFLAGS+= -I${SRCDIR}/display/dc/dsc
CFLAGS+= -I${SRCDIR}/display/dc/hwss
CFLAGS+= -I${SRCDIR}/display/dc/inc
CFLAGS+= -I${SRCDIR}/display/dc/inc/hw
CFLAGS+= -I${SRCDIR}/display/dc/optc
CFLAGS+= -I${SRCDIR}/display/dc/resource
CFLAGS+= -I${SRCDIR}/display/dmub/inc
CFLAGS+= -I${SRCDIR}/display/include
CFLAGS+= -I${SRCDIR}/display/modules/hdcp
CFLAGS+= -I${SRCDIR}/display/modules/inc
CFLAGS+= -I${SRCDIR}/include
CFLAGS+= -I${SRCDIR}/include/asic_reg
CFLAGS+= -I${SRCDIR}/pm
CFLAGS+= -I${SRCDIR}/pm/inc
CFLAGS+= -I${SRCDIR}/pm/legacy-dpm
CFLAGS+= -I${SRCDIR}/pm/powerplay
CFLAGS+= -I${SRCDIR}/pm/powerplay/inc
CFLAGS+= -I${SRCDIR}/pm/powerplay/hwmgr
CFLAGS+= -I${SRCDIR}/pm/powerplay/smumgr
CFLAGS+= -I${SRCDIR}/pm/swsmu
CFLAGS+= -I${SRCDIR}/pm/swsmu/inc
CFLAGS+= -I${SRCDIR}/pm/swsmu/inc/pmfw_if
CFLAGS+= -I${SRCDIR}/pm/swsmu/smu11
CFLAGS+= -I${SRCDIR}/pm/swsmu/smu12
CFLAGS+= -I${SRCDIR}/pm/swsmu/smu13
CFLAGS+= -I${SRCDIR}/pm/swsmu/smu14
CFLAGS+= -I${DRM}/scheduler

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=amdgpu_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS+=	device_if.h vnode_if.h bus_if.h pci_if.h pci_iov_if.h device_if.h iicbus_if.h opt_drm.h \
        opt_vm.h opt_syscons.h opt_acpi.h

.include <bsd.kmod.mk>

CFLAGS.gcc+= -Wno-redundant-decls -Wno-unused-but-set-variable


.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64"
FPUFLAGS= -msse
.endif

CFLAGS.dc.c = -Wno-typedef-redefinition

CFLAGS.dcn_calcs.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn_calc_auto.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn_calc_math.c= ${FPUFLAGS} -mstack-alignment=4 -Wno-tautological-compare
CFLAGS.dcn20_fpu.c= ${FPUFLAGS} -mstack-alignment=4 -Wno-unused-variable
CFLAGS.dcn20_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn21_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn30_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn30_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn301_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn301_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn303_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn31_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn31_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn314_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn32_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn32_resource.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn32_resource_helpers.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn321_fpu.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dcn35_fpu.c= ${FPUFLAGS} -mstack-alignment=4

CFLAGS.display_mode_core.c = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.display_mode_util.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml_display_rq_dlg_calc.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_dc_resource_mgmt.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_mall_phantom.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_policy.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_translation_helper.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_utils.c  = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition
CFLAGS.dml2_wrapper.c = ${FPUFLAGS} -mstack-alignment=4 -Wno-typedef-redefinition

CFLAGS.display_mode_lib.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_20.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_21.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_30.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_31.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_314.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_32.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_util_32.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_pipe_clocks.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.dml1_display_rq_dlg_calc.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_helpers.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_20.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_21.c= ${FPUFLAGS} -mstack-alignment=4 -Wno-missing-prototypes
CFLAGS.display_rq_dlg_calc_314.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_32.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.soc_bounding_box.c= ${FPUFLAGS} -mstack-alignment=4

CFLAGS.display_rq_dlg_calc_20v2.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_mode_vba_20v2.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_30.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_31.c= ${FPUFLAGS} -mstack-alignment=4 -Wno-unused-but-set-variable

CFLAGS.rc_calc.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.rc_calc_dpi.c= ${FPUFLAGS} -mstack-alignment=4
CFLAGS.rc_calc_fpu.c= ${FPUFLAGS} -mstack-alignment=4

.if ${MACHINE_CPUARCH} != "powerpc"
CWARNFLAGS+=	-Wno-expansion-to-defined
.endif
CWARNFLAGS+=	-Wno-pointer-arith -Wno-format -Wno-cast-qual
CWARNFLAGS+=	-Wno-pointer-sign ${CWARNFLAGS.${.IMPSRC:T}}

# amdgpu/
CWARNFLAGS.amdgpu_acpi.c=			-Wno-missing-prototypes -Wno-incompatible-pointer-types
CWARNFLAGS.amdgpu_atpx_handler.c=		-Wno-missing-prototypes
CWARNFLAGS.amdgpu_bo_list.c=			-Wno-unused-but-set-variable -Wno-tautological-constant-out-of-range-compare
CWARNFLAGS.amdgpu_csa.c=			-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_device.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_discovery.c=			-Wno-unused-function
CWARNFLAGS.amdgpu_dm_debugfs.c=			-Wno-pointer-bool-conversion -Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_fence.c=			-Wno-unused-variable
CWARNFLAGS.amdgpu_fdinfo.c=			-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_gfx.c=			-Wno-unused-but-set-variable -Wno-tautological-overlap-compare
CWARNFLAGS.amdgpu_ioc32.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_pm.c=				-Wno-unused-const-variable
CWARNFLAGS.amdgpu_fw_attestation.c=		-Wno-missing-prototypes
CWARNFLAGS.amdgpu_securedisplay.c=		-Wno-unused-const-variable
CWARNFLAGS.amdgpu_kms.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_mes.c=			-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_psp.c=			-Wno-missing-prototypes -Wno-unused-variable
CWARNFLAGS.amdgpu_ras.c=			-Wno-missing-prototypes -Wno-unused-function -Wno-undefined-inline
CWARNFLAGS.amdgpu_ring_mux.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_sched.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ttm.c=			-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_ucode.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_virt.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_vram_mgr.c=			-Wno-unused-variable
CWARNFLAGS.amdgpu_xgmi.c=			-Wno-unused-variable
CWARNFLAGS.aldebaran_ppt.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.aldebaran.c=				-Wno-unused-variable
CWARNFLAGS.arcturus_ppt.c=			-Wno-uninitialized
CWARNFLAGS.aqua_vanjaram_reg_init.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable -Wno-unused-function
CWARNFLAGS.dimgrey_cavefish_reg_init.c=		-Wno-missing-prototypes
CWARNFLAGS.yellow_carp_ppt.c=			-Wno-sometimes-uninitialized
CWARNFLAGS.atombios_encoders.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dce_v6_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.dce_v8_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.gmc_v9_0.c=				-Wno-initializer-overrides
CWARNFLAGS.gfx_v7_0.c=				-Wno-unused-const-variable
CWARNFLAGS.gfx_v9_0.c=				-Wno-missing-prototypes
CWARNFLAGS.gfx_v9_4_2.c=			-Wno-missing-prototypes
CWARNFLAGS.gfx_v9_4_3.c=			-Wno-unused-but-set-variable -Wno-sometimes-uninitialized -Wno-unused-function -Wno-unused-const-variable
CWARNFLAGS.gfx_v10_0.c=				-Wno-unused-variable
CWARNFLAGS.gfx_v11_0.c=				-Wno-sometimes-uninitialized
CWARNFLAGS.gfxhub_v1_2.c=			-Wno-sometimes-uninitialized
CWARNFLAGS.jpeg_v2_5.c=				-Wno-unused-but-set-variable
CWARNFLAGS.jpeg_v3_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.jpeg_v4_0_3.c=			-Wno-uninitialized -Wno-unused-function
CWARNFLAGS.imu_v11_0_3.c=			-Wno-missing-prototypes
CWARNFLAGS.kv_dpm.c=				-Wno-unused-but-set-variable
CWARNFLAGS.mmhub_v1_7.c=			-Wno-missing-prototypes
CWARNFLAGS.mmhub_v1_8.c=			-Wno-unused-function -Wno-unused-const-variable
CWARNFLAGS.nv.c=				-Wno-unused-const-variable
CWARNFLAGS.psp_v10_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.psp_v11_0.c=				-Wno-missing-prototypes
CWARNFLAGS.sdma_v4_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.sdma_v4_4_2.c=			-Wno-unused-but-set-variable -Wno-unused-function
CWARNFLAGS.si_dma.c=				-Wno-unused-but-set-variable
CWARNFLAGS.si_dpm.c=				-Wno-gnu-variable-sized-type-not-at-end
CWARNFLAGS.si_smc.c=				-Wno-gnu-variable-sized-type-not-at-end
CWARNFLAGS.si.c=				-Wno-gnu-variable-sized-type-not-at-end
CWARNFLAGS.sienna_cichlid_ppt.c=		-Wno-missing-prototypes -Wno-uninitialized
CWARNFLAGS.smuio_v9_0.c=			-Wno-missing-prototypes
CWARNFLAGS.smuio_v11_0.c=			-Wno-missing-prototypes
CWARNFLAGS.smuio_v13_0.c=			-Wno-missing-prototypes
CWARNFLAGS.umc_v6_7.c=				-Wno-unused-function
CWARNFLAGS.umc_v8_10.c=				-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.vangogh_ppt.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.vcn_sw_ring.c=			-Wno-missing-prototypes
CWARNFLAGS.vcn_v3_0.c=				-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.vcn_v4_0.c=				-Wno-unused-but-set-variable
CWARNFLAGS.vcn_v4_0_3.c=			-Wno-unused-function

# display/amdgpu_dm
CWARNFLAGS.amdgpu_dm_helpers.c=		-Wno-int-in-bool-context -Wno-uninitialized
CWARNFLAGS.amdgpu_dm_mst_types.c=	-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.amdgpu_dm_plane.c=		-Wno-missing-prototypes
CWARNFLAGS.amdgpu_dm_pp_smu.c=		-Wno-missing-prototypes -Wno-incompatible-function-pointer-types
CWARNFLAGS.amdgpu_dm_psr.c=		-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_dm_services.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_dm.c=			-Wno-missing-prototypes -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-variable

# display/dc
CWARNFLAGS.command_table2.c=		-Wno-unused-but-set-variable
CWARNFLAGS.command_table_helper_dce60.c=		-Wno-missing-prototypes
CWARNFLAGS.conversion.c=		-Wno-missing-prototypes
CWARNFLAGS.fixpt31_32.c=		-Wno-incompatible-pointer-types
CWARNFLAGS.vector.c=			-Wno-missing-prototypes
CWARNFLAGS.bios_parser.c=		-Wno-missing-prototypes
CWARNFLAGS.bios_parser2.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dc.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable -Wno-unused-variable
CWARNFLAGS.dc_hw_sequencer.c=		-Wno-missing-prototypes
CWARNFLAGS.dc_link.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dc_link_ddc.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dc_link_enc_cfg.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dc_link_dp.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dc_resource.c=		-Wno-missing-prototypes
CWARNFLAGS.dc_stat.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dce_aux.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dce_audio.c=			-Wno-missing-prototypes
CWARNFLAGS.dce_calcs.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dce_clock_source.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dce_dmcu.c=			-Wno-missing-prototypes
CWARNFLAGS.dce_i2c_hw.c=		-Wno-missing-prototypes
CWARNFLAGS.dce_i2c_sw.c=		-Wno-missing-prototypes
CWARNFLAGS.dce_link_encoder.c=		-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.dce_transform.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dce_stream_encoder.c=	-Wno-unused-but-set-variable
CWARNFLAGS.dce_opp.c=			-Wno-missing-prototypes
CWARNFLAGS.dce_panel_cntl.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dce100_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce110_compressor.c=		-Wno-missing-prototypes
CWARNFLAGS.dce110_hwseq.c=		-Wno-missing-prototypes
CWARNFLAGS.dce110_mem_input_v.c=	-Wno-missing-prototypes
CWARNFLAGS.dce110_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce110_timing_generator.c=	-Wno-unused-but-set-variable
CWARNFLAGS.dce112_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce120_hwseq.c=		-Wno-unused-const-variable
CWARNFLAGS.dce120_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce120_timing_generator.c=	-Wno-missing-prototypes
CWARNFLAGS.dce60_timing_generator.c=	-Wno-missing-prototypes
CWARNFLAGS.dce60_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce80_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.rn_clk_mgr_vbios_smu.c=	-Wno-missing-prototypes
CWARNFLAGS.rn_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.rv1_clk_mgr_vbios_smu.c=	-Wno-missing-prototypes
CWARNFLAGS.rv1_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn10_dpp_dscl.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_hwseq.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable -Wno-unused-variable
CWARNFLAGS.dcn10_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_link_encoder.c=	-Wno-unused-but-set-variable
CWARNFLAGS.dcn10_stream_encoder.c=	-Wno-unused-but-set-variable
CWARNFLAGS.dcn10_opp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn10_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn20_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn20_dwb_scl.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_hwseq.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn20_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_mpc.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn20_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-variable
CWARNFLAGS.dcn20_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn201_clk_mgr.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn201_dccg.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn201_dpp.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn201_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn201_hwseq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn201_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn201_link_encoder.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn201_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn21_dccg.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn21_hubp.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn21_hubbub.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn21_hwseq.c=		-Wno-unused-variable
CWARNFLAGS.dcn21_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn21_link_encoder.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn21_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-variable
CWARNFLAGS.dcn30_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn30_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn30_mpc.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn30_mmhubbub.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn30_dio_stream_encoder.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn30_hwseq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn30_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn30_resource.c=		-Wno-initializer-overrides -Wno-missing-prototypes -Wno-unused-const-variable
CWARNFLAGS.dcn30_afmt.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn30_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn301_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn301_panel_cntl.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn301_resource.c=		-Wno-initializer-overrides -Wno-missing-prototypes -Wno-unused-const-variable
CWARNFLAGS.dcn301_smu.c=		-Wno-missing-prototypes
CWARNFLAGS.vg_clk_mgr.c=		-Wno-missing-prototypes -Wno-incompatible-pointer-types
CWARNFLAGS.dcn302_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn302_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn303_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn303_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn31_apg.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dcn31_clk_mgr.c=		-Wno-missing-prototypes -Wno-incompatible-pointer-types -Wno-unused-but-set-variable
CWARNFLAGS.dcn31_dccg.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn31_dio_link_encoder.c=	-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn31_hpo_dp_stream_encoder.c=	-Wno-unused-but-set-variable
CWARNFLAGS.dcn31_hubbub.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn31_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn31_hwseq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn31_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn31_panel_cntl.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn31_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn31_smu.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn314_clk_mgr.c=		-Wno-incompatible-pointer-types
CWARNFLAGS.dcn314_dccg.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn314_dio_stream_encoder.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn314_optc.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dcn314_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn315_clk_mgr.c=		-Wno-missing-prototypes -Wno-incompatible-pointer-types -Wno-unused-but-set-variable
CWARNFLAGS.dcn315_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn315_smu.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn316_clk_mgr.c=		-Wno-missing-prototypes -Wno-incompatible-pointer-types -Wno-unused-but-set-variable
CWARNFLAGS.dcn316_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn316_smu.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_dccg.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_dio_link_encoder.c=	-Wno-missing-prototypes -Wno-unused-variable
CWARNFLAGS.dcn32_dio_stream_encoder.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn32_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn32_fpu.c=			-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.dcn32_hwseq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn32_hubbub.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_hwseq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn32_init.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_mmhubbub.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_mpc.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn32_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn32_resource.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable -Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn32_resource_helpers.c=	-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn321_fpu.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dcn321_resource.c=		-Wno-initializer-overrides -Wno-unused-const-variable
CWARNFLAGS.dcn35_clk_mgr.c=		-Wno-incompatible-pointer-types
CWARNFLAGS.dcn35_resource.c=		-Wno-initializer-overrides
CWARNFLAGS.fixpt31_32.c=		-Wno-unused-but-set-variable -Wno-incompatible-pointer-types -Wno-unused-const-variable

CWARNFLAGS.hw_ddc.c=			-Wno-unused-but-set-variable
CWARNFLAGS.hw_factory.c=		-Wno-missing-prototypes
CWARNFLAGS.hw_factory_diag.c=		-Wno-missing-prototypes
CWARNFLAGS.hw_gpio.c=			-Wno-unused-function
CWARNFLAGS.hw_translate_diag.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn10.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn20.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn201.c=	-Wno-missing-prototypes -Wno-unused-const-variable
CWARNFLAGS.irq_service_dcn21.c=		-Wno-missing-prototypes -Wno-unused-const-variable -Wno-initializer-overrides
CWARNFLAGS.irq_service_dcn30.c=		-Wno-missing-prototypes -Wno-unused-const-variable
CWARNFLAGS.irq_service_dcn31.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn315.c=	-Wno-missing-prototypes
CWARNFLAGS.link_detection.c=		-Wno-unused-but-set-variable
CWARNFLAGS.link_dp_capability.c=	-Wno-unused-but-set-variable
CWARNFLAGS.link_dp_dpia_bw.c=		-Wno-unused-but-set-variable
CWARNFLAGS.link_dp_trace.c=		-Wno-missing-prototypes
CWARNFLAGS.link_dp_training.c=		-Wno-unused-but-set-variable
CWARNFLAGS.link_validation.c=		-Wno-missing-prototypes
CWARNFLAGS.power_helpers.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dc_helper.c=			-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.dcn_calcs.c+=		-Wno-missing-prototypes
CWARNFLAGS.dc_dmub_srv.c=		-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.dmub_psr.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dmub_abm.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dmub_abm_lcd.c=		-Wno-missing-prototypes
CWARNFLAGS.dmub_hw_lock_mgr.c=		-Wno-unused-but-set-variable
CWARNFLAGS.virtual_link_hwss.c=		-Wno-missing-prototypes

# display/dmub/*
CWARNFLAGS.dmub_dcn20.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn21.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn30.c=		-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.dmub_dcn301.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn302.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn303.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn31.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn315.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_dcn316.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_outbox.c=		-Wno-unused-but-set-variable
CWARNFLAGS.dmub_reg.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dmub_srv.c=			-Wno-unused-but-set-variable
CWARNFLAGS.dmub_srv_stat.c=		-Wno-unused-but-set-variable

# display/dc/dml/
CWARNFLAGS.display_mode_vba_30.c=	-Wno-unused-but-set-variable
CWARNFLAGS.display_mode_vba_31.c=	-Wno-unused-but-set-variable
CWARNFLAGS.display_mode_vba_314.c=	-Wno-unused-but-set-variable
CWARNFLAGS.display_mode_vba_32.c=	-Wno-enum-conversion -Wno-unused-const-variable
CWARNFLAGS.display_mode_vba_util_32.c=	-Wno-enum-conversion -Wno-missing-prototypes -Wno-unused-local-typedef
CWARNFLAGS.display_rq_dlg_calc_314.c=	-Wno-unused-but-set-variable

# powerplay/*
CWARNFLAGS.amdgpu_smu.c=		-Wno-unused-function -Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.ci_smumgr.c=			-Wno-unused-but-set-variable
CWARNFLAGS.hardwaremanager.c=		-Wno-missing-prototypes
CWARNFLAGS.pp_psm.c=			-Wno-unused-but-set-variable
CWARNFLAGS.iceland_smumgr.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.navi10_ppt.c=		-Wno-unused-function -Wno-unused-const-variable -Wno-unused-label -Wno-uninitialized
CWARNFLAGS.fiji_smumgr.c=		-Wno-unused-but-set-variable
CWARNFLAGS.ppatomctrl.c=		-Wno-unused-but-set-variable
CWARNFLAGS.polaris10_smumgr.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.smu7_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu8_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu9_smumgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu_cmn.c=			-Wno-implicit-function-declaration -Wno-sometimes-uninitialized
CWARNFLAGS.smu_v13_0_0_ppt.c=		-Wno-unused-variable
CWARNFLAGS.smu_v13_0_6_ppt.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.smu_v13_0_7_ppt.c=		-Wno-unused-variable
CWARNFLAGS.tonga_smumgr.c=		-Wno-unused-but-set-variable
CWARNFLAGS.vega10_hwmgr.c=		-Wno-missing-prototypes -Wno-unused-but-set-variable
CWARNFLAGS.vega10_processpptables.c=	-Wno-missing-prototypes
CWARNFLAGS.vega12_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vega20_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vegam_smumgr.c=		-Wno-missing-prototypes

# modules/*
CWARNFLAGS.freesync.c=			-Wno-unused-but-set-variable

EXPORT_SYMS=	YES

# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/amd
DRM= ${.CURDIR:H:H}/drivers/gpu/drm

KMOD=	amdgpu

.PATH:	${SRCDIR}/acp \
	${SRCDIR}/amdgpu \
	${SRCDIR}/amdkfd \
	${SRCDIR}/display \
	${SRCDIR}/display/amdgpu_dm \
	${SRCDIR}/display/dc \
	${SRCDIR}/display/dc/basics \
	${SRCDIR}/display/dc/bios \
	${SRCDIR}/display/dc/bios/dce110 \
	${SRCDIR}/display/dc/bios/dce112 \
	${SRCDIR}/display/dc/bios/dce80 \
	${SRCDIR}/display/dc/calcs \
	${SRCDIR}/display/dc/core \
	${SRCDIR}/display/dc/dce \
	${SRCDIR}/display/dc/dce100 \
	${SRCDIR}/display/dc/dce110 \
	${SRCDIR}/display/dc/dce112 \
	${SRCDIR}/display/dc/dce120 \
	${SRCDIR}/display/dc/dce80 \
	${SRCDIR}/display/dc/dcn10 \
	${SRCDIR}/display/dc/dml \
	${SRCDIR}/display/dc/gpio \
	${SRCDIR}/display/dc/gpio/dce110 \
	${SRCDIR}/display/dc/gpio/dce120 \
	${SRCDIR}/display/dc/gpio/dce80 \
	${SRCDIR}/display/dc/gpio/dcn10 \
	${SRCDIR}/display/dc/gpio/diagnostics \
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
	${SRCDIR}/display/dc/irq/dce80 \
	${SRCDIR}/display/dc/irq/dcn10 \
	${SRCDIR}/display/dc/virtual \
	${SRCDIR}/display/modules \
	${SRCDIR}/display/modules/freesync \
	${SRCDIR}/display/modules/inc \
	${SRCDIR}/lib \
	${SRCDIR}/powerplay \
	${SRCDIR}/powerplay/hwmgr \
	${SRCDIR}/powerplay/inc \
	${SRCDIR}/powerplay/smumgr \
	${DRM}/scheduler

# acp
SRCS=	acp_hw.c

# amdgpu
# amdgpu_acp.c \ # not yet - need i2s
# amdgpu_mn.c \  # disabled
SRCS+=	amdgpu_afmt.c \
	amdgpu_amdkfd.c \
	amdgpu_amdkfd_gfx_v7.c \
	amdgpu_amdkfd_gfx_v8.c \
	amdgpu_atombios.c \
	amdgpu_atomfirmware.c \
	amdgpu_benchmark.c \
	amdgpu_bios.c \
	amdgpu_bo_list.c \
	amdgpu_cgs.c \
	amdgpu_connectors.c \
	amdgpu_cs.c \
	amdgpu_ctx.c \
	amdgpu_debugfs.c \
	amdgpu_device.c \
	amdgpu_display.c \
	amdgpu_dpm.c \
	amdgpu_drv.c \
	amdgpu_encoders.c \
	amdgpu_fb.c \
	amdgpu_fence.c \
	amdgpu_freebsd.c \
	amdgpu_gart.c \
	amdgpu_gem.c \
	amdgpu_gfx.c \
	amdgpu_gtt_mgr.c \
	amdgpu_i2c.c \
	amdgpu_ib.c \
	amdgpu_ids.c \
	amdgpu_ih.c \
	amdgpu_ioc32.c \
	amdgpu_irq.c \
	amdgpu_job.c \
	amdgpu_kms.c \
	amdgpu_object.c \
	amdgpu_pll.c \
	amdgpu_pm.c \
	amdgpu_powerplay.c \
	amdgpu_prime.c \
	amdgpu_psp.c \
	amdgpu_queue_mgr.c \
	amdgpu_ring.c \
	amdgpu_sa.c \
	amdgpu_sched.c \
	amdgpu_sync.c \
	amdgpu_test.c \
	amdgpu_trace_points.c \
	amdgpu_ttm.c \
	amdgpu_ucode.c \
	amdgpu_uvd.c \
	amdgpu_vce.c \
	amdgpu_vcn.c \
	amdgpu_vf_error.c \
	amdgpu_virt.c \
	amdgpu_vm.c \
	amdgpu_vram_mgr.c \
	atom.c \
	atombios_crtc.c \
	atombios_dp.c \
	atombios_encoders.c \
	atombios_i2c.c \
	ci_dpm.c \
	ci_smc.c \
	cik.c \
	cik_ih.c \
	cik_sdma.c \
	cz_ih.c \
	dce_v6_0.c \
	dce_v8_0.c \
	dce_v10_0.c \
	dce_v11_0.c \
	dce_virtual.c \
	gfx_v6_0.c \
	gfx_v7_0.c \
	gfx_v8_0.c \
	gfx_v9_0.c \
	gfxhub_v1_0.c \
	gmc_v6_0.c \
	gmc_v7_0.c \
	gmc_v8_0.c \
	gmc_v9_0.c \
	iceland_ih.c \
	kv_dpm.c \
	kv_smc.c \
	mmhub_v1_0.c \
	mxgpu_ai.c \
	mxgpu_vi.c \
	nbio_v6_1.c \
	nbio_v7_0.c \
	psp_v3_1.c \
	psp_v10_0.c \
	sdma_v2_4.c \
	sdma_v3_0.c \
	sdma_v4_0.c \
	si.c \
	si_dma.c \
	si_dpm.c \
	si_ih.c \
	si_smc.c \
	soc15.c \
	tonga_ih.c \
	uvd_v4_2.c \
	uvd_v5_0.c \
	uvd_v6_0.c \
	uvd_v7_0.c \
	vce_v2_0.c \
	vce_v3_0.c \
	vce_v4_0.c \
	vcn_v1_0.c \
	vega10_ih.c \
	vi.c
.if ${MACHINE_CPUARCH} == "amd64"
SRCS+= amdgpu_acpi.c \
	amdgpu_atpx_handler.c \
	dcn10_cm_common.c \
	dcn10_dpp.c \
	dcn10_dpp_cm.c \
	dcn10_dpp_dscl.c \
	dcn10_hubbub.c \
	dcn10_hubp.c \
	dcn10_hw_sequencer.c \
	dcn10_ipp.c \
	dcn10_mpc.c \
	dcn10_opp.c \
	dcn10_optc.c \
	dcn10_resource.c \
	dcn_calc_auto.c \
	dcn_calcs.c
.endif

# display/*
SRCS+=	amdgpu_dm.c \
	amdgpu_dm_helpers.c \
	amdgpu_dm_irq.c \
	amdgpu_dm_mst_types.c \
	amdgpu_dm_services.c \
	aux_engine.c \
	aux_engine_dce110.c \
	bios_parser.c \
	bios_parser2.c \
	bios_parser_common.c \
	bios_parser_helper.c \
	bios_parser_interface.c \
	bw_fixed.c \
	command_table.c \
	command_table2.c \
	command_table_helper.c \
	command_table_helper2.c \
	command_table_helper2_dce112.c \
	command_table_helper_dce110.c \
	command_table_helper_dce112.c \
	command_table_helper_dce80.c \
	conversion.c \
	custom_float.c \
	dc.c \
	dc_debug.c \
	dc_helper.c \
	dc_hw_sequencer.c \
	dc_link.c \
	dc_link_ddc.c \
	dc_link_dp.c \
	dc_link_hwss.c \
	dc_resource.c \
	dc_sink.c \
	dc_stream.c \
	dc_surface.c \
	dce100_hw_sequencer.c \
	dce100_resource.c \
	dce110_compressor.c \
	dce110_hw_sequencer.c \
	dce110_mem_input_v.c \
	dce110_opp_csc_v.c \
	dce110_opp_regamma_v.c \
	dce110_opp_v.c \
	dce110_resource.c \
	dce110_timing_generator.c \
	dce110_timing_generator_v.c \
	dce110_transform_v.c \
	dce112_compressor.c \
	dce112_hw_sequencer.c \
	dce112_resource.c \
	dce120_hw_sequencer.c \
	dce120_resource.c \
	dce120_timing_generator.c \
	dce80_compressor.c \
	dce80_hw_sequencer.c \
	dce80_resource.c \
	dce80_timing_generator.c \
	dce_abm.c \
	dce_audio.c \
	dce_calcs.c \
	dce_clock_source.c \
	dce_clocks.c \
	dce_dmcu.c \
	dce_hwseq.c \
	dce_ipp.c \
	dce_link_encoder.c \
	dce_mem_input.c \
	dce_opp.c \
	dce_scl_filters.c \
	dce_stream_encoder.c \
	dce_transform.c \
	dcn_calc_math.c \
	display_mode_lib.c \
	display_mode_vba.c \
	display_rq_dlg_calc.c \
	display_rq_dlg_helpers.c \
	dml1_display_rq_dlg_calc.c \
	dml_common_defs.c \
	engine_base.c \
	fixpt31_32.c \
	fixpt32_32.c \
	freesync.c \
	gpio_base.c \
	gpio_service.c \
	hw_ddc.c \
	hw_factory.c \
	hw_factory_dce110.c \
	hw_factory_dce120.c \
	hw_factory_dce80.c \
	hw_factory_dcn10.c \
	hw_factory_diag.c \
	hw_gpio.c \
	hw_hpd.c \
	hw_translate.c \
	hw_translate_dce110.c \
	hw_translate_dce120.c \
	hw_translate_dce80.c \
	hw_translate_dcn10.c \
	hw_translate_diag.c \
	i2c_engine.c \
	i2c_generic_hw_engine.c \
	i2c_hw_engine.c \
	i2c_hw_engine_dce110.c \
	i2c_hw_engine_dce80.c \
	i2c_sw_engine.c \
	i2c_sw_engine_dce110.c \
	i2c_sw_engine_dce80.c \
	i2caux.c \
	i2caux_dce100.c \
	i2caux_dce110.c \
	i2caux_dce112.c \
	i2caux_dce120.c \
	i2caux_dce80.c \
	i2caux_dcn10.c \
	i2caux_diag.c \
	irq_service.c \
	irq_service_dce110.c \
	irq_service_dce120.c \
	irq_service_dce80.c \
	irq_service_dcn10.c \
	log_helpers.c \
	logger.c \
	soc_bounding_box.c \
	vector.c \
	virtual_link_encoder.c \
	virtual_stream_encoder.c

# lib
SRCS+=	chash.c

# powerplay
SRCS+=	amd_powerplay.c \
	ci_smumgr.c \
	cz_clockpowergating.c \
	cz_hwmgr.c \
	cz_smumgr.c \
	fiji_smumgr.c \
	hardwaremanager.c \
	hwmgr.c \
	iceland_smumgr.c \
	polaris10_smumgr.c \
	pp_acpi.c \
	pp_overdriver.c \
	pp_psm.c \
	ppatomctrl.c \
	ppatomfwctrl.c \
	pppcielanes.c \
	process_pptables_v1_0.c \
	processpptables.c \
	rv_hwmgr.c \
	rv_smumgr.c \
	smu7_clockpowergating.c \
	smu7_hwmgr.c \
	smu7_powertune.c \
	smu7_smumgr.c \
	smu7_thermal.c \
	smumgr.c \
	tonga_smumgr.c \
	vega10_hwmgr.c \
	vega10_powertune.c \
	vega10_processpptables.c \
	vega10_reg_init.c \
	vega10_smumgr.c \
	vega10_thermal.c


CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H:H}/include
CFLAGS+= -I${.CURDIR:H:H}/include/drm
CFLAGS+= -I${.CURDIR:H:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${SRCDIR}/acp/include
CFLAGS+= -I${SRCDIR}/amdgpu
CFLAGS+= -I${SRCDIR}/display
CFLAGS+= -I${SRCDIR}/display/amdgpu_dm
CFLAGS+= -I${SRCDIR}/display/dc
CFLAGS+= -I${SRCDIR}/display/dc/inc
CFLAGS+= -I${SRCDIR}/display/dc/inc/hw
CFLAGS+= -I${SRCDIR}/display/include
CFLAGS+= -I${SRCDIR}/display/modules/inc
CFLAGS+= -I${SRCDIR}/include
CFLAGS+= -I${SRCDIR}/include/asic_reg
CFLAGS+= -I${SRCDIR}/powerplay/hwmgr
CFLAGS+= -I${SRCDIR}/powerplay/inc
CFLAGS+= -I${SRCDIR}/powerplay/smumgr
CFLAGS+= -I${DRM}/scheduler

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${SRCDIR:H}/drm_os_config.h

SRCS+=	device_if.h vnode_if.h bus_if.h pci_if.h device_if.h iicbus_if.h opt_drm.h \
        opt_vm.h opt_compat.h opt_syscons.h opt_acpi.h

.include <bsd.kmod.mk>

CFLAGS.gcc+= -Wno-redundant-decls -Wno-cast-qual -Wno-unused-but-set-variable
.if ${MACHINE_CPUARCH} == "powerpc"
CFLAGS.dcn_calcs.c= -maltivec -mhard-float -mfull-toc
CFLAGS.dcn_calc_auto.c= -maltivec -mhard-float -mfull-toc
CFLAGS.dcn_calc_math.c= -maltivec -mhard-float -mfull-toc

CFLAGS.display_mode_vba.c= -maltivec -mhard-float -mfull-toc
CFLAGS.display_mode_lib.c= -maltivec -mhard-float -mfull-toc
CFLAGS.display_pipe_clocks.c= -maltivec -mhard-float -mfull-toc
CFLAGS.display_rq_dlg_calc.c= -maltivec -mhard-float -mfull-toc
CFLAGS.dml1_display_rq_dlg_calc.c= -maltivec -mhard-float -mfull-toc
CFLAGS.display_rq_dlg_helpers.c= -maltivec -mhard-float -mfull-toc
CFLAGS.soc_bounding_box.c= -maltivec -mhard-float -mfull-toc
CFLAGS.dml_common_defs.c= -maltivec -mhard-float -mfull-toc
.else
CFLAGS.dcn_calcs.c= -msse -mstack-alignment=4
CFLAGS.dcn_calc_auto.c= -msse -mstack-alignment=4
CFLAGS.dcn_calc_math.c= -msse -mstack-alignment=4 -Wno-tautological-compare

CFLAGS.display_mode_vba.c= -msse -mstack-alignment=4
CFLAGS.display_mode_lib.c= -msse -mstack-alignment=4
CFLAGS.display_pipe_clocks.c= -msse -mstack-alignment=4
CFLAGS.display_rq_dlg_calc.c= -msse -mstack-alignment=4
CFLAGS.dml1_display_rq_dlg_calc.c= -msse -mstack-alignment=4
CFLAGS.display_rq_dlg_helpers.c= -msse -mstack-alignment=4
CFLAGS.soc_bounding_box.c= -msse -mstack-alignment=4
CFLAGS.dml_common_defs.c= -msse -mstack-alignment=4
.endif
CWARNFLAGS+=	-Wno-pointer-arith -Wno-format
CWARNFLAGS+=	-Wno-pointer-sign ${CWARNFLAGS.${.IMPSRC:T}}
CWARNFLAGS+=	-Wno-expansion-to-defined

CWARNFLAGS.amdgpu_acpi.c=	-Wno-int-conversion -Wno-missing-prototypes -Wno-unused-variable
CWARNFLAGS.amdgpu_amdkfd.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_amdkfd_gfx_v7.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_amdkfd_gfx_v8.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_atomfirmware.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_atpx_handler.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_bo_list.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_cgs.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.amdgpu_connectors.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_cs.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_debugfs.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_device.c=	 -Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.amdgpu_dm.c=		-Wno-cast-qual -Wno-missing-prototypes -Wno-pointer-bool-conversion
CWARNFLAGS.amdgpu_dm_helpers.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_display.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_gfx.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_gtt_mgr.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_job.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ih.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_mn.c=		-Wno-unused-variable
CWARNFLAGS.amdgpu_pll.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_pm.c=		-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.amdgpu_ring.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_sched.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ttm.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ucode.c=	-Wno-incompatible-pointer-types-discards-qualifiers -Wno-cast-qual
CWARNFLAGS.amdgpu_vm.c=		-Wno-missing-prototypes
CWARNFLAGS.amd_powerplay.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.atombios_crtc.c=	-Wno-missing-prototypes
CWARNFLAGS.atombios_i2c.c=	-Wno-missing-prototypes
CWARNFLAGS.bios_parser.c=	-Wno-initializer-overrides
CWARNFLAGS.ci_dpm.c=	-Wno-unused-const-variable
CWARNFLAGS.ci_smc_pp.c=	-Wno-missing-prototypes
CWARNFLAGS.ci_smumgr.c=	-Wno-missing-prototypes
CWARNFLAGS.cik.c=	-Wno-unused-const-variable
CWARNFLAGS.conversion.c=-Wno-missing-prototypes
CWARNFLAGS.cz_smc.c=	-Wno-missing-prototypes
CWARNFLAGS.cz_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.cz_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.dc.c=	-Wno-missing-prototypes
CWARNFLAGS.dc_helper.c=	-Wno-missing-prototypes
CWARNFLAGS.dc_hw_sequencer.c=	-Wno-missing-prototypes
CWARNFLAGS.dc_link.c=	-Wno-enum-conversion 
CWARNFLAGS.dc_link_ddc.c=	-Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.dc_link_dp.c=	-Wno-cast-qual -Wno-enum-conversion
CWARNFLAGS.dce_virtual.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v6_0.c=	 -Wno-unused-const-variable -Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.dce_v8_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v10_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v11_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce100_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dce110_compressor.c=	-Wno-missing-prototypes -Wno-unused-variable
CWARNFLAGS.dce110_hw_sequencer.c=	-Wno-missing-prototypes
CWARNFLAGS.dce110_mem_input_v.c=	-Wno-missing-prototypes
CWARNFLAGS.dce110_opp_csc_v.c=	-Wno-enum-conversion
CWARNFLAGS.dce110_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dce112_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dce120_hw_sequencer.c=	-Wno-unused-variable
CWARNFLAGS.dce120_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dce120_timing_generator.c=	-Wno-missing-prototypes
CWARNFLAGS.dce80_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dce80_timing_generator.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_calcs.c=		-Wno-cast-qual -Wno-parentheses-equality
CWARNFLAGS.dce_clocks.c=	-Wno-enum-conversion
CWARNFLAGS.dce_dmcu.c=		-Wno-missing-prototypes
CWARNFLAGS.dce_transform.c=	-Wno-enum-conversion
CWARNFLAGS.dcn10_dpp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_dpp_dscl.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn10_hw_sequencer.c=-Wno-missing-prototypes
CWARNFLAGS.dcn10_opp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_optc.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn10_resource.c=	-Wno-missing-prototypes	-Wno-initializer-overrides
CWARNFLAGS.dcn_calcs.c=		-Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.dcn_calc_auto.c=	-Wno-cast-qual -Wno-parentheses-equality
CWARNFLAGS.eventtasks.c=	-Wno-missing-prototypes
CWARNFLAGS.fiji_clockpowergating.c= -Wno-enum-conversion
CWARNFLAGS.fiji_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.gmc_v6_0.c=	-Wno-unused-const-variable
CWARNFLAGS.gfx_v6_0.c=	-Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.gfx_v7_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gfx_v8_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gfx_v9_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gpu_scheduler.c=	-Wno-missing-prototypes
CWARNFLAGS.hw_factory.c=	-Wno-missing-prototypes
CWARNFLAGS.hw_factory_diag.c=	-Wno-missing-prototypes
CWARNFLAGS.hw_translate_diag.c=	-Wno-missing-prototypes
CWARNFLAGS.i2c_sw_engine_dce80.c= -Wno-unused-const-variable
CWARNFLAGS.i2caux_dce100.c=	-Wno-missing-prototypes
CWARNFLAGS.i2caux_dce120.c=	-Wno-missing-prototypes
CWARNFLAGS.i2caux_dcn10.c=	-Wno-missing-prototypes
CWARNFLAGS.irq_service.c=	-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn10.c=	-Wno-missing-prototypes
CWARNFLAGS.iceland_hwmgr.c=	-Wno-missing-prototypes  -Wno-cast-qual
CWARNFLAGS.iceland_smumgr.c=	-Wno-missing-prototypes
CWARNFLAGS.iceland_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.kv_dpm.c=	-Wno-unused-const-variable
CWARNFLAGS.mxgpu_vi.c=	-Wno-enum-conversion
CWARNFLAGS.polaris10_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.polaris10_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.polaris10_powertune.c=	-Wno-missing-prototypes
CWARNFLAGS.polaris10_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.polaris10_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.pp_acpi.c=	-Wno-missing-prototypes 
CWARNFLAGS.ppatomctrl.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.ppatomfwctrl.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.processpptables.c=	-Wno-missing-prototypes -Wno-sometimes-uninitialized
CWARNFLAGS.process_pptables_v1_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.psp_v3_1.c=	-Wno-cast-qual
CWARNFLAGS.psp_v10_0.c=	-Wno-cast-qual
CWARNFLAGS.rv_hwmgr.c=	-Wno-cast-qual
CWARNFLAGS.rv_smumgr.c=	-Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.sdma_v3_0.c=		-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.sdma_v4_0.c=		-Wno-missing-prototypes -Wno-cast-qual -Wno-sometimes-uninitialized
CWARNFLAGS.si.c=		-Wno-missing-prototypes
CWARNFLAGS.smu7_clockpowergating.c=		-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.smu7_hwmgr.c=		-Wno-missing-prototypes  -Wno-cast-qual
CWARNFLAGS.soc15.c=		-Wno-missing-prototypes
CWARNFLAGS.tonga_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.tonga_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.tonga_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.tonga_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.vector.c=	-Wno-missing-prototypes
CWARNFLAGS.vega10_hwmgr.c=	-Wno-missing-prototypes -Wno-sometimes-uninitialized -Wno-cast-qual
CWARNFLAGS.vega10_ih.c= 	-Wno-cast-qual
CWARNFLAGS.vega10_powertune.c=	-Wno-unused-const-variable
CWARNFLAGS.vega10_processpptables.c= -Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.vega10_reg_init.c= 	-Wno-cast-qual
CWARNFLAGS.vega10_smumgr.c=	-Wno-missing-prototypes
CWARNFLAGS.vega10_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.vi.c= -Wno-unused-const-variable
CWARNFLAGS.uvd_v7_0.c=	-Wno-missing-prototypes

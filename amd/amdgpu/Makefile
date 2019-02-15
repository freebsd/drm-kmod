# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/amd
DRM= ${.CURDIR:H:H}/drivers/gpu/drm

KMOD=	amdgpu

.include "../../kconfig.mk"

.if ${MACHINE_CPUARCH} == "amd64"
_dml=	${SRCDIR}/display/dc/dml \
	${SRCDIR}/display/dc/dml/dcn20
.endif

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
	${SRCDIR}/display/dc/clk_mgr \
	${SRCDIR}/display/dc/clk_mgr/dce100 \
	${SRCDIR}/display/dc/clk_mgr/dce110 \
	${SRCDIR}/display/dc/clk_mgr/dce112 \
	${SRCDIR}/display/dc/clk_mgr/dce120 \
	${SRCDIR}/display/dc/clk_mgr/dcn10 \
	${SRCDIR}/display/dc/clk_mgr/dcn20 \
	${SRCDIR}/display/dc/dce \
	${SRCDIR}/display/dc/dce100 \
	${SRCDIR}/display/dc/dce110 \
	${SRCDIR}/display/dc/dce112 \
	${SRCDIR}/display/dc/dce120 \
	${SRCDIR}/display/dc/dce80 \
	${SRCDIR}/display/dc/dcn10 \
	${SRCDIR}/display/dc/dcn20 \
	${SRCDIR}/display/dc/dsc \
	${_dml} \
	${SRCDIR}/display/dc/gpio \
	${SRCDIR}/display/dc/gpio/dce110 \
	${SRCDIR}/display/dc/gpio/dce120 \
	${SRCDIR}/display/dc/gpio/dce80 \
	${SRCDIR}/display/dc/gpio/dcn10 \
	${SRCDIR}/display/dc/gpio/dcn20 \
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
	${SRCDIR}/display/dc/irq/dcn20 \
	${SRCDIR}/display/dc/virtual \
	${SRCDIR}/display/modules \
	${SRCDIR}/display/modules/color \
	${SRCDIR}/display/modules/freesync \
	${SRCDIR}/display/modules/inc \
	${SRCDIR}/display/modules/power \
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

# These should only be included if we set CONFIG_HSA_AMD
# amdgpu_amdkfd_fence.c \
# amdgpu_amdkfd_gfx_v8.c \
# amdgpu_amdkfd_gfx_v9.c \
# amdgpu_amdkfd_gpuvm.c \

# This one for CONFIG_DRM_AMDGPU_CIK & CONFIG_HSA_AMD
# amdgpu_amdkfd_gfx_v7.c \

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
	amdgpu_device.c \
	amdgpu_discovery.c \
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
	amdgpu_gmc.c \
	amdgpu_gtt_mgr.c \
	amdgpu_i2c.c \
	amdgpu_ib.c \
	amdgpu_ids.c \
	amdgpu_ih.c \
	amdgpu_ioc32.c \
	amdgpu_irq.c \
	amdgpu_job.c \
	amdgpu_kms.c \
	amdgpu_mmhub.c \
	amdgpu_nbio.c \
	amdgpu_object.c \
	amdgpu_pll.c \
	amdgpu_pm.c \
	amdgpu_dma_buf.c \
	amdgpu_psp.c \
	amdgpu_ring.c \
	amdgpu_ras.c \
	amdgpu_rlc.c \
	amdgpu_sa.c \
	amdgpu_sched.c \
	amdgpu_sdma.c \
	amdgpu_sync.c \
	amdgpu_test.c \
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
	amdgpu_vm_cpu.c \
	amdgpu_vm_sdma.c \
	amdgpu_vram_mgr.c \
	amdgpu_xgmi.c \
	arct_reg_init.c \
	athub_v1_0.c \
	athub_v2_0.c \
	atom.c \
	atombios_crtc.c \
	atombios_dp.c \
	atombios_encoders.c \
	atombios_i2c.c \
	cik_ih.c \
	cik_sdma.c \
	cik.c \
	cz_ih.c \
	dce_v10_0.c \
	dce_v11_0.c \
	dce_v6_0.c \
	dce_v8_0.c \
	dce_virtual.c \
	df_v1_7.c \
	df_v3_6.c \
	emu_soc.c \
	gfx_v6_0.c \
	gfx_v7_0.c \
	gfx_v8_0.c \
	gfx_v9_0.c \
	gfx_v10_0.c \
	gfxhub_v1_0.c \
	gfxhub_v1_1.c \
	gfxhub_v2_0.c \
	gmc_v6_0.c \
	gmc_v7_0.c \
	gmc_v8_0.c \
	gmc_v9_0.c \
	gmc_v10_0.c \
	iceland_ih.c \
	kv_dpm.c \
	kv_smc.c \
	mes_v10_1.c \
	mmhub_v1_0.c \
	mmhub_v2_0.c \
	mmhub_v9_4.c \
	mxgpu_ai.c \
	mxgpu_nv.c \
	mxgpu_vi.c \
	navi10_reg_init.c \
	navi12_reg_init.c \
	navi14_reg_init.c \
	nbio_v2_3.c \
	nbio_v6_1.c \
	nbio_v7_0.c \
	nbio_v7_4.c \
	navi10_ih.c \
	nv.c \
	psp_v3_1.c \
	psp_v10_0.c \
	psp_v11_0.c \
	psp_v12_0.c \
	sdma_v2_4.c \
	sdma_v3_0.c \
	sdma_v4_0.c \
	sdma_v5_0.c \
	si_dma.c \
	si_dpm.c \
	si_ih.c \
	si_smc.c \
	si.c \
	soc15.c \
	tonga_ih.c \
	umc_v6_0.c \
	umc_v6_1.c \
	uvd_v4_2.c \
	uvd_v5_0.c \
	uvd_v6_0.c \
	uvd_v7_0.c \
	vce_v2_0.c \
	vce_v3_0.c \
	vce_v4_0.c \
	vcn_v1_0.c \
	vcn_v2_0.c \
	vcn_v2_5.c \
	vega10_ih.c \
	vega10_reg_init.c \
	vega20_reg_init.c \
	vi.c

# display/amdgpu_dm
SRCS+=	amdgpu_dm_color.c \
	amdgpu_dm_helpers.c \
	amdgpu_dm_irq.c \
	amdgpu_dm_mst_types.c \
	amdgpu_dm_pp_smu.c \
	amdgpu_dm_services.c \
	amdgpu_dm.c

# display/dc/basics
SRCS+=	conversion.c \
	fixpt31_32.c \
	log_helpers.c \
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
	command_table_helper_dce80.c

# display/dc/calcs
SRCS+=	bw_fixed.c \
	custom_float.c \
	dce_calcs.c

# display/dc/core
SRCS+=	dc_debug.c \
	dc_hw_sequencer.c \
	dc_link_ddc.c \
	dc_link_dp.c \
	dc_link_hwss.c \
	dc_link.c \
	dc_resource.c \
	dc_sink.c \
	dc_stream.c \
	dc_surface.c \
	dc.c

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
	dce_scl_filters.c \
	dce_stream_encoder.c \
	dce_transform.c

# display/dc/dce100
SRCS+=	dce100_hw_sequencer.c \
	dce100_resource.c

# display/dc/dce110
SRCS+=	dce110_compressor.c \
	dce110_hw_sequencer.c \
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
	dce112_hw_sequencer.c \
	dce112_resource.c

# display/dc/dce120
SRCS+=	dce120_hw_sequencer.c \
	dce120_resource.c \
	dce120_timing_generator.c

# display/dc/dce80
SRCS+=	dce80_hw_sequencer.c \
	dce80_resource.c \
	dce80_timing_generator.c

# display/dc/clk_mgr
SRCS+=	clk_mgr.c \
	dce_clk_mgr.c \
	dce110_clk_mgr.c \
	dce112_clk_mgr.c \
	dce120_clk_mgr.c \
	rv1_clk_mgr.c \
	rv1_clk_mgr_vbios_smu.c \
	rv2_clk_mgr.c

# display/dc/dcn10
.if ${MACHINE_CPUARCH} == "amd64"
SRCS+=	dcn10_cm_common.c \
	dcn10_dpp.c \
	dcn10_dpp_cm.c \
	dcn10_dpp_dscl.c \
	dcn10_hubbub.c \
	dcn10_hubp.c \
	dcn10_hw_sequencer.c \
	dcn10_hw_sequencer_debug.c \
	dcn10_ipp.c \
	dcn10_link_encoder.c \
	dcn10_mpc.c \
	dcn10_opp.c \
	dcn10_optc.c \
	dcn10_resource.c \
	dcn10_stream_encoder.c

# display/dc/dcn20
SRCS+=	dcn20_dccg.c \
	dcn20_dpp_cm.c \
	dcn20_dpp.c \
	dcn20_dsc.c \
	dcn20_dwb_scl.c \
	dcn20_dwb.c \
	dcn20_hubbub.c \
	dcn20_hubp.c \
	dcn20_hwseq.c \
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

# display/dc/core
SRCS+=	dc_vm_helper.c

# display/dc/clk_mgr
SRCS+=	dcn20_clk_mgr.c

# display/dc/dsc
SRCS+=	rc_calc.c \
	rc_calc_dpi.c \
	dc_dsc.c

.endif

# display/dc/dml
.if ${MACHINE_CPUARCH} == "amd64"
SRCS+=	display_mode_lib.c \
	display_mode_vba.c \
	display_mode_vba_20.c \
	display_rq_dlg_helpers.c \
	display_rq_dlg_calc_20.c \
	dml_common_defs.c \
	dml1_display_rq_dlg_calc.c
.endif

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
	hw_factory_dce80.c \
	hw_factory_dcn10.c \
	hw_factory_dcn20.c \
	hw_factory_diag.c \
	hw_translate_dce110.c \
	hw_translate_dce120.c \
	hw_translate_dce80.c \
	hw_translate_dcn10.c \
	hw_translate_dcn20.c \
	hw_translate_diag.c

# display/dc/irq/*
SRCS+=	irq_service.c \
	irq_service_dce110.c \
	irq_service_dce120.c \
	irq_service_dce80.c \
	irq_service_dcn10.c \
	irq_service_dcn20.c

# display/dc/virtual
SRCS+=	virtual_link_encoder.c \
	virtual_stream_encoder.c

# display/modules/*
SRCS+=	color_gamma.c \
	freesync.c \
	power_helpers.c

# display/dc
SRCS+=	dc_helper.c

# Debug files, not handled for now
SRCS_DEBUG=	amdgpu_dm_crc.c \
		amdgpu_debugfs.c \
		amdgpu_dm_debugfs.c

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "aarch64"
SRCS+=	amdgpu_acpi.c \
	amdgpu_atpx_handler.c
.endif

.if ${MACHINE_CPUARCH} == "amd64"
SRCS+=	dcn_calc_auto.c \
	dcn_calc_math.c \
	dcn_calcs.c
.endif

# powerplay
SRCS+=	amd_powerplay.c \
	amdgpu_smu.c \
	arcturus_ppt.c \
	common_baco.c \
	ci_smumgr.c \
	fiji_smumgr.c \
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
	smu_v11_0.c \
	smu_v12_0.c \
	smu_helper.c \
	smumgr.c \
	tonga_baco.c \
	tonga_smumgr.c \
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
	vega20_ppt.c \
	vega20_processpptables.c \
	vega20_smumgr.c \
	vega20_thermal.c \
	vegam_smumgr.c

.ifdef (CONFIG_DEBUGFS)
SRCS+=	amdgpu_pmu.c
.endif

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
CFLAGS+= -I${SRCDIR}/display/dc/clk_mgr/
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
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=amdgpu_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD} -DLINUXKPI_VERSION=50000
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS+=	device_if.h vnode_if.h bus_if.h pci_if.h pci_iov_if.h device_if.h iicbus_if.h opt_drm.h \
        opt_vm.h opt_compat.h opt_syscons.h opt_acpi.h

.include <bsd.kmod.mk>

CFLAGS.gcc+= -Wno-redundant-decls -Wno-unused-but-set-variable

CFLAGS.dcn_calcs.c= -msse -mstack-alignment=4
CFLAGS.dcn_calc_auto.c= -msse -mstack-alignment=4
CFLAGS.dcn_calc_math.c= -msse -mstack-alignment=4 -Wno-tautological-compare
CFLAGS.dcn20_resource.c= -msse -mstack-alignment=4

CFLAGS.display_mode_lib.c= -msse -mstack-alignment=4
CFLAGS.display_mode_vba.c= -msse -mstack-alignment=4
CFLAGS.display_mode_vba_20.c= -msse -mstack-alignment=4
CFLAGS.display_pipe_clocks.c= -msse -mstack-alignment=4
CFLAGS.dml1_display_rq_dlg_calc.c= -msse -mstack-alignment=4
CFLAGS.display_rq_dlg_helpers.c= -msse -mstack-alignment=4
CFLAGS.display_rq_dlg_calc_20.c= -msse -mstack-alignment=4
CFLAGS.soc_bounding_box.c= -msse -mstack-alignment=4
CFLAGS.dml_common_defs.c= -msse -mstack-alignment=4

CFLAGS.display_rq_dlg_calc_20v2.c= -msse -mstack-alignment=4
CFLAGS.display_mode_vba_20v2.c= -msse -mstack-alignment=4

CFLAGS.rc_calc.c= -msse -mstack-alignment=4
CFLAGS.rc_calc_dpi.c= -msse -mstack-alignment=4

.if ${MACHINE_CPUARCH} != "powerpc"
CWARNFLAGS+=	-Wno-expansion-to-defined
.endif
CWARNFLAGS+=	-Wno-pointer-arith -Wno-format -Wno-cast-qual
CWARNFLAGS+=	-Wno-pointer-sign ${CWARNFLAGS.${.IMPSRC:T}}

# amdgpu/
.if ${MACHINE_CPUARCH} == "powerpc"
CWARNFLAGS.amdgpu_device.c=	-Wno-missing-prototypes
.endif
CWARNFLAGS.amdgpu_atpx_handler.c=		-Wno-missing-prototypes
CWARNFLAGS.amdgpu_dma_buf.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ioc32.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_pm.c=				-Wno-unused-const-variable
CWARNFLAGS.amdgpu_ras.c=			-Wno-missing-prototypes -Wno-unused-function
CWARNFLAGS.amdgpu_sched.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ttm.c=			-Wno-missing-prototypes
CWARNFLAGS.amdgpu_xgmi.c=			-Wno-missing-prototypes
CWARNFLAGS.gfx_v7_0.c=				-Wno-missing-prototypes
CWARNFLAGS.gfx_v8_0.c=				-Wno-missing-prototypes
CWARNFLAGS.gfx_v9_0.c=				-Wno-missing-prototypes
CWARNFLAGS.gfx_v10_0.c=				-Wno-missing-prototypes
CWARNFLAGS.gmc_v6_0.c=				-Wno-unused-const-variable
CWARNFLAGS.mes_v10_1.c=				-Wno-unused-const-variable
CWARNFLAGS.mmhub_v9_4.c=			-Wno-missing-prototypes
CWARNFLAGS.vcn_v2_0.c=				-Wno-missing-prototypes

# display/amdgpu_dm
CWARNFLAGS.amdgpu_dm_pp_smu.c=		-Wno-missing-prototypes -Wno-incompatible-function-pointer-types
CWARNFLAGS.amdgpu_dm_services.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_dm.c=			-Wno-missing-prototypes -Wno-unused-function

# display/dc
CWARNFLAGS.conversion.c=		-Wno-missing-prototypes
CWARNFLAGS.fixpt31_32.c=		-Wno-incompatible-pointer-types
CWARNFLAGS.vector.c=			-Wno-missing-prototypes
CWARNFLAGS.bios_parser.c=		-Wno-missing-prototypes
CWARNFLAGS.dc_hw_sequencer.c=		-Wno-missing-prototypes
CWARNFLAGS.dc_link.c=			-Wno-missing-prototypes
CWARNFLAGS.dc_link_dp.c=		-Wno-missing-prototypes
CWARNFLAGS.dce_aux.c=			-Wno-missing-prototypes
CWARNFLAGS.dce_clock_source.c=		-Wno-unused-const-variable
CWARNFLAGS.dce_dmcu.c=			-Wno-missing-prototypes
CWARNFLAGS.dce_i2c_hw.c=		-Wno-missing-prototypes
CWARNFLAGS.dce_i2c_sw.c=		-Wno-missing-prototypes
CWARNFLAGS.dce100_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce110_compressor.c=		-Wno-missing-prototypes
CWARNFLAGS.dce110_hw_sequencer.c=	-Wno-missing-prototypes
CWARNFLAGS.dce110_mem_input_v.c=	-Wno-missing-prototypes
CWARNFLAGS.dce110_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce112_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce120_hw_sequencer.c=	-Wno-unused-const-variable
CWARNFLAGS.dce120_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dce120_timing_generator.c=	-Wno-missing-prototypes
CWARNFLAGS.dce80_hw_sequencer.c=	-Wno-unused-const-variable
CWARNFLAGS.dce80_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.rv1_clk_mgr_vbios_smu.c=	-Wno-missing-prototypes
CWARNFLAGS.rv1_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn10_dpp_dscl.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_hw_sequencer.c=	-Wno-missing-prototypes
CWARNFLAGS.dcn10_opp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn10_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn10_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.dcn20_dpp.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn20_dwb_scl.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_hwseq.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_hubp.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_mpc.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn20_optc.c=		-Wno-missing-prototypes
CWARNFLAGS.dcn20_resource.c=		-Wno-missing-prototypes -Wno-initializer-overrides -Wno-unused-variable
CWARNFLAGS.dcn20_clk_mgr.c=		-Wno-missing-prototypes
CWARNFLAGS.rc_calc.c=			-Wno-missing-prototypes
CWARNFLAGS.hw_factory.c=		-Wno-missing-prototypes -Wno-unused-function
CWARNFLAGS.hw_factory_diag.c=		-Wno-missing-prototypes
CWARNFLAGS.hw_gpio.c=			-Wno-unused-function
CWARNFLAGS.hw_translate_diag.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn10.c=		-Wno-missing-prototypes
CWARNFLAGS.irq_service_dcn20.c=		-Wno-missing-prototypes
CWARNFLAGS.power_helpers.c=		-Wno-missing-prototypes
CWARNFLAGS.dc_helper.c=			-Wno-missing-prototypes
CWARNFLAGS.dcn_calcs.c+=		-Wno-missing-prototypes -Wno-unused-variable

# powerplay/*
CWARNFLAGS.amdgpu_smu.c=		-Wno-missing-prototypes
CWARNFLAGS.ci_smumgr.c=			-Wno-missing-prototypes
CWARNFLAGS.hardwaremanager.c=		-Wno-missing-prototypes
CWARNFLAGS.iceland_smumgr.c=		-Wno-missing-prototypes
CWARNFLAGS.polaris10_smumgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu7_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu8_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu9_smumgr.c=		-Wno-missing-prototypes
CWARNFLAGS.smu10_hwmgr.c=		-Wno-missing-prototypes -Wno-initializer-overrides
CWARNFLAGS.smu_v11_0.c=			-Wno-missing-prototypes
CWARNFLAGS.tonga_smumgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vega10_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vega10_powertune.c=		-Wno-unused-const-variable
CWARNFLAGS.vega10_processpptables.c=	-Wno-missing-prototypes
CWARNFLAGS.vega10_thermal.c=		-Wno-missing-prototypes
CWARNFLAGS.vega12_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vega12_processpptables.c=	-Wno-missing-prototypes
CWARNFLAGS.vega12_thermal.c=		-Wno-missing-prototypes
CWARNFLAGS.vega20_hwmgr.c=		-Wno-missing-prototypes
CWARNFLAGS.vegam_smumgr.c=		-Wno-missing-prototypes

# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/amd

.PATH:	${SRCDIR}/amdgpu \
	${SRCDIR}/lib \
	${SRCDIR}/scheduler \
	${SRCDIR}/powerplay \
	${SRCDIR}/powerplay/eventmgr \
	${SRCDIR}/powerplay/hwmgr \
	${SRCDIR}/powerplay/smumgr

KMOD=	amdgpu
SRCS=	amdgpu_acpi.c \
	amdgpu_afmt.c \
	amdgpu_amdkfd.c \
	amdgpu_amdkfd_gfx_v7.c \
	amdgpu_amdkfd_gfx_v8.c \
	amdgpu_atombios.c \
	amdgpu_atomfirmware.c \
	amdgpu_atpx_handler.c \
	amdgpu_benchmark.c \
	amdgpu_bios.c \
	amdgpu_bo_list.c \
	amdgpu_cgs.c \
	amdgpu_connectors.c \
	amdgpu_cs.c \
	amdgpu_ctx.c \
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
	amd_powerplay.c \
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
	cz_clockpowergating.c \
	cz_ih.c \
	cz_hwmgr.c \
	cz_smumgr.c \
	dce_v6_0.c \
	dce_v8_0.c \
	dce_v10_0.c \
	dce_v11_0.c \
	dce_virtual.c \
	fiji_smumgr.c \
	gfx_v6_0.c \
	gfx_v7_0.c \
	gfx_v8_0.c \
	gfx_v9_0.c \
	gfxhub_v1_0.c \
	gmc_v6_0.c \
	gmc_v7_0.c \
	gmc_v8_0.c \
	gmc_v9_0.c \
	gpu_scheduler.c \
	hardwaremanager.c \
	hwmgr.c \
	iceland_ih.c \
	iceland_smumgr.c \
	kv_dpm.c \
	kv_smc.c \
	nbio_v6_1.c \
	nbio_v7_0.c \
	mmhub_v1_0.c \
	mxgpu_ai.c \
	mxgpu_vi.c \
	polaris10_smumgr.c \
	pp_acpi.c \
	pp_overdriver.c \
	pp_psm.c \
	ppatomctrl.c \
	ppatomfwctrl.c \
	pppcielanes.c \
	processpptables.c \
	process_pptables_v1_0.c \
	psp_v3_1.c \
	psp_v10_0.c \
	rv_hwmgr.c \
	rv_smumgr.c \
	sched_fence.c \
	sdma_v2_4.c \
	sdma_v3_0.c \
	sdma_v4_0.c \
	si.c \
	si_dma.c \
	si_dpm.c \
	si_ih.c \
	si_smc.c \
	smumgr.c \
	smu7_clockpowergating.c \
	smu7_hwmgr.c \
	smu7_powertune.c \
	smu7_smumgr.c \
	smu7_thermal.c \
	soc15.c \
	tonga_ih.c \
	tonga_smumgr.c \
	uvd_v4_2.c \
	uvd_v5_0.c \
	uvd_v6_0.c \
	uvd_v7_0.c \
	vce_v2_0.c \
	vce_v3_0.c \
	vce_v4_0.c \
	vcn_v1_0.c \
	vega10_hwmgr.c \
	vega10_ih.c \
	vega10_powertune.c \
	vega10_processpptables.c \
	vega10_smumgr.c \
	vega10_thermal.c \
	vi.c \
	chash.c


CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H:H}/include
CFLAGS+= -I${.CURDIR:H:H}/include/drm
CFLAGS+= -I${.CURDIR:H:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${SRCDIR}/include
CFLAGS+= -I${SRCDIR}/amdgpu
CFLAGS+= -I${SRCDIR}/display
CFLAGS+= -I${SRCDIR}/display/amdgpu_dm
CFLAGS+= -I${SRCDIR}/display/dc
CFLAGS+= -I${SRCDIR}/display/include
CFLAGS+= -I${SRCDIR}/acp/include
CFLAGS+= -I${SRCDIR}/powerplay/eventmgr
CFLAGS+= -I${SRCDIR}/powerplay/hwmgr
CFLAGS+= -I${SRCDIR}/powerplay/inc
CFLAGS+= -I${SRCDIR}/powerplay/smumgr
CFLAGS+= -I${SRCDIR}/scheduler
CFLAGS+= -I${SRCDIR}/include/asic_reg

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${SRCDIR:H}/drm_os_config.h

SRCS+=	device_if.h vnode_if.h bus_if.h pci_if.h device_if.h iicbus_if.h opt_drm.h \
        opt_vm.h opt_compat.h opt_syscons.h opt_acpi.h

.include <bsd.kmod.mk>

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
CWARNFLAGS.amdgpu_device.c=	 -Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.amdgpu_gfx.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_gtt_mgr.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_job.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ih.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_mn.c=		-Wno-unused-variable
CWARNFLAGS.amdgpu_pll.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_pm.c=		-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.amdgpu_ring.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_ttm.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ucode.c=	-Wno-incompatible-pointer-types-discards-qualifiers -Wno-cast-qual
CWARNFLAGS.amdgpu_vm.c=		-Wno-missing-prototypes
CWARNFLAGS.amd_powerplay.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.atombios_crtc.c=	-Wno-missing-prototypes
CWARNFLAGS.atombios_i2c.c=	-Wno-missing-prototypes
CWARNFLAGS.ci_dpm.c=	-Wno-unused-const-variable
CWARNFLAGS.ci_smc_pp.c=	-Wno-missing-prototypes
CWARNFLAGS.cik.c=	-Wno-unused-const-variable
CWARNFLAGS.cz_smc.c=	-Wno-missing-prototypes
CWARNFLAGS.cz_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.cz_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.dce_virtual.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v6_0.c=	 -Wno-unused-const-variable -Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.dce_v8_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v10_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v11_0.c=	-Wno-missing-prototypes
CWARNFLAGS.eventtasks.c=	-Wno-missing-prototypes
CWARNFLAGS.fiji_clockpowergating.c=	-Wno-enum-conversion
CWARNFLAGS.fiji_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.gmc_v6_0.c=	-Wno-unused-const-variable
CWARNFLAGS.gfx_v6_0.c=	-Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.gfx_v7_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gfx_v8_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gfx_v9_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gpu_scheduler.c=	-Wno-missing-prototypes
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
CWARNFLAGS.vega10_hwmgr.c=	-Wno-missing-prototypes -Wno-sometimes-uninitialized -Wno-cast-qual
CWARNFLAGS.vega10_ih.c= 	-Wno-cast-qual
CWARNFLAGS.vega10_powertune.c=	-Wno-unused-const-variable
CWARNFLAGS.vega10_processpptables.c= -Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.vega10_smumgr.c=	-Wno-missing-prototypes
CWARNFLAGS.vega10_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.vi.c= -Wno-unused-const-variable
CWARNFLAGS.uvd_v7_0.c=	-Wno-missing-prototypes

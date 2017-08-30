# $FreeBSD$

.PATH:	${.CURDIR:H}/amdgpu \
	${.CURDIR:H}/scheduler \
	${.CURDIR:H}/powerplay \
	${.CURDIR:H}/powerplay/eventmgr \
	${.CURDIR:H}/powerplay/hwmgr \
	${.CURDIR:H}/powerplay/smumgr

KMOD=	amdgpu

SRCS=	amdgpu_acpi.c \
	amdgpu_afmt.c \
	amdgpu_amdkfd.c \
	amdgpu_amdkfd_gfx_v7.c \
	amdgpu_amdkfd_gfx_v8.c \
	amdgpu_atombios.c \
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
	amdgpu_irq.c \
	amdgpu_job.c \
	amdgpu_kms.c \
	amdgpu_object.c \
	amdgpu_pll.c \
	amdgpu_pm.c \
	amdgpu_powerplay.c \
	amdgpu_prime.c \
	amdgpu_ring.c \
	amdgpu_sa.c \
	amdgpu_sync.c \
	amdgpu_test.c \
	amdgpu_trace_points.c \
	amdgpu_ttm.c \
	amdgpu_ucode.c \
	amdgpu_uvd.c \
	amdgpu_vce.c \
	amdgpu_vm.c \
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
	cz_dpm.c \
	cz_ih.c \
	cz_hwmgr.c \
	cz_smc.c \
	cz_smumgr.c \
	dce_v6_0.c \
	dce_v8_0.c \
	dce_v10_0.c \
	dce_v11_0.c \
	dce_virtual.c \
	eventactionchains.c \
	eventinit.c \
	eventmanagement.c \
	eventmgr.c \
	eventsubchains.c \
	eventtasks.c \
	fiji_smumgr.c \
	fiji_smumgr_smc.c \
	functiontables.c \
	gfx_v6_0.c \
	gfx_v7_0.c \
	gfx_v8_0.c \
	gmc_v6_0.c \
	gmc_v7_0.c \
	gmc_v8_0.c \
	gpu_scheduler.c \
	hardwaremanager.c \
	hwmgr.c \
	iceland_ih.c \
	iceland_smumgr.c \
	iceland_smumgr_smc.c \
	kv_dpm.c \
	kv_smc.c \
	polaris10_smc.c \
	polaris10_smumgr.c \
	pp_acpi.c \
	ppatomctrl.c \
	pppcielanes.c \
	processpptables.c \
	process_pptables_v1_0.c \
	psm.c \
	sched_fence.c \
	sdma_v2_4.c \
	sdma_v3_0.c \
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
	tonga_ih.c \
	tonga_smumgr.c \
	tonga_smumgr_smc.c \
	uvd_v4_2.c \
	uvd_v5_0.c \
	uvd_v6_0.c \
	vce_v2_0.c \
	vce_v3_0.c \
	vi.c

CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include

CFLAGS+= -I${.CURDIR:H:H}/include
CFLAGS+= -I${.CURDIR:H:H}/include/drm
CFLAGS+= -I${.CURDIR:H:H}/include/uapi
CFLAGS+= -I${.CURDIR:H:H}

CFLAGS+= -I${.CURDIR}
CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/acp/include
CFLAGS+= -I${.CURDIR:H}/powerplay/eventmgr
CFLAGS+= -I${.CURDIR:H}/powerplay/hwmgr
CFLAGS+= -I${.CURDIR:H}/powerplay/inc
CFLAGS+= -I${.CURDIR:H}/powerplay/smumgr
CFLAGS+= -I${.CURDIR:H}/scheduler
CFLAGS+= -I${.CURDIR:H}/include/asic_reg

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${.CURDIR:H:H}/drm/drm_os_config.h

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
CWARNFLAGS.amdgpu_atpx_handler.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_bo_list.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_cgs.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.amdgpu_connectors.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_cs.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_device.c=	 -Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.amdgpu_gfx.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_job.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ih.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_mn.c=		-Wno-unused-variable
CWARNFLAGS.amdgpu_pll.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_pm.c=		-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.amdgpu_ring.c=	-Wno-cast-qual
CWARNFLAGS.amdgpu_ttm.c=	-Wno-missing-prototypes
CWARNFLAGS.amdgpu_ucode.c=	-Wno-incompatible-pointer-types-discards-qualifiers -Wno-cast-qual
CWARNFLAGS.amd_powerplay.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.atombios_crtc.c=	-Wno-missing-prototypes
CWARNFLAGS.atombios_i2c.c=	-Wno-missing-prototypes
CWARNFLAGS.ci_dpm.c=	-Wno-unused-const-variable
CWARNFLAGS.cik.c=	-Wno-unused-const-variable
CWARNFLAGS.cz_smc.c=	-Wno-missing-prototypes
CWARNFLAGS.cz_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.cz_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.dce_virtual.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v6_0.c=	 -Wno-unused-const-variable
CWARNFLAGS.dce_v8_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v10_0.c=	-Wno-missing-prototypes
CWARNFLAGS.dce_v11_0.c=	-Wno-missing-prototypes
CWARNFLAGS.eventtasks.c=	-Wno-missing-prototypes
CWARNFLAGS.fiji_clockpowergating.c=	-Wno-enum-conversion
CWARNFLAGS.fiji_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.fiji_smumgr_smc.c=	-Wno-missing-prototypes
CWARNFLAGS.fiji_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.gmc_v6_0.c=	-Wno-unused-const-variable
CWARNFLAGS.gfx_v6_0.c=	-Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.gfx_v7_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gfx_v8_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.gpu_scheduler.c=	-Wno-missing-prototypes
CWARNFLAGS.iceland_hwmgr.c=	-Wno-missing-prototypes  -Wno-cast-qual
CWARNFLAGS.iceland_smumgr.c=	-Wno-missing-prototypes
CWARNFLAGS.iceland_smumgr_smc.c=	-Wno-missing-prototypes
CWARNFLAGS.iceland_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.kv_dpm.c=	-Wno-unused-const-variable
CWARNFLAGS.polaris10_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.polaris10_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.polaris10_powertune.c=	-Wno-missing-prototypes
CWARNFLAGS.polaris10_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.polaris10_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.pp_acpi.c=	-Wno-missing-prototypes 
CWARNFLAGS.ppatomctrl.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.processpptables.c=	-Wno-missing-prototypes -Wno-sometimes-uninitialized
CWARNFLAGS.process_pptables_v1_0.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.sdma_v3_0.c=		-Wno-missing-prototypes
CWARNFLAGS.si.c=		-Wno-missing-prototypes
CWARNFLAGS.smu7_clockpowergating.c=		-Wno-missing-prototypes
CWARNFLAGS.smu7_hwmgr.c=		-Wno-missing-prototypes  -Wno-cast-qual
CWARNFLAGS.tonga_clockpowergating.c=	-Wno-missing-prototypes -Wno-enum-conversion
CWARNFLAGS.tonga_hwmgr.c=	-Wno-missing-prototypes -Wno-cast-qual -Wno-unused-const-variable
CWARNFLAGS.tonga_smumgr.c=	-Wno-missing-prototypes -Wno-cast-qual
CWARNFLAGS.tonga_smumgr_smc.c=	-Wno-parentheses-equality
CWARNFLAGS.tonga_thermal.c=	-Wno-missing-prototypes
CWARNFLAGS.vi.c=	-Wno-unused-const-variable

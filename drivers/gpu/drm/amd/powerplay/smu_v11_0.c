/*
 * Copyright 2019 Advanced Micro Devices, Inc.
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
 */

#include "pp_debug.h"
#include <linux/firmware.h>
#include "amdgpu.h"
#include "amdgpu_smu.h"
#include "atomfirmware.h"
#include "amdgpu_atomfirmware.h"
#include "smu_v11_0.h"

static int smu_v11_0_init_microcode(struct smu_context *smu)
{
	struct amdgpu_device *adev = smu->adev;

	return 0;
}

static int smu_v11_0_load_microcode(struct smu_context *smu)
{
	return 0;
}

static int smu_v11_0_check_fw_status(struct smu_context *smu)
{
	return 0;
}

static int smu_v11_0_read_pptable_from_vbios(struct smu_context *smu)
{
	int ret, index;
	uint16_t size;
	uint8_t frev, crev;
	struct smu_11_0_powerplay_table *table;

	index = get_index_into_master_table(atom_master_list_of_data_tables_v2_1,
					    powerplayinfo);

	ret = smu_get_atom_data_table(smu, index, &size, &frev, &crev,
				      (uint8_t **)&table);
	if (ret)
		return ret;

	smu->smu_table.power_play_table = table;
	smu->smu_table.power_play_table_size = size;

	return 0;
}

static int smu_v11_0_init_dpm_context(struct smu_context *smu)
{
	struct smu_dpm_context *smu_dpm = &smu->smu_dpm;

	if (smu_dpm->dpm_context || smu_dpm->dpm_context_size != 0)
		return -EINVAL;

	smu_dpm->dpm_context = kzalloc(sizeof(struct smu_11_0_dpm_context), GFP_KERNEL);
	if (!smu_dpm->dpm_context)
		return -ENOMEM;
	smu_dpm->dpm_context_size = sizeof(struct smu_11_0_dpm_context);

	return 0;
}

static int smu_v11_0_fini_dpm_context(struct smu_context *smu)
{
	struct smu_dpm_context *smu_dpm = &smu->smu_dpm;

	if (!smu_dpm->dpm_context || smu_dpm->dpm_context_size == 0)
		return -EINVAL;

	kfree(smu_dpm->dpm_context);
	smu_dpm->dpm_context = NULL;
	smu_dpm->dpm_context_size = 0;

	return 0;
}

static int smu_v11_0_init_smc_tables(struct smu_context *smu)
{
	struct smu_table_context *smu_table = &smu->smu_table;
	struct smu_table *tables = NULL;
	int ret = 0;

	if (smu_table->tables || smu_table->table_count != 0)
		return -EINVAL;

	tables = kcalloc(TABLE_COUNT, sizeof(struct smu_table), GFP_KERNEL);
	if (!tables)
		return -ENOMEM;

	smu_table->tables = tables;
	smu_table->table_count = TABLE_COUNT;

	SMU_TABLE_INIT(tables, TABLE_PPTABLE, sizeof(PPTable_t),
		       PAGE_SIZE, AMDGPU_GEM_DOMAIN_VRAM);
	SMU_TABLE_INIT(tables, TABLE_WATERMARKS, sizeof(Watermarks_t),
		       PAGE_SIZE, AMDGPU_GEM_DOMAIN_VRAM);
	SMU_TABLE_INIT(tables, TABLE_SMU_METRICS, sizeof(SmuMetrics_t),
		       PAGE_SIZE, AMDGPU_GEM_DOMAIN_VRAM);
	SMU_TABLE_INIT(tables, TABLE_OVERDRIVE, sizeof(OverDriveTable_t),
		       PAGE_SIZE, AMDGPU_GEM_DOMAIN_VRAM);

	ret = smu_v11_0_init_dpm_context(smu);
	if (ret)
		return ret;

	return 0;
}

static int smu_v11_0_fini_smc_tables(struct smu_context *smu)
{
	struct smu_table_context *smu_table = &smu->smu_table;
	int ret = 0;

	if (!smu_table->tables || smu_table->table_count == 0)
		return -EINVAL;

	kfree(smu_table->tables);
	smu_table->tables = NULL;
	smu_table->table_count = 0;

	ret = smu_v11_0_fini_dpm_context(smu);
	if (ret)
		return ret;
	return 0;

}

static int smu_v11_0_init_power(struct smu_context *smu)
{
	struct smu_power_context *smu_power = &smu->smu_power;

	if (smu_power->power_context || smu_power->power_context_size != 0)
		return -EINVAL;

	smu_power->power_context = kzalloc(sizeof(struct smu_11_0_dpm_context),
					   GFP_KERNEL);
	if (!smu_power->power_context)
		return -ENOMEM;
	smu_power->power_context_size = sizeof(struct smu_11_0_dpm_context);

	return 0;
}

static int smu_v11_0_fini_power(struct smu_context *smu)
{
	struct smu_power_context *smu_power = &smu->smu_power;

	if (!smu_power->power_context || smu_power->power_context_size == 0)
		return -EINVAL;

	kfree(smu_power->power_context);
	smu_power->power_context = NULL;
	smu_power->power_context_size = 0;

	return 0;
}

static const struct smu_funcs smu_v11_0_funcs = {
	.init_microcode = smu_v11_0_init_microcode,
	.load_microcode = smu_v11_0_load_microcode,
	.check_fw_status = smu_v11_0_check_fw_status,
	.check_fw_version = smu_v11_0_check_fw_version,
	.send_smc_msg = smu_v11_0_send_msg,
	.send_smc_msg_with_param = smu_v11_0_send_msg_with_param,
	.read_pptable_from_vbios = smu_v11_0_read_pptable_from_vbios,
	.init_smc_tables = smu_v11_0_init_smc_tables,
	.fini_smc_tables = smu_v11_0_fini_smc_tables,
	.init_power = smu_v11_0_init_power,
	.fini_power = smu_v11_0_fini_power,
};

void smu_v11_0_set_smu_funcs(struct smu_context *smu)
{
	smu->funcs = &smu_v11_0_funcs;
}

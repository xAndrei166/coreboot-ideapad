/* SPDX-License-Identifier: GPL-2.0-only */

#include <assert.h>
#include <console/console.h>
#include <fsp/api.h>
#include <soc/romstage.h>
#include <spd_bin.h>
#include <string.h>

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	const u16 rcomp_resistors[3] = { 121, 81, 100 };
	const u16 rcomp_targets[5] = { 100, 40, 20, 20, 26 };

	FSP_M_CONFIG *mem_cfg = &mupd->FspmConfig;

	uintptr_t spd_ptr = spd_cbfs_map(0);
	if (!spd_ptr) {
		printk(BIOS_ERR, "Error: spd.bin not found in CBFS!\n");
		return;
	}

	memcpy(mem_cfg->RcompResistor, rcomp_resistors, sizeof(mem_cfg->RcompResistor));
	memcpy(mem_cfg->RcompTarget, rcomp_targets, sizeof(mem_cfg->RcompTarget));

	mem_cfg->DqPinsInterleaved = 1;
	mem_cfg->CaVrefConfig = 2;
	mem_cfg->MemorySpdDataLen = CONFIG_DIMM_SPD_SIZE;
	mem_cfg->MemorySpdPtr00 = spd_ptr;
	mem_cfg->MemorySpdPtr10 = spd_ptr;
}

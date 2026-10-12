/* SPDX-License-Identifier: GPL-2.0-only */

#include <assert.h>
#include <console/console.h>
#include <fsp/api.h>
#include <soc/romstage.h>
#include <spd_bin.h>
#include <string.h>

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	/* Exact DQ Byte Map extracted from Lenovo Miix 510-12ISK factory BIOS (0x51DF28) */
	const u8 dq_map[2][12] = {
		{ 0x0F, 0xF0, 0x00, 0xF0, 0x0F, 0xF0, 0x0F, 0x00, 0xFF, 0x00, 0xFF, 0x00 },
		{ 0x0F, 0xF0, 0x00, 0xF0, 0x0F, 0xF0, 0x0F, 0x00, 0xFF, 0x00, 0xFF, 0x00 },
	};

	/* Exact DQS CPU<>DRAM Map extracted from factory BIOS (0x51DF70) */
	const u8 dqs_map[2][8] = {
		{ 1, 0, 3, 2, 5, 4, 6, 7 },
		{ 2, 3, 1, 0, 6, 4, 5, 7 },
	};

	/* RCOMP resistors & targets from factory BIOS (0x51DFD0) */
	const u16 rcomp_resistors[3] = { 200, 81, 162 };
	const u16 rcomp_targets[5]   = { 100, 40, 40, 23, 40 };

	FSP_M_CONFIG *mem_cfg = &mupd->FspmConfig;

	uintptr_t spd_ptr = spd_cbfs_map(0);
	if (!spd_ptr) {
		printk(BIOS_ERR, "Error: spd.bin not found in CBFS!\n");
		return;
	}

	memcpy(mem_cfg->DqByteMapCh0, dq_map[0], sizeof(mem_cfg->DqByteMapCh0));
	memcpy(mem_cfg->DqByteMapCh1, dq_map[1], sizeof(mem_cfg->DqByteMapCh1));
	memcpy(mem_cfg->DqsMapCpu2DramCh0, dqs_map[0], sizeof(mem_cfg->DqsMapCpu2DramCh0));
	memcpy(mem_cfg->DqsMapCpu2DramCh1, dqs_map[1], sizeof(mem_cfg->DqsMapCpu2DramCh1));
	memcpy(mem_cfg->RcompResistor, rcomp_resistors, sizeof(mem_cfg->RcompResistor));
	memcpy(mem_cfg->RcompTarget, rcomp_targets, sizeof(mem_cfg->RcompTarget));

	mem_cfg->DqPinsInterleaved = 1;
	mem_cfg->CaVrefConfig = 2;
	mem_cfg->MemorySpdDataLen = CONFIG_DIMM_SPD_SIZE;
	mem_cfg->MemorySpdPtr00 = spd_ptr;
	mem_cfg->MemorySpdPtr10 = spd_ptr;
}

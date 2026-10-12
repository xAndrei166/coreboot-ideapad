/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <fsp/api.h>
#include <gpio.h>
#include <soc/gpio.h>
#include <soc/romstage.h>
#include <spd_bin.h>
#include <string.h>

/*
 * Memory configuration follows the factory InsydeH2O SaPolicyPei OEM memory hook
 * (PE image VA 0xFFF19C15, flash offset 0x719C15):
 *
 *  - The memory ID is strapped on GPP_C12/C13/C14 (bit 0..2). IDs above 5
 *    fall back to ID 3. spd/memidN.spd.hex hold the six factory SPDs (first
 *    128 bytes as fed to the MRC, remainder zero), ordered by memory ID.
 *  - Both channels are populated with the same soldered DDR4 (Ch0/Ch1 DIMM0).
 *  - DQ byte / DQS maps are the Intel RVP values copied by the factory code;
 *    the MRC only uses them for LPDDR3.
 *  - The factory code never sets DqPinsInterleaved, so memory-down DDR4 is
 *    routed non-interleaved (NIL), as on other DDR4 memory-down SKL/KBL-U boards.
 *  - RCOMP values are the SaPolicyPei defaults (equal to the FSP defaults).
 */
#define MEMORY_ID_COUNT		6
#define MEMORY_ID_FALLBACK	3

static u8 get_memory_id(void)
{
	const gpio_t memory_id_gpios[] = { GPP_C12, GPP_C13, GPP_C14 };
	u8 id = gpio_base2_value(memory_id_gpios, ARRAY_SIZE(memory_id_gpios));

	printk(BIOS_DEBUG, "Memory ID strap: %u\n", id);
	return id < MEMORY_ID_COUNT ? id : MEMORY_ID_FALLBACK;
}

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	const u8 dq_map[2][12] = {
		{ 0x0F, 0xF0, 0x00, 0xF0, 0x0F, 0xF0, 0x0F, 0x00, 0xFF, 0x00, 0xFF, 0x00 },
		{ 0x33, 0xCC, 0x00, 0xCC, 0x33, 0xCC, 0x33, 0x00, 0xFF, 0x00, 0xFF, 0x00 },
	};
	const u8 dqs_map[2][8] = {
		{ 0, 1, 3, 2, 4, 5, 6, 7 },
		{ 1, 0, 4, 5, 2, 3, 6, 7 },
	};
	const u16 rcomp_resistors[3] = { 200, 81, 162 };
	const u16 rcomp_targets[5]   = { 100, 40, 40, 23, 40 };

	FSP_M_CONFIG *mem_cfg = &mupd->FspmConfig;

	uintptr_t spd_ptr = spd_cbfs_map(get_memory_id());
	if (!spd_ptr)
		die("SPD for memory ID not found in CBFS spd.bin!\n");

	memcpy(mem_cfg->DqByteMapCh0, dq_map[0], sizeof(mem_cfg->DqByteMapCh0));
	memcpy(mem_cfg->DqByteMapCh1, dq_map[1], sizeof(mem_cfg->DqByteMapCh1));
	memcpy(mem_cfg->DqsMapCpu2DramCh0, dqs_map[0], sizeof(mem_cfg->DqsMapCpu2DramCh0));
	memcpy(mem_cfg->DqsMapCpu2DramCh1, dqs_map[1], sizeof(mem_cfg->DqsMapCpu2DramCh1));
	memcpy(mem_cfg->RcompResistor, rcomp_resistors, sizeof(mem_cfg->RcompResistor));
	memcpy(mem_cfg->RcompTarget, rcomp_targets, sizeof(mem_cfg->RcompTarget));

	mem_cfg->DqPinsInterleaved = 0;
	mem_cfg->CaVrefConfig = 2;	/* VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4) */
	mem_cfg->MemorySpdDataLen = CONFIG_DIMM_SPD_SIZE;
	mem_cfg->MemorySpdPtr00 = spd_ptr;
	mem_cfg->MemorySpdPtr01 = 0;
	mem_cfg->MemorySpdPtr10 = spd_ptr;
	mem_cfg->MemorySpdPtr11 = 0;
}

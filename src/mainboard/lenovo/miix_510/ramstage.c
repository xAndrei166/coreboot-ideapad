/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootstate.h>
#include <device/device.h>
#include <drivers/intel/gma/int15.h>
#include <intelblocks/graphics.h>
#include <soc/pci_devs.h>
#include "gpio.h"

/*
 * The FSP GOP brings up the eDP link and framebuffer during FspSiliconInit,
 * but leaves the PCH backlight PWM off. Panel power and backlight are SoC
 * eDP pins (EDP_VDDEN / EDP_BKLTCTL per the schematic), not GPIOs, so there
 * is nothing to mux early; instead enable the panel/backlight here, once the
 * GOP has run and the IGD BAR is assigned.
 */
static void enable_panel_backlight(void *unused)
{
	struct device *igd = pcidev_path_on_root(SA_DEVFN_IGD);

	if (igd)
		graphics_soc_panel_init(igd);
}

BOOT_STATE_INIT_ENTRY(BS_DEV_ENABLE, BS_ON_EXIT, enable_panel_backlight, NULL);

static void mainboard_enable(struct device *dev)
{
	mainboard_configure_gpios();
	install_intel_vga_int15_handler(GMA_INT15_ACTIVE_LFP_EDP,
					GMA_INT15_PANEL_FIT_DEFAULT,
					GMA_INT15_BOOT_DISPLAY_DEFAULT, 0);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};

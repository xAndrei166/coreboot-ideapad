/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootstate.h>
#include <device/device.h>
#include <device/mmio.h>
#include <device/resource.h>
#include <drivers/intel/gma/int15.h>
#include <drivers/intel/gma/i915_reg.h>
#include <intelblocks/graphics.h>
#include <soc/pci_devs.h>
#include "gpio.h"

/*
 * The FSP GOP brings up the eDP link and framebuffer during FspSiliconInit,
 * but the generic panel init (graphics_soc_panel_init) leaves the panel power
 * port select at LVDS and never asserts panel power, so the panel stays dark.
 * Panel power/backlight are SoC eDP pins, not GPIOs, so there is nothing to
 * mux early; finish the sequence here once the GOP has run and the IGD BAR is
 * assigned.
 */
static void enable_panel_backlight(void *unused)
{
	struct device *igd = pcidev_path_on_root(SA_DEVFN_IGD);
	struct resource *res;
	uint8_t *base;
	u32 v;

	if (!igd)
		return;
	res = probe_resource(igd, PCI_BASE_ADDRESS_0);
	if (!res || !res->base)
		return;
	base = (void *)(uintptr_t)res->base;

	/* Panel power sequencing timing + backlight PWM. */
	graphics_soc_panel_init(igd);

	/* Assert panel power and the eDP backlight enable, deassert reset. */
	v = read32(base + PCH_PP_CONTROL);
	v |= PANEL_POWER_ON | EDP_BLC_ENABLE;
	v &= ~PANEL_POWER_RESET;
	write32(base + PCH_PP_CONTROL, v);

	printk(BIOS_DEBUG,
	       "panel: BAR0=%p PP_ON_DELAYS=%08x PP_CONTROL=%08x "
	       "PP_STATUS=%08x BLC_CTL1=%08x BLC_CTL2=%08x\n",
	       base,
	       read32(base + PCH_PP_ON_DELAYS),
	       read32(base + PCH_PP_CONTROL),
	       read32(base + PCH_PP_STATUS),
	       read32(base + BLC_PWM_PCH_CTL1),
	       read32(base + BLC_PWM_PCH_CTL2));
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

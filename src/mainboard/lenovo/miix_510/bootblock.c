/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <commonlib/helpers.h>
#include <gpio.h>
#include <soc/gpio.h>

/*
 * The rest of the GPIO table is applied in ramstage on purpose, but the UART2
 * pads must be muxed before the bootblock console and before FSP-M runs.
 * They are dedicated debug pads per the schematic, so touching them early is
 * safe.
 */
static const struct pad_config early_uart_pads[] = {
	PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1),	/* UART2_RXD */
	PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1),	/* UART2_TXD */
};

void bootblock_mainboard_early_init(void)
{
	gpio_configure_pads(early_uart_pads, ARRAY_SIZE(early_uart_pads));
}

void bootblock_mainboard_init(void)
{
	/* GPIOs are configured in ramstage to prevent disrupting S5->S0 power sequencing */
}

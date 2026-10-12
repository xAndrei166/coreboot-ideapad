/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include "gpio.h"

void bootblock_mainboard_init(void)
{
	mainboard_configure_gpios();
}

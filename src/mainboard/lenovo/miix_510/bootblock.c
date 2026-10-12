/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>

void bootblock_mainboard_init(void)
{
	/* GPIOs are configured in ramstage to prevent disrupting S5->S0 power sequencing */
}

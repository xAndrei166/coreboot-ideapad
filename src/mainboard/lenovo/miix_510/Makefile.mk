## SPDX-License-Identifier: GPL-2.0-only

subdirs-y += spd

bootblock-y += bootblock.c

romstage-y += romstage.c

ramstage-y += ramstage.c
ramstage-y += gpio.c
ramstage-y += hda_verb.c
ramstage-$(CONFIG_MAINBOARD_USE_LIBGFXINIT) += gma-mainboard.ads

IASL_WARNINGS_LIST += 3133

files_added::
	$(CBFSTOOL) $(obj)/coreboot.rom write -r EC -f src/mainboard/$(MAINBOARDDIR)/ec.bin --fill-upward

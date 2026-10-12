## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c

romstage-y += romstage.c

ramstage-y += ramstage.c
ramstage-y += gpio.c
ramstage-y += hda_verb.c
cbfs-files-y += spd.bin
spd.bin-file := src/mainboard/$(MAINBOARDDIR)/spd.bin
spd.bin-type := spd

IASL_WARNINGS_LIST += 3133

files_added::
	$(CBFSTOOL) $(obj)/coreboot.rom write -r EC -f src/mainboard/$(MAINBOARDDIR)/ec.bin --fill-upward

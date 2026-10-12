## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c
bootblock-y += gpio.c

romstage-y += romstage.c

ramstage-y += ramstage.c
ramstage-y += gpio.c
ramstage-y += hda_verb.c
cbfs-files-y += spd.bin
spd.bin-file := src/mainboard/$(MAINBOARDDIR)/spd.bin
spd.bin-type := spd

IASL_WARNINGS_LIST += 3133

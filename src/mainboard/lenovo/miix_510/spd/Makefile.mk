## SPDX-License-Identifier: GPL-2.0-only

# Indexed by memory ID strap GPP_C12 | GPP_C13 << 1 | GPP_C14 << 2
SPD_SOURCES += memid0	# 0b000  2 GiB/ch, 4Gb dies
SPD_SOURCES += memid1	# 0b001  2 GiB/ch, 4Gb dies
SPD_SOURCES += memid2	# 0b010  2 GiB/ch, 4Gb dies
SPD_SOURCES += memid3	# 0b011  4 GiB/ch, 8Gb dies (also used for IDs >= 6)
SPD_SOURCES += memid4	# 0b100  4 GiB/ch, 8Gb dies
SPD_SOURCES += memid5	# 0b101  4 GiB/ch, 8Gb dies

/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * ITE IT85xx embedded controller.
 *
 * Matches the factory ACPI namespace: same device name, _HID, _UID and I/O
 * resources as H_EC in the stock DSDT. The factory EC RAM map (operation
 * region ECF2), the _GPE assignment, the battery devices (BAT0..BAT2), LID0,
 * the _Qxx query handlers and the thermal zone are not ported yet, so the OS
 * currently has no battery, lid or hotkey support.
 *
 * Inside Scope (\_SB.PCI0.LPCB)
 */
Device (H_EC)
{
	Name (_HID, EisaId ("PNP0C09"))
	Name (_UID, 1)

	Method (_STA, 0, NotSerialized)
	{
		Return (0x0F)
	}

	Name (_CRS, ResourceTemplate ()
	{
		IO (Decode16, 0x0062, 0x0062, 0x01, 0x01)
		IO (Decode16, 0x0066, 0x0066, 0x01, 0x01)
	})

	/* Lenovo Virtual Power Controller (for ideapad_laptop kernel module) */
	Device (VPC0)
	{
		Name (_HID, "VPC2004")
		Name (_UID, 0)

		Method (_STA, 0, NotSerialized)
		{
			Return (0x0F)
		}

		Method (_CFG, 0, NotSerialized)
		{
			Return (0x100F0015)
		}
	}
}

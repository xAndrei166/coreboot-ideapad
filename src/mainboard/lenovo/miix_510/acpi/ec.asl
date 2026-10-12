/* SPDX-License-Identifier: GPL-2.0-only */

/* Inside Scope (\_SB.PCI0.LPCB) */
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

	/* Standard ACPI Embedded Controller OperationRegion */
	OperationRegion (ECOR, EmbeddedControl, 0x00, 0xFF)
	Field (ECOR, ByteAcc, Lock, Preserve)
	{
		Offset (0x80),
		MBAT, 8,
		MCUR, 16,
		MBVO, 16,
		MBRC, 16,
		MBFC, 16,
	}

	/* Lenovo Virtual Power Controller (for ideapad_laptop kernel module) */
	Device (VPC0)
	{
		Name (_HID, "VPC2004")
		Name (_UID, 0)
		Name (VPCD, 0x100F0015)

		Method (_STA, 0, NotSerialized)
		{
			Return (0x0F)
		}

		Method (_CFG, 0, NotSerialized)
		{
			Return (VPCD)
		}
	}
}

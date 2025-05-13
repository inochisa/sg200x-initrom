
#include "common.h"
#include "ddr.h"
#include "log.h"

void ddr_pinmux_init(void)
{
	switch (ddr_vendor) {
	case DDR_VENDOR_NY_4G:
		// DDR3_4G
		writel(0x12141013, DDR_BASE + 0x0000);
		writel(0x0C041503, DDR_BASE + 0x0004);
		writel(0x06050001, DDR_BASE + 0x0008);
		writel(0x08070B02, DDR_BASE + 0x000C);
		writel(0x0A0F0E09, DDR_BASE + 0x0010);
		writel(0x0016110D, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x02136574, DDR_BASE + 0x0020);
		writel(0x00000008, DDR_BASE + 0x0024);
		writel(0x76512308, DDR_BASE + 0x0028);
		writel(0x00000004, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_NY_2G:
		// DDR3_2G
		writel(0x08070D09, DDR_BASE + 0x0000);
		writel(0x0605020B, DDR_BASE + 0x0004);
		writel(0x14040100, DDR_BASE + 0x0008);
		writel(0x15030E0C, DDR_BASE + 0x000C);
		writel(0x0A0F1213, DDR_BASE + 0x0010);
		writel(0x00111016, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x82135764, DDR_BASE + 0x0020);
		writel(0x00000000, DDR_BASE + 0x0024);
		writel(0x67513028, DDR_BASE + 0x0028);
		writel(0x00000004, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ESMT_1G:
		// DDR3_1G
		writel(0x08070B09, DDR_BASE + 0x0000);
		writel(0x05000206, DDR_BASE + 0x0004);
		writel(0x0C04010D, DDR_BASE + 0x0008);
		writel(0x15030A14, DDR_BASE + 0x000C);
		writel(0x10111213, DDR_BASE + 0x0010);
		writel(0x000F160E, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x31756024, DDR_BASE + 0x0020);
		writel(0x00000008, DDR_BASE + 0x0024);
		writel(0x26473518, DDR_BASE + 0x0028);
		writel(0x00000000, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ESMT_512M_DDR2:
		// N25_DDR2_512
		writel(0x0C06080B, DDR_BASE + 0x0000);
		writel(0x070D0904, DDR_BASE + 0x0004);
		writel(0x00010502, DDR_BASE + 0x0008);
		writel(0x110A0E03, DDR_BASE + 0x000C);
		writel(0x0F141610, DDR_BASE + 0x0010);
		writel(0x00151312, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x71840532, DDR_BASE + 0x0020);
		writel(0x00000006, DDR_BASE + 0x0024);
		writel(0x76103425, DDR_BASE + 0x0028);
		writel(0x00000008, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ESMT_2G:
		// DDR3_2G
		writel(0x080B0D06, DDR_BASE + 0x0000);
		writel(0x09010407, DDR_BASE + 0x0004);
		writel(0x1405020C, DDR_BASE + 0x0008);
		writel(0x15000E03, DDR_BASE + 0x000C);
		writel(0x0A0F1213, DDR_BASE + 0x0010);
		writel(0x00111016, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x82135764, DDR_BASE + 0x0020);
		writel(0x00000000, DDR_BASE + 0x0024);
		writel(0x67513208, DDR_BASE + 0x0028);
		writel(0x00000004, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ETRON_1G:
		// ETRON_DDR3_1G
		writel(0x0B060908, DDR_BASE + 0x0000);
		writel(0x02000107, DDR_BASE + 0x0004);
		writel(0x0C05040D, DDR_BASE + 0x0008);
		writel(0x13141503, DDR_BASE + 0x000C);
		writel(0x160A1112, DDR_BASE + 0x0010);
		writel(0x000F100E, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x28137564, DDR_BASE + 0x0020);
		writel(0x00000000, DDR_BASE + 0x0024);
		writel(0x76158320, DDR_BASE + 0x0028);
		writel(0x00000004, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ESMT_N25_1G:
		// ESMT_N25_DDR3_1G
		writel(0x08060B09, DDR_BASE + 0x0000);
		writel(0x02040701, DDR_BASE + 0x0004);
		writel(0x0C00050D, DDR_BASE + 0x0008);
		writel(0x13150314, DDR_BASE + 0x000C);
		writel(0x10111216, DDR_BASE + 0x0010);
		writel(0x000F0A0E, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x82135674, DDR_BASE + 0x0020);
		writel(0x00000000, DDR_BASE + 0x0024);
		writel(0x76153280, DDR_BASE + 0x0028);
		writel(0x00000004, DDR_BASE + 0x002C);
		break;
	case DDR_VENDOR_ETRON_512M_DDR2:
		// ETRON_DDR2_512
		writel(0x070B090C, DDR_BASE + 0x0000);
		writel(0x04050608, DDR_BASE + 0x0004);
		writel(0x0E02030D, DDR_BASE + 0x0008);
		writel(0x110A0100, DDR_BASE + 0x000C);
		writel(0x0F131614, DDR_BASE + 0x0010);
		writel(0x00151012, DDR_BASE + 0x0014);
		writel(0x00000000, DDR_BASE + 0x0018);
		writel(0x00000100, DDR_BASE + 0x001C);
		writel(0x86014532, DDR_BASE + 0x0020);
		writel(0x00000007, DDR_BASE + 0x0024);
		writel(0x76012345, DDR_BASE + 0x0028);
		writel(0x00000008, DDR_BASE + 0x002C);
		break;
	}

	INFO("DDR pinmux init finish");
}

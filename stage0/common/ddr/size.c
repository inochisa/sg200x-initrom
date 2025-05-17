
#include "common.h"
#include "ddr.h"
#include "bits.h"

void ddr_init_detect_dram_size(uint8_t *dram_cap)
{
	uint8_t cap_in_mbyte = 0;
	uint32_t cmd[6];
	uint32_t val;

	if (ddr_type == DDR_TYPE_DDR3) {
		// dram_cap = 4;
		cap_in_mbyte = 4;

		for (unsigned int i = 0; i < 6; i++)
			cmd[i] = 0x0;

		// Axsize = 3, axlen = 4, cgen
		writel(0x000e0006, DDR_BIST_BASE + 0x0);

		// DDR space
		writel(0x00000000, DDR_BIST_BASE + 0x10);
		writel(0xffffffff, DDR_BIST_BASE + 0x14);

		// specified AXI address step
		writel(0x00000004, DDR_BIST_BASE + 0x18);

		// write PRBS to 0x0 as background {{{

		// write 16 UI prbs
		cmd[0] = (1 << 30) | (0 << 21) | (3 << 12) |
			 (5 << 9) | (0 << 8) | (0 << 0);

		// write cmd queue
		for (unsigned int i = 0; i < 6; i++)
			writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

		// bist_enable
		writel(0x00010001, DDR_BIST_BASE + 0x0);

		// polling BIST done
		while ((readl(DDR_BIST_BASE + 0x80) & BIT(2)) == 0)
			;
		val = readl(DDR_BIST_BASE + 0x80);

		// bist disable
		writel(0x00010000, DDR_BIST_BASE + 0x0);
		// }}}

		do {
			// *dram_cap++;
			cap_in_mbyte++;

			// write ~PRBS to (0x1 << *dram_cap) {{{

			// DDR space
			writel(1 << (cap_in_mbyte + 20 - 4), DDR_BIST_BASE + 0x10);

			cmd[0] = (1 << 30) + (0 << 21) + (3 << 12) + (5 << 9) + (1 << 8) + (0 << 0);//write 16 UI~prbs

			// write cmd queue
			for (unsigned int i = 0; i < 6; i++)
				writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

			// bist_enable
			writel(0x00010001, DDR_BIST_BASE + 0x0);

			// polling BIST done
			while ((readl(DDR_BIST_BASE + 0x80) & BIT(2)) == 0)
				;
			val = readl(DDR_BIST_BASE + 0x80);

			// bist disable
			writel(0x00010000, DDR_BIST_BASE + 0x0);
			// }}}

			// check PRBS at 0x0 {{{

			// DDR space
			writel(0x00000000, DDR_BIST_BASE + 0x10);
			cmd[0] = (2 << 30) + (0 << 21) + (3 << 12) + (5 << 9) + (0 << 8) + (0 << 0); // read 16 UI prbs

			// write cmd queue
			for (unsigned int i = 0; i < 6; i++)
				writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

			// bist_enable
			writel(0x00010001, DDR_BIST_BASE + 0x0);

			// polling BIST done
			while ((readl(DDR_BIST_BASE + 0x80) & BIT(2)) == 0)
				;
			val = readl(DDR_BIST_BASE + 0x80);

			// bist disable
			writel(0x00010000, DDR_BIST_BASE + 0x0);
			// }}}

		} while ((FIELD_GET(BIT(3), val) == 0) && (cap_in_mbyte < 15)); // BIST fail stop the loop
	} else if (ddr_type == DDR_TYPE_DDR2)
		// fix size for DDR2
		cap_in_mbyte = 6;

	*dram_cap = cap_in_mbyte;

	// save dram_cap
	val = cap_in_mbyte;
	writel(val, 0x0208 + DDR_BASE);

	// cgen disable
	writel(0x00040000, DDR_BIST_BASE + 0x0);

}

void ddr_init_update_by_dram_size(uint8_t dram_cap)
{
	uint32_t val;
	uint8_t dram_cap_in_mbyte_per_dev;

	val = readl(0x08004000 + 0x0);
	dram_cap_in_mbyte_per_dev = dram_cap;
	// change sys cap to x16 cap
	dram_cap_in_mbyte_per_dev >>= (1 - FIELD_GET(GENMASK(13, 12), val));
	// change x16 cap to device cap
	dram_cap_in_mbyte_per_dev >>= (2 - FIELD_GET(GENMASK(31, 30), val));

	switch (ddr_rate) {
	case 1333:
		switch (dram_cap_in_mbyte_per_dev) {
		case 5:
			writel(0x00510019, 0x08004000 + 0x64);
			writel(0x0B011610, 0x08004000 + 0x100);
			writel(0x00000502, 0x08004000 + 0x120);
			break;
		case 6:
			writel(0x0051002C, 0x08004000 + 0x64);
			writel(0x0B011610, 0x08004000 + 0x100);
			writel(0x00000503, 0x08004000 + 0x120);
			break;
		case 7:
			writel(0x0051002B, 0x08004000 + 0x64);
			writel(0x0B0F1610, 0x08004000 + 0x100);
			writel(0x00000503, 0x08004000 + 0x120);
			break;
		case 8:
			writel(0x00510041, 0x08004000 + 0x64);
			writel(0x0B0F1610, 0x08004000 + 0x100);
			writel(0x00000504, 0x08004000 + 0x120);
			break;
		case 9:
			writel(0x0051006E, 0x08004000 + 0x64);
			writel(0x0B0F1610, 0x08004000 + 0x100);
			writel(0x00000505, 0x08004000 + 0x120);
			break;
		}

		switch (dram_cap_in_mbyte_per_dev) {
		case 5:
			writel(0x00001F1F, 0x08004000 + 0x200);
			writel(0x003F0606, 0x08004000 + 0x204);
			writel(0x00000000, 0x08004000 + 0x208);
			writel(0x1F1F0000, 0x08004000 + 0x20c);
			writel(0x00001F1F, 0x08004000 + 0x210);
			writel(0x040F0404, 0x08004000 + 0x214);
			writel(0x04040404, 0x08004000 + 0x218);
			writel(0x00000404, 0x08004000 + 0x21c);
			writel(0x00003F3F, 0x08004000 + 0x220);
			writel(0x04040404, 0x08004000 + 0x224);
			writel(0x04040404, 0x08004000 + 0x228);
			writel(0x001F1F04, 0x08004000 + 0x22c);
			break;
		case 6:
			writel(0x00001F1F, 0x08004000 + 0x200);
			writel(0x003F0707, 0x08004000 + 0x204);
			writel(0x00000000, 0x08004000 + 0x208);
			writel(0x1F000000, 0x08004000 + 0x20c);
			writel(0x00001F1F, 0x08004000 + 0x210);
			writel(0x050F0505, 0x08004000 + 0x214);
			writel(0x05050505, 0x08004000 + 0x218);
			writel(0x00000505, 0x08004000 + 0x21c);
			writel(0x00003F3F, 0x08004000 + 0x220);
			writel(0x05050505, 0x08004000 + 0x224);
			writel(0x05050505, 0x08004000 + 0x228);
			writel(0x001F1F05, 0x08004000 + 0x22c);
			break;
		case 7:
		case 8:
		case 9:
			writel(0x00001F1F, 0x08004000 + 0x200);
			writel(0x00070707, 0x08004000 + 0x204);
			writel(0x00000000, 0x08004000 + 0x208);
			writel(0x1F000000, 0x08004000 + 0x20c);
			writel(0x00001F1F, 0x08004000 + 0x210);
			writel(0x060F0606, 0x08004000 + 0x214);
			writel(0x06060606, 0x08004000 + 0x218);
			writel(0x00000606, 0x08004000 + 0x21c);
			writel(0x00003F3F, 0x08004000 + 0x220);
			writel(0x06060606, 0x08004000 + 0x224);
			writel(0x06060606, 0x08004000 + 0x228);
			writel(0x001F1F06, 0x08004000 + 0x22c);
			break;
		}
	case 1866:
		switch (dram_cap_in_mbyte_per_dev) {
		case 6:
			writel(0x0071002A, 0x08004000 + 0x64);
			writel(0x00000903, 0x08004000 + 0x120);
			break;
		case 7:
			writel(0x00710034, 0x08004000 + 0x64);
			writel(0x00000903, 0x08004000 + 0x120);
			break;
		case 8:
			writel(0x0071004B, 0x08004000 + 0x64);
			writel(0x00000904, 0x08004000 + 0x120);
			break;
		case 9:
			writel(0x0071007A, 0x08004000 + 0x64);
			writel(0x00000905, 0x08004000 + 0x120);
			break;
		case 10:
			writel(0x007100A4, 0x08004000 + 0x64);
			writel(0x00000907, 0x08004000 + 0x120);
			break;
		}
		break;
	case 2133:
		switch (dram_cap_in_mbyte_per_dev) {
		case 6:
			writel(0x00820030, 0x08004000 + 0x64);
			writel(0x00000903, 0x08004000 + 0x120);
			break;
		case 7:
			writel(0x0082003B, 0x08004000 + 0x64);
			writel(0x00000903, 0x08004000 + 0x120);
			break;
		case 8:
			writel(0x00820056, 0x08004000 + 0x64);
			writel(0x00000904, 0x08004000 + 0x120);
			break;
		case 9:
			writel(0x0082008B, 0x08004000 + 0x64);
			writel(0x00000906, 0x08004000 + 0x120);
			break;
		case 10:
			writel(0x008200BB, 0x08004000 + 0x64);
			writel(0x00000907, 0x08004000 + 0x120);
			break;
		}
		break;
	}

	// toggle refresh_update_level
	writel(0x00000002, 0x08004000 + 0x60);
	writel(0x00000000, 0x08004000 + 0x60);
}

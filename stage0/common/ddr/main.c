
#include "common.h"
#include "ddr.h"

extern const struct ddr_patch_reg ddrc_patch[];
extern const unsigned int ddrc_patch_size;

static void ddrc_init(void)
{
	ddr_update_patch_reg(ddrc_patch, ddrc_patch_size);

	writel(0, DDR_TOP_BASE + 0x20);
}

extern const struct ddr_patch_reg ddr_phy_patch[];
extern const unsigned int ddr_phy_patch_size;

static void ddr_phy_init(void)
{
	ddr_update_patch_reg(ddr_phy_patch, ddr_phy_patch_size);

	writel(0x02620504, DDR_BASE + 0xa4);
}

void ddr_init(void)
{
	ddr_pll_init();
	ddrc_init();

	// set axi QOS
	// M1 = 0xA (VIP realtime)
	// M2 = 0x8 (VIP offline)
	// M3 = 0x7 (CPU)
	// M4 = 0x0 (TPU)
	// M5 = 0x9 (Video codec)
	// M6 = 0x2 (high speed peri)
	writel(0x007788aa, TOP_SYSCON_BASE + 0x1d8);
	writel(0x00002299, TOP_SYSCON_BASE + 0x1dc);

	ddr_phy_init();

	// TODO: check setting

	ddr_pinmux_init();

	// TODO: load patch reg
}

void ddr_update_patch_reg(const struct ddr_patch_reg *regs, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; ++i) {
		const struct ddr_patch_reg *reg = &regs[i];
		writel(reg->val, (unsigned long)reg->addr);
	}
}

void ddr_update_mask_patch_reg(const struct ddr_mask_patch_reg *regs, unsigned int size)
{
	unsigned int i;
	uint32_t val;

	for (i = 0; i < size; ++i) {
		const struct ddr_mask_patch_reg *reg = &regs[i];
		unsigned long addr = reg->addr;
		val = readl(addr);
		val &= reg->mask;
		val |= reg->val;
		writel(val, addr);
	}
}

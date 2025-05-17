
#include "common.h"
#include "ddr.h"
#include "log.h"

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

extern const struct ddr_mask_patch_reg ddr_patch_regs[];
extern const unsigned int ddr_patch_size;

static void ddr_set_patch(void)
{
	ddr_update_mask_patch_reg(ddr_patch_regs, ddr_patch_size);
}

extern const struct ddr_patch_reg ddr_low_patch_regs[];
extern const unsigned int ddr_low_patch_size;

static void ddr_set_low_patch(void)
{
	ddr_update_patch_reg(ddr_low_patch_regs, ddr_low_patch_size);
}

extern const struct ddr_patch_reg ddr_high_patch_regs[];
extern const unsigned int ddr_high_patch_size;

static void ddr_set_high_patch(void)
{
	ddr_update_patch_reg(ddr_high_patch_regs, ddr_high_patch_size);
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

	ddr_setting_check();

	ddr_pinmux_init();

	ddr_set_patch();

	ddr_set_dfi_init_start();
	ddr_phy_power_on_seq1();
	ddr_polling_dfi_init_start();
	ddr_int_isr_08();
	ddr_phy_power_on_seq2();
	ddr_set_dfi_init_complete();
	ddr_phy_power_on_seq3();
	ddr_polling_dfi_init_complete();
	ddr_polling_sync_normal_mode();

	ddr_do_bist_prbs_check();

	INFO("TODO\n");

	ddr_set_low_patch();

	if (ddr_type == DDR_TYPE_DDR3) {
		// cvx16_wrlvl_req();
	}

	ddr_do_bist_prbs_check();
	// TODO: cvx16_rdglvl_req
	ddr_do_bist_prbs_check();
	// TODO: cvx16_wdqlvl_req 1 2
	// TODO: cvx16_wdqlvl_req 1 1
	// TODO: cvx16_wdqlvl_req 1 0
	ddr_do_bist_prbs_check();
	// TODO: misc update
	// TODO: cvx16_rdlvl_req 1
	ddr_do_bist_prbs_check();
	ddr_set_high_patch();
	// TODO: ctrl_init_detect_dram_size
	// TODO: ctrl_init_update_by_dram_size
	// TODO: cvx16_dram_cap_check
	// TODO: cvx16_clk_gating_enable
	ddr_do_bist_prbs_check();
	// TODO: ddr_do_bist_sram_check
	// TODO: axi_mon_latency_setting
	// TODO: axi_mon_start_all
}

void ddr_update_patch_reg(const struct ddr_patch_reg *regs, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; ++i) {
		const struct ddr_patch_reg *reg = &regs[i];

		DEBUG("DDR: update 0x%08lx: val 0x%08x\n", addr, reg->val);

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

		DEBUG("DDR: update 0x%08lx: val 0x%08x, mask 0x%08x\n",
		      addr, reg->val, reg->mask);

		val = readl(addr);
		val &= ~reg->mask;
		val |= reg->val & reg->mask;
		writel(val, addr);
	}
}

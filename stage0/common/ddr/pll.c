
#include "common.h"
#include "bits.h"
#include "ddr.h"
#include "soc/clk.h"
#include "log.h"

#define DDR_FREQ_IN		752
#define DDR_MOD_FREQ		100
#define DDR_DEV_FREQ		15

#define DDR_RATE_SHIFT		(1 << 4)

#define DDR_CLK_SPAN_MASK	GENMASK(15, 0)
#define DDR_CLK_STEP_MASK	GENMASK(23, 0)

#define DDR_SSC_EN		BIT(1)
#define DDR_SSC_MODE		GENMASK(3, 2)
#define DDR_SSC_BYPASS		BIT(4)
#define DDR_SSC_EXTPULSE	BIT(5)
#define DDR_SSC_FIXDIV		BIT(6)

uint32_t pll_set;
uint32_t pll_span;
uint32_t pll_step;

static void ddr_clk_init_freq(void)
{
	uint32_t val;

	//==============================================================
	// SSC_EN =0
	//==============================================================
	writel(pll_set, CLK_BASE + REG_DPLL_SSC_SYN_SET);
	writel(FIELD_PREP(DDR_CLK_SPAN_MASK, pll_span), CLK_BASE + REG_DPLL_SSC_SYN_SPAN);
	writel(FIELD_PREP(DDR_CLK_STEP_MASK, pll_step), CLK_BASE + REG_DPLL_SSC_SYN_STEP);

	val = readl(CLK_BASE + REG_DPLL_SSC_SYN_CTRL);
	val &= ~(DDR_SSC_EN | DDR_SSC_MODE | DDR_SSC_BYPASS | DDR_SSC_FIXDIV);
	val |= DDR_SSC_EXTPULSE;
	writel(val, CLK_BASE + REG_DPLL_SSC_SYN_CTRL);


	INFO("DDR: set PLL as normal frequency\n");
}

void ddr_pll_init(void)
{
	uint32_t val;

	// TODO: SSC_EN support
	/*
	 * target = ddr_rate >> 4
	 * SET = DDR_FREQ_IN << 25 / target
	 * SPAN = target * 250 / DDR_MOD_FREQ
	 * step = SET * DDR_DEV_FREQ / SPAN * 1000
	 */
	pll_set = (((uint64_t)DDR_FREQ_IN << 25) * DDR_RATE_SHIFT) / ddr_rate;
	pll_span = ddr_rate * 250 / (DDR_RATE_SHIFT * DDR_MOD_FREQ);
	// step =
	pll_step = ((uint64_t)DDR_FREQ_IN << 25) * DDR_RATE_SHIFT * DDR_RATE_SHIFT /
			(uint64_t)250000 * ddr_rate * ddr_rate;

	// TX_VREF_PD
	writel(0x00000000, DDR_PHYD_BASE + 0x28);
	// ZQ_240 OPTION
	writel(0x00080001, DDR_PHYD_BASE + 0x54);

	if (ddr_type == DDR_TYPE_DDR3) {
		// TOP_REG_TX_DDR3_GPO_IN =1
		writel(0x01010808, DDR_PHYD_BASE + 0x58);
	}

	ddr_clk_init_freq();

	val = readl(DDR_PHYD_BASE + 0x0c);
	// [0]     = 1;      // TOP_REG_DDRPLL_EN_DLLCLK
	// [1]     = 1;      // TOP_REG_DDRPLL_EN_LCKDET
	// [2]     = 0;      // TOP_REG_DDRPLL_EN_TST
	// [5:3]   = 0b001;  // TOP_REG_DDRPLL_ICTRL
	// [6]     = 0;      // TOP_REG_DDRPLL_MAS_DIV_SEL
	// [7]     = 0;      // TOP_REG_DDRPLL_MAS_RSTZ_DIV
	// [8]     = 1;      // TOP_REG_DDRPLL_SEL_4BIT
	// [10:9]  = 0b01;   // TOP_REG_DDRPLL_SEL_MODE
	// [12:11] = 0b00;   // Rev
	// [13]    = 0;      // TOP_REG_DDRPLL_SEL_LOW_SPEED
	// [14]    = 0;      // TOP_REG_DDRPLL_MAS_DIV_OUT_SEL
	// [15]    = 0;      // TOP_REG_DDRPLL_PD
	val &= ~GENMASK(15, 0);
	val |= 0x030b;
	writel(val, DDR_PHYD_BASE + 0x0c);

	val = readl(DDR_PHYD_BASE + 0x10);
	// [7:0]   = 0x0;    // TOP_REG_DDRPLL_TEST
	val &= ~GENMASK(7, 0);
	writel(val, DDR_PHYD_BASE + 0x10);

	// [0]     = 1;      // TOP_REG_RESETZ_DIV
	writel(BIT(0), DDR_PHYD_BASE + 0x04);

	val = readl(DDR_PHYD_BASE + 0x0c);
	// [7]     = 0;      // TOP_REG_DDRPLL_MAS_RSTZ_DIV
	val |= BIT(7);
	writel(val, DDR_PHYD_BASE + 0x0c);

	INFO("Polling DDR PLL lock...\n");

	while (!(readl(DDR_PHYD_BASE + 0x10) & BIT(15)))
	{ }

	INFO("DDR PLL init finish.\n");
}

void ddr_clk_set_div2(void)
{
	uint32_t val;

	val = readl(DDR_PHYD_BASE + 0x0c);
	// val[14] = 1  ;  // TOP_REG_DDRPLL_MAS_DIV_OUT_SEL 1
	val |= BIT(14);
	writel(val, DDR_PHYD_BASE + 0x0c);

	INFO("DDR: set PLL as D2 frequency\n");
}

void ddr_clk_set_div40(void)
{
	uint32_t val;

	val = readl(DDR_PHYD_BASE + 0x0c);
	// TOP_REG_DDRPLL_SEL_LOW_SPEED =1
	val |= BIT(13);
	writel(val, DDR_PHYD_BASE + 0x0c);

	INFO("DDR: set PLL as low D40 frequency\n");
}

void ddr_clk_set_normal(void)
{
	uint32_t val;

	val = readl(DDR_PHYD_BASE + 0x0c);
	// val[13] TOP_REG_DDRPLL_SEL_LOW_SPEED 0
	// val[14] TOP_REG_DDRPLL_MAS_DIV_OUT_SEL 0
	val &= ~(BIT(13) | BIT(14));
	writel(val, DDR_PHYD_BASE + 0x0c);

	ddr_clk_init_freq();
}

#define DDR_PLL_EN_CHG			BIT(0)
#define DDR_CUR_PLL_SPEED_MASK		GENMASK(5, 4)
#define DDR_NEXT_PLL_SPEED_MASK		GENMASK(9, 8)

void ddr_adjust_pll_freq(void)
{
	uint32_t val;
	uint32_t EN_PLL_SPEED_CHG;
	uint32_t CUR_PLL_SPEED;
	uint32_t NEXT_PLL_SPEED;

	// Change PLL frequency
	// TOP_REG_RESETZ_DIV =0
	writel(0x00000000, DDR_PHYD_BASE + 0x04);
	// TOP_REG_RESETZ_DQS =0
	writel(0x00000000, DDR_PHYD_BASE + 0x08);

	// TOP_REG_DDRPLL_MAS_RSTZ_DIV  =0
	val = readl(DDR_PHYD_BASE + 0x0c);
	val &= ~BIT(7);
	writel(val, DDR_PHYD_BASE + 0x0c);

	// RSTZ_DIV = 0
	// val = readl(DDR_PHYD_BASE + 0x4c);
	// val = readl(DDR_PHYD_BASE + 0x4c);
	// val = readl(DDR_PHYD_BASE + 0x4c);
	// val = readl(DDR_PHYD_BASE + 0x4c);
	// val = readl(DDR_PHYD_BASE + 0x4c);
	val = readl(DDR_PHYD_BASE + 0x4c);
	EN_PLL_SPEED_CHG = FIELD_GET(DDR_PLL_EN_CHG, val);
	CUR_PLL_SPEED = FIELD_GET(DDR_CUR_PLL_SPEED_MASK, val);
	NEXT_PLL_SPEED = FIELD_GET(DDR_NEXT_PLL_SPEED_MASK, val);

	if (EN_PLL_SPEED_CHG) {
		INFO("DDR: next pll speed %u\n", NEXT_PLL_SPEED);
		if (NEXT_PLL_SPEED == 0) { // next clk_div40
			FIELD_MOD(val, DDR_NEXT_PLL_SPEED_MASK, CUR_PLL_SPEED);
			FIELD_MOD(val, DDR_CUR_PLL_SPEED_MASK, NEXT_PLL_SPEED);
			writel(val, DDR_PHYD_BASE + 0x4c);

			ddr_clk_set_div40();
		} else if (NEXT_PLL_SPEED == 0x2) { // next clk normal
			FIELD_MOD(val, DDR_NEXT_PLL_SPEED_MASK, CUR_PLL_SPEED);
			FIELD_MOD(val, DDR_CUR_PLL_SPEED_MASK, NEXT_PLL_SPEED);
			writel(val, DDR_PHYD_BASE + 0x4c);

			ddr_clk_set_normal();
		} else if (NEXT_PLL_SPEED == 0x1) { // next clk normal div_2
			FIELD_MOD(val, DDR_NEXT_PLL_SPEED_MASK, CUR_PLL_SPEED);
			FIELD_MOD(val, DDR_CUR_PLL_SPEED_MASK, NEXT_PLL_SPEED);
			writel(val, DDR_PHYD_BASE + 0x4c);

			ddr_clk_set_div2();
		}

		udelay(100);
	}
	// TOP_REG_RESETZ_DIV  =1
	writel(0x00000001, DDR_PHYD_BASE + 0x04);

	val = readl(DDR_PHYD_BASE + 0x0c);
	// val[7]   = 1;    //TOP_REG_DDRPLL_MAS_RSTZ_DIV
	val |= BIT(7);
	writel(val, DDR_PHYD_BASE + 0x0c);

	// val[0]   = 1;    //TOP_REG_RESETZ_DQS
	writel(0x00000001, DDR_PHYD_BASE + 0x08);

	do {
		udelay(1);
	} while ((readl(DDR_PHYD_BASE + 0x10) & BIT(15)) == 0);
}

void ddr_dll_cal(void)
{
	uint32_t val;
	uint32_t EN_PLL_SPEED_CHG;
	uint32_t CUR_PLL_SPEED;
	uint32_t NEXT_PLL_SPEED;

	// TOP_REG_EN_PLL_SPEED_CHG
	//     <= #RD (~pwstrb_mask[0] & TOP_REG_EN_PLL_SPEED_CHG) |  pwstrb_mask_pwdata[0];
	// TOP_REG_CUR_PLL_SPEED   [1:0]
	//     <= #RD (~pwstrb_mask[5:4] & TOP_REG_CUR_PLL_SPEED[1:0]) |  pwstrb_mask_pwdata[5:4];
	// TOP_REG_NEXT_PLL_SPEED  [1:0]
	//     <= #RD (~pwstrb_mask[9:8] & TOP_REG_NEXT_PLL_SPEED[1:0]) |  pwstrb_mask_pwdata[9:8];
	val = readl(DDR_PHYD_BASE + 0x4c);
	EN_PLL_SPEED_CHG = FIELD_GET(DDR_PLL_EN_CHG, val);
	CUR_PLL_SPEED = FIELD_GET(DDR_CUR_PLL_SPEED_MASK, val);
	NEXT_PLL_SPEED = FIELD_GET(DDR_NEXT_PLL_SPEED_MASK, val);

	// param_phyd_dll_rx_start_cal <= int_regin[1];
	// param_phyd_dll_tx_start_cal <= int_regin[17];
	val = readl(DDR_BASE + 0x0040);
	val &= ~BIT(1);
	val &= ~BIT(17);
	writel(val, DDR_BASE + 0x0040);

	if (CUR_PLL_SPEED != 0) { // only do calibration and update when high speed
		// param_phyd_dll_rx_start_cal <= int_regin[1];
		// param_phyd_dll_tx_start_cal <= int_regin[17];
		val = readl(DDR_BASE + 0x0040);
		val |= BIT(1);
		val |= BIT(17);
		writel(val, DDR_BASE + 0x0040);

		val = 0x00000000;

		while ((readl(DDR_BASE + 0x3014) & BIT(16)) == 0)
			;
	}

	INFO("DDR: DDLCAL is OK\n");
}

void ddr_clk_disable_gating(void)
{
	uint32_t val;

	// TOP_REG_CG_EN_PHYD_TOP      0
	// TOP_REG_CG_EN_CALVL         1
	// TOP_REG_CG_EN_WRLVL         2
	// N/A                         3
	// TOP_REG_CG_EN_WRDQ          4
	// TOP_REG_CG_EN_RDDQ          5
	// TOP_REG_CG_EN_PIGTLVL       6
	// TOP_REG_CG_EN_RGTRACK       7
	// TOP_REG_CG_EN_DQSOSC        8
	// TOP_REG_CG_EN_LB            9
	// TOP_REG_CG_EN_DLL_SLAVE     10 //0:a-on
	// TOP_REG_CG_EN_DLL_MST       11 //0:a-on
	// TOP_REG_CG_EN_ZQ            12
	// TOP_REG_CG_EN_PHY_PARAM     13 //0:a-on
	// 0b01001011110101
	writel(0x000012F5, DDR_PHYD_BASE + 0x44);
	// PHYD_SHIFT_GATING_EN
	writel(0x00000000, DDR_BASE + 0x00F4);

	val = readl(DDR_CTRL_BASE + 0x30); // phyd_stop_clk
	val &= ~BIT(9);
	writel(val, DDR_CTRL_BASE + 0x30);

	val = readl(DDR_CTRL_BASE + 0x148); // dfi read/write clock gatting
	val &= ~BIT(23);
	val &= ~BIT(31);
	writel(val, DDR_CTRL_BASE + 0x148);

	INFO("DDR: disable clk gating\n");
}

void ddr_clk_enable_gating(void)
{
	uint32_t val;
	// TOP_REG_CG_EN_PHYD_TOP      0
	// TOP_REG_CG_EN_CALVL         1
	// TOP_REG_CG_EN_WRLVL         2
	// N/A                         3
	// TOP_REG_CG_EN_WRDQ          4
	// TOP_REG_CG_EN_RDDQ          5
	// TOP_REG_CG_EN_PIGTLVL       6
	// TOP_REG_CG_EN_RGTRACK       7
	// TOP_REG_CG_EN_DQSOSC        8
	// TOP_REG_CG_EN_LB            9
	// TOP_REG_CG_EN_DLL_SLAVE     10 //0:a-on
	// TOP_REG_CG_EN_DLL_MST       11 //0:a-on
	// TOP_REG_CG_EN_ZQ            12
	// TOP_REG_CG_EN_PHY_PARAM     13 //0:a-on
	// 0b10110010000001
	writel(0x00002C81, DDR_PHYD_BASE + 0x44);

	//    #ifdef _mem_freq_1333
	//    #ifdef DDR2
	val = readl(DDR_CTRL_BASE + 0x190);
	FIELD_MOD(val, GENMASK(28, 24), 6);
	writel(val, DDR_CTRL_BASE + 0x190);
	//    #endif

	// PHYD_SHIFT_GATING_EN
	writel(0x00030033, DDR_BASE + 0x00f4);

	val = readl(DDR_CTRL_BASE + 0x30); // phyd_stop_clk
	val |= BIT(9);
	writel(val, DDR_CTRL_BASE + 0x30);

	val = readl(DDR_CTRL_BASE + 0x148); // dfi read/write clock gatting
	val |= BIT(23);
	val |= BIT(31);
	writel(val, DDR_CTRL_BASE + 0x148);

	INFO("DDR: enable clk gating\n");
}

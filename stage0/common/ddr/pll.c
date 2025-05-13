
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

void ddr_pll_init(void)
{
	// TODO: SSC_EN support
	/*
	 * target = ddr_rate >> 4
	 * SET = DDR_FREQ_IN << 25 / target
	 * SPAN = target * 250 / DDR_MOD_FREQ
	 * step = SET * DDR_DEV_FREQ / SPAN * 1000
	 */
	uint32_t set = (((uint64_t)DDR_FREQ_IN << 25) * DDR_RATE_SHIFT) / ddr_rate;
	uint32_t span = ddr_rate * 250 / (DDR_RATE_SHIFT * DDR_MOD_FREQ);
	// step =
	uint32_t step = ((uint64_t)DDR_FREQ_IN << 25) * DDR_RATE_SHIFT * DDR_RATE_SHIFT /
			(uint64_t)250000 * ddr_rate * ddr_rate;
	uint32_t val;

	// TX_VREF_PD
	writel(0x00000000, DDR_PHYD_BASE + 0x28);
	// ZQ_240 OPTION
	writel(0x00080001, DDR_PHYD_BASE + 0x54);

	if (ddr_type == DDR_TYPE_DDR3) {
		// TOP_REG_TX_DDR3_GPO_IN =1
		writel(0x01010808, DDR_PHYD_BASE + 0x58);
	}

	writel(set, CLK_BASE + REG_DPLL_SSC_SYN_SET);
	writel(FIELD_PREP(DDR_CLK_SPAN_MASK, span), CLK_BASE + REG_DPLL_SSC_SYN_SPAN);
	writel(FIELD_PREP(DDR_CLK_STEP_MASK, step), CLK_BASE + REG_DPLL_SSC_SYN_STEP);

	val = readl(CLK_BASE + REG_DPLL_SSC_SYN_CTRL);
	val &= ~(DDR_SSC_EN | DDR_SSC_MODE | DDR_SSC_BYPASS | DDR_SSC_FIXDIV);
	val |= DDR_SSC_EXTPULSE;
	writel(val, CLK_BASE + REG_DPLL_SSC_SYN_CTRL);

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


#include "common.h"
#include "bits.h"
#include "log.h"
#include "soc/rtc.h"

void rtc_init_stage0(void)
{
	uint32_t val;

	INFO("RTC ctrl version 0x%x\n", readl(RTC_BASE + RTC_CTRL_VERSION));

	val = readl(RTC_BASE + RTC_CTRL0);
	if (FIELD_GET(RTC_CTRL0_RTC_SOURCE, val) == 1) {
		WARN("RTC clock source is already processed\n");
		goto end;
	}

	writel(RTC_CTRL0_UNLOCK_KEK, RTC_BASE + RTC_CTRL0_UNLOCK);
	val = readl(RTC_BASE + RTC_CTRL0);
	val = 0x08000000 | (val & ~RTC_CTRL0_CLK32K_EN);
	writel(val, RTC_BASE + RTC_CTRL0);

	do {
		val = readl(RTC_BASE + RTC_CTRL0_STATUS0);
	} while (val & RTC_CTRL0_STATUS_CLK32K_OUT);

	val = readl(RTC_BASE + RTC_CTRL0);
	val = 0x04000000 | val | RTC_CTRL0_RTC_SOURCE;
	writel(val, RTC_BASE + RTC_CTRL0);

	writel(1, RTC_BASE + RTC_MACRO_DA_SOC_READY);
	writel(0, RTC_BASE + RTC_MACRO_DA_SOC_READY);

	udelay(200);

	val = readl(RTC_BASE + RTC_CTRL0);
	val = 0x0c000000 | val | RTC_CTRL0_CLK32K_EN;
	writel(val, RTC_BASE + RTC_CTRL0);

end:
	writel(0x1, RTC_BASE + RTC_CORE_EN_SHDN_REQ);
	while (readl(RTC_BASE + RTC_CORE_EN_SHDN_REQ) != 0x1)
	{ }

	writel(0x1, RTC_BASE + RTC_CORE_EN_WARM_RST_REQ);
	while (readl(RTC_BASE + RTC_CORE_EN_WARM_RST_REQ) != 0x1)
	{ }

	writel(0x1, RTC_BASE + RTC_CORE_EN_PWR_CYC_REQ);
	while (readl(RTC_BASE + RTC_CORE_EN_PWR_CYC_REQ) != 0x1)
	{ }

	writel(0x1, RTC_BASE + RTC_CORE_EN_WDG_RST_REQ);
	while (readl(RTC_BASE + RTC_CORE_EN_WDG_RST_REQ) != 0x1)
	{ }

	writel(0x1, RTC_BASE + RTC_POR_RST_CTRL);

	writel(RTC_CTRL0_UNLOCK_KEK, RTC_BASE + RTC_CTRL0_UNLOCK);
	val = readl(RTC_BASE + RTC_CTRL0);
	val = 0xffff0000 | val | RTC_CTRL0_CLK32K_EN | RTC_CTRL0_HW_WDG_RST_EN;
	writel(val, RTC_BASE + RTC_CTRL0);

	val = readl(RTC_BASE + RTC_CORE_EN_PWR_VBAT_DET);
	val &= ~RTC_CORE_EN_AUTO_POWER_UP;
	writel(val, RTC_BASE + RTC_CORE_EN_PWR_VBAT_DET);

	INFO("RTC is at stage0 state\n");
	INFO("MCU ctrl0: 0x%08x, ctrl1: 0x%08x\n",
	     readl(RTC_BASE + RTC_MCU_CTRL0), readl(RTC_BASE + RTC_MCU_CTRL1));
}

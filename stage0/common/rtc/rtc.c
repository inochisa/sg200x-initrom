
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
		WARN("RTC is already inited\n");
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
	INFO("RTC is at stage0 state\n");
	INFO("MCU ctrl0: 0x%08x, ctrl1: 0x%08x\n",
	     readl(RTC_BASE + RTC_MCU_CTRL0), readl(RTC_BASE + RTC_MCU_CTRL1));
}

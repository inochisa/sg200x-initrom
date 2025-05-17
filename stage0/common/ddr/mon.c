
#include "common.h"
#include "ddr.h"
#include "bits.h"

#define AXIMON_M1_WRITE			0x000
#define AXIMON_M1_READ			0x080
#define AXIMON_M2_WRITE			0x100
#define AXIMON_M2_READ			0x180
#define AXIMON_M3_WRITE			0x200
#define AXIMON_M3_READ			0x280
#define AXIMON_M4_WRITE			0x300
#define AXIMON_M4_READ			0x380
#define AXIMON_M5_WRITE			0x400
#define AXIMON_M5_READ			0x480
#define AXIMON_M6_WRITE			0x500
#define AXIMON_M6_READ			0x580

#define AXIMON_OFFSET_CYCLE 0x24
#define AXIMON_OFFSET_BYTECNTS 0x2C
#define AXIMON_OFFSET_LATENCYCNTS 0x34
#define AXIMON_OFFSET_HITCNTS 0x28
#define AXIMON_OFFSET_LAT_BIN_SIZE_SEL 0x50

#define AXIMON_START_REGVALUE 0x30001
#define AXIMON_STOP_REGVALUE 0x30002

void axi_mon_latency_setting(uint32_t lat_bin_size_sel)
{
	uint32_t val;

	//for ddr3 1866: bin_size_sel=0d'5
	writel(lat_bin_size_sel, (AXI_MON_BASE + AXIMON_M1_WRITE + AXIMON_OFFSET_LAT_BIN_SIZE_SEL));
	writel(lat_bin_size_sel, (AXI_MON_BASE + AXIMON_M1_READ + AXIMON_OFFSET_LAT_BIN_SIZE_SEL));

	//input clk sel
	writel(0x01000100, AXI_MON_BASE + AXIMON_M1_WRITE + 0x00);

	//hit sel setting
	val = readl(AXI_MON_BASE + AXIMON_M1_WRITE + 0x04);
	val = val & 0xfffffc00;
	val = val | 0x00000000;
	writel(val, AXI_MON_BASE + AXIMON_M1_WRITE + 0x04);

	writel(0x01000100, AXI_MON_BASE + AXIMON_M1_READ + 0x00);
	val = readl(AXI_MON_BASE + AXIMON_M1_READ + 0x04);
	val = val & 0xfffffc00;
	val = val | 0x00000000;
	writel(val, AXI_MON_BASE + AXIMON_M1_READ + 0x04);

	writel(lat_bin_size_sel, AXI_MON_BASE + AXIMON_M5_WRITE + AXIMON_OFFSET_LAT_BIN_SIZE_SEL);
	writel(lat_bin_size_sel, AXI_MON_BASE + AXIMON_M5_READ + AXIMON_OFFSET_LAT_BIN_SIZE_SEL);

	writel(0x01000100, AXI_MON_BASE + AXIMON_M5_WRITE + 0x00);
	val = readl(AXI_MON_BASE + AXIMON_M5_WRITE + 0x04);
	val = val & 0xfffffc00;
	val = val | 0x00000000;
	writel(val, AXI_MON_BASE + AXIMON_M5_WRITE + 0x04);

	writel(0x01000100, AXI_MON_BASE + AXIMON_M5_READ + 0x00);
	val = readl(AXI_MON_BASE + AXIMON_M5_READ + 0x04);
	val = val & 0xfffffc00;
	val = val | 0x00000000;
	writel(val, AXI_MON_BASE + AXIMON_M5_READ + 0x04);

	//ERROR("mon cg en.\n");
	val = readl(DDR_TOP_BASE + 0x14);
	val = val | 0x00000100;
	writel(val, DDR_TOP_BASE + 0x14);
}

void axi_mon_start_all(void)
{
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M1_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M1_READ);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M2_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M2_READ);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M3_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M3_READ);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M4_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M4_READ);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M5_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M5_READ);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M6_WRITE);
	writel(AXIMON_START_REGVALUE, AXI_MON_BASE + AXIMON_M6_READ);
}

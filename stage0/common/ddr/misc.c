
#include "common.h"
#include "ddr.h"
#include "log.h"
#include "bits.h"

#define DDR_X16_MODE

const char *ddr_finish_format = "DDR: %s finish\n";

void ddr_setting_check(void)
{
	uint32_t val;
	uint32_t wrlat, val_en;
	uint32_t _wrlat, _val_en;

	INFO("ddr phy version: %08x\n", readl(DDR_PHY_BASE + 0x1000));

	val = readl(DDR_CTRL_BASE + 0x190);

	if (FIELD_GET(GENMASK(29, 24), val) != 0x4) {
		ERR("ddr dfi_t_ctrl_delay not 0x4\n");
	}

	if (FIELD_GET(GENMASK(13, 8), val) != 0x3) {
		ERR("ddr dfi_tphy_wrdata not 0x3\n");
	}

	wrlat = FIELD_GET(GENMASK(5, 0), val);
	val_en = FIELD_GET(GENMASK(22, 16), val);

	val = readl(DDR_CTRL_BASE + 0x194);

	if (FIELD_GET(GENMASK(20, 16), val) != 0x7) {
		ERR("ddr dfi_t_wrdata_delay not 0x7\n");
	}

	switch (ddr_rate) {
		case 2133:
			_wrlat = 0x6;
			_val_en = 0xa;
			break;
		case 1866:
			_wrlat = 0x5;
			_val_en = 0xa;
			break;
		case 1600:
			_wrlat = 0x4;
			_val_en = 0x8;
			break;
		case 1333: /* fall-through */
		case 1066:
			_wrlat = 0x2;
			_val_en = 0x5;
			break;
		default:
			ERR("Unsupported ddr rate, setting failed\n");
	}

	if (wrlat != _wrlat) {
		ERR("ddr dfi_tphy_wrlat not 0x%x\n", _wrlat);
	}

	if (val_en != _val_en) {
		ERR("ddr dfi_t_val_en not 0x%x\n", _val_en);
	}
}

void ddr_en_rec_vol_mode(void)
{
	if (ddr_type == DDR_TYPE_DDR2) {
		writel(0x00001001, DDR_BASE + 0x0500);
		writel(0x00001001, DDR_BASE + 0x0540);
	}

	INFO(ddr_finish_format, "en_rec_vol_mode");
}

void ddr_set_dfi_init_start(void)
{
	uint32_t val;

	// synp setting
	// phy is ready for initial dfi_init_start request
	// set umctl2 to tigger dfi_init_start
	writel(0, DDR_CTRL_BASE + 0x00000320);

	val = readl(DDR_CTRL_BASE + 0x000001b0); // dfi_init_start @ val[5];
	val |= BIT(5);
	writel(val, DDR_CTRL_BASE + 0x000001b0);

	writel(1, DDR_CTRL_BASE + 0x00000320);

	INFO(ddr_finish_format, "dfi_init_start");
}

void ddr_polling_dfi_init_start(void)
{
	INFO("DDR: polling dfi init start\n");

	while ((readl(DDR_BASE + 0x3028) & BIT(8)) == 0)
		;

	INFO(ddr_finish_format, "polling dfi init start");
}

void ddr_set_dfi_init_complete(void)
{
	uint32_t val;

	udelay(20);

	// init_complete = 1;
	writel(0x00000010, DDR_BASE + 0x0120);
	// param_phyd_clkctrl_init_complete   <= int_regin[0];
	writel(0x00000001, DDR_BASE + 0x0118);
}

void ddr_polling_dfi_init_complete(void)
{
	uint32_t val;

	INFO("DDR: polling dfi complete\n");

	while ((readl(DDR_CTRL_BASE + 0x001bc) & BIT(0)) == 0)
		;

	writel(0x00000000, DDR_CTRL_BASE + 0x00000320);

	val = readl(DDR_CTRL_BASE + 0x000001b0);
	val &= ~GENMASK(5, 0);
	val |= FIELD_PREP(GENMASK(5, 0), 5);
	writel(val, DDR_CTRL_BASE + 0x000001b0);

	writel(0x00000001, DDR_CTRL_BASE + 0x00000320);

	INFO(ddr_finish_format, "polling dfi init complete");
}

void ddr_polling_sync_normal_mode(void)
{
	INFO("DDR: polling sync normal mode\n");
	while (FIELD_GET(GENMASK(2, 0), readl(DDR_CTRL_BASE + 0x00000004)) != 1)
		;

	INFO(ddr_finish_format, "polling sync normal mod");
}

static const struct ddr_mask_patch_reg ddr_phy_power1[] = {
	// RESETZ/CKE PD=0
	// TOP_REG_TX_CA_PD_CKE0
	// TOP_REG_TX_CA_PD_RESETZ
	DDR_MASK_PATCH_REG(DDR_PHYD_BASE + 0x40, BIT(24) | BIT(30), 0),
	// CA PD=0
	// All PHYA CA PD=0
	DDR_MASK_PATCH_REG(DDR_PHYD_BASE + 0x40, 0xffffffff, 0),
	// TOP_REG_TX_SEL_GPIO = 1 (DQ)
	DDR_MASK_PATCH_REG(DDR_PHYD_BASE + 0x1c, BIT(7), BIT(7)),
	// DQ PD=0
	// TOP_REG_TX_BYTE0_PD
	// TOP_REG_TX_BYTE1_PD
	DDR_MASK_PATCH_REG(DDR_PHYD_BASE + 0x00, 0xffffffff, 0),
	// TOP_REG_TX_SEL_GPIO = 0 (DQ)
	DDR_MASK_PATCH_REG(DDR_PHYD_BASE + 0x1c, BIT(7), 0),
};

void ddr_phy_power_on_seq1(void)
{
	ddr_update_mask_patch_reg(ddr_phy_power1, ARRAY_SIZE(ddr_phy_power1));
}

void ddr_phy_power_on_seq2(void)
{
	uint32_t val;

	ddr_adjust_pll_freq();

	// OEN
	// param_phyd_sel_cke_oenz        <= `PI_SD int_regin[0];
	val = readl(DDR_BASE + 0x0154);
	val &= ~BIT(0);
	writel(val, DDR_BASE + 0x0154);

	// param_phyd_tx_ca_oenz          <= `PI_SD int_regin[0];
	// param_phyd_tx_ca_clk0_oenz     <= `PI_SD int_regin[8];
	// param_phyd_tx_ca_clk1_oenz     <= `PI_SD int_regin[16];
	writel(0x00000000, DDR_BASE + 0x0130);

	ddr_dll_cal();

	// CA PD =0
	// All PHYA CA PD=0
	writel(0x80000000, DDR_PHYD_BASE + 0x40);

	// BYTE PD =0
	writel(0x00000000, DDR_PHYD_BASE + 0x00);
}

static const struct ddr_mask_patch_reg ddr_phy_power3[] = {
	// RESETYZ/CKE OENZ
	// param_phyd_sel_cke_oenz        <= `PI_SD int_regin[0];
	DDR_MASK_PATCH_REG(DDR_BASE + 0x0154, BIT(0), 0),
	// param_phyd_tx_ca_oenz          <= `PI_SD int_regin[0];
	// param_phyd_tx_ca_clk0_oenz     <= `PI_SD int_regin[8];
	// param_phyd_tx_ca_clk1_oenz     <= `PI_SD int_regin[16];
	DDR_MASK_PATCH_REG(DDR_BASE + 0x0130, 0xffffffff, 0),

	// clock gated for power save
	// param_phya_reg_tx_byte0_en_extend_oenz_gated_dline <= `PI_SD int_regin[0];
	// param_phya_reg_tx_byte1_en_extend_oenz_gated_dline <= `PI_SD int_regin[1];
	// param_phya_reg_tx_byte2_en_extend_oenz_gated_dline <= `PI_SD int_regin[2];
	// param_phya_reg_tx_byte3_en_extend_oenz_gated_dline <= `PI_SD int_regin[3];
	DDR_MASK_PATCH_REG(DDR_BASE + 0x0204, BIT(18), BIT(18)),
	DDR_MASK_PATCH_REG(DDR_BASE + 0x0224, BIT(18), BIT(18)),
};

void ddr_phy_power_on_seq3(void)
{
	ddr_update_mask_patch_reg(ddr_phy_power3, ARRAY_SIZE(ddr_phy_power3));
}

void ddr_bist_wr_prbs_init(void)
{
	// reg [31:0] cmd[5:0];
	// reg [31:0] sram_st;
	// reg [31:0] sram_sp;
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	int i;

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	sram_st = 0;
	sram_sp = 511;

	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// W  1~17  prbs  repeat0
	cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
	         (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
	// R  1~17  prbs  repeat0
	cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
	// NOP
	cmd[2] = 0;
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (i = 0; i < 6; i = i + 1) {
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));
	};
	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);
	// specified AXI address step
#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif

	INFO(ddr_finish_format, "bist_wr_prbs_init");
}

void ddr_int_isr_08(void)
{
	// param_phyd_clkctrl_init_complete   <= int_regin[0];
	writel(0, DDR_BASE + 0x0118);
}

int ddr_bist_start_check(uint64_t *err_data_odd, uint64_t *err_data_even)
{
	int ret;
	uint32_t val;
	uint64_t err_data_even_l;
	uint64_t err_data_even_h;
	uint64_t err_data_odd_l;
	uint64_t err_data_odd_h;

	// bist enable
#ifdef DDR_X16_MODE
	writel(0x00030003, DDR_BIST_BASE + 0x0);
#else
	writel(0x00010001, DDR_BIST_BASE + 0x0);
#endif
	// polling bist done
	//    while (get_bits_from_value(2)  == 0x0 = readl(DDR_BIST_BASE + 0x80),2);
	INFO("DDR: polling bist done\n");

	while ((readl(DDR_BIST_BASE + 0x80) & BIT(2)) == 0)
		;

	INFO(ddr_finish_format, "polling bist");

	val = readl(DDR_BIST_BASE + 0x80);
	if (val & BIT(3)) {
		/* failed */
		ret = -1;
		err_data_odd_l = readl(DDR_BIST_BASE + 0x88);
		err_data_odd_h = readl(DDR_BIST_BASE + 0x8c);
		err_data_even_l = readl(DDR_BIST_BASE + 0x90);
		err_data_even_h = readl(DDR_BIST_BASE + 0x94);
		*err_data_odd = err_data_odd_h << 32 | err_data_odd_l;
		*err_data_even = err_data_even_h << 32 | err_data_even_l;
	} else {
		/* OK */
		ret = 0;
		*err_data_odd = 0;
		*err_data_even = 0;
	}

	// bist disable
	writel(0x00050000, DDR_BIST_BASE + 0x0);

	return ret;
}

void ddr_do_bist_prbs_check(void)
{
	int ret;
	uint64_t err_data_odd, err_data_even;

	ddr_bist_wr_prbs_init();
	if (ddr_bist_start_check(&err_data_odd, &err_data_even)) {
		ERR("DDR: do prbs bist failed, err data 0x%016lx 0x%016lx\n",
		    err_data_odd, err_data_even);
	}

	return;
}

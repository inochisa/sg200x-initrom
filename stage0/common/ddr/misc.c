
#include "common.h"
#include "ddr.h"
#include "log.h"
#include "bits.h"

#define DDR_X16_MODE

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
	DEBUG(ddr_polling_format, "dfi init start");

	while ((readl(DDR_BASE + 0x3028) & BIT(8)) == 0)
		;

	DEBUG(ddr_polling_finsh);
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

	DEBUG(ddr_polling_format, "dfi complete");

	while ((readl(DDR_CTRL_BASE + 0x001bc) & BIT(0)) == 0)
		;

	DEBUG(ddr_polling_finsh);

	writel(0x00000000, DDR_CTRL_BASE + 0x00000320);

	val = readl(DDR_CTRL_BASE + 0x000001b0);
	FIELD_MOD(val, GENMASK(5, 0), 5);
	writel(val, DDR_CTRL_BASE + 0x000001b0);

	writel(0x00000001, DDR_CTRL_BASE + 0x00000320);
}

void ddr_polling_sync_normal_mode(void)
{
	DEBUG(ddr_polling_format, "sync normal mode");
	while (FIELD_GET(GENMASK(2, 0), readl(DDR_CTRL_BASE + 0x00000004)) != 1)
		;

	DEBUG(ddr_polling_finsh);
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

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	sram_st = 0;
	sram_sp = 511;

	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// W  1~17 prbs repeat0
	cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
	         (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
	// R  1~17 prbs repeat0
	cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
	// NOP
	cmd[2] = 0;
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

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
	DEBUG(ddr_polling_format, "bist done");

	while ((readl(DDR_BIST_BASE + 0x80) & BIT(2)) == 0)
		;

	DEBUG(ddr_polling_finsh);

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

	INFO("DDR: prbs check is OK\n");

	return;
}

void ddr_bist_wr_sram_init(void)
{
	const int byte_per_page = 256;
	const int axi_per_page = byte_per_page / 64;
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	uint32_t fmax;
	uint32_t fmin;
	int i;

	// bist clock enable, axi_len 8
	writel(0x000C000C, DDR_BIST_BASE + 0x0);

	fmax = 15;
	fmin = 5;
	sram_st = 0;
	sram_sp = 9 * (fmin + fmax) * (fmax - fmin + 1) / 2 / 4 + (fmax - fmin + 1); // 8*f/4 -1

	// bist sso_period
	writel((fmax << 8) + fmin, DDR_BIST_BASE + 0x24);
	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// W  1~17 sram repeat0
	cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (15 << 0);
	// R  1~17 sram repeat0
	cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (15 << 0);
	// NOP
	cmd[2] = 0;
	// GOTO addr_not_reset loop_cnt
	cmd[3] = (3 << 30) | (0 << 20) | (1 << 0);
	// NOP
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);
#ifdef DDR_X16_MODE
	// specified AXI address step to 2KB
	writel(2048 / axi_per_page / 16, DDR_BIST_BASE + 0x18);
#else
	// TBD
#endif
	INFO(ddr_finish_format, "bist_wr_sram_init");
}

void ddr_do_bist_sram_check(void)
{
	int ret;
	uint64_t err_data_odd, err_data_even;

	ddr_bist_wr_sram_init();
	if (ddr_bist_start_check(&err_data_odd, &err_data_even)) {
		ERR("DDR: do sram bist failed, err data 0x%016lx 0x%016lx\n",
		    err_data_odd, err_data_even);
	}

	INFO("DDR: sram check is OK\n");

	return;
}

void ddr_bist_wrlvl_init(void)
{
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	int i;

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	sram_st = 0;
	// sram_sp = 511;
	sram_sp = 0;

	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// W  1~17 prbs repeat0
	cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
		 (0 << 0);
	// NOP
	cmd[1] = 0;
	cmd[2] = 0;
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);

#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif

	INFO(ddr_finish_format, "bist_wrlvl_init");
}

void ddr_bist_rdglvl_init(void)
{
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	sram_st = 0;
	// sram_sp = 511;
	sram_sp = 3;

	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// R  1~17 prbs repeat0
	cmd[0] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
	// NOP
	cmd[1] = 0;
	cmd[2] = 0;
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);

#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif
	INFO(ddr_finish_format, "bist_rdglvl_init");
}

void ddr_bist_wdmlvl_init(void)
{
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	uint32_t fmax;
	uint32_t fmin;

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	sram_st = 0;
	fmax = 15;
	fmin = 5;
	sram_st = 0;
	sram_sp = 9 * (fmin + fmax) * (fmax - fmin + 1) / 2 / 4 + (fmax - fmin + 1); // 8*f/4 -1

	// bist sso_period
	writel((fmax << 8) + fmin, DDR_BIST_BASE + 0x24);
	// cmd queue
	// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
	// bist write pat 3 = 0x0f
	cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (3 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
		 (0 << 0);
	// bist write pat 7 = sso_8x1 DM toggle verison
	cmd[1] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (7 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
		 (0 << 0);
	// bist read pat 7
	cmd[2] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
		 (7 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
		 (0 << 0);
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);

#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif
	INFO(ddr_finish_format, "bist_wdmlvl_init");
}

void ddr_bist_wdqlvl_init(uint32_t mode)
{
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	uint32_t fmax;
	uint32_t fmin;
	uint32_t val;

	INFO("DDR: bist_wdqlvl_init with mode %u\n", mode);

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	//----for Error enject simulation only
	// param_phyd_dfi_wdqlvl_vref_start
	// param_phyd_dfi_wdqlvl_vref_end
	// param_phyd_dfi_wdqlvl_vref_step
	if (mode == 0x11) {
		val = FIELD_PREP(GENMASK(6, 0), 0x1) |
		      FIELD_PREP(GENMASK(14, 8), 0x3) |
		      FIELD_PREP(GENMASK(19, 16), 0x1);
		writel(val, DDR_BASE + 0x0190);
	} else if (mode == 0x12) {
		val = FIELD_PREP(GENMASK(6, 0), 0x1d) |
		      FIELD_PREP(GENMASK(14, 8), 0x1f) |
		      FIELD_PREP(GENMASK(19, 16), 0x1);
		writel(val, DDR_BASE + 0x0190);
	}
	//----for Error enject simulation only

	switch (mode) {
	case 0x1: // bist write/read
		fmax = 15;
		fmin = 5;
		sram_st = 0;
		sram_sp = 9 * (fmin + fmax) * (fmax - fmin + 1) / 2 / 4 + (fmax - fmin + 1); // 8*f/4 -1

		// bist sso_period
		writel((fmax << 8) + fmin, DDR_BIST_BASE + 0x24);

		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// W  1~17 prbs repeat0
		cmd[0] = (1 << 30) | (sram_st << 21) | (511 << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// R  1~17 prbs repeat0
		cmd[1] = (2 << 30) | (sram_st << 21) | (511 << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// W  1~17 sram repeat0
		cmd[2] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// R  1~17 sram repeat0
		cmd[3] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// GOTO addr_not_reset loop_cnt
		cmd[4] = 0;
		// NOP
		cmd[5] = 0;
		break;
	case 0x0: // phyd pattern
	case 0x11: // bist write/read
	case 0x12: // bist write/read
	default:
		sram_st = 0;
		// sram_sp = 511;
		sram_sp = 3;

		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// W  1~17 prbs repeat0
		cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// R  1~17 prbs repeat0
		cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// NOP
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;
		break;
	}

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);

#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif
	INFO(ddr_finish_format, "bist_wdqlvl_init");
}

void ddr_bist_rdlvl_init(uint32_t mode)
{
	uint32_t cmd[6];
	uint32_t sram_st;
	uint32_t sram_sp;
	uint32_t fmax;
	uint32_t fmin;
	uint32_t val;

	// mode = 0x0  : MPR mode, DDR3 only.
	// mode = 0x1  : sram write/read continuous goto
	// mode = 0x2  : multi- bist write/read
	// mode = 0x12 : with Error enject,  multi- bist write/read

	// bist clock enable
	writel(0x00060006, DDR_BIST_BASE + 0x0);

	if (mode == 0x0) { // MPR mode
		sram_st = 0;
		// sram_sp = 511;
		sram_sp = 3;
		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// R  1~17 prbs repeat0
		cmd[0] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
			 (0 << 0);
		// NOP
		cmd[1] = 0;
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;
	} else if (mode == 0x1) { // sram write/read continuous goto
		fmax = 15;
		fmin = 5;
		sram_st = 0;
		sram_sp = 9 * (fmin + fmax) * (fmax - fmin + 1) / 2 / 4 + (fmax - fmin + 1); // 8*f/4 -1

		// bist sso_period
		writel((fmax << 8) + fmin, DDR_BIST_BASE + 0x24);
		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// W  1~17 prbs repeat0
		cmd[0] = (1 << 30) | (sram_st << 21) | (511 << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (2 << 0);
		// R  1~17 prbs repeat0
		cmd[1] = (2 << 30) | (sram_st << 21) | (511 << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (2 << 0);
		// W  1~17 sram repeat0
		cmd[2] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (2 << 0);
		// R  1~17 sram repeat0
		cmd[3] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (6 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (2 << 0);
		// GOTO addr_not_reset loop_cnt
		cmd[4] = (3 << 30) | (0 << 20) | (1 << 0);
		// NOP
		cmd[5] = 0;
	} else if (mode == 0x2) { // multi- bist write/read
		sram_st = 0;
		sram_sp = 7;
		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// W  1~17 prbs repeat0
		cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// R  1~17 prbs repeat0
		cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// NOP
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;
	} else if (mode == 0x12) { // with Error enject,  multi- bist write/read
		//----for Error enject simulation only
		// param_phyd_pirdlvl_trig_lvl_start
		// param_phyd_pirdlvl_trig_lvl_end
		val = readl(DDR_BASE + 0x0084);
		FIELD_MOD(val, GENMASK(4, 0), 0x1d);
		FIELD_MOD(val, GENMASK(12, 8), 0x1f);
		writel(val, DDR_BASE + 0x0084);
		//----for Error enject simulation only

		sram_st = 0;
		sram_sp = 7;
		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// W  1~17 prbs repeat0
		cmd[0] = (1 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// R  1~17 prbs repeat0
		cmd[1] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) | (0 << 0);
		// NOP
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;
	} else if (mode == 0x10) { // with Error enject,  multi- bist write/read
		//----for Error enject simulation only
		// param_phyd_pirdlvl_trig_lvl_start
		// param_phyd_pirdlvl_trig_lvl_end
		val = readl(DDR_BASE + 0x0084);
		FIELD_MOD(val, GENMASK(4, 0), 1);
		FIELD_MOD(val, GENMASK(12, 8), 3);
		writel(val, DDR_BASE + 0x0084);
		//----for Error enject simulation only

		sram_st = 0;
		sram_sp = 3;
		// cmd queue
		// op_code start stop pattern dq_inv dm_inv dq_rotate repeat
		// R  1~17 prbs repeat0
		cmd[0] = (2 << 30) | (sram_st << 21) | (sram_sp << 12) |
			 (5 << 9) | (0 << 8) | (0 << 7) | (0 << 4) |
			 (0 << 0);
		// NOP
		cmd[1] = 0;
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;
	}

	// write cmd queue
	for (unsigned int i = 0; i < 6; i++)
		writel(cmd[i], DDR_BIST_BASE + 0x40 + i * sizeof(uint32_t));

	// specified DDR space
	writel(0x00000000, DDR_BIST_BASE + 0x10);
	writel(0x000fffff, DDR_BIST_BASE + 0x14);

#ifdef DDR_X16_MODE
	writel(0x00000004, DDR_BIST_BASE + 0x18);
#else
	writel(0x00000008, DDR_BIST_BASE + 0x18);
#endif
	INFO(ddr_finish_format, "bist_rdlvl_init");
}

void ddr_synp_mrw(uint32_t addr, uint32_t data)
{
	uint32_t val;
	uint32_t init_dis_auto_zq;

	INFO("DDR: synp mrw at 0x%x with 0x%04x\n", addr, data);

	// ZQCTL0.dis_auto_zq to 1.
	val = readl(DDR_CTRL_BASE + 0x180);
	// if (val[31] == 0b0) {
	init_dis_auto_zq = FIELD_GET(BIT(31), val);

	if (init_dis_auto_zq == 0) {
		val |= BIT(31);
		writel(val, DDR_CTRL_BASE + 0x180);
		INFO("DDR: set ZQCTL0.dis_auto_zq to 1\n");
	}

	// Poll MRSTAT.mr_wr_busy until it is 0
	DEBUG(ddr_polling_format, "MRSTAT.mr_wr_busy");

	while ((readl(DDR_CTRL_BASE + 0x18) & BIT(0)) != 0)
		;

	DEBUG(ddr_polling_finsh);

	// Write MRCTRL0.mr_type, MRCTRL0.mr_addr, MRCTRL0.mr_rank
	// val[31:0]  = 0;
	// val[0]     = 0;       // mr_type  0:write   1:read
	// val[5:4]   = 1;       // mr_rank
	// val[15:12] = addr;    // mr_addr
	val = FIELD_PREP(BIT(0), 0);
	FIELD_MOD(val, GENMASK(5, 4), 1);
	FIELD_MOD(val, GENMASK(15, 12), addr);
	writel(val, DDR_CTRL_BASE + 0x10);

	// Write MRCTRL1.mr_data (for MRWs)
	// val[31:0] = 0;
	// val[15:0] = data;     // mr_data
	val = FIELD_PREP(GENMASK(31, 0), data);
	writel(val, DDR_CTRL_BASE + 0x14);

	// Write MRCTRL0.mr_wr to 1
	val = readl(DDR_CTRL_BASE + 0x10);
	// val[31] = 1;
	val |= BIT(31);
	writel(val, DDR_CTRL_BASE + 0x10);

	if (init_dis_auto_zq == 0) {
		// ZQCTL0.dis_auto_zq to 0.
		val = readl(DDR_CTRL_BASE + 0x180);
		// val[31] = 0;
		val &= ~BIT(31);
		writel(val, DDR_CTRL_BASE + 0x180);
	}
}

void ddr_wrlvl_req(void)
{
	const uint32_t PORT_NUM = 4;
	uint32_t val;
	uint32_t selfref_sw;
	uint32_t en_dfi_dram_clk_disable;
	uint32_t powerdown_en;
	uint32_t selfref_en;
	uint32_t wr_odt_en;
	uint32_t rtt_wr;
	uint32_t rtt_nom;

	if (ddr_type != DDR_TYPE_DDR2) {
		// Note: training need ctrl_low_patch first
		// wrlvl response only DQ0
		writel(0x00FE0000, DDR_BASE + 0x005C);
		// Write 0 to PCTRL_n.port_en, without port 0
		// port number = 0,1,2,3

		for (unsigned int i = 1; i < PORT_NUM; i++)
			writel(0x0, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);

		// Poll PSTAT.rd_port_busy_n = 0
		// Poll PSTAT.wr_port_busy_n = 0
		DEBUG(ddr_polling_format, "PSTAT port_busy");

		while (readl(DDR_CTRL_BASE + 0x3fc) != 0)
			;

		DEBUG(ddr_polling_finsh);

		// disable PWRCTL.powerdown_en, PWRCTL.selfref_en
		val = readl(DDR_CTRL_BASE + 0x30);
		selfref_sw = FIELD_GET(BIT(5), val);
		en_dfi_dram_clk_disable = FIELD_GET(BIT(3), val);
		powerdown_en = FIELD_GET(BIT(1), val);
		selfref_en = FIELD_GET(BIT(0), val);
		val &= ~(BIT(5) | BIT(3) | BIT(1) | BIT(0));
		writel(val, DDR_CTRL_BASE + 0x30);

		ddr_clk_disable_gating();

		// save ctrl wr_odt_en
		val = readl(DDR_CTRL_BASE + 0x244);
		wr_odt_en = FIELD_GET(BIT(0), val);
		// bist setting for dfi wrlvl
		ddr_bist_wrlvl_init();

		// // RFSHCTL3.dis_auto_refresh =1
		// val = readl(DDR_CTRL_BASE + 0x60);
		// val |= BIT(0); // RFSHCTL3.dis_auto_refresh
		// writel(val, DDR_CTRL_BASE + 0x60);
	}

	if (ddr_type == DDR_TYPE_DDR3) {
		rtt_nom = 0;

		if (wr_odt_en == 1) {
			// save rtt_wr
			val = readl(DDR_CTRL_BASE + 0xe0);
			rtt_wr = FIELD_GET(GENMASK(26, 25), val);

			if (rtt_wr != 0x0) {
				// disable rtt_wr
				val &= ~GENMASK(26, 25);

				// MR2
				ddr_synp_mrw(0x2, FIELD_GET(GENMASK(31, 16), val));

				// set rtt_nom
				val = readl(DDR_CTRL_BASE + 0xdc);
				// rtt_nom[2]=0
				rtt_nom = val & ~BIT(9);
				// rtt_nom[1] = rtt_wr[1]
				FIELD_MOD(rtt_nom, BIT(6), FIELD_GET(BIT(1), rtt_wr));
				// rtt_nom[0] = rtt_wr[0]
				FIELD_MOD(rtt_nom, BIT(2), FIELD_GET(BIT(0), rtt_wr));
			}
		} else {
			// set rtt_nom = 120ohm
			val = readl(DDR_CTRL_BASE + 0xdc);
			// rtt_nom[2]=0
			rtt_nom = val & ~BIT(9);
			// rtt_nom[1]=1
			rtt_nom |= BIT(6);
			// rtt_nom[0]=0
			rtt_nom &= ~BIT(2);

			ddr_synp_mrw(0x1, FIELD_GET(GENMASK(15, 0), rtt_nom));
		}

		// Write leveling enable
		rtt_nom |= BIT(7);

		ddr_synp_mrw(0x1, FIELD_GET(GENMASK(15, 0), rtt_nom));
	}

	if (ddr_type != DDR_TYPE_DDR2) {
		val = readl(DDR_BASE + 0x0180);
		// param_phyd_dfi_wrlvl_req
		val |= BIT(0);
		// param_phyd_dfi_wrlvl_odt_en
		FIELD_MOD(val, BIT(4), wr_odt_en);
		writel(val, DDR_BASE + 0x0180);

		//[0] param_phyd_dfi_wrlvl_done
		//[1] param_phyd_dfi_rdglvl_done
		//[2] param_phyd_dfi_rdlvl_done
		//[3] param_phyd_dfi_wdqlvl_done
		while ((readl(DDR_BASE + 0x3444) & BIT(0)) == 0)
			;

		writel(0x00040000, DDR_BIST_BASE + 0x0);

		// RFSHCTL3.dis_auto_refresh =0
		val = readl(DDR_CTRL_BASE + 0x60);
		val &= ~BIT(0);
		writel(val, DDR_CTRL_BASE + 0x60);
	}

	if (ddr_type == DDR_TYPE_DDR3) {
		val = readl(DDR_CTRL_BASE + 0xdc);
		ddr_synp_mrw(0x1, FIELD_GET(GENMASK(15, 0), val));
		// MR2
		val = readl(DDR_CTRL_BASE + 0xe0);
		ddr_synp_mrw(0x2, FIELD_GET(GENMASK(31, 16), val));
	}

	if (ddr_type != DDR_TYPE_DDR2) {
		// // RFSHCTL3.dis_auto_refresh =0
		// val = readl(DDR_CTRL_BASE + 0x60);
		// val &= ~BIT(0); // RFSHCTL3.dis_auto_refresh
		// writel(val, DDR_CTRL_BASE + 0x60);
		// restore PWRCTL.powerdown_en, PWRCTL.selfref_en
		val = readl(DDR_CTRL_BASE + 0x30);
		// PWRCTL.selfref_sw
		FIELD_MOD(val, BIT(5), selfref_sw);
		// PWRCTL.en_dfi_dram_clk_disable
		FIELD_MOD(val, BIT(3), en_dfi_dram_clk_disable);
		// PWRCTL.powerdown_en
		FIELD_MOD(val, BIT(1), powerdown_en);
		// PWRCTL.selfref_en
		FIELD_MOD(val, BIT(0), selfref_en);
		writel(val, DDR_CTRL_BASE + 0x30);

		// Write 1 to PCTRL_n.port_en
		for (int i = 1; i < PORT_NUM; i++) {
			writel(0x1, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);
		}

		ddr_clk_enable_gating();
	}
}

void ddr_rdglvl_req(void)
{
	const uint32_t PORT_NUM = 4;
	uint32_t val;
	uint32_t selfref_sw;
	uint32_t en_dfi_dram_clk_disable;
	uint32_t powerdown_en;
	uint32_t selfref_en;
	uint32_t ddr3_mpr_mode;

	// Note: training need ctrl_low_patch first
	//  Write 0 to PCTRL_n.port_en, without port 0
	//  port number = 0,1,2,3
	for (unsigned int i = 1; i < PORT_NUM; i++)
		writel(0x0, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);

	// Poll PSTAT.rd_port_busy_n = 0
	// Poll PSTAT.wr_port_busy_n = 0
	DEBUG(ddr_polling_format, "PSTAT port_busy");

	while (readl(DDR_CTRL_BASE + 0x3fc) != 0)
		;

	DEBUG(ddr_polling_finsh);

	// disable PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	selfref_sw = FIELD_GET(BIT(5), val);
	en_dfi_dram_clk_disable = FIELD_GET(BIT(3), val);
	powerdown_en = FIELD_GET(BIT(1), val);
	selfref_en = FIELD_GET(BIT(0), val);
	val &= ~(BIT(5) | BIT(3) | BIT(1) | BIT(0));
	writel(val, DDR_CTRL_BASE + 0x30);

	ddr_clk_disable_gating();

	if (ddr_type == DDR_TYPE_DDR3) {
		val = readl(DDR_BASE + 0x0184);
		ddr3_mpr_mode = FIELD_GET(BIT(4), val);

		if (ddr3_mpr_mode) {
			// RFSHCTL3.dis_auto_refresh =1
			val = readl(DDR_CTRL_BASE + 0x60);
			val |= BIT(0);
			writel(val, DDR_CTRL_BASE + 0x60);

			// MR3
			val = readl(DDR_CTRL_BASE + 0xe0);
			// Dataflow from MPR
			val |= BIT(2);

			ddr_synp_mrw(0x3, FIELD_GET(GENMASK(15, 0), val));
		}
	}

	// bist setting for dfi rdglvl
	ddr_bist_rdglvl_init();

	val = readl(DDR_BASE + 0x0184);
	// param_phyd_dfi_rdglvl_req
	val |= BIT(0);
	writel(val, DDR_BASE + 0x0184);

	//[0] param_phyd_dfi_wrlvl_done
	//[1] param_phyd_dfi_rdglvl_done
	//[2] param_phyd_dfi_rdlvl_done
	//[3] param_phyd_dfi_wdqlvl_done
	while ((readl(DDR_BASE + 0x3444) & BIT(1)) == 0)
		;

	writel(0x00040000, DDR_BIST_BASE + 0x0);

	if (ddr_type == DDR_TYPE_DDR3) {
		if (ddr3_mpr_mode) {
			// MR3
			val = readl(DDR_CTRL_BASE + 0xe0);
			val &= ~BIT(2);

			ddr_synp_mrw(0x3, FIELD_GET(GENMASK(15, 0), val));

			// RFSHCTL3.dis_auto_refresh =0
			val = readl(DDR_CTRL_BASE + 0x60);
			val &= ~BIT(0);
			writel(val, DDR_CTRL_BASE + 0x60);
		}
	}

	// // RFSHCTL3.dis_auto_refresh =0
	// val = readl(DDR_CTRL_BASE + 0x60);
	// val &= ~BIT(0); // RFSHCTL3.dis_auto_refresh
	// writel(val, DDR_CTRL_BASE + 0x60);
	// restore PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	// PWRCTL.selfref_sw
	FIELD_MOD(val, BIT(5), selfref_sw);
	// PWRCTL.en_dfi_dram_clk_disable
	FIELD_MOD(val, BIT(3), en_dfi_dram_clk_disable);
	// PWRCTL.powerdown_en
	FIELD_MOD(val, BIT(1), powerdown_en);
	// PWRCTL.selfref_en
	FIELD_MOD(val, BIT(0), selfref_en);
	writel(val, DDR_CTRL_BASE + 0x30);

	// Write 1 to PCTRL_n.port_en
	for (int i = 1; i < PORT_NUM; i++) {
		writel(0x1, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);
	}

	ddr_clk_enable_gating();
}

void ddr_dfi_ca_park_prbs(uint32_t cap_enable)
{
	// param_phyd_sw_dfi_phyupd_req =1
	writel(0x00000001, DDR_BASE + 0x0174);

	while (FIELD_GET(GENMASK(9, 8), readl(DDR_BASE + 0x3030)) != 3)
		;

	// DDR3
	//   cfg_det_en = 0b1;
	//   cfg_cs_det_en = 0b1;
	//   cap_prbs_en = 0b1;
	//   cfg_cs_polarity = 0b1;
	//   cap_prbs_1t = 0b0;
	//   cfg_ca_reference = {0b0,0x0_ffff,0x7,0x0,0b1,0b0,0b1,0b1};
	//   cfg_cs_retain_cycle = 0b0000_0001;
	//   cfg_ca_retain_cycle = 0b0000_0000;
	//   cfg_ca_park_value = 0x3fff_ffff;
	if (cap_enable == 1)
		writel(0x1B, DDR_TOP_BASE + 0x00);
	else
		writel(0x0, DDR_TOP_BASE + 0x00);

	writel(0x01, DDR_TOP_BASE + 0x04);
	writel(0x1ffffcb, DDR_TOP_BASE + 0x08);
	writel(0x3fffffff, DDR_TOP_BASE + 0x0c);

	// param_phyd_sw_dfi_phyupd_req_clr =1
	writel(0x00000010, DDR_BASE + 0x0174);
}

void ddr_wdqlvl_req(uint32_t data_mode, uint32_t lvl_mode)
{
	const uint32_t PORT_NUM = 4;
	uint32_t val;
	uint32_t selfref_sw;
	uint32_t en_dfi_dram_clk_disable;
	uint32_t powerdown_en;
	uint32_t selfref_en;

	// Note: training need ctrl_low_patch first
	//  Write 0 to PCTRL_n.port_en, without port 0
	//  port number = 0,1,2,3
	for (unsigned int i = 1; i < PORT_NUM; i++)
		writel(0x0, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);

	// Poll PSTAT.rd_port_busy_n = 0
	// Poll PSTAT.wr_port_busy_n = 0
	DEBUG(ddr_polling_format, "PSTAT port_busy");

	while (readl(DDR_CTRL_BASE + 0x3fc) != 0)
		;

	DEBUG(ddr_polling_finsh);

	// disable PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	selfref_sw = FIELD_GET(BIT(5), val);
	en_dfi_dram_clk_disable = FIELD_GET(BIT(3), val);
	powerdown_en = FIELD_GET(BIT(1), val);
	selfref_en = FIELD_GET(BIT(0), val);
	val &= ~(BIT(5) | BIT(3) | BIT(1) | BIT(0));
	writel(val, DDR_CTRL_BASE + 0x30);

	ddr_clk_disable_gating();

	ddr_dfi_ca_park_prbs(1);

	// param_phyd_piwdqlvl_dq_mode
	//     <= #RD (~pwstrb_mask[12] & param_phyd_piwdqlvl_dq_mode) |  pwstrb_mask_pwdata[12];
	// param_phyd_piwdqlvl_dm_mode
	//     <= #RD (~pwstrb_mask[13] & param_phyd_piwdqlvl_dm_mode) |  pwstrb_mask_pwdata[13];
	val = readl(DDR_BASE + 0x00bc);
	// lvl_mode =0x0, wdmlvl
	// lvl_mode =0x1, wdqlvl
	// lvl_mode =0x2, wdqlvl and wdmlvl
	if (lvl_mode == 0x0) {
		// param_phyd_piwdqlvl_dq_mode
		FIELD_MOD(val, BIT(12), 0);
		// param_phyd_piwdqlvl_dm_mode
		FIELD_MOD(val, BIT(13), 1);
	} else if (lvl_mode == 0x1) {
		// param_phyd_piwdqlvl_dq_mode
		FIELD_MOD(val, BIT(12), 1);
		// param_phyd_piwdqlvl_dm_mode
		FIELD_MOD(val, BIT(13), 0);
	} else if (lvl_mode == 0x2) {
		// param_phyd_piwdqlvl_dq_mode
		FIELD_MOD(val, BIT(12), 1);
		// param_phyd_piwdqlvl_dm_mode
		FIELD_MOD(val, BIT(13), 1);
	}
	writel(val, DDR_BASE + 0x00bc);

	if (lvl_mode == 0x0) {
		val = readl(DDR_CTRL_BASE + 0xC);
		val |= BIT(7);
		writel(val, DDR_CTRL_BASE + 0xC);

		ddr_bist_wdmlvl_init();
	} else {
		// bist setting for dfi rdglvl
		// data_mode = 0x0 : phyd pattern
		// data_mode = 0x1 : bist read/write
		// data_mode = 0x11: with Error enject,  multi- bist write/read
		// data_mode = 0x12: with Error enject,  multi- bist write/read
		//         cvx16_bist_wdqlvl_init(data_mode, sram_sp);
		ddr_bist_wdqlvl_init(data_mode);
	}

	val = readl(DDR_BASE + 0x018c);
	val |= BIT(0);
	// param_phyd_dfi_wdqlvl_vref_train_en
	if (lvl_mode == 0x0)
		val &= ~BIT(10);
	else
		val |= BIT(10);
	// param_phyd_dfi_wdqlvl_bist_data_en
	if ((data_mode == 0x01) || (data_mode == 0x11) || (data_mode == 0x12))
		val |= BIT(4);
	else
		val &= ~BIT(4);
	writel(val, DDR_BASE + 0x018c);

	//[0] param_phyd_dfi_wrlvl_done
	//[1] param_phyd_dfi_rdglvl_done
	//[2] param_phyd_dfi_rdlvl_done
	//[3] param_phyd_dfi_wdqlvl_done
	while ((readl(DDR_BASE + 0x3444) & BIT(3)) == 0)
		;

	val = readl(DDR_CTRL_BASE + 0xC);
	val &= ~BIT(7);
	writel(val, DDR_CTRL_BASE + 0xC);

	// bist clock disable
	writel(0x00040000, DDR_BIST_BASE + 0x0);

	ddr_dfi_ca_park_prbs(0);

	// restore PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	// PWRCTL.selfref_sw
	FIELD_MOD(val, BIT(5), selfref_sw);
	// PWRCTL.en_dfi_dram_clk_disable
	FIELD_MOD(val, BIT(3), en_dfi_dram_clk_disable);
	// PWRCTL.powerdown_en
	FIELD_MOD(val, BIT(1), powerdown_en);
	// PWRCTL.selfref_en
	FIELD_MOD(val, BIT(0), selfref_en);
	writel(val, DDR_CTRL_BASE + 0x30);

	// Write 1 to PCTRL_n.port_en
	for (int i = 1; i < PORT_NUM; i++) {
		writel(0x1, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);
	}

	ddr_clk_enable_gating();
}

void ddr_rdvld_train(void)
{
	uint32_t byte0_vld;
	uint32_t byte1_vld;
	uint32_t rdvld_offset;
	int bist_result;
	uint64_t err_data_odd;
	uint64_t err_data_even;

	ddr_bist_wr_prbs_init();

	byte0_vld = readl(DDR_BASE + 0x0b14);
	byte1_vld = readl(DDR_BASE + 0x0b44);
	rdvld_offset = FIELD_GET(GENMASK(3, 0), readl(DDR_BASE + 0x0094));

	for (int i = 9; i > 1; i--) {
		FIELD_MOD(byte0_vld, GENMASK(20, 16), i);
		writel(byte0_vld, DDR_BASE + 0x0b14);
		FIELD_MOD(byte1_vld, GENMASK(20, 16), i);
		writel(byte0_vld, DDR_BASE + 0x0b44);

		bist_result = ddr_bist_start_check(&err_data_odd, &err_data_even);

		if (bist_result < 0) {
			INFO("DDR: vld end = %x, sel = %x\n", i, i + 1 + rdvld_offset);
			i = i + 1 + rdvld_offset;

			FIELD_MOD(byte0_vld, GENMASK(20, 16), i);
			writel(byte0_vld, DDR_BASE + 0x0b14);
			FIELD_MOD(byte1_vld, GENMASK(20, 16), i);
			writel(byte0_vld, DDR_BASE + 0x0b44);
			break;
		}
	}
}

void ddr_rdlvl_req(uint32_t mode)
{
	const uint32_t PORT_NUM = 4;
	uint32_t val;
	uint32_t selfref_sw;
	uint32_t en_dfi_dram_clk_disable;
	uint32_t powerdown_en;
	uint32_t selfref_en;
	uint32_t ddr3_mpr_mode;
	uint32_t vref_training_en;

	// Note: training need ctrl_low_patch first
	//  Write 0 to PCTRL_n.port_en, without port 0
	//  port number = 0,1,2,3
	for (unsigned int i = 1; i < PORT_NUM; i++)
		writel(0x0, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);

	// Poll PSTAT.rd_port_busy_n = 0
	// Poll PSTAT.wr_port_busy_n = 0
	DEBUG(ddr_polling_format, "PSTAT port_busy");

	while (readl(DDR_CTRL_BASE + 0x3fc) != 0)
		;

	DEBUG(ddr_polling_finsh);

	// disable PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	selfref_sw = FIELD_GET(BIT(5), val);
	en_dfi_dram_clk_disable = FIELD_GET(BIT(3), val);
	powerdown_en = FIELD_GET(BIT(1), val);
	selfref_en = FIELD_GET(BIT(0), val);
	val &= ~(BIT(5) | BIT(3) | BIT(1) | BIT(0));
	writel(val, DDR_CTRL_BASE + 0x30);

	ddr_clk_disable_gating();

	ddr_dfi_ca_park_prbs(1);

	// deskew start from 0x20
	val = readl(DDR_BASE + 0x0080);
	// param_phyd_pirdlvl_deskew_start
	FIELD_MOD(val, GENMASK(22, 16), 0x20);
	// param_phyd_pirdlvl_deskew_end
	FIELD_MOD(val, GENMASK(30, 24), 0x1f);
	writel(val, DDR_BASE + 0x0080);

	// save param_phyd_pirdlvl_vref_training_en
	val = readl(DDR_BASE + 0x008c);
	vref_training_en = FIELD_GET(BIT(2), val);
	// param_phyd_pirdlvl_rx_init_deskew_en
	// param_phyd_pirdlvl_vref_training_en
	// param_phyd_pirdlvl_rdvld_training_en = 0
	val &= ~(BIT(1) | BIT(2) | BIT(3));
	writel(val, DDR_BASE + 0x008c);

	if (ddr_type == DDR_TYPE_DDR3) {
		val = readl(DDR_BASE + 0x0188);
		ddr3_mpr_mode = FIELD_GET(BIT(4), val);

		if (ddr3_mpr_mode) {
			// RFSHCTL3.dis_auto_refresh =1
			val = readl(DDR_CTRL_BASE + 0x60);
			val |= BIT(0);
			writel(val, DDR_CTRL_BASE + 0x60);

			// MR3
			val = readl(DDR_CTRL_BASE + 0xe0);
			// Dataflow from MPR
			val |= BIT(2);

			ddr_synp_mrw(0x3, FIELD_GET(GENMASK(15, 0), val));
		}
	}

	// bist setting for dfi rdglvl
	ddr_bist_rdlvl_init(mode);

	val = readl(DDR_BASE + 0x0188);
	// param_phyd_dfi_rdlvl_req
	val |= BIT(0);
	writel(val, DDR_BASE + 0x0188);

	while ((readl(DDR_BASE + 0x3444) & BIT(2)) == 0)
		;

	if (vref_training_en == 0x1) {
		val = readl(DDR_BASE + 0x008c);
		// param_phyd_pirdlvl_vref_training_en
		val &= ~BIT(2);
		writel(val, DDR_BASE + 0x008c);

		val = readl(DDR_BASE + 0x0188);
		// param_phyd_dfi_rdlvl_req
		val |= BIT(0);
		writel(val, DDR_BASE + 0x0188);

		while ((readl(DDR_BASE + 0x3444) & BIT(2)) == 0)
			;

		val = readl(DDR_BASE + 0x008c);
		// param_phyd_pirdlvl_vref_training_en
		FIELD_MOD(val, BIT(2), vref_training_en);
		writel(val, DDR_BASE + 0x008c);
	}

	if (ddr_type == DDR_TYPE_DDR3) {
		if (ddr3_mpr_mode) {
			// MR3
			val = readl(DDR_CTRL_BASE + 0xe0);
			val &= ~BIT(2);

			ddr_synp_mrw(0x3, FIELD_GET(GENMASK(15, 0), val));

			// RFSHCTL3.dis_auto_refresh =0
			val = readl(DDR_CTRL_BASE + 0x60);
			val &= ~BIT(0);
			writel(val, DDR_CTRL_BASE + 0x60);
		}
	}

	ddr_rdvld_train();

	// bist clock disable
	writel(0x00040000, DDR_BIST_BASE + 0x0);

	ddr_dfi_ca_park_prbs(0);

	// // RFSHCTL3.dis_auto_refresh =0
	// val = readl(DDR_CTRL_BASE + 0x60);
	// val &= ~BIT(0); // RFSHCTL3.dis_auto_refresh
	// writel(val, DDR_CTRL_BASE + 0x60);
	// restore PWRCTL.powerdown_en, PWRCTL.selfref_en
	val = readl(DDR_CTRL_BASE + 0x30);
	// PWRCTL.selfref_sw
	FIELD_MOD(val, BIT(5), selfref_sw);
	// PWRCTL.en_dfi_dram_clk_disable
	FIELD_MOD(val, BIT(3), en_dfi_dram_clk_disable);
	// PWRCTL.powerdown_en
	FIELD_MOD(val, BIT(1), powerdown_en);
	// PWRCTL.selfref_en
	FIELD_MOD(val, BIT(0), selfref_en);
	writel(val, DDR_CTRL_BASE + 0x30);

	// Write 1 to PCTRL_n.port_en
	for (int i = 1; i < PORT_NUM; i++) {
		writel(0x1, (unsigned long)DDR_CTRL_BASE + 0x490 + 0xb0 * i);
	}

	ddr_clk_enable_gating();
}

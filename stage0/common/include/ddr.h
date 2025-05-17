
#ifndef _STAGE0_DDR_H
#define _STAGE0_DDR_H

#include "types.h"

#define DDR_VENDOR_UNKNOWN		0b00000
#define DDR_VENDOR_NY_4G		0b00001
#define DDR_VENDOR_NY_2G		0b00010
#define DDR_VENDOR_ESMT_1G		0b00011
#define DDR_VENDOR_ESMT_512M_DDR2	0b00100
#define DDR_VENDOR_ETRON_1G		0b00101
#define DDR_VENDOR_ESMT_2G		0b00110
#define DDR_VENDOR_PM_2G		0b00111
#define DDR_VENDOR_PM_1G		0b01000
#define DDR_VENDOR_ETRON_512M_DDR2	0b01001
#define DDR_VENDOR_ESMT_N25_1G		0b01010

#define DDR_CAPACITY_UNKNOWN		0b000
#define DDR_CAPACITY_512M		0b001
#define DDR_CAPACITY_1G			0b010
#define DDR_CAPACITY_2G			0b011
#define DDR_CAPACITY_4G			0b100

#define DDR_TYPE_UNKNOWN		0
#define DDR_TYPE_DDR2			1
#define DDR_TYPE_DDR3			2

extern uint8_t ddr_vendor;
extern uint8_t ddr_capacity;
extern uint8_t ddr_type;
extern uint16_t ddr_rate;

extern const char *ddr_finish_format;
extern const char *ddr_polling_format;
extern const char *ddr_polling_finsh;

#define DDR_PATCH_REG(_addr, _val) \
	{ .addr = (_addr), .val = (_val) }

struct ddr_patch_reg {
	uint32_t addr;
	uint32_t val;
};

#define DDR_MASK_PATCH_REG(_addr, _mask, _val) \
	{ .addr = (_addr), .mask = (_mask), .val = (_val) }

struct ddr_mask_patch_reg {
	uint32_t addr;
	uint32_t mask;
	uint32_t val;
};

void ddr_init(void);
void ddr_pinmux_init(void);
void ddr_update_patch_reg(const struct ddr_patch_reg *regs, unsigned int size);
void ddr_update_mask_patch_reg(const struct ddr_mask_patch_reg *regs, unsigned int size);

void ddr_pll_init(void);
void ddr_adjust_pll_freq(void);
void ddr_dll_cal(void);
void ddr_clk_disable_gating(void);
void ddr_clk_enable_gating(void);

void ddr_setting_check(void);
void ddr_en_rec_vol_mode(void);
void ddr_set_dfi_init_start(void);
void ddr_polling_dfi_init_start(void);
void ddr_set_dfi_init_complete(void);
void ddr_polling_dfi_init_complete(void);
void ddr_polling_sync_normal_mode(void);
void ddr_phy_power_on_seq1(void);
void ddr_phy_power_on_seq2(void);
void ddr_phy_power_on_seq3(void);
void ddr_int_isr_08(void);
void ddr_bist_wr_prbs_init(void);
int ddr_bist_start_check(uint64_t *err_data_odd, uint64_t *err_data_even);
void ddr_do_bist_prbs_check(void);
void ddr_bist_wr_sram_init(void);
void ddr_do_bist_sram_check(void);
void ddr_bist_wrlvl_init(void);
void ddr_bist_rdglvl_init(void);
void ddr_bist_wdmlvl_init(void);
void ddr_bist_wdqlvl_init(uint32_t mode);
void ddr_bist_rdlvl_init(uint32_t mode);
void ddr_synp_mrw(uint32_t addr, uint32_t data);
void ddr_rdvld_train(void);
void ddr_wrlvl_req(void);
void ddr_rdglvl_req(void);
void ddr_wdqlvl_req(uint32_t data_mode, uint32_t lvl_mode);
void ddr_rdlvl_req(uint32_t mode);
void ddr_dfi_ca_park_prbs(uint32_t cap_enable);

void ddr_init_detect_dram_size(uint8_t *dram_cap);
void ddr_init_update_by_dram_size(uint8_t dram_cap);

void axi_mon_latency_setting(uint32_t lat_bin_size_sel);
void axi_mon_start_all(void);

#endif /* _STAGE0_DDR_H */

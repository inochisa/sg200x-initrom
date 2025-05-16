
#include "common.h"
#include "ddr.h"

const struct ddr_patch_reg ddrc_patch[] = {
	DDR_PATCH_REG(0x08004000 + 0xc, 0x63746371),
	// PATCH0.use_blk_ext}:0:2:=0x1
	// PATCH0.dis_auto_ref_cnt_fix:2:1:=0x0
	// PATCH0.dis_auto_ref_algn_to_8:3:1:=0x0
	// PATCH0.starve_stall_at_dfi_ctrlupd:4:1:=0x1
	// PATCH0.starve_stall_at_abr:5:1:=0x1
	// PATCH0.dis_rdwr_switch_at_abr:6:1:=0x1
	// PATCH0.dfi_wdata_same_to_axi:7:1:=0x0
	// PATCH0.pagematch_limit_threshold:8:3=0x3
	// PATCH0.qos_sel:12:2:=0x2
	// PATCH0.burst_rdwr_xpi:16:4:=0x4
	// PATCH0.always_critical_when_urgent_hpr:20:1:=0x1
	// PATCH0.always_critical_when_urgent_lpr:21:1:=0x1
	// PATCH0.always_critical_when_urgent_wr:22:1:=0x1
	// PATCH0.disable_hif_rcmd_stall_path:24:1:=0x1
	// PATCH0.disable_hif_wcmd_stall_path:25:1:=0x1
	// PATCH0.derate_sys_en:29:1:=0x1
	// PATCH0.ref_4x_sys_high_temp:30:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x44, 0x00000000),
	// PATCH1.ref_adv_stop_threshold:0:7:=0x0
	// PATCH1.ref_adv_dec_threshold:8:7:=0x0
	// PATCH1.ref_adv_max:16:7:=0x0
	DDR_PATCH_REG(0x08004000 + 0x148, 0x999F0000),
	// PATCH4.t_phyd_rden:16:6=0x0
	// PATCH4.phyd_rd_clk_stop:23:1=0x0
	// PATCH4.t_phyd_wren:24:6=0x0
	// PATCH4.phyd_wr_clk_stop:31:1=0x0
	// auto gen.
	DDR_PATCH_REG(0x08004000 + 0x0, 0x81041401),
	DDR_PATCH_REG(0x08004000 + 0x30, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0x34, 0x00930001),
	DDR_PATCH_REG(0x08004000 + 0x38, 0x00020000),
	DDR_PATCH_REG(0x08004000 + 0x50, 0x00201070),
	DDR_PATCH_REG(0x08004000 + 0x60, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0x64, 0x007100A4),
	DDR_PATCH_REG(0x08004000 + 0xc0, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0xc4, 0x00000000),
#ifdef DDR_INIT_SPEED_UP
	DDR_PATCH_REG(0x08004000 + 0xd0, 0x00010002),
	DDR_PATCH_REG(0x08004000 + 0xd4, 0x00020000),
#else
	DDR_PATCH_REG(0x08004000 + 0xd0, 0x000100E5),
	DDR_PATCH_REG(0x08004000 + 0xd4, 0x006A0000),
#endif
	DDR_PATCH_REG(0x08004000 + 0xdc, 0x1F140040),
#ifdef DDR_DODT
	DDR_PATCH_REG(0x08004000 + 0xe0, 0x04600000),
#else
	DDR_PATCH_REG(0x08004000 + 0xe0, 0x00600000),
#endif
	DDR_PATCH_REG(0x08004000 + 0xe4, 0x000B03BF),
	DDR_PATCH_REG(0x08004000 + 0x100, 0x0E111F10),
	DDR_PATCH_REG(0x08004000 + 0x104, 0x00030417),
	DDR_PATCH_REG(0x08004000 + 0x108, 0x0507060A),
	DDR_PATCH_REG(0x08004000 + 0x10c, 0x00002007),
	DDR_PATCH_REG(0x08004000 + 0x110, 0x07020307),
	DDR_PATCH_REG(0x08004000 + 0x114, 0x05050303),
	DDR_PATCH_REG(0x08004000 + 0x120, 0x00000907),
	DDR_PATCH_REG(0x08004000 + 0x13c, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0x180, 0xC0960026),
	DDR_PATCH_REG(0x08004000 + 0x184, 0x00000001),
	// phyd related
	DDR_PATCH_REG(0x08004000 + 0x190, 0x048a8305),
	// DFITMG0.dfi_t_ctrl_delay:24:5:=0x4
	// DFITMG0.dfi_rddata_use_dfi_phy_clk:23:1:=0x1
	// DFITMG0.dfi_t_rddata_en:16:7:=0xa
	// DFITMG0.dfi_wrdata_use_dfi_phy_clk:15:1:=0x1
	// DFITMG0.dfi_tphy_wrdata:8:6:=0x3
	// DFITMG0.dfi_tphy_wrlat:0:6:=0x5
	DDR_PATCH_REG(0x08004000 + 0x194, 0x00070202),
	// DFITMG1.dfi_t_cmd_lat:28:4:=0x0
	// DFITMG1.dfi_t_parin_lat:24:2:=0x0
	// DFITMG1.dfi_t_wrdata_delay:16:5:=0x7
	// DFITMG1.dfi_t_dram_clk_disable:8:5:=0x2
	// DFITMG1.dfi_t_dram_clk_enable:0:5:=0x2
	DDR_PATCH_REG(0x08004000 + 0x198, 0x07c13121),
	// DFILPCFG0.dfi_tlp_resp:24:5:=0x7
	// DFILPCFG0.dfi_lp_wakeup_dpd:20:4:=0xc
	// DFILPCFG0.dfi_lp_en_dpd:16:1:=0x1
	// DFILPCFG0.dfi_lp_wakeup_sr:12:4:=0x3
	// DFILPCFG0.dfi_lp_en_sr:8:1:=0x1
	// DFILPCFG0.dfi_lp_wakeup_pd:4:4:=0x2
	// DFILPCFG0.dfi_lp_en_pd:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x19c, 0x00000021),
	// DFILPCFG1.dfi_lp_wakeup_mpsm:4:4:=0x2
	// DFILPCFG1.dfi_lp_en_mpsm:0:1:=0x1
	// auto gen.
	DDR_PATCH_REG(0x08004000 + 0x1a0, 0xC0400018),
	DDR_PATCH_REG(0x08004000 + 0x1a4, 0x00FE00FF),
	DDR_PATCH_REG(0x08004000 + 0x1a8, 0x80000000),
	DDR_PATCH_REG(0x08004000 + 0x1b0, 0x000002C1),
	DDR_PATCH_REG(0x08004000 + 0x1c0, 0x00000001),
	DDR_PATCH_REG(0x08004000 + 0x1c4, 0x00000001),
	// address map, auto gen.
	DDR_PATCH_REG(0x08004000 + 0x200, 0x00001F1F),
	DDR_PATCH_REG(0x08004000 + 0x204, 0x00070707),
	DDR_PATCH_REG(0x08004000 + 0x208, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0x20c, 0x1F000000),
	DDR_PATCH_REG(0x08004000 + 0x210, 0x00001F1F),
	DDR_PATCH_REG(0x08004000 + 0x214, 0x060F0606),
	DDR_PATCH_REG(0x08004000 + 0x218, 0x06060606),
	DDR_PATCH_REG(0x08004000 + 0x21c, 0x00000606),
	DDR_PATCH_REG(0x08004000 + 0x220, 0x00003F3F),
	DDR_PATCH_REG(0x08004000 + 0x224, 0x06060606),
	DDR_PATCH_REG(0x08004000 + 0x228, 0x06060606),
	DDR_PATCH_REG(0x08004000 + 0x22c, 0x001F1F06),
	// auto gen.
	DDR_PATCH_REG(0x08004000 + 0x240, 0x08000610),
#ifdef DDR_DODT
	DDR_PATCH_REG(0x08004000 + 0x244, 0x00000001),
#else
	DDR_PATCH_REG(0x08004000 + 0x244, 0x00000000),
#endif
	DDR_PATCH_REG(0x08004000 + 0x250, 0x00003F85),
	// SCHED.opt_vprw_sch:31:1:=0x0
	// SCHED.rdwr_idle_gap:24:7:=0x0
	// SCHED.go2critical_hysteresis:16:8:=0x0
	// SCHED.lpddr4_opt_act_timing:15:1:=0x0
	// SCHED.lpr_num_entries:8:7:=0x1f
	// SCHED.autopre_rmw:7:1:=0x1
	// SCHED.dis_opt_ntt_by_pre:6:1:=0x0
	// SCHED.dis_opt_ntt_by_act:5:1:=0x0
	// SCHED.opt_wrcam_fill_level:4:1:=0x0
	// SCHED.rdwr_switch_policy_sel:3:1:=0x0
	// SCHED.pageclose:2:1:=0x1
	// SCHED.prefer_write:1:1:=0x0
	// SCHED.dis_opt_wrecc_collision_flush:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x254, 0x00000000),
	// SCHED1.page_hit_limit_rd:28:3:=0x0
	// SCHED1.page_hit_limit_wr:24:3:=0x0
	// SCHED1.visible_window_limit_rd:20:3:=0x0
	// SCHED1.visible_window_limit_wr:16:3:=0x0
	// SCHED1.delay_switch_write:12:4:=0x0
	// SCHED1.pageclose_timer:0:8:=0x0
	// auto gen.
	DDR_PATCH_REG(0x08004000 + 0x25c, 0x100000F0),
	// PERFHPR1.hpr_xact_run_length:24:8:=0x20
	// PERFHPR1.hpr_max_starve:0:16:=0x6a
	DDR_PATCH_REG(0x08004000 + 0x264, 0x100000F0),
	// PERFLPR1.lpr_xact_run_length:24:8:=0x20
	// PERFLPR1.lpr_max_starve:0:16:=0x6a
	DDR_PATCH_REG(0x08004000 + 0x26c, 0x100000F0),
	// PERFWR1.w_xact_run_length:24:8:=0x20
	// PERFWR1.w_max_starve:0:16:=0x1a8
	DDR_PATCH_REG(0x08004000 + 0x300, 0x00000000),
	// DBG0.dis_max_rank_wr_opt:7:1:=0x0
	// DBG0.dis_max_rank_rd_opt:6:1:=0x0
	// DBG0.dis_collision_page_opt:4:1:=0x0
	// DBG0.dis_act_bypass:2:1:=0x0
	// DBG0.dis_rd_bypass:1:1:=0x0
	// DBG0.dis_wc:0:1:=0x0
	DDR_PATCH_REG(0x08004000 + 0x304, 0x00000000),
	// DBG1.dis_hif:1:1:=0x0
	// DBG1.dis_dq:0:1:=0x0
	DDR_PATCH_REG(0x08004000 + 0x30c, 0x00000000),
	DDR_PATCH_REG(0x08004000 + 0x320, 0x00000001),
	// SWCTL.sw_done:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x36c, 0x00000000),
	// POISONCFG.rd_poison_intr_clr:24:1:=0x0
	// POISONCFG.rd_poison_intr_en:20:1:=0x0
	// POISONCFG.rd_poison_slverr_en:16:1:=0x0
	// POISONCFG.wr_poison_intr_clr:8:1:=0x0
	// POISONCFG.wr_poison_intr_en:4:1:=0x0
	// POISONCFG.wr_poison_slverr_en:0:1:=0x0
	DDR_PATCH_REG(0x08004000 + 0x400, 0x00000011),
	// PCCFG.dch_density_ratio:12:2:=0x0
	// PCCFG.bl_exp_mode:8:1:=0x0
	// PCCFG.pagematch_limit:4:1:=0x1
	// PCCFG.go2critical_en:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x404, 0x00006000),
	// PCFGR_0.rdwr_ordered_en:16:1:=0x0
	// PCFGR_0.rd_port_pagematch_en:14:1:=0x1
	// PCFGR_0.rd_port_urgent_en:13:1:=0x1
	// PCFGR_0.rd_port_aging_en:12:1:=0x0
	// PCFGR_0.read_reorder_bypass_en:11:1:=0x0
	// PCFGR_0.rd_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x408, 0x00006000),
	// PCFGW_0.wr_port_pagematch_en:14:1:=0x1
	// PCFGW_0.wr_port_urgent_en:13:1:=0x1
	// PCFGW_0.wr_port_aging_en:12:1:=0x0
	// PCFGW_0.wr_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x490, 0x00000001),
	// PCTRL_0.port_en:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x494, 0x00000007),
	// PCFGQOS0_0.rqos_map_region2:24:8:=0x0
	// PCFGQOS0_0.rqos_map_region1:20:4:=0x0
	// PCFGQOS0_0.rqos_map_region0:16:4:=0x0
	// PCFGQOS0_0.rqos_map_level2:8:8:=0x0
	// PCFGQOS0_0.rqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x498, 0x0000006a),
	// PCFGQOS1_0.rqos_map_timeoutr:16:16:=0x0
	// PCFGQOS1_0.rqos_map_timeoutb:0:16:=0x6a
	DDR_PATCH_REG(0x08004000 + 0x49c, 0x00000e07),
	// PCFGWQOS0_0.wqos_map_region2:24:8:=0x0
	// PCFGWQOS0_0.wqos_map_region1:20:4:=0x0
	// PCFGWQOS0_0.wqos_map_region0:16:4:=0x0
	// PCFGWQOS0_0.wqos_map_level2:8:8:=0xe
	// PCFGWQOS0_0.wqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x4a0, 0x01a801a8),
	// PCFGWQOS1_0.wqos_map_timeout2:16:16:=0x1a8
	// PCFGWQOS1_0.wqos_map_timeout1:0:16:=0x1a8
	DDR_PATCH_REG(0x08004000 + 0x4b4, 0x00006000),
	// PCFGR_1.rdwr_ordered_en:16:1:=0x0
	// PCFGR_1.rd_port_pagematch_en:14:1:=0x1
	// PCFGR_1.rd_port_urgent_en:13:1:=0x1
	// PCFGR_1.rd_port_aging_en:12:1:=0x0
	// PCFGR_1.read_reorder_bypass_en:11:1:=0x0
	// PCFGR_1.rd_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x4b8, 0x00006000),
	// PCFGW_1.wr_port_pagematch_en:14:1:=0x1
	// PCFGW_1.wr_port_urgent_en:13:1:=0x1
	// PCFGW_1.wr_port_aging_en:12:1:=0x0
	// PCFGW_1.wr_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x540, 0x00000001),
	// PCTRL_1.port_en:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x544, 0x00000007),
	// PCFGQOS0_1.rqos_map_region2:24:8:=0x0
	// PCFGQOS0_1.rqos_map_region1:20:4:=0x0
	// PCFGQOS0_1.rqos_map_region0:16:4:=0x0
	// PCFGQOS0_1.rqos_map_level2:8:8:=0x0
	// PCFGQOS0_1.rqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x548, 0x0000006a),
	// PCFGQOS1_1.rqos_map_timeoutr:16:16:=0x0
	// PCFGQOS1_1.rqos_map_timeoutb:0:16:=0x6a
	DDR_PATCH_REG(0x08004000 + 0x54c, 0x00000e07),
	// PCFGWQOS0_1.wqos_map_region2:24:8:=0x0
	// PCFGWQOS0_1.wqos_map_region1:20:4:=0x0
	// PCFGWQOS0_1.wqos_map_region0:16:4:=0x0
	// PCFGWQOS0_1.wqos_map_level2:8:8:=0xe
	// PCFGWQOS0_1.wqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x550, 0x01a801a8),
	// PCFGWQOS1_1.wqos_map_timeout2:16:16:=0x1a8
	// PCFGWQOS1_1.wqos_map_timeout1:0:16:=0x1a8
	DDR_PATCH_REG(0x08004000 + 0x564, 0x00006000),
	// PCFGR_2.rdwr_ordered_en:16:1:=0x0
	// PCFGR_2.rd_port_pagematch_en:14:1:=0x1
	// PCFGR_2.rd_port_urgent_en:13:1:=0x1
	// PCFGR_2.rd_port_aging_en:12:1:=0x0
	// PCFGR_2.read_reorder_bypass_en:11:1:=0x0
	// PCFGR_2.rd_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x568, 0x00006000),
	// PCFGW_2.wr_port_pagematch_en:14:1:=0x1
	// PCFGW_2.wr_port_urgent_en:13:1:=0x1
	// PCFGW_2.wr_port_aging_en:12:1:=0x0
	// PCFGW_2.wr_port_priority:0:10:=0x0
	DDR_PATCH_REG(0x08004000 + 0x5f0, 0x00000001),
	// PCTRL_2.port_en:0:1:=0x1
	DDR_PATCH_REG(0x08004000 + 0x5f4, 0x00000007),
	// PCFGQOS0_2.rqos_map_region2:24:8:=0x0
	// PCFGQOS0_2.rqos_map_region1:20:4:=0x0
	// PCFGQOS0_2.rqos_map_region0:16:4:=0x0
	// PCFGQOS0_2.rqos_map_level2:8:8:=0x0
	// PCFGQOS0_2.rqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x5f8, 0x0000006a),
	// PCFGQOS1_2.rqos_map_timeoutr:16:16:=0x0
	// PCFGQOS1_2.rqos_map_timeoutb:0:16:=0x6a
	DDR_PATCH_REG(0x08004000 + 0x5fc, 0x00000e07),
	// PCFGWQOS0_2.wqos_map_region2:24:8:=0x0
	// PCFGWQOS0_2.wqos_map_region1:20:4:=0x0
	// PCFGWQOS0_2.wqos_map_region0:16:4:=0x0
	// PCFGWQOS0_2.wqos_map_level2:8:8:=0xe
	// PCFGWQOS0_2.wqos_map_level1:0:8:=0x7
	DDR_PATCH_REG(0x08004000 + 0x600, 0x01a801a8),
	// PCFGWQOS1_2.wqos_map_timeout2:16:16:=0x1a8
	// PCFGWQOS1_2.wqos_map_timeout1:0:16:=0x1a8
};

const unsigned int ddrc_patch_size = ARRAY_SIZE(ddrc_patch);


#define  DDR_PHY_REG_0_DATA  0b00000011000000100000000100000000
	// param_phyd_swap_ca0:[4:0]=0b00000
	// param_phyd_swap_ca1:[12:8]=0b00001
	// param_phyd_swap_ca2:[20:16]=0b00010
	// param_phyd_swap_ca3:[28:24]=0b00011
#define  DDR_PHY_REG_1_DATA  0b00000111000001100000010100000100
	// param_phyd_swap_ca4:[4:0]=0b00100
	// param_phyd_swap_ca5:[12:8]=0b00101
	// param_phyd_swap_ca6:[20:16]=0b00110
	// param_phyd_swap_ca7:[28:24]=0b00111
#define  DDR_PHY_REG_2_DATA  0b00001011000010100000100100001000
	// param_phyd_swap_ca8:[4:0]=0b01000
	// param_phyd_swap_ca9:[12:8]=0b01001
	// param_phyd_swap_ca10:[20:16]=0b01010
	// param_phyd_swap_ca11:[28:24]=0b01011
#define  DDR_PHY_REG_3_DATA  0b00001111000011100000110100001100
	// param_phyd_swap_ca12:[4:0]=0b01100
	// param_phyd_swap_ca13:[12:8]=0b01101
	// param_phyd_swap_ca14:[20:16]=0b01110
	// param_phyd_swap_ca15:[28:24]=0b01111
#define  DDR_PHY_REG_4_DATA  0b00010011000100100001000100010000
	// param_phyd_swap_ca16:[4:0]=0b10000
	// param_phyd_swap_ca17:[12:8]=0b10001
	// param_phyd_swap_ca18:[20:16]=0b10010
	// param_phyd_swap_ca19:[28:24]=0b10011
#define  DDR_PHY_REG_5_DATA  0b00000000000101100001010100010100
	// param_phyd_swap_ca20:[4:0]=0b10100
	// param_phyd_swap_ca21:[12:8]=0b10101
	// param_phyd_swap_ca22:[20:16]=0b10110
#define  DDR_PHY_REG_6_DATA  0b00000000000000000000000000000000
	// param_phyd_swap_cke0:[0:0]=0b0
	// param_phyd_swap_cs0:[4:4]=0b0
#define  DDR_PHY_REG_7_DATA  0b00000000000000000000000100000000
	// param_phyd_data_byte_swap_slice0:[1:0]=0b00
	// param_phyd_data_byte_swap_slice1:[9:8]=0b01
#define  DDR_PHY_REG_8_DATA  0b01110110010101000011001000010000
	// param_phyd_swap_byte0_dq0_mux:[3:0]=0b0000
	// param_phyd_swap_byte0_dq1_mux:[7:4]=0b0001
	// param_phyd_swap_byte0_dq2_mux:[11:8]=0b0010
	// param_phyd_swap_byte0_dq3_mux:[15:12]=0b0011
	// param_phyd_swap_byte0_dq4_mux:[19:16]=0b0100
	// param_phyd_swap_byte0_dq5_mux:[23:20]=0b0101
	// param_phyd_swap_byte0_dq6_mux:[27:24]=0b0110
	// param_phyd_swap_byte0_dq7_mux:[31:28]=0b0111
#define  DDR_PHY_REG_9_DATA  0b00000000000000000000000000001000
	// param_phyd_swap_byte0_dm_mux:[3:0]=0b1000
#define  DDR_PHY_REG_10_DATA  0b01110110010101000011001000010000
	// param_phyd_swap_byte1_dq0_mux:[3:0]=0b0000
	// param_phyd_swap_byte1_dq1_mux:[7:4]=0b0001
	// param_phyd_swap_byte1_dq2_mux:[11:8]=0b0010
	// param_phyd_swap_byte1_dq3_mux:[15:12]=0b0011
	// param_phyd_swap_byte1_dq4_mux:[19:16]=0b0100
	// param_phyd_swap_byte1_dq5_mux:[23:20]=0b0101
	// param_phyd_swap_byte1_dq6_mux:[27:24]=0b0110
	// param_phyd_swap_byte1_dq7_mux:[31:28]=0b0111
#define  DDR_PHY_REG_11_DATA  0b00000000000000000000000000001000
	// param_phyd_swap_byte1_dm_mux:[3:0]=0b1000
#define  DDR_PHY_REG_16_DATA  0b00000000000000000000000000000000
	// param_phyd_dll_rx_sw_mode:[0:0]=0b0
	// param_phyd_dll_rx_start_cal:[1:1]=0b0
	// param_phyd_dll_rx_cntr_mode:[2:2]=0b0
	// param_phyd_dll_rx_hwrst_time:[3:3]=0b0
	// param_phyd_dll_tx_sw_mode:[16:16]=0b0
	// param_phyd_dll_tx_start_cal:[17:17]=0b0
	// param_phyd_dll_tx_cntr_mode:[18:18]=0b0
	// param_phyd_dll_tx_hwrst_time:[19:19]=0b0
#define  DDR_PHY_REG_17_DATA  0b00000000011111110000000000001101
	// param_phyd_dll_slave_delay_en:[0:0]=0b1
	// param_phyd_dll_rw_en:[1:1]=0b0
	// param_phyd_dll_avg_mode:[2:2]=0b1
	// param_phyd_dll_upd_wait:[6:3]=0b0001
	// param_phyd_dll_sw_clr:[7:7]=0b0
	// param_phyd_dll_sw_code_mode:[8:8]=0b0
	// param_phyd_dll_sw_code:[23:16]=0b01111111
#define  DDR_PHY_REG_18_DATA  0b00000000000000000000000000000000
	// param_phya_reg_tx_clk_tx_dline_code_clkn0:[6:0]=0b0000000
	// param_phya_reg_tx_clk_tx_dline_code_clkp0:[14:8]=0b0000000
#define  DDR_PHY_REG_19_DATA  0b00000000000000000000000000001000
	// param_phya_reg_sel_ddr4_mode:[0:0]=0b0
	// param_phya_reg_sel_lpddr3_mode:[1:1]=0b0
	// param_phya_reg_sel_lpddr4_mode:[2:2]=0b0
	// param_phya_reg_sel_ddr3_mode:[3:3]=0b1
	// param_phya_reg_sel_ddr2_mode:[4:4]=0b0
#define  DDR_PHY_REG_20_DATA  0b00000000000000000000000000000110
	// param_phyd_dram_class:[3:0]=0b0110
#define  DDR_PHY_REG_21_DATA  0b00001100000000000000101100000000
	// param_phyd_wrlvl_start_delay_code:[6:0]=0b0000000
	// param_phyd_wrlvl_start_shift_code:[13:8]=0b001011
	// param_phyd_wrlvl_end_delay_code:[22:16]=0b0000000
	// param_phyd_wrlvl_end_shift_code:[29:24]=0b001100
#define  DDR_PHY_REG_22_DATA  0b00001001000101100000000001001111
	// param_phyd_wrlvl_capture_cnt:[3:0]=0b1111
	// param_phyd_wrlvl_dly_step:[7:4]=0b0100
	// param_phyd_wrlvl_disable:[11:8]=0b0000
	// param_phyd_wrlvl_resp_wait_cnt:[21:16]=0b010110
	// param_phyd_oenz_lead_cnt:[26:23]=0b0010
	// param_phyd_wrlvl_mode:[27:27]=0b1
#define  DDR_PHY_REG_23_DATA  0b00000000000000000000000000000000
	// param_phyd_wrlvl_sw:[0:0]=0b0
	// param_phyd_wrlvl_sw_upd_req:[1:1]=0b0
	// param_phyd_wrlvl_sw_resp:[2:2]=0b0
	// param_phyd_wrlvl_data_mask:[23:16]=0b00000000
#define  DDR_PHY_REG_24_DATA  0b00000100000000000000001101110000
	// param_phyd_pigtlvl_back_step:[7:0]=0b01110000
	// param_phyd_pigtlvl_capture_cnt:[11:8]=0b0011
	// param_phyd_pigtlvl_disable:[19:16]=0b0000
	// param_phyd_pigtlvl_dly_step:[27:24]=0b0100
#define  DDR_PHY_REG_25_DATA  0b00001101000000000000101000000000
	// param_phyd_pigtlvl_start_delay_code:[6:0]=0b0000000
	// param_phyd_pigtlvl_start_shift_code:[13:8]=0b001010
	// param_phyd_pigtlvl_end_delay_code:[22:16]=0b0000000
	// param_phyd_pigtlvl_end_shift_code:[29:24]=0b001101
#define  DDR_PHY_REG_26_DATA  0b00000000100000000000000000000000
	// param_phyd_pigtlvl_resp_wait_cnt:[5:0]=0b000000
	// param_phyd_pigtlvl_sw:[8:8]=0b0
	// param_phyd_pigtlvl_sw_resp:[13:12]=0b00
	// param_phyd_pigtlvl_sw_upd_req:[16:16]=0b0
	// param_phyd_rx_en_lead_cnt:[23:20]=0b1000
#define  DDR_PHY_REG_28_DATA  0b00000000000000000000000100001000
	// param_phyd_rgtrack_threshold:[4:0]=0b01000
	// param_phyd_rgtrack_dly_step:[11:8]=0b0001
	// param_phyd_rgtrack_disable:[19:16]=0b0000
#define  DDR_PHY_REG_29_DATA  0b00000000000001110010000000000000
	// param_phyd_zqcal_wait_count:[3:0]=0b0000
	// param_phyd_zqcal_cycle_count:[15:8]=0b00100000
	// param_phyd_zqcal_hw_mode:[18:16]=0b111
#define  DDR_PHY_REG_32_DATA  0b00011111000000001110000000000000
	// param_phyd_pirdlvl_dlie_code_start:[7:0]=0b00000000
	// param_phyd_pirdlvl_dlie_code_end:[15:8]=0b11100000
	// param_phyd_pirdlvl_deskew_start:[22:16]=0b0000000
	// param_phyd_pirdlvl_deskew_end:[30:24]=0b0011111
#define  DDR_PHY_REG_33_DATA  0b00000001000010110000110000001010
	// param_phyd_pirdlvl_trig_lvl_start:[4:0]=0b01010
	// param_phyd_pirdlvl_trig_lvl_end:[12:8]=0b01100
	// param_phyd_pirdlvl_rdvld_start:[20:16]=0b01011
	// param_phyd_pirdlvl_rdvld_end:[28:24]=0b00001
#define  DDR_PHY_REG_34_DATA  0b00001010000000010000000100010100
	// param_phyd_pirdlvl_dly_step:[3:0]=0b0100
	// param_phyd_pirdlvl_ds_dly_step:[7:4]=0b0001
	// param_phyd_pirdlvl_vref_step:[11:8]=0b0001
	// param_phyd_pirdlvl_disable:[15:12]=0b0000
	// param_phyd_pirdlvl_resp_wait_cnt:[21:16]=0b000001
	// param_phyd_pirdlvl_vref_wait_cnt:[31:24]=0b00001010
#define  DDR_PHY_REG_35_DATA  0b10101010101010100000000010001111
	// param_phyd_pirdlvl_rx_prebit_deskew_en:[0:0]=0b1
	// param_phyd_pirdlvl_rx_init_deskew_en:[1:1]=0b1
	// param_phyd_pirdlvl_vref_training_en:[2:2]=0b1
	// param_phyd_pirdlvl_rdvld_training_en:[3:3]=0b1
	// param_phyd_pirdlvl_capture_cnt:[7:4]=0b1000
	// param_phyd_pirdlvl_MR1520_BYTE:[15:8]=0b00000000
	// param_phyd_pirdlvl_MR3240:[31:16]=0b1010101010101010
#define  DDR_PHY_REG_36_DATA  0b00000000000000000011100000000000
	// param_phyd_pirdlvl_data_mask:[8:0]=0b000000000
	// param_phyd_pirdlvl_sw:[9:9]=0b0
	// param_phyd_pirdlvl_sw_upd_req:[10:10]=0b0
	// param_phyd_pirdlvl_sw_resp:[12:11]=0b11
	// param_phyd_pirdlvl_trig_lvl_dqs_follow_dq:[13:13]=0b1
#define  DDR_PHY_REG_37_DATA  0b00000000000000000000100000000001
	// param_phyd_pirdlvl_rdvld_offset:[3:0]=0b0001
	// param_phyd_pirdlvl_found_cnt_limite:[15:8]=0b00001000
#define  DDR_PHY_REG_40_DATA  0b00000111010000000000010101000000
	// param_phyd_piwdqlvl_start_delay_code:[6:0]=0b1000000
	// param_phyd_piwdqlvl_start_shift_code:[13:8]=0b000101
	// param_phyd_piwdqlvl_end_delay_code:[22:16]=0b1000000
	// param_phyd_piwdqlvl_end_shift_code:[29:24]=0b000111
#define  DDR_PHY_REG_41_DATA  0b00000001010000100000010100000100
	// param_phyd_piwdqlvl_tx_vref_start:[4:0]=0b00100
	// param_phyd_piwdqlvl_tx_vref_end:[12:8]=0b00101
	// param_phyd_piwdqlvl_capture_cnt:[19:16]=0b0010
	// param_phyd_piwdqlvl_dly_step:[23:20]=0b0100
	// param_phyd_piwdqlvl_vref_step:[27:24]=0b0001
	// param_phyd_piwdqlvl_disable:[31:28]=0b0000
#define  DDR_PHY_REG_42_DATA  0b00000000010101010000000000001010
	// param_phyd_piwdqlvl_vref_wait_cnt:[7:0]=0b00001010
	// param_phyd_piwdqlvl_tx_vref_training_en:[8:8]=0b0
	// param_phyd_piwdqlvl_byte_invert_0:[23:16]=0b01010101
#define  DDR_PHY_REG_43_DATA  0b00000000010101010011110001011010
	// param_phyd_piwdqlvl_dq_pattern_0:[15:0]=0b0011110001011010
	// param_phyd_piwdqlvl_byte_invert_1:[23:16]=0b01010101
#define  DDR_PHY_REG_44_DATA  0b00000000101010101010010111000011
	// param_phyd_piwdqlvl_dq_pattern_1:[15:0]=0b1010010111000011
	// param_phyd_piwdqlvl_byte_invert_2:[23:16]=0b10101010
#define  DDR_PHY_REG_45_DATA  0b00000000101010101111000011110000
	// param_phyd_piwdqlvl_dq_pattern_2:[15:0]=0b1111000011110000
	// param_phyd_piwdqlvl_byte_invert_3:[23:16]=0b10101010
#define  DDR_PHY_REG_46_DATA  0b00011110000000000000111100001111
	// param_phyd_piwdqlvl_dq_pattern_3:[15:0]=0b0000111100001111
	// param_phyd_piwdqlvl_data_mask:[24:16]=0b000000000
	// param_phyd_piwdqlvl_pattern_sel:[28:25]=0b1111
#define  DDR_PHY_REG_47_DATA  0b00000000000010000011111000010011
	// param_phyd_piwdqlvl_tdfi_phy_wrdata:[2:0]=0b011
	// param_phyd_piwdqlvl_oenz_lead_cnt:[6:3]=0b0010
	// param_phyd_piwdqlvl_sw:[7:7]=0b0
	// param_phyd_piwdqlvl_sw_upd_req:[8:8]=0b0
	// param_phyd_piwdqlvl_sw_resp:[10:9]=0b11
	// param_phyd_piwdqlvl_sw_result:[11:11]=0b1
	// param_phyd_piwdqlvl_dq_mode:[12:12]=0b1
	// param_phyd_piwdqlvl_dm_mode:[13:13]=0b1
	// param_phyd_piwdqlvl_found_cnt_limite:[23:16]=0b00001000
#define  DDR_PHY_REG_60_DATA  0b00000000000000000000000000000000
	// param_phyd_patch_revision:[31:0]=0b00000000000000000000000000000000
#define  DDR_PHY_REG_61_DATA  0b00000000000000110000000000110011
	// param_phyd_ca_shift_gating_en:[0:0]=0b1
	// param_phyd_cs_shift_gating_en:[1:1]=0b1
	// param_phyd_cke_shift_gating_en:[2:2]=0b0
	// param_phyd_resetz_shift_gating_en:[3:3]=0b0
	// param_phyd_tx_byte0_shift_gating_en:[4:4]=0b1
	// param_phyd_tx_byte1_shift_gating_en:[5:5]=0b1
	// param_phyd_rx_byte0_shift_gating_en:[16:16]=0b1
	// param_phyd_rx_byte1_shift_gating_en:[17:17]=0b1
#define  DDR_PHY_REG_62_DATA  0b00000000001000010000000000101100
	// param_phyd_lb_lfsr_seed0:[8:0]=0b000101100
	// param_phyd_lb_lfsr_seed1:[24:16]=0b000100001
#define  DDR_PHY_REG_63_DATA  0b00000000001101110000000000010110
	// param_phyd_lb_lfsr_seed2:[8:0]=0b000010110
	// param_phyd_lb_lfsr_seed3:[24:16]=0b000110111
#define  DDR_PHY_REG_64_DATA  0b00000100000000000000000000000000
	// param_phyd_lb_dq_en:[0:0]=0b0
	// param_phyd_lb_dq_go:[1:1]=0b0
	// param_phyd_lb_sw_en:[2:2]=0b0
	// param_phyd_lb_sw_rx_en:[3:3]=0b0
	// param_phyd_lb_sw_rx_mask:[4:4]=0b0
	// param_phyd_lb_sw_odt_en:[5:5]=0b0
	// param_phyd_lb_sw_ca_clkpattern:[6:6]=0b0
	// param_phyd_lb_sync_len:[31:16]=0b0000010000000000
#define  DDR_PHY_REG_65_DATA  0b00000000000000000000000000000000
	// param_phyd_lb_sw_dout0:[8:0]=0b000000000
	// param_phyd_lb_sw_dout1:[24:16]=0b000000000
#define  DDR_PHY_REG_67_DATA  0b00000000000000000000000000000000
	// param_phyd_lb_sw_oenz_dout0:[0:0]=0b0
	// param_phyd_lb_sw_oenz_dout1:[1:1]=0b0
	// param_phyd_lb_sw_dqsn0:[4:4]=0b0
	// param_phyd_lb_sw_dqsn1:[5:5]=0b0
	// param_phyd_lb_sw_dqsp0:[8:8]=0b0
	// param_phyd_lb_sw_dqsp1:[9:9]=0b0
	// param_phyd_lb_sw_oenz_dqs_dout0:[12:12]=0b0
	// param_phyd_lb_sw_oenz_dqs_dout1:[13:13]=0b0
#define  DDR_PHY_REG_68_DATA  0b00000000000000000000000000000000
	// param_phyd_lb_sw_ca_dout:[22:0]=0b00000000000000000000000
#define  DDR_PHY_REG_69_DATA  0b00000000000000000000000000000000
	// param_phyd_lb_sw_clkn0_dout:[0:0]=0b0
	// param_phyd_lb_sw_clkp0_dout:[4:4]=0b0
	// param_phyd_lb_sw_cke0_dout:[8:8]=0b0
	// param_phyd_lb_sw_resetz_dout:[12:12]=0b0
	// param_phyd_lb_sw_csb0_dout:[16:16]=0b0
#define  DDR_PHY_REG_70_DATA  0b00000000000000000000000000000000
	// param_phyd_clkctrl_init_complete:[0:0]=0b0
#define  DDR_PHY_REG_71_DATA  0b00000000000000000110101000010000
	// param_phyd_reg_resetz_dqs_rd_en:[4:4]=0b1
	// param_phyd_rx_upd_tx_sel:[9:8]=0b10
	// param_phyd_tx_upd_rx_sel:[11:10]=0b10
	// param_phyd_rx_en_ext_win:[15:12]=0b0110
	// param_phyd_fifo_rst_sel:[18:16]=0b000
	// param_phyd_fifo_sw_rst:[20:20]=0b0
#define  DDR_PHY_REG_72_DATA  0b00000000000000000000000000000000
	// param_phyd_phy_int_ack:[31:0]=0b00000000000000000000000000000000
#define  DDR_PHY_REG_73_DATA  0b11111111111111111111111011110111
	// param_phyd_phy_int_mask:[31:0]=0b11111111111111111111111011110111
#define  DDR_PHY_REG_74_DATA  0b00000000000000000000000000011111
	// param_phyd_calvl_rst_n:[0:0]=0b1
	// param_phyd_pigtlvl_rst_n:[1:1]=0b1
	// param_phyd_pirdlvl_rst_n:[2:2]=0b1
	// param_phyd_piwdqlvl_rst_n:[3:3]=0b1
	// param_phyd_wrlvl_rst_n:[4:4]=0b1
#define  DDR_PHY_REG_75_DATA  0b00000000000000000000000000000001
	// param_phyd_clk0_pol:[0:0]=0b1
#define  DDR_PHY_REG_76_DATA  0b00000000000000000000000100000001
	// param_phyd_tx_ca_oenz:[0:0]=0b1
	// param_phyd_tx_ca_clk0_oenz:[8:8]=0b1
#define  DDR_PHY_REG_77_DATA  0b00000000000000000000000100000000
	// param_phya_reg_en_test:[0:0]=0b0
	// param_phya_reg_tx_ca_test_en:[1:1]=0b0
	// param_phya_reg_tx_ca_en_ca_loop_back:[2:2]=0b0
	// param_phya_reg_tx_sel_4bit_mode_tx:[8:8]=0b1
	// param_phya_reg_tx_gpio_in:[16:16]=0b0
#define  DDR_PHY_REG_78_DATA  0b00000000000000000000000000010100
	// param_phya_reg_rx_en_ca_train_mode:[0:0]=0b0
	// param_phya_reg_rx_en_pream_train_mode:[1:1]=0b0
	// param_phya_reg_rx_sel_dqs_wo_pream_mode:[2:2]=0b1
	// param_phya_reg_rx_en_rec_offset:[3:3]=0b0
	// param_phya_reg_rx_sel_4bit_mode_rx:[4:4]=0b1
#define  DDR_PHY_REG_80_DATA  0b00000000000000000000000000000000
	// param_phya_reg_rx_ddrdll_enautok:[0:0]=0b0
	// param_phya_reg_rx_ddrdll_rstb:[1:1]=0b0
	// param_phya_reg_rx_ddrdll_selckout:[5:4]=0b00
	// param_phya_reg_rx_ddrdll_test:[7:6]=0b00
	// param_phya_reg_rx_ddrdll_sel:[15:8]=0b00000000
	// param_phya_reg_tx_ddrdll_enautok:[16:16]=0b0
	// param_phya_reg_tx_ddrdll_rstb:[17:17]=0b0
	// param_phya_reg_tx_ddrdll_selckout:[21:20]=0b00
	// param_phya_reg_tx_ddrdll_test:[23:22]=0b00
	// param_phya_reg_tx_ddrdll_sel:[31:24]=0b00000000
#define  DDR_PHY_REG_81_DATA  0b00000000000000000000000000000000
	// param_phya_reg_tx_zq_cmp_en:[0:0]=0b0
	// param_phya_reg_tx_zq_cmp_offset_cal_en:[1:1]=0b0
	// param_phya_reg_tx_zq_ph_en:[2:2]=0b0
	// param_phya_reg_tx_zq_pl_en:[3:3]=0b0
	// param_phya_reg_tx_zq_step2_en:[4:4]=0b0
	// param_phya_reg_tx_zq_cmp_offset:[12:8]=0b00000
	// param_phya_reg_tx_zq_sel_vref:[20:16]=0b00000
#define  DDR_PHY_REG_82_DATA  0b00000000000000000000100000001000
	// param_phya_reg_tx_zq_golden_drvn:[4:0]=0b01000
	// param_phya_reg_tx_zq_golden_drvp:[12:8]=0b01000
	// param_phya_reg_tx_zq_drvn:[20:16]=0b00000
	// param_phya_reg_tx_zq_drvp:[28:24]=0b00000
#define  DDR_PHY_REG_83_DATA  0b00000000000000000000000000000000
	// param_phya_reg_tx_zq_en_test_aux:[0:0]=0b0
	// param_phya_reg_tx_zq_en_test_mux:[1:1]=0b0
	// param_phya_reg_sel_zq_high_swing:[2:2]=0b0
	// param_phya_reg_zq_sel_test_out0:[7:4]=0b0000
	// param_phya_reg_zq_sel_test_out1:[11:8]=0b0000
	// param_phya_reg_tx_zq_sel_test_ana_in:[15:12]=0b0000
	// param_phya_reg_tx_zq_sel_gpio_in:[17:16]=0b00
#define  DDR_PHY_REG_84_DATA  0b00000000000000000000000000000101
	// param_phya_reg_tune_damp_r:[3:0]=0b0101
#define  DDR_PHY_REG_85_DATA  0b00000000000000000000000100000001
	// param_phyd_sel_cke_oenz:[0:0]=0b1
	// param_phyd_tx_dqsn_default_value:[8:8]=0b1
	// param_phyd_tx_dqsp_default_value:[12:12]=0b0
	// param_phyd_ddr4_2t_preamble:[16:16]=0b0
#define  DDR_PHY_REG_86_DATA  0b00000000000000000000000000000000
	// param_phya_reg_zqcal_done:[0:0]=0b0
#define  DDR_PHY_REG_87_DATA  0b00000000000000000000000000000000
	// param_phyd_dbg_sel:[7:0]=0b00000000
	// param_phyd_dbg_sel_en:[16:16]=0b0
#define  DDR_PHY_REG_89_DATA  0b00000000000000000000000000000000
	// param_phyd_reg_dfs_sel:[0:0]=0b0
#define  DDR_PHY_REG_90_DATA  0b00000000001100110011001100110001
	// param_phyd_ca_sw_dline_en:[0:0]=0b1
	// param_phyd_byte0_wr_sw_dline_en:[4:4]=0b1
	// param_phyd_byte1_wr_sw_dline_en:[5:5]=0b1
	// param_phyd_byte0_wdqs_sw_dline_en:[8:8]=0b1
	// param_phyd_byte1_wdqs_sw_dline_en:[9:9]=0b1
	// param_phyd_byte0_rd_sw_dline_en:[12:12]=0b1
	// param_phyd_byte1_rd_sw_dline_en:[13:13]=0b1
	// param_phyd_byte0_rdg_sw_dline_en:[16:16]=0b1
	// param_phyd_byte1_rdg_sw_dline_en:[17:17]=0b1
	// param_phyd_byte0_rdqs_sw_dline_en:[20:20]=0b1
	// param_phyd_byte1_rdqs_sw_dline_en:[21:21]=0b1
#define  DDR_PHY_REG_91_DATA  0b00000000000000000000000000000000
	// param_phyd_ca_raw_dline_upd:[0:0]=0b0
	// param_phyd_byte0_wr_raw_dline_upd:[4:4]=0b0
	// param_phyd_byte1_wr_raw_dline_upd:[5:5]=0b0
	// param_phyd_byte0_wdqs_raw_dline_upd:[8:8]=0b0
	// param_phyd_byte1_wdqs_raw_dline_upd:[9:9]=0b0
	// param_phyd_byte0_rd_raw_dline_upd:[12:12]=0b0
	// param_phyd_byte1_rd_raw_dline_upd:[13:13]=0b0
	// param_phyd_byte0_rdg_raw_dline_upd:[16:16]=0b0
	// param_phyd_byte1_rdg_raw_dline_upd:[17:17]=0b0
	// param_phyd_byte0_rdqs_raw_dline_upd:[20:20]=0b0
	// param_phyd_byte1_rdqs_raw_dline_upd:[21:21]=0b0
#define  DDR_PHY_REG_92_DATA  0b00000000000000000000000000000000
	// param_phyd_sw_dline_upd_req:[0:0]=0b0
#define  DDR_PHY_REG_93_DATA  0b00000000000000000000000100000000
	// param_phyd_sw_dfi_phyupd_req:[0:0]=0b0
	// param_phyd_sw_dfi_phyupd_req_clr:[4:4]=0b0
	// param_phyd_sw_phyupd_dline:[8:8]=0b1
#define  DDR_PHY_REG_96_DATA  0b00000000000000100000000000010000
	// param_phyd_dfi_wrlvl_req:[0:0]=0b0
	// param_phyd_dfi_wrlvl_odt_en:[4:4]=0b1
	// param_phyd_dfi_wrlvl_strobe_cnt:[19:16]=0b0010
#define  DDR_PHY_REG_97_DATA  0b00000000000000000000000000000000
	// param_phyd_dfi_rdglvl_req:[0:0]=0b0
	// param_phyd_dfi_rdglvl_ddr3_mpr:[4:4]=0b0
#define  DDR_PHY_REG_98_DATA  0b00000000000000000000000000000000
	// param_phyd_dfi_rdlvl_req:[0:0]=0b0
	// param_phyd_dfi_rdlvl_ddr3_mpr:[4:4]=0b0
#define  DDR_PHY_REG_99_DATA  0b00000000000010010000010000000000
	// param_phyd_dfi_wdqlvl_req:[0:0]=0b0
	// param_phyd_dfi_wdqlvl_bist_data_en:[4:4]=0b0
	// param_phyd_dfi_wdqlvl_vref_train_en:[10:10]=0b1
	// param_phyd_dfi_wdqlvl_vref_wait_cnt:[23:16]=0b00001001
#define  DDR_PHY_REG_100_DATA  0b00000000000000100001001000001110
	// param_phyd_dfi_wdqlvl_vref_start:[6:0]=0b0001110
	// param_phyd_dfi_wdqlvl_vref_end:[14:8]=0b0010010
	// param_phyd_dfi_wdqlvl_vref_step:[19:16]=0b0010
#define  DDR_PHY_REG_128_DATA  0b00000000000000000000000000000000
	// param_phya_reg_byte0_test_en:[0:0]=0b0
	// param_phya_reg_tx_byte0_ddr_test:[15:8]=0b00000000
	// param_phya_reg_rx_byte0_sel_test_in0:[19:16]=0b0000
	// param_phya_reg_rx_byte0_sel_test_in1:[23:20]=0b0000
#define  DDR_PHY_REG_129_DATA  0b00000000000000000000010001000000
	// param_phya_reg_tx_byte0_en_rx_awys_on:[0:0]=0b0
	// param_phya_reg_tx_byte0_sel_en_rx_dly:[5:4]=0b00
	// param_phya_reg_rx_byte0_sel_en_rx_gen_rst:[6:6]=0b1
	// param_phya_reg_byte0_mask_oenz:[8:8]=0b0
	// param_phya_reg_tx_byte0_en_mask:[10:10]=0b1
	// param_phya_reg_rx_byte0_sel_cnt_mode:[13:12]=0b00
	// param_phya_reg_tx_byte0_sel_int_loop_back:[14:14]=0b0
	// param_phya_reg_rx_byte0_sel_dqs_dly_for_gated:[17:16]=0b00
	// param_phya_reg_tx_byte0_en_extend_oenz_gated_dline:[18:18]=0b0
#define  DDR_PHY_REG_130_DATA  0b00000000000000000000000000000000
	// param_phyd_reg_reserved_byte0:[31:0]=0b00000000000000000000000000000000
#define  DDR_PHY_REG_136_DATA  0b00000000000000000000000000000000
	// param_phya_reg_byte1_test_en:[0:0]=0b0
	// param_phya_reg_tx_byte1_ddr_test:[15:8]=0b00000000
	// param_phya_reg_rx_byte1_sel_test_in0:[19:16]=0b0000
	// param_phya_reg_rx_byte1_sel_test_in1:[23:20]=0b0000
#define  DDR_PHY_REG_137_DATA  0b00000000000000000000010001000000
	// param_phya_reg_tx_byte1_en_rx_awys_on:[0:0]=0b0
	// param_phya_reg_tx_byte1_sel_en_rx_dly:[5:4]=0b00
	// param_phya_reg_rx_byte1_sel_en_rx_gen_rst:[6:6]=0b1
	// param_phya_reg_byte1_mask_oenz:[8:8]=0b0
	// param_phya_reg_tx_byte1_en_mask:[10:10]=0b1
	// param_phya_reg_rx_byte1_sel_cnt_mode:[13:12]=0b00
	// param_phya_reg_tx_byte1_sel_int_loop_back:[14:14]=0b0
	// param_phya_reg_rx_byte1_sel_dqs_dly_for_gated:[17:16]=0b00
	// param_phya_reg_tx_byte1_en_extend_oenz_gated_dline:[18:18]=0b0
#define  DDR_PHY_REG_138_DATA  0b00000000000000000000000000000000
	// param_phyd_reg_reserved_byte1:[31:0]=0b00000000000000000000000000000000
#define  DDR_PHY_REG_0_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_ca_sel_lpddr4_pmos_ph_ca:[3:3]=0b0
	// f0_param_phya_reg_tx_clk_sel_lpddr4_pmos_ph_clk:[4:4]=0b0
	// f0_param_phya_reg_tx_sel_lpddr4_pmos_ph:[5:5]=0b0
#define  DDR_PHY_REG_1_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_ca_drvn_de:[1:0]=0b00
	// f0_param_phya_reg_tx_ca_drvp_de:[5:4]=0b00
	// f0_param_phya_reg_tx_clk0_drvn_de:[9:8]=0b00
	// f0_param_phya_reg_tx_clk0_drvp_de:[13:12]=0b00
	// f0_param_phya_reg_tx_csb_drvn_de:[17:16]=0b00
	// f0_param_phya_reg_tx_csb_drvp_de:[21:20]=0b00
	// f0_param_phya_reg_tx_ca_en_tx_de:[24:24]=0b0
	// f0_param_phya_reg_tx_clk0_en_tx_de:[28:28]=0b0
	// f0_param_phya_reg_tx_csb_en_tx_de:[30:30]=0b0
#define  DDR_PHY_REG_2_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_ca_sel_dly1t_ca:[22:0]=0b00000000000000000000000
#define  DDR_PHY_REG_3_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_clk_sel_dly1t_clk0:[0:0]=0b0
	// f0_param_phya_reg_tx_ca_sel_dly1t_cke0:[8:8]=0b0
	// f0_param_phya_reg_tx_ca_sel_dly1t_csb0:[16:16]=0b0
#define  DDR_PHY_REG_4_F0_DATA  0b00000000000100000000000000000000
	// f0_param_phya_reg_tx_vref_en_free_offset:[0:0]=0b0
	// f0_param_phya_reg_tx_vref_en_rangex2:[1:1]=0b0
	// f0_param_phya_reg_tx_vref_sel_lpddr4divby2p5:[2:2]=0b0
	// f0_param_phya_reg_tx_vref_sel_lpddr4divby3:[3:3]=0b0
	// f0_param_phya_reg_tx_vref_offset:[14:8]=0b0000000
	// f0_param_phya_reg_tx_vref_sel:[20:16]=0b10000
#define  DDR_PHY_REG_5_F0_DATA  0b00000000000100000000000000000000
	// f0_param_phya_reg_tx_vrefca_en_free_offset:[0:0]=0b0
	// f0_param_phya_reg_tx_vrefca_en_rangex2:[1:1]=0b0
	// f0_param_phya_reg_tx_vrefca_offset:[14:8]=0b0000000
	// f0_param_phya_reg_tx_vrefca_sel:[20:16]=0b10000
#define  DDR_PHY_REG_6_F0_DATA  0b00000000000000000000000000000010
	// f0_param_phyd_tx_byte_dqs_extend:[2:0]=0b010
#define  DDR_PHY_REG_7_F0_DATA  0b00000000000000000100000001000000
	// f0_param_phya_reg_rx_byte0_odt_reg:[4:0]=0b00000
	// f0_param_phya_reg_rx_byte0_sel_odt_reg_mode:[6:6]=0b1
	// f0_param_phya_reg_rx_byte1_odt_reg:[12:8]=0b00000
	// f0_param_phya_reg_rx_byte1_sel_odt_reg_mode:[14:14]=0b1
#define  DDR_PHY_REG_64_F0_DATA  0b00000000000000000001000000000001
	// f0_param_phya_reg_rx_byte0_en_lsmode:[0:0]=0b1
	// f0_param_phya_reg_rx_byte0_hystr:[5:4]=0b00
	// f0_param_phya_reg_rx_byte0_sel_dqs_rec_vref_mode:[8:8]=0b0
	// f0_param_phya_reg_rx_byte0_sel_odt_center_tap:[10:10]=0b0
	// f0_param_phya_reg_byte0_en_rec_vol_mode:[12:12]=0b1
	// f0_param_phya_reg_tx_byte0_force_en_lvstl_ph:[14:14]=0b0
	// f0_param_phya_reg_rx_byte0_force_en_lvstl_odt:[16:16]=0b0
	// f0_param_phya_reg_rx_byte0_en_trig_lvl_rangex2:[18:18]=0b0
	// f0_param_phya_reg_rx_byte0_trig_lvl_en_free_offset:[20:20]=0b0
#define  DDR_PHY_REG_65_F0_DATA  0b00000000000100000000000000000000
	// f0_param_phya_reg_tx_byte0_drvn_de_dq:[1:0]=0b00
	// f0_param_phya_reg_tx_byte0_drvp_de_dq:[5:4]=0b00
	// f0_param_phya_reg_tx_byte0_drvn_de_dqs:[9:8]=0b00
	// f0_param_phya_reg_tx_byte0_drvp_de_dqs:[13:12]=0b00
	// f0_param_phya_reg_tx_byte0_en_tx_de_dq:[16:16]=0b0
	// f0_param_phya_reg_tx_byte0_en_tx_de_dqs:[20:20]=0b1
#define  DDR_PHY_REG_66_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_byte0_sel_dly1t_dq:[8:0]=0b000000000
	// f0_param_phya_reg_tx_byte0_sel_dly1t_dqs:[12:12]=0b0
	// f0_param_phya_reg_tx_byte0_sel_dly1t_mask_ranka:[16:16]=0b0
#define  DDR_PHY_REG_67_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_byte0_vref_sel_lpddr4divby2p5:[0:0]=0b0
	// f0_param_phya_reg_tx_byte0_vref_sel_lpddr4divby3:[4:4]=0b0
	// f0_param_phya_reg_tx_byte0_vref_sel_lpddr4x_voh0p5:[8:8]=0b0
	// f0_param_phya_reg_tx_byte0_vref_sel_lpddr4x_voh0p6:[12:12]=0b0
#define  DDR_PHY_REG_68_F0_DATA  0b00000000000000000000000000000100
	// f0_param_phyd_reg_rx_byte0_resetz_dqs_offset:[3:0]=0b0100
#define  DDR_PHY_REG_69_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte0_dq0_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte0_dq1_offset:[14:8]=0b0000000
	// f0_param_phyd_reg_byte0_dq2_offset:[22:16]=0b0000000
	// f0_param_phyd_reg_byte0_dq3_offset:[30:24]=0b0000000
#define  DDR_PHY_REG_70_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte0_dq4_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte0_dq5_offset:[14:8]=0b0000000
	// f0_param_phyd_reg_byte0_dq6_offset:[22:16]=0b0000000
	// f0_param_phyd_reg_byte0_dq7_offset:[30:24]=0b0000000
#define  DDR_PHY_REG_71_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte0_dm_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte0_dqsn_offset:[19:16]=0b0000
	// f0_param_phyd_reg_byte0_dqsp_offset:[27:24]=0b0000
#define  DDR_PHY_REG_72_F0_DATA  0b00000000000000000000000000000011
	// f0_param_phyd_tx_byte0_tx_oenz_extend:[2:0]=0b011
#define  DDR_PHY_REG_80_F0_DATA  0b00000000000000000001000000000001
	// f0_param_phya_reg_rx_byte1_en_lsmode:[0:0]=0b1
	// f0_param_phya_reg_rx_byte1_hystr:[5:4]=0b00
	// f0_param_phya_reg_rx_byte1_sel_dqs_rec_vref_mode:[8:8]=0b0
	// f0_param_phya_reg_rx_byte1_sel_odt_center_tap:[10:10]=0b0
	// f0_param_phya_reg_byte1_en_rec_vol_mode:[12:12]=0b1
	// f0_param_phya_reg_tx_byte1_force_en_lvstl_ph:[14:14]=0b0
	// f0_param_phya_reg_rx_byte1_force_en_lvstl_odt:[16:16]=0b0
	// f0_param_phya_reg_rx_byte1_en_trig_lvl_rangex2:[18:18]=0b0
	// f0_param_phya_reg_rx_byte1_trig_lvl_en_free_offset:[20:20]=0b0
#define  DDR_PHY_REG_81_F0_DATA  0b00000000000100000000000000000000
	// f0_param_phya_reg_tx_byte1_drvn_de_dq:[1:0]=0b00
	// f0_param_phya_reg_tx_byte1_drvp_de_dq:[5:4]=0b00
	// f0_param_phya_reg_tx_byte1_drvn_de_dqs:[9:8]=0b00
	// f0_param_phya_reg_tx_byte1_drvp_de_dqs:[13:12]=0b00
	// f0_param_phya_reg_tx_byte1_en_tx_de_dq:[16:16]=0b0
	// f0_param_phya_reg_tx_byte1_en_tx_de_dqs:[20:20]=0b1
#define  DDR_PHY_REG_82_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_byte1_sel_dly1t_dq:[8:0]=0b000000000
	// f0_param_phya_reg_tx_byte1_sel_dly1t_dqs:[12:12]=0b0
	// f0_param_phya_reg_tx_byte1_sel_dly1t_mask_ranka:[16:16]=0b0
#define  DDR_PHY_REG_83_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_tx_byte1_vref_sel_lpddr4divby2p5:[0:0]=0b0
	// f0_param_phya_reg_tx_byte1_vref_sel_lpddr4divby3:[4:4]=0b0
	// f0_param_phya_reg_tx_byte1_vref_sel_lpddr4x_voh0p5:[8:8]=0b0
	// f0_param_phya_reg_tx_byte1_vref_sel_lpddr4x_voh0p6:[12:12]=0b0
#define  DDR_PHY_REG_84_F0_DATA  0b00000000000000000000000000000100
	// f0_param_phyd_reg_rx_byte1_resetz_dqs_offset:[3:0]=0b0100
#define  DDR_PHY_REG_85_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte1_dq0_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte1_dq1_offset:[14:8]=0b0000000
	// f0_param_phyd_reg_byte1_dq2_offset:[22:16]=0b0000000
	// f0_param_phyd_reg_byte1_dq3_offset:[30:24]=0b0000000
#define  DDR_PHY_REG_86_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte1_dq4_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte1_dq5_offset:[14:8]=0b0000000
	// f0_param_phyd_reg_byte1_dq6_offset:[22:16]=0b0000000
	// f0_param_phyd_reg_byte1_dq7_offset:[30:24]=0b0000000
#define  DDR_PHY_REG_87_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_byte1_dm_offset:[6:0]=0b0000000
	// f0_param_phyd_reg_byte1_dqsn_offset:[19:16]=0b0000
	// f0_param_phyd_reg_byte1_dqsp_offset:[27:24]=0b0000
#define  DDR_PHY_REG_88_F0_DATA  0b00000000000000000000000000000011
	// f0_param_phyd_tx_byte1_tx_oenz_extend:[2:0]=0b011
#define  DDR_PHY_REG_320_F0_DATA  0b00000000000000000000010000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_ca0_sw:[6:0]=0b0000000
	// f0_param_phyd_tx_ca0_shift_sel:[13:8]=0b000100
#define  DDR_PHY_REG_331_F0_DATA  0b00000000000000000000010000000000
	// f0_param_phyd_tx_ca22_shift_sel:[13:8]=0b000100
#define  DDR_PHY_REG_332_F0_DATA  0b00000000000000000000010000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_cke0_sw:[6:0]=0b0000000
	// f0_param_phyd_tx_cke0_shift_sel:[13:8]=0b000100
#define  DDR_PHY_REG_333_F0_DATA  0b00000000000000000000010000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_csb0_sw:[6:0]=0b0000000
	// f0_param_phyd_tx_cs0_shift_sel:[13:8]=0b000100
#define  DDR_PHY_REG_334_F0_DATA  0b00000000000000000000010000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_resetz_sw:[6:0]=0b0000000
	// f0_param_phyd_tx_reset_shift_sel:[13:8]=0b000100
#define  DDR_PHY_REG_336_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_ca0_raw:[6:0]=0b0000000
#define  DDR_PHY_REG_348_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_cke0_raw:[6:0]=0b0000000
#define  DDR_PHY_REG_349_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_csb0_raw:[6:0]=0b0000000
#define  DDR_PHY_REG_350_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_ca_tx_dline_code_resetz_raw:[6:0]=0b0000000
#define  DDR_PHY_REG_351_F0_DATA  0b00001000000010000000010000000100
	// f0_param_phya_reg_tx_ca_drvn_ca:[4:0]=0b00100
	// f0_param_phya_reg_tx_ca_drvp_ca:[12:8]=0b00100
	// f0_param_phya_reg_tx_ca_drvn_csb:[20:16]=0b01000
	// f0_param_phya_reg_tx_ca_drvp_csb:[28:24]=0b01000
#define  DDR_PHY_REG_352_F0_DATA  0b00001000000010000000100000001000
	// f0_param_phya_reg_tx_clk_drvn_clkn0:[4:0]=0b01000
	// f0_param_phya_reg_tx_clk_drvp_clkn0:[12:8]=0b01000
	// f0_param_phya_reg_tx_clk_drvn_clkp0:[20:16]=0b01000
	// f0_param_phya_reg_tx_clk_drvp_clkp0:[28:24]=0b01000
#define  DDR_PHY_REG_384_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq0_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte0_bit0_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq1_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte0_bit1_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_385_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq2_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte0_bit2_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq3_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte0_bit3_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_386_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq4_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte0_bit4_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq5_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte0_bit5_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_387_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq6_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte0_bit6_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq7_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte0_bit7_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_388_F0_DATA  0b00000000000000000000011001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq8_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte0_bit8_data_shift:[13:8]=0b000110
#define  DDR_PHY_REG_389_F0_DATA  0b00001011000000000000000000000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dqsn_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dqsp_sw:[22:16]=0b0000000
	// f0_param_phyd_tx_byte0_dqs_shift:[29:24]=0b001011
#define  DDR_PHY_REG_390_F0_DATA  0b00001010000000000000100100000000
	// f0_param_phyd_tx_byte0_oenz_dqs_shift:[13:8]=0b001001
	// f0_param_phyd_tx_byte0_oenz_shift:[29:24]=0b001010
#define  DDR_PHY_REG_391_F0_DATA  0b00000000000001000000011000000000
	// f0_param_phyd_tx_byte0_oenz_dqs_extend:[11:8]=0b0110
	// f0_param_phyd_tx_byte0_oenz_extend:[19:16]=0b0100
#define  DDR_PHY_REG_392_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq0_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq1_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_393_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq2_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq3_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_394_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq4_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq5_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_395_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq6_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq7_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_396_F0_DATA  0b00000000000000000000000001000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dq8_raw:[6:0]=0b1000000
#define  DDR_PHY_REG_397_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dqsn_raw:[6:0]=0b0000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_dqsp_raw:[22:16]=0b0000000
#define  DDR_PHY_REG_398_F0_DATA  0b00000000000000000000100000001000
	// f0_param_phya_reg_tx_byte0_drvn_dq:[4:0]=0b01000
	// f0_param_phya_reg_tx_byte0_drvp_dq:[12:8]=0b01000
#define  DDR_PHY_REG_399_F0_DATA  0b00001000000010000000100000001000
	// f0_param_phya_reg_tx_byte0_drvn_dqsn:[4:0]=0b01000
	// f0_param_phya_reg_tx_byte0_drvp_dqsn:[12:8]=0b01000
	// f0_param_phya_reg_tx_byte0_drvn_dqsp:[20:16]=0b01000
	// f0_param_phya_reg_tx_byte0_drvp_dqsp:[28:24]=0b01000
#define  DDR_PHY_REG_400_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq0_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte1_bit0_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq1_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte1_bit1_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_401_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq2_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte1_bit2_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq3_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte1_bit3_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_402_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq4_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte1_bit4_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq5_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte1_bit5_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_403_F0_DATA  0b00000110010000000000011001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq6_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte1_bit6_data_shift:[13:8]=0b000110
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq7_sw:[22:16]=0b1000000
	// f0_param_phyd_tx_byte1_bit7_data_shift:[29:24]=0b000110
#define  DDR_PHY_REG_404_F0_DATA  0b00000000000000000000011001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq8_sw:[6:0]=0b1000000
	// f0_param_phyd_tx_byte1_bit8_data_shift:[13:8]=0b000110
#define  DDR_PHY_REG_405_F0_DATA  0b00001011000000000000000000000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dqsn_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dqsp_sw:[22:16]=0b0000000
	// f0_param_phyd_tx_byte1_dqs_shift:[29:24]=0b001011
#define  DDR_PHY_REG_406_F0_DATA  0b00001010000000000000100100000000
	// f0_param_phyd_tx_byte1_oenz_dqs_shift:[13:8]=0b001001
	// f0_param_phyd_tx_byte1_oenz_shift:[29:24]=0b001010
#define  DDR_PHY_REG_407_F0_DATA  0b00000000000001000000011000000000
	// f0_param_phyd_tx_byte1_oenz_dqs_extend:[11:8]=0b0110
	// f0_param_phyd_tx_byte1_oenz_extend:[19:16]=0b0100
#define  DDR_PHY_REG_408_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq0_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq1_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_409_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq2_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq3_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_410_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq4_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq5_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_411_F0_DATA  0b00000000010000000000000001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq6_raw:[6:0]=0b1000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq7_raw:[22:16]=0b1000000
#define  DDR_PHY_REG_412_F0_DATA  0b00000000000000000000000001000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dq8_raw:[6:0]=0b1000000
#define  DDR_PHY_REG_413_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dqsn_raw:[6:0]=0b0000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_dqsp_raw:[22:16]=0b0000000
#define  DDR_PHY_REG_414_F0_DATA  0b00000000000000000000100000001000
	// f0_param_phya_reg_tx_byte1_drvn_dq:[4:0]=0b01000
	// f0_param_phya_reg_tx_byte1_drvp_dq:[12:8]=0b01000
#define  DDR_PHY_REG_415_F0_DATA  0b00001000000010000000100000001000
	// f0_param_phya_reg_tx_byte1_drvn_dqsn:[4:0]=0b01000
	// f0_param_phya_reg_tx_byte1_drvp_dqsn:[12:8]=0b01000
	// f0_param_phya_reg_tx_byte1_drvn_dqsp:[20:16]=0b01000
	// f0_param_phya_reg_tx_byte1_drvp_dqsp:[28:24]=0b01000
#define  DDR_PHY_REG_448_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_rx_byte0_rx_dq0_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq1_deskew_sw:[14:8]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq2_deskew_sw:[22:16]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq3_deskew_sw:[30:24]=0b0000000
#define  DDR_PHY_REG_449_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_rx_byte0_rx_dq4_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq5_deskew_sw:[14:8]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq6_deskew_sw:[22:16]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dq7_deskew_sw:[30:24]=0b0000000
#define  DDR_PHY_REG_450_F0_DATA  0b00000000010000000100000000000000
	// f0_param_phyd_reg_rx_byte0_rx_dq8_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte0_rx_dqs_dlie_code_neg_ranka_sw:[15:8]=0b01000000
	// f0_param_phyd_reg_rx_byte0_rx_dqs_dlie_code_pos_ranka_sw:[23:16]=0b01000000
#define  DDR_PHY_REG_451_F0_DATA  0b00000000000000000000101000000000
	// f0_param_phyd_reg_tx_byte0_tx_dline_code_mask_ranka_sw:[6:0]=0b0000000
	// f0_param_phyd_rx_byte0_mask_shift:[13:8]=0b001010
#define  DDR_PHY_REG_452_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_rx_byte0_en_shift:[13:8]=0b000000
	// f0_param_phyd_rx_byte0_odt_en_shift:[29:24]=0b000000
#define  DDR_PHY_REG_453_F0_DATA  0b00000000000010000000111000001110
	// f0_param_phyd_rx_byte0_en_extend:[3:0]=0b1110
	// f0_param_phyd_rx_byte0_odt_en_extend:[11:8]=0b1110
	// f0_param_phyd_rx_byte0_rden_to_rdvld:[20:16]=0b01000
#define  DDR_PHY_REG_454_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_rx_byte0_rx_dq0_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq1_deskew_raw:[12:8]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq2_deskew_raw:[20:16]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq3_deskew_raw:[28:24]=0b00000
#define  DDR_PHY_REG_455_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_rx_byte0_rx_dq4_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq5_deskew_raw:[12:8]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq6_deskew_raw:[20:16]=0b00000
	// f0_param_phya_reg_rx_byte0_rx_dq7_deskew_raw:[28:24]=0b00000
#define  DDR_PHY_REG_456_F0_DATA  0b01000000010000000000000000000000
	// f0_param_phya_reg_rx_byte0_rx_dq8_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_tx_byte0_tx_dline_code_mask_ranka_raw:[14:8]=0b0000000
	// f0_param_phya_reg_rx_byte0_rx_dqs_dlie_code_neg_ranka_raw:[22:16]=0b1000000
	// f0_param_phya_reg_rx_byte0_rx_dqs_dlie_code_pos_ranka_raw:[30:24]=0b1000000
#define  DDR_PHY_REG_457_F0_DATA  0b00000000000100000000000000010000
	// f0_param_phya_reg_rx_byte0_trig_lvl_dq:[4:0]=0b10000
	// f0_param_phya_reg_rx_byte0_trig_lvl_dq_offset:[14:8]=0b0000000
	// f0_param_phya_reg_rx_byte0_trig_lvl_dqs:[20:16]=0b10000
	// f0_param_phya_reg_rx_byte0_trig_lvl_dqs_offset:[30:24]=0b0000000
#define  DDR_PHY_REG_460_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_rx_byte1_rx_dq0_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq1_deskew_sw:[14:8]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq2_deskew_sw:[22:16]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq3_deskew_sw:[30:24]=0b0000000
#define  DDR_PHY_REG_461_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_reg_rx_byte1_rx_dq4_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq5_deskew_sw:[14:8]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq6_deskew_sw:[22:16]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dq7_deskew_sw:[30:24]=0b0000000
#define  DDR_PHY_REG_462_F0_DATA  0b00000000010000000100000000000000
	// f0_param_phyd_reg_rx_byte1_rx_dq8_deskew_sw:[6:0]=0b0000000
	// f0_param_phyd_reg_rx_byte1_rx_dqs_dlie_code_neg_ranka_sw:[15:8]=0b01000000
	// f0_param_phyd_reg_rx_byte1_rx_dqs_dlie_code_pos_ranka_sw:[23:16]=0b01000000
#define  DDR_PHY_REG_463_F0_DATA  0b00000000000000000000101000000000
	// f0_param_phyd_reg_tx_byte1_tx_dline_code_mask_ranka_sw:[6:0]=0b0000000
	// f0_param_phyd_rx_byte1_mask_shift:[13:8]=0b001010
#define  DDR_PHY_REG_464_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phyd_rx_byte1_en_shift:[13:8]=0b000000
	// f0_param_phyd_rx_byte1_odt_en_shift:[29:24]=0b000000
#define  DDR_PHY_REG_465_F0_DATA  0b00000000000010000000111000001110
	// f0_param_phyd_rx_byte1_en_extend:[3:0]=0b1110
	// f0_param_phyd_rx_byte1_odt_en_extend:[11:8]=0b1110
	// f0_param_phyd_rx_byte1_rden_to_rdvld:[20:16]=0b01000
#define  DDR_PHY_REG_466_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_rx_byte1_rx_dq0_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq1_deskew_raw:[12:8]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq2_deskew_raw:[20:16]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq3_deskew_raw:[28:24]=0b00000
#define  DDR_PHY_REG_467_F0_DATA  0b00000000000000000000000000000000
	// f0_param_phya_reg_rx_byte1_rx_dq4_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq5_deskew_raw:[12:8]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq6_deskew_raw:[20:16]=0b00000
	// f0_param_phya_reg_rx_byte1_rx_dq7_deskew_raw:[28:24]=0b00000
#define  DDR_PHY_REG_468_F0_DATA  0b01000000010000000000000000000000
	// f0_param_phya_reg_rx_byte1_rx_dq8_deskew_raw:[4:0]=0b00000
	// f0_param_phya_reg_tx_byte1_tx_dline_code_mask_ranka_raw:[14:8]=0b0000000
	// f0_param_phya_reg_rx_byte1_rx_dqs_dlie_code_neg_ranka_raw:[22:16]=0b1000000
	// f0_param_phya_reg_rx_byte1_rx_dqs_dlie_code_pos_ranka_raw:[30:24]=0b1000000
#define  DDR_PHY_REG_469_F0_DATA  0b00000000000100000000000000010000
	// f0_param_phya_reg_rx_byte1_trig_lvl_dq:[4:0]=0b10000
	// f0_param_phya_reg_rx_byte1_trig_lvl_dq_offset:[14:8]=0b0000000
	// f0_param_phya_reg_rx_byte1_trig_lvl_dqs:[20:16]=0b10000
	// f0_param_phya_reg_rx_byte1_trig_lvl_dqs_offset:[30:24]=0b0000000

const struct ddr_patch_reg ddr_phy_patch[] = {
	DDR_PATCH_REG(0x0 + DDR_BASE, DDR_PHY_REG_0_DATA),
	DDR_PATCH_REG(0x4 + DDR_BASE, DDR_PHY_REG_1_DATA),
	DDR_PATCH_REG(0x8 + DDR_BASE, DDR_PHY_REG_2_DATA),
	DDR_PATCH_REG(0xc + DDR_BASE, DDR_PHY_REG_3_DATA),
	DDR_PATCH_REG(0x10 + DDR_BASE, DDR_PHY_REG_4_DATA),
	DDR_PATCH_REG(0x14 + DDR_BASE, DDR_PHY_REG_5_DATA),
	DDR_PATCH_REG(0x18 + DDR_BASE, DDR_PHY_REG_6_DATA),
	DDR_PATCH_REG(0x1c + DDR_BASE, DDR_PHY_REG_7_DATA),
	DDR_PATCH_REG(0x20 + DDR_BASE, DDR_PHY_REG_8_DATA),
	DDR_PATCH_REG(0x24 + DDR_BASE, DDR_PHY_REG_9_DATA),
	DDR_PATCH_REG(0x28 + DDR_BASE, DDR_PHY_REG_10_DATA),
	DDR_PATCH_REG(0x2c + DDR_BASE, DDR_PHY_REG_11_DATA),
	DDR_PATCH_REG(0x40 + DDR_BASE, DDR_PHY_REG_16_DATA),
	DDR_PATCH_REG(0x44 + DDR_BASE, DDR_PHY_REG_17_DATA),
	DDR_PATCH_REG(0x48 + DDR_BASE, DDR_PHY_REG_18_DATA),
	DDR_PATCH_REG(0x4c + DDR_BASE, DDR_PHY_REG_19_DATA),
	DDR_PATCH_REG(0x50 + DDR_BASE, DDR_PHY_REG_20_DATA),
	DDR_PATCH_REG(0x54 + DDR_BASE, DDR_PHY_REG_21_DATA),
	DDR_PATCH_REG(0x58 + DDR_BASE, DDR_PHY_REG_22_DATA),
	DDR_PATCH_REG(0x5c + DDR_BASE, DDR_PHY_REG_23_DATA),
	DDR_PATCH_REG(0x60 + DDR_BASE, DDR_PHY_REG_24_DATA),
	DDR_PATCH_REG(0x64 + DDR_BASE, DDR_PHY_REG_25_DATA),
	DDR_PATCH_REG(0x68 + DDR_BASE, DDR_PHY_REG_26_DATA),
	DDR_PATCH_REG(0x70 + DDR_BASE, DDR_PHY_REG_28_DATA),
	DDR_PATCH_REG(0x74 + DDR_BASE, DDR_PHY_REG_29_DATA),
	DDR_PATCH_REG(0x80 + DDR_BASE, DDR_PHY_REG_32_DATA),
	DDR_PATCH_REG(0x84 + DDR_BASE, DDR_PHY_REG_33_DATA),
	DDR_PATCH_REG(0x88 + DDR_BASE, DDR_PHY_REG_34_DATA),
	DDR_PATCH_REG(0x8c + DDR_BASE, DDR_PHY_REG_35_DATA),
	DDR_PATCH_REG(0x90 + DDR_BASE, DDR_PHY_REG_36_DATA),
	DDR_PATCH_REG(0x94 + DDR_BASE, DDR_PHY_REG_37_DATA),
	DDR_PATCH_REG(0xa0 + DDR_BASE, DDR_PHY_REG_40_DATA),
	DDR_PATCH_REG(0xa4 + DDR_BASE, DDR_PHY_REG_41_DATA),
	DDR_PATCH_REG(0xa8 + DDR_BASE, DDR_PHY_REG_42_DATA),
	DDR_PATCH_REG(0xac + DDR_BASE, DDR_PHY_REG_43_DATA),
	DDR_PATCH_REG(0xb0 + DDR_BASE, DDR_PHY_REG_44_DATA),
	DDR_PATCH_REG(0xb4 + DDR_BASE, DDR_PHY_REG_45_DATA),
	DDR_PATCH_REG(0xb8 + DDR_BASE, DDR_PHY_REG_46_DATA),
	DDR_PATCH_REG(0xbc + DDR_BASE, DDR_PHY_REG_47_DATA),
	DDR_PATCH_REG(0xf0 + DDR_BASE, DDR_PHY_REG_60_DATA),
	DDR_PATCH_REG(0xf4 + DDR_BASE, DDR_PHY_REG_61_DATA),
	DDR_PATCH_REG(0xf8 + DDR_BASE, DDR_PHY_REG_62_DATA),
	DDR_PATCH_REG(0xfc + DDR_BASE, DDR_PHY_REG_63_DATA),
	DDR_PATCH_REG(0x100 + DDR_BASE, DDR_PHY_REG_64_DATA),
	DDR_PATCH_REG(0x104 + DDR_BASE, DDR_PHY_REG_65_DATA),
	DDR_PATCH_REG(0x10c + DDR_BASE, DDR_PHY_REG_67_DATA),
	DDR_PATCH_REG(0x110 + DDR_BASE, DDR_PHY_REG_68_DATA),
	DDR_PATCH_REG(0x114 + DDR_BASE, DDR_PHY_REG_69_DATA),
	DDR_PATCH_REG(0x118 + DDR_BASE, DDR_PHY_REG_70_DATA),
	DDR_PATCH_REG(0x11c + DDR_BASE, DDR_PHY_REG_71_DATA),
	DDR_PATCH_REG(0x120 + DDR_BASE, DDR_PHY_REG_72_DATA),
	DDR_PATCH_REG(0x124 + DDR_BASE, DDR_PHY_REG_73_DATA),
	DDR_PATCH_REG(0x128 + DDR_BASE, DDR_PHY_REG_74_DATA),
	DDR_PATCH_REG(0x12c + DDR_BASE, DDR_PHY_REG_75_DATA),
	DDR_PATCH_REG(0x130 + DDR_BASE, DDR_PHY_REG_76_DATA),
	DDR_PATCH_REG(0x134 + DDR_BASE, DDR_PHY_REG_77_DATA),
	DDR_PATCH_REG(0x138 + DDR_BASE, DDR_PHY_REG_78_DATA),
	DDR_PATCH_REG(0x140 + DDR_BASE, DDR_PHY_REG_80_DATA),
	DDR_PATCH_REG(0x144 + DDR_BASE, DDR_PHY_REG_81_DATA),
	DDR_PATCH_REG(0x148 + DDR_BASE, DDR_PHY_REG_82_DATA),
	DDR_PATCH_REG(0x14c + DDR_BASE, DDR_PHY_REG_83_DATA),
	DDR_PATCH_REG(0x150 + DDR_BASE, DDR_PHY_REG_84_DATA),
	DDR_PATCH_REG(0x154 + DDR_BASE, DDR_PHY_REG_85_DATA),
	DDR_PATCH_REG(0x158 + DDR_BASE, DDR_PHY_REG_86_DATA),
	DDR_PATCH_REG(0x15c + DDR_BASE, DDR_PHY_REG_87_DATA),
	DDR_PATCH_REG(0x164 + DDR_BASE, DDR_PHY_REG_89_DATA),
	DDR_PATCH_REG(0x168 + DDR_BASE, DDR_PHY_REG_90_DATA),
	DDR_PATCH_REG(0x16c + DDR_BASE, DDR_PHY_REG_91_DATA),
	DDR_PATCH_REG(0x170 + DDR_BASE, DDR_PHY_REG_92_DATA),
	DDR_PATCH_REG(0x174 + DDR_BASE, DDR_PHY_REG_93_DATA),
	DDR_PATCH_REG(0x180 + DDR_BASE, DDR_PHY_REG_96_DATA),
	DDR_PATCH_REG(0x184 + DDR_BASE, DDR_PHY_REG_97_DATA),
	DDR_PATCH_REG(0x188 + DDR_BASE, DDR_PHY_REG_98_DATA),
	DDR_PATCH_REG(0x18c + DDR_BASE, DDR_PHY_REG_99_DATA),
	DDR_PATCH_REG(0x190 + DDR_BASE, DDR_PHY_REG_100_DATA),
	DDR_PATCH_REG(0x200 + DDR_BASE, DDR_PHY_REG_128_DATA),
	DDR_PATCH_REG(0x204 + DDR_BASE, DDR_PHY_REG_129_DATA),
	DDR_PATCH_REG(0x208 + DDR_BASE, DDR_PHY_REG_130_DATA),
	DDR_PATCH_REG(0x220 + DDR_BASE, DDR_PHY_REG_136_DATA),
	DDR_PATCH_REG(0x224 + DDR_BASE, DDR_PHY_REG_137_DATA),
	DDR_PATCH_REG(0x228 + DDR_BASE, DDR_PHY_REG_138_DATA),
	DDR_PATCH_REG(0x400 + DDR_BASE, DDR_PHY_REG_0_F0_DATA),
	DDR_PATCH_REG(0x404 + DDR_BASE, DDR_PHY_REG_1_F0_DATA),
	DDR_PATCH_REG(0x408 + DDR_BASE, DDR_PHY_REG_2_F0_DATA),
	DDR_PATCH_REG(0x40c + DDR_BASE, DDR_PHY_REG_3_F0_DATA),
	DDR_PATCH_REG(0x410 + DDR_BASE, DDR_PHY_REG_4_F0_DATA),
	DDR_PATCH_REG(0x414 + DDR_BASE, DDR_PHY_REG_5_F0_DATA),
	DDR_PATCH_REG(0x418 + DDR_BASE, DDR_PHY_REG_6_F0_DATA),
	DDR_PATCH_REG(0x41c + DDR_BASE, DDR_PHY_REG_7_F0_DATA),
	DDR_PATCH_REG(0x500 + DDR_BASE, DDR_PHY_REG_64_F0_DATA),
	DDR_PATCH_REG(0x504 + DDR_BASE, DDR_PHY_REG_65_F0_DATA),
	DDR_PATCH_REG(0x508 + DDR_BASE, DDR_PHY_REG_66_F0_DATA),
	DDR_PATCH_REG(0x50c + DDR_BASE, DDR_PHY_REG_67_F0_DATA),
	DDR_PATCH_REG(0x510 + DDR_BASE, DDR_PHY_REG_68_F0_DATA),
	DDR_PATCH_REG(0x514 + DDR_BASE, DDR_PHY_REG_69_F0_DATA),
	DDR_PATCH_REG(0x518 + DDR_BASE, DDR_PHY_REG_70_F0_DATA),
	DDR_PATCH_REG(0x51c + DDR_BASE, DDR_PHY_REG_71_F0_DATA),
	DDR_PATCH_REG(0x520 + DDR_BASE, DDR_PHY_REG_72_F0_DATA),
	DDR_PATCH_REG(0x540 + DDR_BASE, DDR_PHY_REG_80_F0_DATA),
	DDR_PATCH_REG(0x544 + DDR_BASE, DDR_PHY_REG_81_F0_DATA),
	DDR_PATCH_REG(0x548 + DDR_BASE, DDR_PHY_REG_82_F0_DATA),
	DDR_PATCH_REG(0x54c + DDR_BASE, DDR_PHY_REG_83_F0_DATA),
	DDR_PATCH_REG(0x550 + DDR_BASE, DDR_PHY_REG_84_F0_DATA),
	DDR_PATCH_REG(0x554 + DDR_BASE, DDR_PHY_REG_85_F0_DATA),
	DDR_PATCH_REG(0x558 + DDR_BASE, DDR_PHY_REG_86_F0_DATA),
	DDR_PATCH_REG(0x55c + DDR_BASE, DDR_PHY_REG_87_F0_DATA),
	DDR_PATCH_REG(0x560 + DDR_BASE, DDR_PHY_REG_88_F0_DATA),
	DDR_PATCH_REG(0x900 + DDR_BASE, DDR_PHY_REG_320_F0_DATA),
	DDR_PATCH_REG(0x92c + DDR_BASE, DDR_PHY_REG_331_F0_DATA),
	DDR_PATCH_REG(0x930 + DDR_BASE, DDR_PHY_REG_332_F0_DATA),
	DDR_PATCH_REG(0x934 + DDR_BASE, DDR_PHY_REG_333_F0_DATA),
	DDR_PATCH_REG(0x938 + DDR_BASE, DDR_PHY_REG_334_F0_DATA),
	DDR_PATCH_REG(0x940 + DDR_BASE, DDR_PHY_REG_336_F0_DATA),
	DDR_PATCH_REG(0x970 + DDR_BASE, DDR_PHY_REG_348_F0_DATA),
	DDR_PATCH_REG(0x974 + DDR_BASE, DDR_PHY_REG_349_F0_DATA),
	DDR_PATCH_REG(0x978 + DDR_BASE, DDR_PHY_REG_350_F0_DATA),
	DDR_PATCH_REG(0x97c + DDR_BASE, DDR_PHY_REG_351_F0_DATA),
	DDR_PATCH_REG(0x980 + DDR_BASE, DDR_PHY_REG_352_F0_DATA),
	DDR_PATCH_REG(0xa00 + DDR_BASE, DDR_PHY_REG_384_F0_DATA),
	DDR_PATCH_REG(0xa04 + DDR_BASE, DDR_PHY_REG_385_F0_DATA),
	DDR_PATCH_REG(0xa08 + DDR_BASE, DDR_PHY_REG_386_F0_DATA),
	DDR_PATCH_REG(0xa0c + DDR_BASE, DDR_PHY_REG_387_F0_DATA),
	DDR_PATCH_REG(0xa10 + DDR_BASE, DDR_PHY_REG_388_F0_DATA),
	DDR_PATCH_REG(0xa14 + DDR_BASE, DDR_PHY_REG_389_F0_DATA),
	DDR_PATCH_REG(0xa18 + DDR_BASE, DDR_PHY_REG_390_F0_DATA),
	DDR_PATCH_REG(0xa1c + DDR_BASE, DDR_PHY_REG_391_F0_DATA),
	DDR_PATCH_REG(0xa20 + DDR_BASE, DDR_PHY_REG_392_F0_DATA),
	DDR_PATCH_REG(0xa24 + DDR_BASE, DDR_PHY_REG_393_F0_DATA),
	DDR_PATCH_REG(0xa28 + DDR_BASE, DDR_PHY_REG_394_F0_DATA),
	DDR_PATCH_REG(0xa2c + DDR_BASE, DDR_PHY_REG_395_F0_DATA),
	DDR_PATCH_REG(0xa30 + DDR_BASE, DDR_PHY_REG_396_F0_DATA),
	DDR_PATCH_REG(0xa34 + DDR_BASE, DDR_PHY_REG_397_F0_DATA),
	DDR_PATCH_REG(0xa38 + DDR_BASE, DDR_PHY_REG_398_F0_DATA),
	DDR_PATCH_REG(0xa3c + DDR_BASE, DDR_PHY_REG_399_F0_DATA),
	DDR_PATCH_REG(0xa40 + DDR_BASE, DDR_PHY_REG_400_F0_DATA),
	DDR_PATCH_REG(0xa44 + DDR_BASE, DDR_PHY_REG_401_F0_DATA),
	DDR_PATCH_REG(0xa48 + DDR_BASE, DDR_PHY_REG_402_F0_DATA),
	DDR_PATCH_REG(0xa4c + DDR_BASE, DDR_PHY_REG_403_F0_DATA),
	DDR_PATCH_REG(0xa50 + DDR_BASE, DDR_PHY_REG_404_F0_DATA),
	DDR_PATCH_REG(0xa54 + DDR_BASE, DDR_PHY_REG_405_F0_DATA),
	DDR_PATCH_REG(0xa58 + DDR_BASE, DDR_PHY_REG_406_F0_DATA),
	DDR_PATCH_REG(0xa5c + DDR_BASE, DDR_PHY_REG_407_F0_DATA),
	DDR_PATCH_REG(0xa60 + DDR_BASE, DDR_PHY_REG_408_F0_DATA),
	DDR_PATCH_REG(0xa64 + DDR_BASE, DDR_PHY_REG_409_F0_DATA),
	DDR_PATCH_REG(0xa68 + DDR_BASE, DDR_PHY_REG_410_F0_DATA),
	DDR_PATCH_REG(0xa6c + DDR_BASE, DDR_PHY_REG_411_F0_DATA),
	DDR_PATCH_REG(0xa70 + DDR_BASE, DDR_PHY_REG_412_F0_DATA),
	DDR_PATCH_REG(0xa74 + DDR_BASE, DDR_PHY_REG_413_F0_DATA),
	DDR_PATCH_REG(0xa78 + DDR_BASE, DDR_PHY_REG_414_F0_DATA),
	DDR_PATCH_REG(0xa7c + DDR_BASE, DDR_PHY_REG_415_F0_DATA),
	DDR_PATCH_REG(0xb00 + DDR_BASE, DDR_PHY_REG_448_F0_DATA),
	DDR_PATCH_REG(0xb04 + DDR_BASE, DDR_PHY_REG_449_F0_DATA),
	DDR_PATCH_REG(0xb08 + DDR_BASE, DDR_PHY_REG_450_F0_DATA),
	DDR_PATCH_REG(0xb0c + DDR_BASE, DDR_PHY_REG_451_F0_DATA),
	DDR_PATCH_REG(0xb10 + DDR_BASE, DDR_PHY_REG_452_F0_DATA),
	DDR_PATCH_REG(0xb14 + DDR_BASE, DDR_PHY_REG_453_F0_DATA),
	DDR_PATCH_REG(0xb18 + DDR_BASE, DDR_PHY_REG_454_F0_DATA),
	DDR_PATCH_REG(0xb1c + DDR_BASE, DDR_PHY_REG_455_F0_DATA),
	DDR_PATCH_REG(0xb20 + DDR_BASE, DDR_PHY_REG_456_F0_DATA),
	DDR_PATCH_REG(0xb24 + DDR_BASE, DDR_PHY_REG_457_F0_DATA),
	DDR_PATCH_REG(0xb30 + DDR_BASE, DDR_PHY_REG_460_F0_DATA),
	DDR_PATCH_REG(0xb34 + DDR_BASE, DDR_PHY_REG_461_F0_DATA),
	DDR_PATCH_REG(0xb38 + DDR_BASE, DDR_PHY_REG_462_F0_DATA),
	DDR_PATCH_REG(0xb3c + DDR_BASE, DDR_PHY_REG_463_F0_DATA),
	DDR_PATCH_REG(0xb40 + DDR_BASE, DDR_PHY_REG_464_F0_DATA),
	DDR_PATCH_REG(0xb44 + DDR_BASE, DDR_PHY_REG_465_F0_DATA),
	DDR_PATCH_REG(0xb48 + DDR_BASE, DDR_PHY_REG_466_F0_DATA),
	DDR_PATCH_REG(0xb4c + DDR_BASE, DDR_PHY_REG_467_F0_DATA),
	DDR_PATCH_REG(0xb50 + DDR_BASE, DDR_PHY_REG_468_F0_DATA),
	DDR_PATCH_REG(0xb54 + DDR_BASE, DDR_PHY_REG_469_F0_DATA),
};

const unsigned int ddr_phy_patch_size = ARRAY_SIZE(ddr_phy_patch);

const struct ddr_mask_patch_reg ddr_patch_regs[] = {
	// tune damp //////
	DDR_MASK_PATCH_REG(0x08000150, 0xFFFFFFFF, 0x00000005),

	// CSB & CA driving
	DDR_MASK_PATCH_REG(0x0800097c, 0xFFFFFFFF, 0x08080404),

	// CLK driving
	DDR_MASK_PATCH_REG(0x08000980, 0xFFFFFFFF, 0x08080808),

	// DQ driving // BYTE0
	DDR_MASK_PATCH_REG(0x08000a38, 0xFFFFFFFF, 0x00000606),
	// DQS driving // BYTE0
	DDR_MASK_PATCH_REG(0x08000a3c, 0xFFFFFFFF, 0x06060606),
	// DQ driving // BYTE1
	DDR_MASK_PATCH_REG(0x08000a78, 0xFFFFFFFF, 0x00000606),
	// DQS driving // BYTE1
	DDR_MASK_PATCH_REG(0x08000a7c, 0xFFFFFFFF, 0x06060606),

	//trigger level //////
	// BYTE0
	DDR_MASK_PATCH_REG(0x08000b24, 0xFFFFFFFF, 0x00100010),
	// BYTE1
	DDR_MASK_PATCH_REG(0x08000b54, 0xFFFFFFFF, 0x00100010),

	//APHY TX VREFDQ rangex2 [1]
	//VREF DQ   //
	DDR_MASK_PATCH_REG(0x08000410, 0xFFFFFFFF, 0x00120002),
	//APHY TX VREFCA rangex2 [1]
	//VREF CA  //
	DDR_MASK_PATCH_REG(0x08000414, 0xFFFFFFFF, 0x00100002),

	// tx dline code
	//  BYTE0 DQ
	DDR_MASK_PATCH_REG(0x08000a00, 0xFFFFFFFF, 0x06430643),
	DDR_MASK_PATCH_REG(0x08000a04, 0xFFFFFFFF, 0x06430643),
	DDR_MASK_PATCH_REG(0x08000a08, 0xFFFFFFFF, 0x06430643),
	DDR_MASK_PATCH_REG(0x08000a0c, 0xFFFFFFFF, 0x06430643),
	DDR_MASK_PATCH_REG(0x08000a10, 0xFFFFFFFF, 0x00000643),
	DDR_MASK_PATCH_REG(0x08000a14, 0xFFFFFFFF, 0x0a7e007e),
	//  BYTE1 DQ
	DDR_MASK_PATCH_REG(0x08000a40, 0xFFFFFFFF, 0x06480648),
	DDR_MASK_PATCH_REG(0x08000a44, 0xFFFFFFFF, 0x06480648),
	DDR_MASK_PATCH_REG(0x08000a48, 0xFFFFFFFF, 0x06480648),
	DDR_MASK_PATCH_REG(0x08000a4c, 0xFFFFFFFF, 0x06480648),
	DDR_MASK_PATCH_REG(0x08000a50, 0xFFFFFFFF, 0x00000648),
	DDR_MASK_PATCH_REG(0x08000a54, 0xFFFFFFFF, 0x0a7e007e),

	//APHY RX TRIG rangex2[18] & disable lsmode[0]
	//f0_param_phya_reg_rx_byte0_en_lsmode[0]
	//f0_param_phya_reg_byte0_en_rec_vol_mode[12]
	//f0_param_phya_reg_rx_byte0_force_en_lvstl_odt[16]
	//f0_param_phya_reg_rx_byte0_sel_dqs_rec_vref_mode[8]
	//param_phya_reg_rx_byte0_en_trig_lvl_rangex2[18]
	// BYTE0 [0]
	DDR_MASK_PATCH_REG(0x08000500, 0xFFFFFFFF, 0x00041001),
	//f0_param_phya_reg_rx_byte1_en_lsmode[0]
	//f0_param_phya_reg_byte1_en_rec_vol_mode[12]
	//f0_param_phya_reg_rx_byte0_force_en_lvstl_odt[16]
	//f0_param_phya_reg_rx_byte0_sel_dqs_rec_vref_mode[8]
	//param_phya_reg_rx_byte0_en_trig_lvl_rangex2[18]
	// BYTE1 [0]
	DDR_MASK_PATCH_REG(0x08000540, 0xFFFFFFFF, 0x00041001),

	////////  FOR U02 ///////
	/////////// U02 enable DQS voltage mode receiver
	// f0_param_phya_reg_tx_byte0_en_tx_de_dqs[20]
	DDR_MASK_PATCH_REG(0x08000504, 0xFFFFFFFF, 0x00100000),
	// f0_param_phya_reg_tx_byte1_en_tx_de_dqs[20]
	DDR_MASK_PATCH_REG(0x08000544, 0xFFFFFFFF, 0x00100000),
	/////////// U02 enable MASK voltage mode receiver
	// param_phya_reg_rx_sel_dqs_wo_pream_mode[2]
	DDR_MASK_PATCH_REG(0x08000138, 0xFFFFFFFF, 0x00000014),

	// BYTE0 RX DQ deskew
	DDR_MASK_PATCH_REG(0x08000b00, 0xFFFFFFFF, 0x00020402),
	DDR_MASK_PATCH_REG(0x08000b04, 0xFFFFFFFF, 0x05020401),
	// BYTE0  DQ8 deskew [6:0] neg DQS  [15:8]  ;  pos DQS  [23:16]
	DDR_MASK_PATCH_REG(0x08000b08, 0xFFFFFFFF, 0x00313902),

	// BYTE1 RX DQ deskew
	DDR_MASK_PATCH_REG(0x08000b30, 0xFFFFFFFF, 0x06000100),
	DDR_MASK_PATCH_REG(0x08000b34, 0xFFFFFFFF, 0x02010303),
	// BYTE1  DQ8 deskew [6:0] neg DQS  [15:8]  ;  pos DQS  [23:16]
	DDR_MASK_PATCH_REG(0x08000b38, 0xFFFFFFFF, 0x00323900),

	//Read gate TX dline + shift
	// BYTE0
	DDR_MASK_PATCH_REG(0x08000b0c, 0xFFFFFFFF, 0x00000a14),
	// BYTE1
	DDR_MASK_PATCH_REG(0x08000b3c, 0xFFFFFFFF, 0x00000a14),

	// CKE dline + shift CKE0 [6:0]+[13:8] ; CKE1 [22:16]+[29:24]
	DDR_MASK_PATCH_REG(0x08000930, 0xFFFFFFFF, 0x04000400),
	// CSB dline + shift CSB0 [6:0]+[13:8] ; CSB1 [22:16]+[29:24]
	DDR_MASK_PATCH_REG(0x08000934, 0xFFFFFFFF, 0x04000400),
};

const unsigned int ddr_patch_size = ARRAY_SIZE(ddr_patch_regs);

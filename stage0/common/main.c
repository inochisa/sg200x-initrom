
#include "common.h"
#include "serial.h"
#include "board.h"
#include "ddr.h"

static void read_board_info(void)
{
	uint32_t conf_info = readl(TOP_SYSCON_BASE + TOP_SYSCON_CONF_INFO);

	uint32_t pkg_type = FIELD_GET(TOP_SYSCON_CONF_INFO_PKG_MASK, conf_info);
	parse_board_info(pkg_type);

	fsbl_printf("chipid %x\n", readl(TOP_SYSCON_BASE + TOP_SYSCON_GP_REG3));
	fsbl_printf("conf_info=0x%08x package_type=%u\n", conf_info, package_type);
	fsbl_printf("ddr vendor %u, %s/%uMT %uM\n",
		    ddr_vendor, ddr_type == DDR_TYPE_DDR2 ? "DDR2" : "DDR3",
		    ddr_rate, (ddr_capacity << 7));
}

__attribute__((noreturn))
int main(void)
{
	fsbl_init_console(UART0_BASE, 25000000, 115200);

	fsbl_printf("\nhello fip\n\n");

	read_board_info();

	while (1)
		;
}

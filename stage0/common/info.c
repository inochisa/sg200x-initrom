
#include "common.h"
#include "bits.h"
#include "ddr.h"
#include "board.h"

uint8_t package_type;

uint8_t ddr_vendor;
uint8_t ddr_capacity;
uint8_t ddr_type;
uint16_t ddr_rate;

#define DDR_VENDOR_MASK		GENMASK(25, 21)
#define DDR_CAPACITY_MASK	GENMASK(28, 26)
#define PKG_TYPE		GENMASK(31, 29)

#define CHIPID_DDRCAP_MASK	GENMASK(7, 4)
#define CHIPID_PAGEAGE_MASK	GENMASK(3, 0)

static int build_chip_id(unsigned int initid)
{
	uint32_t chip_id;

	switch (ddr_vendor) {
	case DDR_VENDOR_ESMT_512M_DDR2:
	case DDR_VENDOR_ETRON_512M_DDR2:
		ddr_type = DDR_TYPE_DDR2;
		ddr_rate = 1333;
		break;
	case DDR_VENDOR_NY_4G:
	case DDR_VENDOR_NY_2G:
	case DDR_VENDOR_ESMT_1G:
	case DDR_VENDOR_ETRON_1G:
	case DDR_VENDOR_ESMT_2G:
	case DDR_VENDOR_PM_2G:
	case DDR_VENDOR_PM_1G:
	case DDR_VENDOR_ESMT_N25_1G:
		ddr_type = DDR_TYPE_DDR3;
		ddr_rate = 1866;
		break;
	default:
		return -1;
	}

	if ((ddr_capacity < DDR_CAPACITY_512M) || (ddr_capacity > DDR_CAPACITY_4G))
		return -1;

	chip_id = initid;
	chip_id |= FIELD_PREP(CHIPID_DDRCAP_MASK, ddr_capacity - 1);

	switch (package_type) {
	case PKG_QFN88:
		chip_id |= FIELD_PREP(CHIPID_PAGEAGE_MASK, 0xc);
		break;
	case PKG_BGA205:
		chip_id |= FIELD_PREP(CHIPID_PAGEAGE_MASK, 0xf);
		break;
	case PKG_QFN68:
		chip_id |= FIELD_PREP(CHIPID_PAGEAGE_MASK, 0xb);
		break;
	default:
		return -1;
	}
	writel(chip_id, TOP_SYSCON_BASE + TOP_SYSCON_GP_REG3);

	return 0;
}


#ifdef CV180X
int parse_board_info(unsigned int pkg_type)
{
	uint32_t efuse_leakage = readl(0x03050108);


	switch (pkg_type) {
	case 0x0: // 2nd src need to read from efuse
		ddr_vendor = FIELD_GET(efuse_leakage, DDR_VENDOR_MASK);
		ddr_capacity = FIELD_GET(efuse_leakage, DDR_CAPACITY_MASK);
		package_type = FIELD_GET(efuse_leakage, PKG_TYPE);
		break;
	case 0x2: // QFN68 7x7, SIP 1Gb DDR3
		ddr_vendor = DDR_VENDOR_ESMT_1G;
		ddr_capacity = DDR_CAPACITY_1G;
		package_type = PKG_QFN68;
		break;
	case 0x3: // QFN68 7x7, SIP 512Mb DDR2(25nm)
		ddr_vendor = DDR_VENDOR_ESMT_512M_DDR2;
		ddr_capacity = DDR_CAPACITY_512M;
		package_type = PKG_QFN68;
		break;
	case 0x4: // 2nd src need to read from efuse
		ddr_vendor = FIELD_GET(efuse_leakage, DDR_VENDOR_MASK);
		ddr_capacity = FIELD_GET(efuse_leakage, DDR_CAPACITY_MASK);
		package_type = FIELD_GET(efuse_leakage, PKG_TYPE);
		break;
	case 0x6: // QFN9x9, SIP 1Gb DDR3
		ddr_vendor = DDR_VENDOR_ESMT_1G;
		ddr_capacity = DDR_CAPACITY_1G;
		package_type = PKG_QFN88;
		break;
	case 0x7: // QFN9x9, SIP 512Mb DDR2(25nm)
		ddr_vendor = DDR_VENDOR_ESMT_512M_DDR2;
		ddr_capacity = DDR_CAPACITY_512M;
		package_type = PKG_QFN88;
		break;
	default:
		return -1;
	}

	return build_chip_id(0x18000);
}
#elif defined (CV181X)
int parse_board_info(unsigned int pkg_type)
{
	uint32_t efuse_leakage = readl(0x03050108);

	switch (pkg_type) {
	case 0x0: // BGA 10x10, SIP 2Gb DDR3
		ddr_vendor = DDR_VENDOR_NY_2G;
		ddr_capacity = DDR_CAPACITY_2G;
		package_type = PKG_BGA205;
		break;
	case 0x1: // BGA 10x10, SIP 4Gb DDR3
		ddr_vendor = DDR_VENDOR_NY_4G;
		ddr_capacity = DDR_CAPACITY_4G;
		package_type = PKG_BGA205;
		break;
	case 0x2: // BGA 10x10, SIP 1Gb DDR3
		ddr_vendor = DDR_VENDOR_ESMT_1G;
		ddr_capacity = DDR_CAPACITY_1G;
		package_type = PKG_BGA205;
		break;
	case 0x4: // 2nd src need to read from efuse
		ddr_vendor = FIELD_GET(efuse_leakage, DDR_VENDOR_MASK);
		ddr_capacity = FIELD_GET(efuse_leakage, DDR_CAPACITY_MASK);
		package_type = FIELD_GET(efuse_leakage, PKG_TYPE);
		break;
	case 0x5: // QFN9x9, SIP 2Gb DDR3
		ddr_vendor = DDR_VENDOR_NY_2G;
		ddr_capacity = DDR_CAPACITY_2G;
		package_type = PKG_QFN88;
		break;
	case 0x6: // QFN9x9, SIP 1Gb DDR3
		ddr_vendor = DDR_VENDOR_ESMT_1G;
		ddr_capacity = DDR_CAPACITY_1G;
		package_type = PKG_QFN88;
		break;
	case 0x7: // QFN9x9, SIP 512Mb DDR2
		ddr_vendor = DDR_VENDOR_ESMT_512M_DDR2;
		ddr_capacity = DDR_CAPACITY_512M;
		package_type = PKG_QFN88;
		break;
	default:
		return -1;
	}

	return build_chip_id(0x18100);
}
#else
#error "Unsupport chip series detected"
#endif

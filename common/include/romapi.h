
#ifndef _ROMAPI_H
#define _ROMAPI_H

#include "types.h"

enum RSA_TYPE {
	RSA_TYPE_2048_BITS,
	RSA_TYPE_4096_BITS,
};

#define BOOT_SRC_TAG 0xCE00

enum BOOT_SOURCE {
	// Read from flash
	BOOT_SRC_SPI_NAND	= 0xce00,
	BOOT_SRC_SPI_NOR	= 0xce02,
	BOOT_SRC_EMMC		= 0xce03,

	// Download
	BOOT_SRC_SD		= 0xcea0,
	BOOT_SRC_USB		= 0xcea3,
	BOOT_SRC_UART		= 0xcea5,
};

int romapi_load_image(void *buf, uint32_t offset, size_t image_size, int retry_num);
uint32_t romapi_image_crc(const void *buf, int len);
int romapi_flash_init(void);
enum BOOT_SOURCE romapi_get_boot_src(void);
void romapi_set_boot_src(enum BOOT_SOURCE src);
int romapi_get_number_of_retries(void);
int romapi_verify_rsa(void *message, size_t n, void *sig, enum RSA_TYPE rsa_size);
int romapi_cryptodma_aes_decrypt(void *plain, const void *encrypted, uint64_t len, uint8_t *key, uint8_t *iv);

#endif /* _ROMAPI_H */

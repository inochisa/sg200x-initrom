
#include "romapi.h"

struct romapi *get_romapi(void)
{
	static struct romapi api = {
		.load_image		= (void *)0x40000100,
		.image_crc		= (void *)0x40000080,
		.flash_init		= (void *)0x40000020,
		.get_boot_src		= (void *)0x400000c0,
		.set_boot_src		= (void *)0x400000a0,
		.get_number_of_retries	= (void *)0x40000060,
		.verify_rsa		= (void *)0x40000040,
		.cryptodma_aes_decrypt	= (void *)0x400000e0,
	};

	return &api;
}

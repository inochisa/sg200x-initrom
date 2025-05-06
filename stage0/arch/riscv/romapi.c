
#include "romapi.h"

struct romapi *get_romapi(void)
{
	static struct romapi api = {
		.load_image		= (void *)0x04418100,
		.image_crc		= (void *)0x04418080,
		.flash_init		= (void *)0x04418020,
		.get_boot_src		= (void *)0x044180c0,
		.set_boot_src		= (void *)0x044180a0,
		.get_number_of_retries	= (void *)0x04418060,
		.verify_rsa		= (void *)0x04418040,
		.cryptodma_aes_decrypt	= (void *)0x044180e0,
	};

	return &api;
}

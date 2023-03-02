#ifndef config
#define config
#include "../YamlParser/parser.h"

/* Starting sector to start reading in sectors from. */
#define sector_after_mbr		(uint8)0x02

int32 main(int args, char *argv[]);

#define config_assert(cond, err_msg, ...)           \
    if(!(cond)) {                                   \
        fprintf(stderr, err_msg, ##__VA_ARGS__);    \
        exit(EXIT_FAILURE);                         \
    }

uint8 *read_format(const uint8 *filename, uint8 access[2])
{
	FILE* format = fopen(filename, access);
	uint8 *raw_format = NULL;

	config_assert(format, "Error openeing ../boot/boot_format.\n\tWas it deleted?\n")

	fseek(format, 0, SEEK_END);
	size_t format_size = ftell(format);
	fseek(format, 0, SEEK_SET);

	config_assert(format_size > 0, "Error with size of ../boot/bformat2.\n\tWas all the content removed?\n")

	raw_format = calloc(format_size, sizeof(*format));
	fread(raw_format, sizeof(uint8), format_size, format);
	fclose(format);

	return raw_format;
}

#endif
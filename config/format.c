#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bytes_per_sector		512
#define minimum_sectors			2
typedef unsigned char			uint8;
typedef unsigned short			uint16;

int main(int args, uint8 *argv[])
{
	/* `format.o` requires a file argument be passed, and perferrably a binary(.bin) file. */
	if(!(args > 1))
		exit(EXIT_FAILURE);
	
	/* Open up the file, check if it exists. If not, error. */
	FILE* f = fopen(argv[1], "rb");

	if(!(f))
	{
		fprintf(stderr, "Error!");
		exit(EXIT_FAILURE);
	}

	/* Get the size. */
	fseek(f, 0, SEEK_END);
	size_t file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* If the size is zero, error. */
	if(file_size == 0)
	{
		fprintf(stderr, "File has zero bytes.\n");
		exit(EXIT_FAILURE);
	}
	fclose(f);

	/* Get the correct amount of sectors. */
	uint8 sectors = (file_size / bytes_per_sector) + 1;

	/* Make sure the sectors is at least 2. If not, set it to two. */
	//if(!(sectors >= minimum_sectors))
	//	sectors = 2;
	
	/* Calculate the amount of padding we need to pad out binary to multiples of 512-byte blocks(sectors). */
	uint16 padding_needed = ((sectors * bytes_per_sector) - file_size);

	/* Allocate static memory, init all indexes with zero, then write. */
	uint8 padding[padding_needed];
	memset(padding, 0, padding_needed);

	/* Write padding to file. */
	f = fopen(argv[1], "a");

	if(!(f))
	{
		fprintf(stderr, "Error!");
		exit(EXIT_FAILURE);
	}

	fwrite(padding, padding_needed, sizeof(uint8), f);
	fclose(f);

	return 0;
}
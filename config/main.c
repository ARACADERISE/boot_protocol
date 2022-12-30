#include "config.h"

uint8* strdel(uint8* text, uint32 deleteStartPosition, uint32 deleteLength) {
  // Get end position
  uint32 deleteEndPosition = deleteStartPosition + deleteLength;
  
  if(deleteEndPosition > strlen(text))
  {
      fprintf(stderr, "End position surpasses length of text.\n");
      exit(EXIT_FAILURE);
  }

  // Copy the remaining text to the start of the deleted fragment
  // text + deleteStartPosition = destination
  // text + deleteEndPosition = src
  strcpy(text + deleteStartPosition, text + deleteEndPosition);

  // Return the modified text
  return text;
}

int32 main(int args, char *argv[])
{
	/* Get all the information we need. */
	_yaml_os_data yod = open_and_parse_yaml("../boot.yaml");

	/* Open MBR format. */
	FILE* boot_format = fopen("../boot/bformat2", "rb");

	config_assert(boot_format, "Error openeing ../boot/bformat2.\n\tWas it deleted?\n")

	fseek(boot_format, 0, SEEK_END);
	size_t format_size = ftell(boot_format);
	fseek(boot_format, 0, SEEK_SET);

	config_assert(format_size > 0, "Error with size of ../boot/bformat2.\n\tWas all the content removed?\n")

	uint8 *format = calloc(format_size, sizeof(*format));
	fread(format, sizeof(uint8), format_size, boot_format);

	fclose(boot_format);

	/* Write MBR. */
	FILE* boot_file = fopen("../boot/boot.asm", "w");

	format = realloc(format, (strlen(format) + 60) * sizeof(*format));

	uint8 format2[strlen(format)];
	sprintf(format2, format, 
		yod.ss_addr*16, 							// jmp 0x0:second_stage_addr
		yod.ss_addr,								// .second_stage_addr dw addr 
		yod.ss_addr*16, 							// .second_stage_loc dw addr
		yod.kern_addr, 								// .kernel_addr dw addr
		yod.kern_addr*16, 							// .kernel_loc dw addr
		strdel(yod.ss_filename_bin_name, 0, 3), 	// second_stage: incbin second stage binary
		strdel(yod.kern_filename_bin_name, 0, 3));	// kernel: incbin kernel binary
	fwrite(format2, sizeof(uint8), strlen(format2), boot_file);

	fclose(boot_file);

	FILE* makefile_format = fopen("makefile_format", "r");

	config_assert(makefile_format, "Error opening makefile format.\n")

	fseek(makefile_format, 0, SEEK_END);
	size_t makefile_size = ftell(makefile_format);
	fseek(makefile_format, 0, SEEK_SET);

	config_assert(makefile_size > 0, "Error with size of makefile format. Was it deleted?\n")

	uint8 *mformat = calloc(makefile_size, sizeof(*mformat));
	fread(mformat, sizeof(uint8), makefile_size, makefile_format);

	fclose(makefile_format);

	FILE* makefile = fopen("../Makefile", "w");

	mformat = realloc(mformat, (strlen(mformat) + 60) * sizeof(*mformat));

	uint8 mformat2[strlen(mformat)];
	sprintf(mformat2, mformat, 
		strdel(yod.ss_filename_bin_name,0,3),
		yod.ss_filename,
		strdel(yod.kern_filename_bin_name,0,3),
		yod.kern_filename,
		yod.ss_filename_bin_name,
		yod.kern_filename_bin_name);
	
	fwrite(mformat2, sizeof(uint8), strlen(mformat2), makefile);

	fclose(makefile);

	free(format);
	free(mformat);

	return 0;
}
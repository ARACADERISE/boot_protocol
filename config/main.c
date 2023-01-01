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
	FILE* boot_format = fopen("formats/boot_format", "rb");

	config_assert(boot_format, "Error openeing ../boot/boot_format.\n\tWas it deleted?\n")

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

	/* Read in format(`protocol/gdt/gdt_ideals.asm`) and write in the address. */
	FILE* bit32_jump_format = fopen("../protocol/gdt/gdt_ideals.asm", "rb");

	config_assert(bit32_jump_format, "Error opening `../protocol/gdt/gdt_ideals.asm`.\n")

	fseek(bit32_jump_format, 0, SEEK_END);
	size_t bit32_jump_format_size = ftell(bit32_jump_format);
	fseek(bit32_jump_format, 0, SEEK_SET);

	config_assert(bit32_jump_format_size > 0, "File must've been deleted :(.\n")

	uint8 *jump_format = calloc(bit32_jump_format_size, sizeof(*jump_format));
	fread(jump_format, bit32_jump_format_size, sizeof(uint8), bit32_jump_format);

	fclose(bit32_jump_format);

	FILE* bit32_jump = fopen("../protocol/gdt/gdt_ideals.asm", "wb");

	config_assert(bit32_jump, "Error opening `../protocol/gdt/gdt_ideals.asm`.\n")

	jump_format = realloc(jump_format, (strlen(jump_format) + 60) * sizeof(*jump_format));
	uint8 jump_format2[strlen(jump_format)];
	sprintf(jump_format2, jump_format, yod.kern_addr*16);
	fwrite(jump_format2, strlen(jump_format2), sizeof(uint8), bit32_jump);

	fclose(bit32_jump);

	FILE* makefile_format = fopen("formats/makefile_format", "r");

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
		strdel(yod.ss_filename_bin_o_name,0,3),
		yod.ss_filename,
		strdel(yod.kern_filename_bin_o_name,0,3),
		yod.kern_filename,
		yod.ss_filename_bin_o_name,
		yod.kern_filename_bin_o_name);
	
	fwrite(mformat2, sizeof(uint8), strlen(mformat2), makefile);

	fclose(makefile);

	free(format);
	free(mformat);

	return 0;
}
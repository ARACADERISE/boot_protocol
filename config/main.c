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

int32 main(int args, char *argv[])
{
	/* Get all the information we need. */
	_yaml_os_data yod = open_and_parse_yaml("../../boot.yaml");

	/* Local variables. */
	uint8 *format = NULL;
	uint8 ss_o_filename[50] = "../";
	uint8 kernel_o_filename[50] = "../";
	uint8 ss_bin_file[50] = "../";
	uint8 kern_bin_file[50] = "../";

	/* Files. */
	FILE* boot_file = fopen("../boot/boot.s", "w");
	FILE* makefile = fopen("../Makefile", "w");

	if(yod.has_second_stage == true)
	{
		/* Open MBR format. */
		format = read_format((const uint8 *)"formats/boot_format", "rb");

		/* Write MBR. */
		format = realloc(format, (strlen(format) + 60) * sizeof(*format));

		uint8 format2[strlen(format)];
		sprintf(format2, format,
			yod.kern_addr, 								// .kernel_addr dw addr
			yod.kern_addr*16, 							// .kernel_loc dw addr
			strcat(ss_bin_file, yod.ss_filename_bin_name), 	// second_stage: incbin second stage binary
			strcat(kern_bin_file, yod.kern_filename_bin_name));	// kernel: incbin kernel binary
		fwrite(format2, sizeof(uint8), strlen(format2), boot_file);

		fclose(boot_file);
	} else
	{
		format = read_format((const uint8 *)"formats/boot_format2", "rb");

		/* Write MBR. */
		format = realloc(format, (strlen(format) + 60) * sizeof(*format));

		uint8 format2[strlen(format)];
		sprintf(format2, format,
			yod.kern_addr*16,							// jmp 0x8:kernel_address
			yod.kern_addr, 								// .kernel_addr dw addr
			yod.kern_addr*16, 							// .kernel_loc dw addr
			strdel(yod.kern_filename_bin_name, 0, 3)	// kernel: incbin kernel binary
		);
		fwrite(format2, sizeof(uint8), strlen(format2), boot_file);

		fclose(boot_file);
	}

	/* Read in format(`protocol/gdt/gdt_ideals.asm`) and write in the address. */
	/*format = read_format((const uint8 *)"../protocol/gdt/gdt_ideals.s", "rb");

	FILE* bit32_jump = fopen("../protocol/gdt/gdt_ideals.s", "wb");

	config_assert(bit32_jump, "Error opening `../protocol/gdt/gdt_ideals.s`.\n")

	format = realloc(format, (strlen(format) + 60) * sizeof(*format));
	uint8 jump_format2[strlen(format)];
	sprintf(jump_format2, format, yod.kern_addr*16);
	fwrite(jump_format2, strlen(jump_format2), sizeof(uint8), bit32_jump);

	fclose(bit32_jump);*/

	uint8 *mformat = NULL;
	if(yod.has_second_stage == true)
	{
		mformat = read_format((const uint8 *)"formats/makefile_format", "r");

		/* Create static memory. Allow for additional 60 characters. */
		uint8 mformat1[strlen(mformat)+60];

		/* Apply values and write. */
		sprintf(mformat1, mformat, 
			strcat(ss_o_filename, yod.ss_filename_bin_o_name),
			yod.ss_filename,
			strcat(kernel_o_filename, yod.kern_filename_bin_o_name),
			yod.kern_filename,
			yod.ss_filename_bin_o_name,
			yod.kern_filename_bin_o_name,
			ss_bin_file, kern_bin_file,
			ss_bin_file, kern_bin_file);
		
		fwrite(mformat1, strlen(mformat1), sizeof(*mformat1), makefile);

		fclose(makefile);
	} else
	{
		mformat = read_format("formats/makefile_format2", "r");

		/* Create static memory. Allow for additional 60 characters. */
		uint8 mformat2[strlen(mformat)+60];

		/* Apply values and write. */
		sprintf(mformat2, mformat, 
			strcat(kernel_o_filename, yod.kern_filename_bin_o_name),
			yod.kern_filename,
			yod.kern_filename_bin_o_name);
		
		fwrite(mformat2, strlen(mformat2)-1, sizeof(*mformat2), makefile);

		fclose(makefile);
	}

	/* Write the users Makefile. */
	FILE *user_makefile = fopen("../../Makefile", "w");

	config_assert(user_makefile, "Error creating users-makefile.\n")
	format = read_format((const uint8 *)"formats/user_makefile_format", "rb");

	/* Write. */
	fwrite(format, strlen(format), sizeof(*format), user_makefile);
	fclose(user_makefile);

	free(format);
	free(mformat);

	return 0;
}
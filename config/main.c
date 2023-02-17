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

		/* Open `mbr_partition_table.bin` to see how many sectors it is then add that to `sector_after_mbr`. */
		FILE* mbr_part_table_bin = fopen("../bin/mbr_partition_table.bin", "rb");

		if(!(mbr_part_table_bin))
		{
			fclose(mbr_part_table_bin);
			exit(1);
		}

		fseek(mbr_part_table_bin, 0, SEEK_END);
		size_t bin_size = ftell(mbr_part_table_bin);
		fseek(mbr_part_table_bin, 0, SEEK_SET);

		fclose(mbr_part_table_bin);

		uint8 format2[strlen(format)+120];
		uint8 sector = sector_after_mbr + (bin_size / 512);
		sprintf(format2, format,
			bin_size/512,
			sector,
			sector + (yod.ss_filename_bin_size / 512),
			yod.ss_filename_bin_size / 512,
			sector + (yod.ss_filename_bin_size / 512),
			sector + ((yod.ss_filename_bin_size / 512) + (yod.kern_filename_bin_size / 512)),
			yod.kern_filename_bin_size / 512,
			sector + ((yod.ss_filename_bin_size / 512) + (yod.kern_filename_bin_size / 512)),
			0x15, 0x15,
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

	/* TODO: Figure out what we're doing here. Do we want to format `gdt_ideals.s` with C, or continue to do it via `quick_edit.py`?
	 *
	 * As of current, `quick_edit.py` deals with making sure the file `boot.yaml` has all the
	 * required data as well as writing to the file `protocol/gdt/gdt_ideals.asm`.
	 * I would like to make this happen in C, however, currently, there is a road block as
	 * the kernel/second-stage binary files HAVE to exist for this file(`main.c`,`main.o`) to run, however, this file NEEDS to run(if its configuring `gdt_ideals.s`)
	 * before the according binary files are generated. So, currently, we're in a stalemate.
	 */

	if(yod.has_second_stage == true)
	{
		format = read_format((const uint8 *)"formats/makefile_format", "r");

		/* Create static memory. Allow for additional 120 characters. */
		uint8 mformat1[strlen(format)+120];

		/* Apply values and write. */
		sprintf(mformat1, format, 
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
		format = read_format("formats/makefile_format2", "r");

		/* Create static memory. Allow for additional 120 characters. */
		uint8 mformat2[strlen(format)+120];

		/* Apply values and write. */
		sprintf(mformat2, format, 
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

	return 0;
}
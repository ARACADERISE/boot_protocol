#include "config_util.h"
#include <sys/stat.h>

int32 main(int args, char *argv[])
{
	/* Get all the information we need. */
	_yaml_os_data yod = open_and_parse_yaml("../../boot.yaml");

	/* Format that gets read from the file. */
	uint8 *format_ = NULL;
	uint8 *temp_binary_data = NULL;

	/* Format that gets written to the destination file. */
	uint8 format2[bytes_per_sector * 8];
	memset(format2, 0, bytes_per_sector * 8);

	/* Other local variables. */
	uint8 kernel_o_file[80] = "../";
	uint8 kernel_bin_file[80] = "../../";

	/* Actual paths of binary files when not dealing with them from a different directory. */
	const uint8 *MBR_binary_file_path   			= (const uint8 *)initiate_path(yod.bin_folder, "/boot.bin");
	const uint8 *second_stage_binary_file_path		= (const uint8 *)initiate_path(yod.bin_folder, "/second_stage.bin");
	const uint8 *mbr_part_table_bin_file_path		= (const uint8 *)initiate_path(FAMP_bin_folder, "/mbr_partition_table.bin");
	const uint8 *higher_half_kernel_bin_file_path	= (const uint8 *)initiate_path(FAMP_bin_folder, "/higher_half_kernel.bin");
	const uint8 *kernel_bin_file_path				= (const uint8 *)initiate_path(yod.bin_folder, "/kernel.bin");
	const uint8 *temp_disk_image_path				= (const uint8 *)initiate_path("../bin/", initiate_path(yod.disk_name, ".fimg"));

	/* Get the entire path to the disk image. */
	const uint8 *FAMP_disk_image_path 				= (const uint8 *)initiate_path(FAMP_disk_image_folder, yod.disk_name);

	/* The overall disk image. Allocate with initial 512 bytes for MBR. */
	disk_image = calloc(MBR_size, sizeof(*disk_image));

	/* Keep track of the position we're at in `disk_image`. */
	size_t disk_image_size	= 0;

	/* Initialize first 512 bytes to zero. "Reserved" for when the bootloader assembly code gets assembled. */
	memset(disk_image, 0, MBR_size);
	disk_image_size += MBR_size;

	/* Binary sizes. */
	size_t mbr_tbl_bin_size = 0;
	size_t ssb_size		= 0;

	/* Files. 
	 * In exact order for the disk image:
	 *		`mbr_part_table_bin` -> `second_stage_bin` -> `higher_half_kern_bin` -> kernel binary.
	 *
	 * Open up binary file for second stage bootloader, check if the file is valid and get the size.
	 * */
	FILE *second_stage_bin 		= open_and_assert("../bin/second_stage.bin", "rb");
	ssb_size = get_file_size(second_stage_bin, NULL);
	disk_image_size += reallocate_disk_image_size(disk_image_size, ssb_size);

	/* Open up binary file for the program that works with the MBR partition table, check if the file is valid and get the size. */
	FILE *mbr_part_table_bin 	= open_and_assert("../bin/mbr_partition_table.bin", "rb");
	mbr_tbl_bin_size = get_file_size(mbr_part_table_bin, NULL);
	disk_image_size += reallocate_disk_image_size(disk_image_size, mbr_tbl_bin_size);
	
	/* Open up binary file for the program that relocates the kernel into higher half, check if the file is valid and get the size. */
	FILE *higher_half_kern_bin	= open_and_assert("../bin/higher_half_kernel.bin", "rb");
	disk_image_size += reallocate_disk_image_size(disk_image_size, get_file_size(higher_half_kern_bin, NULL));

	/* Concat `kernel_bin_file` to `absolute_kernel_binary_path` to get the correct path to the kernels binary file.
	 * This program resides in `boot_protocol/config` whilst majority of binary files reside outside of the directory `boot_protocol`.
	 */
	strcat(kernel_bin_file, yod.kern_filename_bin_name);

	/* Make sure that `50` is enough static memory for the path. */
	config_assert(strlen(kernel_bin_file) <= 80, 
		"File path is too large. Make sure the binary file for the kernel is not too long.\nPaths are allowed up to 50 characters.\n", false, NULL)

	/* Open the kernel binary file, and make sure the file is valid. */
	FILE *kernel_bin = open_and_assert(kernel_bin_file, "rb");

	/* Make sure that `yod.kern_filename_bin_size` is the actual size. */
	config_assert(get_file_size(kernel_bin, NULL) == yod.kern_filename_bin_size, 
		"Size mismatch for kernel binary file located at %s.\n", false, NULL, kernel_bin_file_path)
	disk_image_size += reallocate_disk_image_size(disk_image_size, get_file_size(kernel_bin, NULL));

	// This will get opened later on
	FILE *MBR_bin				= NULL;

	/* Temporary FS binary file. 
	 * TODO: We need to make it to where this is all in a function of itself.
	 *       `format_disk_image.c` will reformat `boot.s` so that way the MBR partition table entry representing the FS has the correct data.
	 */
	FILE *FS_bin				= NULL;

	/* Sector count(LBA #). */
	uint8 sector = sector_after_mbr + (mbr_tbl_bin_size / 512);

	/* FS size, this will be passed to `write_MBR` so we know if the FS size is 1 * 512 or 5 * 512. */
	size_t FS_size = 0;

	if(yod.auto_format == false)
	{
		FS_size = 1 * bytes_per_sector;
		/* Make sure `disk_image` will have enough memory for the FS. */
		disk_image_size += reallocate_disk_image_size(disk_image_size, FS_size);

		/* Make sure the protocol will be able to tell there is no FS mounted to the disk image. */
		FS_bin = open_and_assert(initiate_path("../", "tools_bin/temp_FS_part_header.bin"), "wb");

		temp_binary_data = calloc(FS_size, sizeof(*temp_binary_data));
		for(uint8 i = 0; i < sizeof(no_fs)/sizeof(no_fs[0]); i++) memset(&temp_binary_data[i], no_fs[i], 1);
		fwrite(temp_binary_data, sizeof(*temp_binary_data), FS_size, FS_bin);
		
		free(temp_binary_data);
		fclose(FS_bin);
	} else
	{
		FS_size = 5 * bytes_per_sector;
		disk_image_size += reallocate_disk_image_size(disk_image_size, FS_size);
		FS_bin = open_and_assert(initiate_path("../", "tools_bin/temp_FS_part_header.bin"), "wb");
		
		temp_binary_data = calloc((FS_size) - sizeof(_PartitionHeader), sizeof(*temp_binary_data));

		/* Get the partition header assigned. */
		_PartitionHeader *part_header = calloc(1, sizeof(*part_header));
		init_FS_type_and_part_type(part_header, true, &yod, NULL, NULL, get_file_size(kernel_bin, NULL));
		set_start_and_end_LBA(part_header, (((sector - 1) * bytes_per_sector) + ssb_size + get_file_size(higher_half_kern_bin, NULL) + yod.kern_filename_bin_size) / 512, FS_size);

		fwrite(part_header, sizeof(*part_header), 1, FS_bin);
		fwrite(temp_binary_data, sizeof(*temp_binary_data), (FS_size) - sizeof(_PartitionHeader), FS_bin);

		fclose(FS_bin);
	}

	if(args > 1 && strcmp(argv[1], "eve") == 0) goto everything_else;

	/* Write MBR. This "scope" is just to write information to the file `boot.s`. */
	{
		write_MBR(&yod, sector, ssb_size, FS_size, NULL, NULL, 0, NULL, 0, 0);

		fclose(second_stage_bin);
		fclose(mbr_part_table_bin);
		fclose(higher_half_kern_bin);
		fclose(kernel_bin);

		if(args > 1 && strcmp(argv[1], "mbr") == 0) return 0;
	}

	everything_else:


	/* Open up MBR binary, and perform according checks. */
	MBR_bin = open_and_assert("../../bin/boot.bin", "rb");

	/* See if the temporary disk image exists. */
	if(access(temp_disk_image_path, F_OK) == 0)
	{
		FILE *dimg_bin = open_and_assert(temp_disk_image_path, "rb");

		/* Array of all the binary file sizes, files and names. */
		size_t sizes[] = {MBR_size, mbr_tbl_bin_size, ssb_size, get_file_size(higher_half_kern_bin, NULL), yod.kern_filename_bin_size};
		FILE *fs[] = {MBR_bin, mbr_part_table_bin, second_stage_bin, higher_half_kern_bin, kernel_bin};
		uint8 *names[] = {"../../bin/boot.bin", "../bin/mbr_partition_table.bin", "../bin/second_stage.bin", "../bin/higher_half_kernel.bin", "../../bin/kernel.bin"};
		
		/* Set `disk_image_size` to zero so we have a variable to keep track of the position. */
		disk_image_size = 0;

		fread(disk_image, get_file_size(dimg_bin, NULL), sizeof(*disk_image), dimg_bin);

		/* Lets begin checking! */
		while(ind < sizeof(sizes)/sizeof(sizes[0]))
		{
			dicd[ind] = check_disk_chunk(disk_image, fs[ind], names[ind], sizes[ind], disk_image_size, ind == 2 || ind == 4 ? true : false, ind == 2 || ind == 4 ? true : false);

			/* Check memory stamp to the data occupied from the chunk of data.
			 * Perform the check only when the index is 2 or 4(ind of 2 = second stage, ind of 4 = kernel)
			 */
			if(ind == 2 || ind == 4)
				config_assert(is_memory_stamp_good(dicd[ind], ind == 2 ? boot_id_2 : kernel_id) == good_memory_stamp,
					"The memory stamp residing in the current chunk of the binary file %s does not match the metadata of the binary file itself.\n", false, NULL, names[ind])

			disk_image_size += dicd[ind].bytes_checked;
			disk_image = dicd[ind].image;
			ind++;
		}

		fclose(dimg_bin);
		goto end;
	}

	/* Lets start to fill out the `disk_image`. */
	size_t index = 0;
	index = fill_disk_image(MBR_bin, MBR_size, index, disk_image_size);
	index = fill_disk_image(mbr_part_table_bin, mbr_tbl_bin_size, index, disk_image_size);
	index = fill_disk_image(second_stage_bin, ssb_size, index, disk_image_size);
	index = fill_disk_image(higher_half_kern_bin, get_file_size(higher_half_kern_bin, NULL), index, disk_image_size);
	index = fill_disk_image(kernel_bin, yod.kern_filename_bin_size, index, disk_image_size);

	/* Make sure the temporary FS binary file exists. */
	config_assert(access(initiate_path("../", "tools_bin/temp_FS_part_header.bin"), F_OK) == 0,
		"The temporary FS binary file does not exist. It must have been deleted.\n", false, NULL)
	
	FS_bin = open_and_assert(initiate_path("../", "tools_bin/temp_FS_part_header.bin"), "rb");
	index = fill_disk_image(FS_bin, get_file_size(FS_bin, NULL), index, disk_image_size);

	write_file(temp_disk_image_path, disk_image, disk_image_size);
	
	/* If the `if` statement above runs, the function `check_disk_chunk` closes all of the files after reading the files data.
	 * The if statement, at the end, then jumps to `end`. We only want to close these files if the above `if` statement does not run.
	 */
	fclose(mbr_part_table_bin);
	fclose(higher_half_kern_bin);
	fclose(MBR_bin);

	end:
	/* Write Makefile. This "scope" is just to write data to the `Makefile`. */
	{
		format_ = read_format((const uint8 *)"formats/makefile_format", "r");

		sprintf(format2, format_,
			initiate_path(FAMP_disk_image_folder, initiate_path(yod.disk_name, ".fimg")),
			strcat(kernel_o_file, yod.kern_filename_bin_o_name),
			yod.kern_filename_bin_o_name,
			yod.kern_filename,
			yod.kern_filename_bin_o_name,
			kernel_bin_file,
			kernel_bin_file,
			yod.kern_filename_bin_o_name);
		
		write_file("../Makefile", format2, strlen(format2));
		free(format_);
	}

	/* Write users Makefile. */
	{
		format_ = read_format((const uint8 *)"formats/user_makefile_format", "rb");
		write_file("../../Makefile", format_, strlen(format_));
		free(format_);
	}

	/* Configuring tools. */
	{
		format_ = read_format((const uint8 *)"formats/FAMP_fdi_format", "rb");
		sprintf(format2, format_, initiate_path(FAMP_disk_image_folder, initiate_path(yod.disk_name, ".fimg")));
		write_file("scripts/FAMP_fdi", format2, strlen(format2));
	}

	/* Write second stage linker.
	 * TODO: Do we need to write the kernels linker too so it knows where the second stage bootloader is located?
	 */
	{
		format_ = read_format((const uint8 *)"formats/ss_linker_format", "rb");
		sprintf(format2, format_, (0x7E00 + get_file_size(second_stage_bin, NULL)) - 512, (0xA000 + get_file_size(kernel_bin, NULL)) - 512, get_file_size(kernel_bin, NULL), 0xA000);
		write_file("../linker/linker.ld", format2, strlen(format2));
	}

	fclose(second_stage_bin);
	fclose(kernel_bin);

	free((uint8 *)MBR_binary_file_path);
	free((uint8 *)second_stage_binary_file_path);
	free((uint8 *)mbr_part_table_bin_file_path);
	free((uint8 *)higher_half_kernel_bin_file_path);
	free((uint8 *)kernel_bin_file_path);
	free((uint8 *)temp_disk_image_path);

	return 0;
}
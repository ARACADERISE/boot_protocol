#include "config_util.h"
#include <sys/stat.h>

// Dumb little function to shorten the amount of code
uint8 *initiate_path(uint8 data1[], uint8 data2[])
{
	/* `data1`, at least, has to be valid and not NULL. */
	config_assert(data1, "Cannot initiate the path. No data given to configure the path.\n", false, NULL)

	/* Initiate `array` and concat `data1` to it. */
	uint8 *array = calloc(strlen(data1), sizeof(*array));
	strcat(array, data1);

	/* If `data2` is not NULL, go ahead and perform the same action as above. */
	if(data2 != NULL)
	{
		array = realloc(array, ((strlen(data1) + 1) + strlen(data2)) * sizeof(*array));
		
		config_assert(strlen(data1) + strlen(data2) <= 50, 
			"Path is too large: %s.\nFAMP only allows up to 50 characters for a path.\n", false, NULL, strcat(array, data2))
		
		strcat(array, data2);
	}

	return array;
}

int32 main(int args, char *argv[])
{
	/* Get all the information we need. */
	_yaml_os_data yod = open_and_parse_yaml("../../boot.yaml");

	/* Local variables. */
	uint8 *format_ = NULL;
	uint8 kernel_o_file[50] = "../";
	uint8 kernel_bin_file[50] = "../../";

	/* Actual paths of binary files when not dealing with them from a different directory. */
	const uint8 *MBR_binary_file_path   			= (const uint8 *)initiate_path(yod.bin_folder, "/boot.bin");
	const uint8 *second_stage_binary_file_path		= (const uint8 *)initiate_path(yod.bin_folder, "/second_stage.bin");
	const uint8 *mbr_part_table_bin_file_path		= (const uint8 *)initiate_path(FAMP_bin_folder, "/mbr_partition_table.bin");
	const uint8 *higher_half_kernel_bin_file_path	= (const uint8 *)initiate_path(FAMP_bin_folder, "/higher_half_kernel.bin");
	const uint8 *kernel_bin_file_path				= (const uint8 *)initiate_path(yod.bin_folder, "/kernel.bin");
	const uint8 *temp_disk_image_path				= (const uint8 *)initiate_path("../bin/", "temp.fimg");

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
	FILE *second_stage_bin 		= fopen("../bin/second_stage.bin", "rb");
	config_assert(second_stage_bin, 
		"Error finding binary file for second stage bootloader. Should be located at: %s.\n", true, second_stage_bin, second_stage_binary_file_path)
	ssb_size = get_file_size(second_stage_bin, NULL);
	disk_image_size += reallocate_disk_image_size(disk_image_size, ssb_size);

	/* Open up binary file for the program that works with the MBR partition table, check if the file is valid and get the size. */
	FILE *mbr_part_table_bin 	= fopen("../bin/mbr_partition_table.bin", "rb");
	config_assert(mbr_part_table_bin, 
		"Error finding program to parse the MBR partition table. Should be located at: %s.\n", false, NULL, mbr_part_table_bin_file_path)
	mbr_tbl_bin_size = get_file_size(mbr_part_table_bin, NULL);
	disk_image_size += reallocate_disk_image_size(disk_image_size, mbr_tbl_bin_size);
	
	/* Open up binary file for the program that relocates the kernel into higher half, check if the file is valid and get the size. */
	FILE *higher_half_kern_bin	= fopen("../bin/higher_half_kernel.bin", "rb");
	config_assert(higher_half_kern_bin,
		"Error finding program that relocates the kernel into higher half. Should be located at: %s.\n", false, NULL, higher_half_kernel_bin_file_path)
	disk_image_size += reallocate_disk_image_size(disk_image_size, get_file_size(higher_half_kern_bin, NULL));

	// The get opened later on
	FILE *kernel_bin			= NULL;
	FILE *MBR_bin				= NULL;
	FILE *dimg_bin				= NULL;

	if(args > 1 && strcmp(argv[1], "eve") == 0) goto everything_else;

	/* Write MBR. */
	{
		/* This "scope" is just to write information to the file `boot.s`. */
		FILE* boot_file				= fopen("../boot/boot.s", "w");

		/* Obtain the MBR format. */
		format_ = read_format((const uint8 *)"formats/boot_format", "rb");
		format_ = realloc(format_, (strlen(format_) + 60) * sizeof(*format_));

		uint8 format2[strlen(format_)+120];
		uint8 sector = sector_after_mbr + (mbr_tbl_bin_size / 512);
		sprintf(format2, format_,
			mbr_tbl_bin_size/512,
			yod.FS_type,
			yod.OS_name,
			yod.OS_version,
			yod.type,
			sector,
			sector + (ssb_size / 512),
			ssb_size / 512,
			sector + (ssb_size / 512),
			sector + ((ssb_size / 512) + (yod.kern_filename_bin_size / 512)),
			yod.kern_filename_bin_size / 512,
			sector + ((ssb_size / 512) + (yod.kern_filename_bin_size / 512)),
			0x15, 0x15);
			//second_stage_binary_file_path,//"bin/second_stage.bin",//strcat(ss_bin_file, yod.ss_filename_bin_name), 	// second_stage: incbin second stage binary
			//strcat(kernel_bin_file, yod.kern_filename_bin_name));	// kernel: incbin kernel binary
		fwrite(format2, sizeof(uint8), strlen(format2), boot_file);

		fclose(boot_file);
		free(format_);

		if(args > 1 && strcmp(argv[1], "mbr") == 0) return 0;
	}

	everything_else:

	/* Open up MBR binary, and perform according checks. */
	MBR_bin = fopen("../../bin/boot.bin", "rb");
	config_assert(MBR_bin, 
		"Error finding binary file for MBR.\n", true, MBR_bin)
	
	/* Make sure that `50` is enough static memory for the path. */
	config_assert(strlen(kernel_bin_file) + 3 <= 50, 
		"File path is too large. Make sure the binary file for the kernel is not too long.\nPaths are allowed up to 50 characters.\n", false, NULL)

	/* If the above assertion passes, concat `kernel_bin_file` to `absolute_kernel_binary_path` to get the correct path to the kernels binary file.
	 * This program resides in `boot_protocol/config` whilst majority of binary files reside outside of the directory `boot_protocol`.
	 */
	strcat(kernel_bin_file, yod.kern_filename_bin_name);

	/* Open the kernel binary file, and make sure the file is valid. */
	kernel_bin = fopen(kernel_bin_file, "rb");
	config_assert(kernel_bin, 
		"Error finding binary file for kernel. Should be located at: %s\n", true, kernel_bin, kernel_bin_file)

	/* Make sure that `yod.kern_filename_bin_size` is the actual size. */
	config_assert(get_file_size(kernel_bin, NULL) == yod.kern_filename_bin_size, 
		"Size mismatch for kernel binary file located at %s.\n", false, NULL, kernel_bin_file_path)
	disk_image_size += reallocate_disk_image_size(disk_image_size, get_file_size(kernel_bin, NULL));

	/* See if the temporary disk image exists. */
	if(access(temp_disk_image_path, F_OK) == 0)
	{
		dimg_bin = fopen(temp_disk_image_path, "rb");

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
			dicd[ind] = check_disk_chunk(disk_image, fs[ind], names[ind], sizes[ind], disk_image_size, ind == 2 || ind == 4 ? true : false);

			/* Check memory stamp to the data occupied from the chunk of data.
			 * Perform the check only when the index is 2 or 4(ind of 2 = second stage, ind of 4 = kernel)
			 */
			if(ind == 2 || ind == 4)
				config_assert(is_memory_stamp_good(dicd[ind], ind == 2 ? boot_id_2 : kernel_id) == true,
					"The memory stamp residing in the current chunk of the binary file %s does not match the metadata of the binary file itself.\n", false, NULL, names[ind])

			disk_image_size += dicd[ind].bytes_checked;
			disk_image = dicd[ind].image;
			ind++;
		}

		for(char i = 0; i < 5; i++)
		{
			printf("\n\n\tBytes Checked: %ld\n\tStatus: %s\n\tBad Bytes Found: %d\n\tBad Bytes Fixed: %d\n\t",
				dicd[i].bytes_checked, 
				(dicd[i].status[0] == good || dicd[i].status[1] == good) || (dicd[i].status[0] == fixed || dicd[i].status[1] == fixed) ? "good/fixed" : "bad :c",
				dicd[i].bad_bytes, dicd[i].corrected_bytes);
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

	dimg_bin = fopen(temp_disk_image_path, "wb");
	config_assert(dimg_bin, 
		"Error opening up %s.\n", true, dimg_bin, FAMP_disk_image_path)
	fwrite(disk_image, disk_image_size, sizeof(*disk_image), dimg_bin);
	fclose(dimg_bin);

	fclose(second_stage_bin);
	fclose(mbr_part_table_bin);
	fclose(kernel_bin);
	fclose(MBR_bin);
	
	end:
	/* Write Makefile. */
	{
		/* This "scope" is just to write data to the `Makefile`. */
		FILE* makefile				= fopen("../Makefile", "w");

		config_assert(makefile, 
			"Error editing(or opening) the protocols Makefile. This normally occurrs when files have been tweaked by the user.\n", true, makefile)

		format_ = read_format((const uint8 *)"formats/makefile_format", "r");
		uint8 mformat1[strlen(format_)+180];

		sprintf(mformat1, format_,
			initiate_path(FAMP_bin_folder, "/temp.fimg"),//"../bin/OS.image",
			strcat(kernel_o_file, yod.kern_filename_bin_o_name),
			yod.kern_filename_bin_o_name,
			yod.kern_filename,
			yod.kern_filename_bin_o_name,
			kernel_bin_file,
			kernel_bin_file,
			yod.kern_filename_bin_o_name);
		
		fwrite(mformat1, strlen(mformat1), sizeof(*mformat1), makefile);

		fclose(makefile);
		free(format_);
	}

	/* Write users Makefile. */
	{
		/* Write the users Makefile. */
		FILE *user_makefile = fopen("../../Makefile", "w");

		/* Make sure the Makefile got opened(or created). */
		config_assert(user_makefile, 
			"Error creating users-makefile.\n", true, user_makefile)

		format_ = read_format((const uint8 *)"formats/user_makefile_format", "rb");
		fwrite(format_, strlen(format_), sizeof(*format_), user_makefile);
		fclose(user_makefile);
	}
	free(format_);

	return 0;
}

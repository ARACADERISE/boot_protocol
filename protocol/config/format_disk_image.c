#include "format_disk_image.h"

/* format_disk_image.c - Program that formats a disk image with a FS. */

int main(int args, char *argv[])
{
    if(args <= 5)
    {
        fprintf(stderr, "Expected at least three arguments: [type_of_filesystem] [part_type] [size_of_filesystem]\n\tWhere `size_of_filesystem` has to be >= 5 sectors.\n");
        fprintf(stderr, "For more information, run `FAMP_fdi --h`. FAMP tools should be installed if you followed documentation and ran `make tools`. If you did not do this, it is adviced to go ahead and do it. If you have already ran your build simply run `make clean`, enter the directory\n`boot_protocol` and run `make tools`.\n\n");
        exit(EXIT_FAILURE);
    }

    /* TODO: We need to, somehow, make it to where the disk image can be formatted as many times as the user pleases.
     *       For now, formatting it only once is good enough.
     * TODO: For the time being, and as ever long as this restriction is in place, we need to make it to where the user can just clean out all `.fimg` binary files in
     *       `/usr/lib/FAMP_disk_images`. We also need to make it to where this script(`FAMP_fdi`) can create, and format, a disk image from nothing.
     */
    if(access(initiate_path(argv[1], "boot_protocol/tools_bin/format_done"), F_OK) == 0)
    {
        fprintf(stderr, "Disk image has already been formatted. Clean out the directory storing the disk image and reformat.\n");
        return 0;
    }

    _PartitionHeader *part_header = calloc(1, sizeof(*part_header));
    size_t FS_size = atoi(argv[5]) * bytes_per_sector;
    const uint8 *temp_fs_pheader_file_name = (const uint8 *)initiate_path(argv[1], "boot_protocol/tools_bin/temp_FS_part_header.bin");
    const uint8 *temp_fimg_binary_file_name = (const uint8 *)initiate_path(argv[1], "boot_protocol/bin/temp_image.fimg");

    /* Obtain OS information. */
    FILE *MBR = open_and_assert(initiate_path(argv[1], "bin/boot.bin"), "rb");

    /* FAMP OS information resides 256 bytes into the binary file. */
    fseek(MBR, 0x100, SEEK_SET);

    /* Get the data. */
    uint8 FS_type;
    uint8 OS_name[15];
    uint8 OS_version[5];
    uint8 OS_type;
    fread(&FS_type, sizeof(uint8), 1, MBR);
    fread(&OS_name, sizeof(uint8), 15, MBR);
    fread(&OS_version, sizeof(uint8), 5, MBR);
    fread(&OS_type, sizeof(uint8), 1, MBR);
    fclose(MBR);

    /* Check the wanted FS size. If it is not, just give the FS size the bare minimum(5 sectors). */
    if(FS_size >= 15 * bytes_per_sector) FS_size = 5 * bytes_per_sector;
    
    /* `argv[2]` should resemble the disk image, with the extension `.fimg`. */
    FILE *dimg_bin = open_and_assert(argv[2], "rb");

    /* Make sure we have 1 byte allocated so we can go ahead and reallocate memory for the disk image. */
    disk_image = calloc(1, sizeof(*disk_image));

    size_t bytes = reallocate_disk_image_size(0, get_file_size(dimg_bin, NULL));
    config_assert(bytes > 0,
        "Error reading disk image.\nNo bytes were read into memory.\n", true, dimg_bin)

    /* Read the disk image and store it in `disk_image`. */
    bytes = fill_disk_image(dimg_bin, bytes, 0, bytes);

    //fclose(dimg_bin);

    /* Add room for FS. */
    bytes += reallocate_disk_image_size(bytes, FS_size);
    
    set_start_and_end_LBA(part_header, bytes - FS_size, FS_size);

    /* Ignore CHS. */
    part_header->cylinder = part_header->head = part_header->sector = 0;
    
    /* Write the partition header to a temporary binary file so we can efficiently fill out the disk image. */
    FILE *temp_FS_part_header_file = open_and_assert(temp_fs_pheader_file_name, "wb");
    fwrite(part_header, 1, sizeof(*part_header), temp_FS_part_header_file);
    fclose(temp_FS_part_header_file);
    printf("\n%ld, %ld\n", bytes, bytes - FS_size);

    /* Fill out the disk image accordingly. */
    bytes = fill_disk_image(open_and_assert(temp_fs_pheader_file_name, "rb"), sizeof(_PartitionHeader), bytes - FS_size, bytes);

    write_file(temp_fimg_binary_file_name, disk_image, FS_size + get_file_size(dimg_bin, NULL));
    fclose(dimg_bin);

    /* This just lets `FAMP_fdi` know that the disk image has already been formatted. */
    FILE *already_formatted = fopen(initiate_path(argv[1], "boot_protocol/tools_bin/format_done"), "wb");
    fclose(already_formatted);

    /* Rewrite the MBR. */
    size_t second_stage_bin_size = get_file_size(open_and_assert(initiate_path(argv[1], "boot_protocol/bin/second_stage.bin"), "rb"), NULL);
    size_t mbr_part_table_bin_size = get_file_size(open_and_assert(initiate_path(argv[1], "boot_protocol/bin/mbr_partition_table.bin"), "rb"), NULL);
    size_t higher_half_kernel_bin_size = get_file_size(open_and_assert(initiate_path(argv[1], "boot_protocol/bin/higher_half_kernel.bin"), "rb"), NULL);
    size_t kernel_bin_size = get_file_size(open_and_assert(initiate_path(argv[1], "bin/kernel.bin"), "rb"), NULL);

    init_FS_type_and_part_type(part_header, false, NULL, argv[3], argv[4], kernel_bin_size);

    write_MBR(NULL, 2 + (mbr_part_table_bin_size / 512), second_stage_bin_size, FS_size, argv[1], OS_name, FS_type, OS_version, OS_type, kernel_bin_size);

    /* Make sure the temporary binary file is deleted. */
    config_assert(remove(temp_fs_pheader_file_name) == 0,
        "Error deleting the temporary FS partition header binary file at %s.\nAborting.\n", false, NULL, temp_fs_pheader_file_name)
    
    free((uint8 *)temp_FS_part_header_file);
    free((uint8 *)temp_fimg_binary_file_name);

    return 0;
}
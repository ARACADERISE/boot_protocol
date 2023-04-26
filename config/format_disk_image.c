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

    _PartitionHeader *part_header = calloc(1, sizeof(*part_header));
    size_t FS_size = atoi(argv[4]);
    const uint8 *temp_fs_pheader_file_name = (const uint8 *)initiate_path(argv[1], "boot_protocol/tools_bin/temp_FS_part_header.bin");

    /* Check the wanted FS size. If it is not, just give the FS size the bare minimum(5 sectors). */
    if(FS_size < 512 && FS_size < 32) FS_size = FS_size * 512; // If the user put in the amount of sectors they want we will multiply
    if(FS_size / 512 < 5) FS_size = 5 * bytes_per_sector;
    
    /* Check the type of FS. */
    if(strcmp(argv[3], "FAMP_CFS") == 0)
    {
        default_fs:
        part_header->FS_type = FS_FAMP_custom;
        part_header->FAMP_custom_FS_revision = 1;

        goto out;
    }
    else goto default_fs;

    out:
    /* Check partition type. */
    if(strcmp(argv[4], "UKA") == 0)     { part_header->partition_type = part_type_1; goto rest; } // User & Kernel Access
    if(strcmp(argv[4], "KOA") == 0)     { part_header->partition_type = part_type_2; goto rest; } // Kernel Access Only
    if(strcmp(argv[4], "CDKOA") == 0)   { part_header->partition_type = part_type_3; goto rest; } // Critical Data, Kernel Access Only
    if(strcmp(argv[4], "CDUKA") == 0)   { part_header->partition_type = part_type_4; goto rest; } // Critical Data, User & Kernel Access
    if(strcmp(argv[4], "E") == 0)       { part_header->partition_type = part_type_5; goto rest; } // Extra. This partition can resemble a FS without any regulations, or it can just resemble a random program.

    rest:
    /* `argv[2]` should resemble the disk image, with the extension `.fimg`. */
    FILE *dimg_bin = open_and_assert(argv[2], "rb");

    /* Make sure we have 1 byte allocated so we can go ahead and reallocate memory for the disk image. */
    disk_image = calloc(1, sizeof(*disk_image));

    size_t bytes = reallocate_disk_image_size(0, get_file_size(dimg_bin, NULL));
    config_assert(bytes > 0,
        "Error reading disk image.\nNo bytes were read into memory.\n", true, dimg_bin)

    /* Read the disk image and store it in `disk_image`. */
    bytes = fill_disk_image(dimg_bin, bytes, 0, bytes);

    fclose(dimg_bin);

    /* Add room for FS. */
    bytes += reallocate_disk_image_size(bytes, FS_size);

    /* Begin filling out the FS partition header. */
    for(uint8 i = 0; i < 6; i++)
        memset(&part_header->header_start[i], part_header_start[i], 1);
    
    part_header->starting_LBA = (bytes - FS_size) / 512;
    part_header->ending_LBA = bytes / 512;

    /* Ignore CHS. */
    part_header->cylinder = part_header->head = part_header->sector = 0;

    for(uint8 i = 0; i < 6; i++)
        memset(&part_header->header_end[i], part_header_end[i], 1);
    
    /* Write the partition header to a temporary binary file so we can efficiently fill out the disk image. */
    FILE *temp_FS_part_header_file = open_and_assert(temp_fs_pheader_file_name, "wb");
    fwrite(part_header, 1, sizeof(*part_header), temp_FS_part_header_file);
    fclose(temp_FS_part_header_file);

    /* Fill out the disk image accordingly. */
    bytes = fill_disk_image(open_and_assert(temp_fs_pheader_file_name, "rb"), sizeof(_PartitionHeader), bytes - FS_size, bytes);

    /* Make sure the temporary binary file is deleted. */
    config_assert(remove(temp_fs_pheader_file_name) == 0,
        "Error deleting the temporary FS partition header binary file at %s.\nAborting.\n", false, NULL, temp_fs_pheader_file_name)

    return 0;
}

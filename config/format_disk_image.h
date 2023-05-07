#ifndef protocol_disk_image
#define protocol_disk_image
#include "config_util.h"

#define FS_address      (uint16)0xF000

/* Partition types.
 * The FS dwells in a partition; rather, the FS is a partition in and of itself.
 * FAMP would like to know how to handle the partition. To the protocol, a partition is a set of memory that has a description for it.
 * For the FS, there is what is called a `partition header` which gives more in-depth detail/information over the partition. With the `partition header`,
 * FAMP will be able to decipher whether the given partition is a FS or not(which should not be too hard).
 * There are aims to use a partition header in a more wide variety of ways if FAMP ever ends up supporting the extended partition table.
 * For now, if FAMP finds a partition header it will default to assuming it found the FS, since the last partition in the MBR partition table is where the FS (should) be located.
 */
enum partition_type {
    part_type_1     = 1 << 0x00, // Available to user/kernel
    part_type_2     = 1 << 0x01, // Available to kernel only
    part_type_3     = 1 << 0x02, // Critical data, available only to kernel
    part_type_4     = 1 << 0x03, // Critical data, available to user and kernel(useful for if user has some sort of data/file they want to keep secured)
    part_type_5     = 1 << 0x04, // Extra. The partition(in specific, the FS within the partition) doesn't care what it has/where it is used
};

/* Types of filesystems. */
enum FS_types {
    FS_fat32        = 1 << 0x05,
    FS_ext2         = 1 << 0x06,
    FS_FAMP_custom  = 1 << 0x07,
    FS_ISO9660      = 1 << 0x08
};

/* Partition header beginning/endinng "tags". */
static const uint8 part_header_start[6]     = {'P', 'A', 'R', 'T', 'S', '\0'};
static const uint8 part_header_end[15]       = {'P', 'A', 'R', 'T', 'E', '\0'};

/* 44-byte wide partition header describing the partition. */
typedef struct PartitionHeader
{
    /* Signify the start of partition header. */
    uint8            header_start[6]; // "PARTS  ", in binary: "P A R T S 00"

    /* Partition type(available to user, not available to user/available 
     * to user AND the kernel etc). 
     */
    uint8            partition_type;

    /* LBA values where partition starts/ends. */
    uint32           starting_LBA;
    uint32           ending_LBA;

    /* Address of partition(really, address of FS). */
    uint16           partition_address;

    /* Virtual address of partition. */
    uint32           virtual_address;

    /* What type of FS does the user want? FAMPs custom FS, or a pre-existing one? */
    uint8            FS_type;

    /* If `FS_type` is `FS_FAMP_custom` then what revision of FAMPs custom FS is currently being used? 
     * Set to zero if we are using a FS other than FAMPs custom FS. 
     */
    uint32           FAMP_custom_FS_revision;

    /* CHS(Cylinder, Head, Sector). Just in case we ever do need it.
     * A later revision of `struct PartitionHeader` will, more than likely, remove support for CHS.
     */
    uint16           cylinder;
    uint8            head;
    uint32           sector;

    /* 9 of the 15 bytes will be padded out with zero, 
     * the last 6 bytes will represent `PARTE  `, in binary: `P A R T E 00`(indicating the end of the partition header). 
     */
    uint8            header_end[15];
} __attribute__((packed)) _PartitionHeader;

uint8 no_fs[] = "NO_FS_MOUNTED";

void init_FS_type_and_part_type(_PartitionHeader *pheader, bool use_yaml_data, _yaml_os_data *odata, uint8 *FS_type, uint8 *part_type, size_t kernel_size)
{
    config_assert((use_yaml_data && odata != NULL) || (!use_yaml_data && FS_type != NULL) || (!use_yaml_data && part_type != NULL),
        "Cannot initiate FS type and size.\nThere is no data to use to configure, or there is some wrong data passed to `init_FS_type_and_size`. Aborting.\n", false, NULL);
    
    memcpy(pheader->header_start, part_header_start, 6);
    memcpy(pheader->header_end, part_header_end, 15);

    if(use_yaml_data)
    {
        switch(odata->FS_type)
        {
            case 1: pheader->FS_type = (uint8)FS_FAMP_custom;pheader->FAMP_custom_FS_revision = 1;break;
            case 2: pheader->FS_type = (uint8)FS_fat32;break;
            case 3: pheader->FS_type = (uint8)FS_ext2;break;
            case 4: pheader->FS_type = (uint8)FS_ISO9660;break;
            default: pheader->FS_type =(uint8) FS_FAMP_custom;break;
        }

        /* If we are using the yaml data, by default the FS will be UKA(User & Kernel Access).
         * TODO: Make it to where `boot.yaml` supports a way for the user to tell FAMP what the type of partition is. For now, this is okay.
         */
        pheader->partition_type = part_type_1;

        goto end;
    }

    if(strcmp(FS_type, "FAMP_CFS") == 0) 
    {
        default_fs:
        pheader->FS_type = FS_FAMP_custom;
        pheader->FAMP_custom_FS_revision = 1;

        goto out;
    }
    else goto default_fs;

    out:
    /* Check partition type. */
    if(strcmp(part_type, "UKA") == 0)     { pheader->partition_type = part_type_1; goto end; } // User & Kernel Access
    if(strcmp(part_type, "KOA") == 0)     { pheader->partition_type = part_type_2; goto end; } // Kernel Access Only
    if(strcmp(part_type, "CDKOA") == 0)   { pheader->partition_type = part_type_3; goto end; } // Critical Data, Kernel Access Only
    if(strcmp(part_type, "CDUKA") == 0)   { pheader->partition_type = part_type_4; goto end; } // Critical Data, User & Kernel Access
    if(strcmp(part_type, "E") == 0)       { pheader->partition_type = part_type_5; goto end; } // Extra. This partition can resemble a FS without any regulations, or it can just resemble a random program.

    end:
    pheader->partition_address = FS_address;
    pheader->virtual_address = 0x80000000 + kernel_size;
    return;
}

void set_start_and_end_LBA(_PartitionHeader *pheader, size_t start_LBA, size_t FS_size)
{
    pheader->starting_LBA = start_LBA;
    pheader->ending_LBA = start_LBA + (FS_size / 512);
}

#endif
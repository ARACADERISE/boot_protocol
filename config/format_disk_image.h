#ifndef protocol_disk_image
#define protocol_disk_image
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* My own types. */
typedef unsigned char       uint8;
typedef signed char         int8;
typedef unsigned short      uint16;
typedef signed short        int16;
typedef unsigned int        uint32;
typedef signed int          int32;
typedef unsigned long       size;
typedef unsigned long long  lsize;

/* Booleans. */
#define true            1
#define false           0
#define FAMP_assert(cond, err_msg, ...)         \
    if(!(cond))     {                           \
        fprintf(stderr, err_msg, ##__VA_ARGS__);\
        exit(EXIT_FAILURE);                     \
    }

/* Binary file where the user wants the disk image to be located. */
static FILE *disk_image = NULL;

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
static const uint8 part_header_end[6]       = {'P', 'A', 'R', 'T', 'E', '\0'};

/* Default partition ID. This is supported just in case FAMP ends up supporting an extended partition table.
 * If it does, it will resemble each partition with a different partition ID(if the partition stores something important such as a FS).
 * Partition IDs will more than likely only be required(when and if extended partition table is introduced) for partitions that store a FS/separate disk image.
 */
static const uint8 default_partition_id[] = "DEFAULT_PART_ID";

#endif
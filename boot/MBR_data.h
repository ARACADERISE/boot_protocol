#ifndef protocol_MBR_data
#define protocol_MBR_data

#ifdef __cplusplus
extern "C" {
#endif

/* Custom types because.. why not :) */
typedef char                    int8;
typedef unsigned char           uint8;
typedef signed char             sint8;
typedef unsigned short          uint16;
typedef signed short            sint16;
typedef unsigned int            uint32;
typedef signed int              sint32;

/* Sizes(size_t). */
typedef unsigned long       size;
typedef unsigned long long  lsize;

/* Boolean data. */
typedef uint8                   bool;
#define true                    1
#define false                   0

/* Location of MBR. */
#define MBR_address                             0x7C00

/* Location of second stage bootloader. */
const uint16 second_stage_bootloader_addr = 0x7E00;

/* Location of kernel. We will use this value + amount of sectors * 512 to figure out
 * where the kernel ends for the program that is located between the second stage and
 * the kernel. This program will need to know the ending address of the kernel so it
 * knows the region of memory to copy to a higher-half address.
 */
#define kernel_addr                             0xA000

/* Offset from beginning of `MBR_address` where the MBR partition table
 * entries should be located. 
 */
#define MBR_partition_table_entries_offset     0x1BE
#define second_MBR_partition_table_entry       (0x1BE + 0) + 16
#define third_MBR_partition_table_entry        (0x1BE + 16) + 16
#define fourth_MBR_partition_table_entry       (0x1BE + 32) + 16

/* Entry types for `MBR_partition_table_entry`. */
#define extended_partition_table_id_1           0x05 // extended partition table not supported
#define extended_partition_table_id_2           0x0F // extended partition table not supported
#define partition_entry_ssb_id                  0x0E // ssb = second-stage bootloader
#define partition_entry_kern_id                 0x0D
#define partition_entry_FS_id                   0x0C
#define partition_entry_critical_FS_id          0x0B
#define partition_entry_unused                  0x00
// TODO: Come up with more types for the `entry_type` in `MBR_partition_table_entry`.

/* Data that each MBR partition table entry holds. */
typedef struct MBR_partition_table_entry
{
    // Is the entry being described bootable?(0x0 = not bootable, 0x80 = bootable)
    bool        is_bootable;

    // Starting sector/cylinder where sector resides
    uint16      starting_sector_and_cylinder;

    // Entry type(0x05/0x0F = extened partition table)
    uint8       entry_type;

    // Is the protocol going to automatically read in the needed memory?
    bool        auto_read_program;

    // Last sector/cylinder where the last sector resides
    uint16      last_sector_and_cylinder;

    // Relative sector(set to value of the lower 8-bits of `starting_sector_and_cylinder`)
    uint32      relative_sector;

    // How many sectors does the entry need for whatever program its describing?
    uint8      sector_amnt;

    // Padding
    uint32       pad;
} __attribute__((packed)) _MBR_partition_table_entry;

#ifdef __cplusplus
}
#endif

#endif
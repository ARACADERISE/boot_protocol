#include "mbr_partition_table.h"

void read_in_memory(uint16 addr, uint8 start_sector, uint8 sector_amount)
{
    uint16 a = (0x02 << 8) | sector_amount;

    /* Read from disk. This will be replaced when ATA PIO support is added. */
    __asm__("mov ax, %0" : : "dN"((uint16)addr));
    __asm__("mov es, ax\nxor bx, bx\nmov ax, %0" : : "d"((uint16) a));
    __asm__("mov ch, 0x00\nmov cl, %0" : : "dN"((uint8) start_sector));
    __asm__("mov dh, 0x00\nmov dl, 0x80\nint 0x13");
    return;
}

void check_not_extended_part_id(_MBR_partition_table_entry *entry, uint8 part_id_to_check)
{
    /* We don't support extended partition table. Error if the partition entry
     * got tweaked with to have one.
     */
    if(entry->entry_type == extended_partition_table_id_1 ||
       entry->entry_type == extended_partition_table_id_2 ||
       !(entry->entry_type == part_id_to_check))
       {
            /* Extended partition table is not supported. */
            __asm__("mov ah, 0x0E\nmov al, 'E'\nint 0x10\n");
            __asm__("cli\nhlt");
        }
    return;
}

void __attribute__((section("__start"))) main()
{
    //test_read();

    /* Obtain MBR partition table entries. */
    _MBR_partition_table_entry *entry = (_MBR_partition_table_entry *) (MBR_address + MBR_partition_table_entries_offset);

    /* Make sure the entry type ID is not extended partition, and make sure it refers to
     * `partition_entry_ssb_id`.
     */
    check_not_extended_part_id(entry, partition_entry_ssb_id);

    /* Read in memory for second stage. */
    read_in_memory(second_stage_bootloader_addr/16, entry->starting_sector_and_cylinder, entry->sector_amnt);
    
    /* Load the program that allows us to have a higher half kernel(32-bit program. First program to run before kernel).
     * This program is located right after the kernel and only needs 1 sector to do its job.
     */
    read_in_memory((second_stage_bootloader_addr + (entry->sector_amnt * 512)) / 16, entry->last_sector_and_cylinder, 1);

    /* This can just be `entry++`, but I like being extra. */
    entry = (_MBR_partition_table_entry *) (MBR_address + second_MBR_partition_table_entry);

    /* Make sure the entry type ID is not extended partition, and make sure it refers to
     * `partition_entry_kern_id`
     */
    check_not_extended_part_id(entry, partition_entry_kern_id);

    /* Read in memory for kernel. */
    read_in_memory(kernel_addr/16, entry->starting_sector_and_cylinder+1, entry->sector_amnt);

    /* Jump. */
    __asm__("jmp 0x0:0x7E00");

    /* We shouldn't get here, but if we do halt. */
    while(1);
}
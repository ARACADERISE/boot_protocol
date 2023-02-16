#include "mbr_partition_table.h"

void __attribute__((section("__start"))) main()
{
    test_read();

    /* Obtain MBR partition table entries. */
    _MBR_partition_table_entry *entry = (_MBR_partition_table_entry *) (MBR_address + MBR_partition_table_entries_offset);

    /* Second stage bootloader is located at 0x7E00. Where does it end? */
    uint16 second_stage_end = 0x7F00 + (entry->sector_amnt * 512);

    if(second_stage_end == 0x9100)
        __asm__("mov ah, 0x0E\nmov al, 'A'\nint 0x10");
    
    while(1);

    /*__asm__(
        "mov ax, %0\nmov es, ax\nxor bx, bx\n" : : "dN"((uint16) bin_data->second_stage_preset_loc)
    );
    __asm__(
        "mov ah, 0x02\nmov al, %0\n" : : "dN"((uint8) bin_data->second_stage_size)
    );
    __asm__(
        "mov ch, 0x00\nmov cl, 0x03\nmov dh, 0x00\nmov dl, 0x80\nint 0x13\njmp 0x0:0x7E00"
    );*/

    while(1);
}
#ifndef protocol_MBR_data
#include "MBR_data.h"
#endif

/* Starting address for kernel. */
static uint8 *kernel_start_address            = (uint8 *)0xA000;

/* Kernels "higher-half" address. */
static uint8 *kernel_higher_half_addr         = (uint8 *)0x80000000;

void __attribute__((section("__start"))) main()
{
    uint8 *d = (uint8 *)0xA000;
    
    _MBR_partition_table_entry *entry = (_MBR_partition_table_entry *) (0x7C00 + second_MBR_partition_table_entry);

    uint32 bytes_to_copy = entry->sector_amnt * 512;

    for(uint32 i = 0; i < bytes_to_copy; i++)
    {
        *kernel_higher_half_addr = *kernel_start_address;
        kernel_higher_half_addr++;
        kernel_start_address++;
    }
    __asm__("jmp 0x8:0x80000000");
    while(1);
}
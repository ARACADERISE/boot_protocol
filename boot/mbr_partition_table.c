#include "mbr_partition_table.h"

void __attribute__((section("__start"))) main()
{
    _MBR_partition_table_entry *entry = (_MBR_partition_table_entry *) (MBR_address + MBR_partition_table_entries_offset);

    // Halt
    while(1);
}
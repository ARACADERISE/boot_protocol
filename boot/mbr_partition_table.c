#include "mbr_partition_table.h"

void __attribute__((section("__start"))) main()
{
    __asm__("pusha\nmov ah, 0x0E\nmov al, 'H'\nint 0x10");

    // Halt
    while(1);
}
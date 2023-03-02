#define using_b8000
#include "boot_protocol/protocol/kernel_header.h"

void starting_point main()
{
    clear_screen(black);

    /* The first byte in the program `mbr_partition_table.c`(binary `mbr_partition_tabe.bin`)
     * should have the value 0xF3.
     */
    if(instant_free_addresses[0][0] == 0xF3)
        print("\n\n\n\tHello, @r World@re\n\t\t*evil laugh*");

    /* Halt. */
    while(1);
}
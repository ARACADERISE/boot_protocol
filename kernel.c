#define using_b8000
#include "boot_protocol/protocol/kernel_header.h"

void starting_point main()
{
    clear_screen(blue);
    init_cursor_pos();
    
    uint8 key = 1;

    while(1)
    {
        key = get_key();
        if(key == 0x01) break;
        if(key == 0) continue;
        put_char(key);
    }

    /* The first byte in the program `mbr_partition_table.c`(binary `mbr_partition_tabe.bin`)
     * should have the value 0x66.
     */
    if(instant_free_addresses[0][0] == 0x66)
        print("\n\n\n\tHello, @r World@re\n\t\t*evil laugh*");

    /* Halt. */
    while(1);
}

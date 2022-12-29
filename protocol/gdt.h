#ifndef protocol_gdt_api
#define protocol_gdt_api

/* GDT api. Home to functionality that makes the users code less messy :) */

#ifndef protocol_types
#include "types.h"
#endif

#ifndef protocol_gdt
#include "gdt/gdt.h"
#endif

/* Load the GDT and jump to 32-bit C code. */
static inline void load_kernel()
{
    /* `c` is referencing the bytecode value. So, the first bytecode of
     * `bin/bootloader.bin`(which is the boot sector program, 0x7C00), is 0x31.*/
    /*unsigned char *c = (unsigned char *)0x7C00;
    print_str(c);
    if(*c == 0x31)
        print_str("YES");
    __asm__("cli;hlt");*/

    setup_gdt_and_gdt_desc();
    load_32bit();
}

#endif
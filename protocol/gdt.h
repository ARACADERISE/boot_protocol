#ifndef protocol_gdt_api
#define protocol_gdt_api

#ifdef __cplusplus
extern "C" {
#endif

/* GDT api. Home to functionality that makes the users code less messy :) */

#ifndef protocol_types
#include "types.h"
#endif

#ifndef protocol_gdt
#include "gdt/gdt.h"
#endif

/*
 *  load_kernel: "front-end" function
 *
 *  Load GDT description into memory, setup 32-bit mode and jump
 *
 *  Input: None
 *  Output: None
 *  On Error: This function does not error directly
 */
static inline void load_kernel()
{
    /* `c` is referencing the bytecode value. So, the first bytecode of
     * `bin/bootloader.bin`(which is the boot sector program, 0x7C00), is 0x31.*/
    /*unsigned char *c = (unsigned char *)0x7C00;
    print_str(c);
    if(*c == 0x31)
        print_str("YES");
    __asm__("cli;hlt");*/

    __setup_gdt_and_gdt_desc();
    __load_32bit();
}

#ifdef __cplusplus
}
#endif

#endif
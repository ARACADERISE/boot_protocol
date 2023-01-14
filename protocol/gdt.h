#ifndef protocol_gdt_api
#define protocol_gdt_api

/* GDT api. Home to functionality that makes the users code less messy :) */

#ifndef protocol_types
#include "types.h"
#endif

#ifndef protocol_gdt
#include "gdt/gdt.h"
#endif

/* GDT status settings for `set_gdt_status`. */
enum gdt_status_settings
{
    NO_GDT              = 0x00,
    BIT16_BIT32_GDT     = 0x01,
    BIT32_ONLY_GDT      = 0x02
};

/* Manually set the GDT status with a setting(`NO_GDT`, `BIT16_BIT32_GDT` or `BIT32_ONLY_GDT`).
 * 
 * Side Note: `set_gdt_status(NO_GDT)` is quite useless, especially if you load a clean GDT/GDT description into memory.
 *            `set_gdt_status(NO_GDT)` can also screw you up if you load a already-working GDT/GDT description into memory -
 *             this will force you to either make the protocol fill out the GDT, or you yourself. 
 */
static void set_gdt_status(enum gdt_status_settings setting)
{
    *gdt_status = setting;
}

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
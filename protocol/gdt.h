#ifndef protocol_gdt_api
#define protocol_gdt_api

#ifndef protocol_types
#include "types.h"
#endif

#ifndef protocol_gdt
#include "gdt/gdt.h"
#endif

static inline void load_kernel()
{
    /*unsigned char *c = (unsigned char *)0x7C00;
    if(*c == 0x31)
        print_str("YES");
    __asm__("cli;hlt");*/

    // Load in any needed memory
    //load_needed_memory(dri);

    setup_gdt_and_gdt_desc();
    load_32bit();
}

#endif
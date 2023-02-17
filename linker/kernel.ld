ENTRY(main)
SECTIONS {
    /* Critical memory addresses for FAMP. */
    g_gdt_address           = 0x0900;
    g_gdt_desc_address      = 0x0930;
    g_gdt_status_address    = 0x0938;

    /* Second stage starts at 0x7E00. */
    . = 0xA000;
    kernel_start = 0x800000;

    .text : AT(0xA000)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(.rodata*);
        *(COMMON*);
    }


    kernel_end = . + kernel_start;
}
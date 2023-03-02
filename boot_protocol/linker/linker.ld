ENTRY(main)
SECTIONS {
    /* Critical memory addresses for FAMP. */
    g_gdt_address           = 0x0900;
    g_gdt_desc_address      = 0x0930;
    g_gdt_status_address    = 0x0938;

    /* Second stage starts at 0x7E00. */
    . = 0x7E00;
    second_stage_start = .;

    .text : AT(0x7E00)
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


    second_stage_end = .;
}
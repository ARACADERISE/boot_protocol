ENTRY(main)
SECTIONS
{
    /* Important memory addresses for FAMP. */
    g_gdt_address = 0x7E00;
    g_gdt_desc_address = 0x7E30;
    g_gdt_status_address = 0x7E38;

    /* Second-stage starts at 0x7F00. */
    . = 0x7F00;
    second_stage_start = .;
    .text : AT(0x7F00)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(COMMON*);
    }
    
    second_stage_end = .;
}
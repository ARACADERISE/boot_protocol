ENTRY(main)
SECTIONS {
    /* Critical memory addresses for FAMP. */
    g_gdt_address           = 0x0900;
    g_gdt_desc_address      = 0x0930;
    g_gdt_status_address    = 0x0938;

    /* Kernel is, initially, read into memory at 0xA000. */
    . = 0xA000;
    
    /* The program `higher_half_kernel.c` changes this, however. */
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
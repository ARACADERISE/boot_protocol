ENTRY(main)
SECTIONS {
    /* Program starts at 0x0500. */
    . = 0x0500;

    .text : AT(0x0500)
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

}
ENTRY(main)
SECTIONS {
    /* Program starts at 0x9000. */
    . = 0x9000;

    .text : AT(0x9000)
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
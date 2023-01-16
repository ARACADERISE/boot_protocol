ENTRY(main)
SECTIONS
{
    . = 0x7E00;
    begin_symbol = .;
    .text : AT(0x7E00)
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
    end_symbol = .;

    /*.sig : AT(0x8F00) {SHORT(0xFF);}*/
}
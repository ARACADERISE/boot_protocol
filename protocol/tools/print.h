#ifndef protocol_print
#define protocol_print

/* Print a character via BIOS teletyping(int 0x10, AH = 0x0E). */
extern void __move_cursor(uint8, uint8);
extern void __print_word_hex();

static void print(uint8 *str)
{
    while(*str)
    {
        newline:
        if(*str == '\n')
        {
            str++;

            /* Manually write newline. */
            __asm__("mov ah, 0x0E\nmov al, 0x0D\nint 0x10\n\nmov ah, 0x0E\nmov al, 0x0A\nint 0x10\n");

            /* Check if the dereferenced value of `str` is another newline. If so, repeat the functionality. */
            if(*str == '\n') goto newline;
        }

        __asm__("mov ah, 0x0e\nmov al, %0\nint 0x10" : : "dN"(*str));
        str++;
    }
}

#endif
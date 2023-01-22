#ifndef protocol_print
#define protocol_print

/* Move cursor depending on x/y values. */
extern void __move_cursor(uint8, uint8);

/* Print 16-bit(word) hex value. */
extern void __print_word_hex();

/* Print 8-bit(byte) hex value. */
extern void __print_byte_hex();

/*
 *  print: "front-end" function
 *
 *  Print a string to emulator screen using BIOS teletyping(AH=0x0E, AL=character, INT=0x10)
 *
 *  Input: uint8 *str(the string to be printed)
 *  Ouput: None
 *  On Error: This function does not error
 */
static void print(uint8 *str)
{
    /* Save the stack. */
    __asm__("pusha");
    
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
        if(*str == '\0') break;

        __asm__("mov ah, 0x0e\nmov al, %0\nint 0x10" : : "dN"(*str));
        str++;
    }

    /* Restore the stack. */
    __asm__("popa");
}

#endif
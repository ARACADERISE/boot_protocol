#ifndef protocol_print
#define protocol_print

#ifdef __cplusplus
extern "C" {
#endif

/* Move cursor depending on x/y values. */
extern void __move_cursor(uint8, uint8);

/* Print 16-bit(word) hex value. */
extern void __print_word_hex();

/* Print 8-bit(byte) hex value. */
extern void __print_byte_hex();

/* Color attributes for `__print_char_color`. */
enum color_attr
{
    black           = 0x00,
    blue            = 0x01,
    green           = 0x02,
    cyan            = 0x03,
    red             = 0x04,
    magenta         = 0x05,
    brown           = 0x06,
    light_grey      = 0x07,
    dark_grey       = 0x08,
    light_blue      = 0x09,
    lime_green      = 0x0A,
    light_cyan      = 0x0B,
    light_red       = 0x0C,
    light_magenta   = 0x0D,
    yellow          = 0x0E,
    white           = 0x0F
};

/* Print character in color via BIOS teletyping(AH=0x09, AL=character, INT=0x10). */
extern void __print_char_color(uint8,uint8,uint8);

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
        if(*str == '\t')
        {
            while(*str == '\t')
            {
                str++;
                __asm__("mov ah, 0x0E\nmov al, ' '\nint 0x10\n");
            }
        }
        if(*str == '\0') break;

        __asm__("mov ah, 0x0e\nmov al, %0\nint 0x10" : : "dN"(*str));
        str++;
    }

    /* Restore the stack. */
    __asm__("popa");
}

#ifdef __cplusplus
}
#endif

#endif
#ifndef protocol_print
#define protocol_print

/* Print a character via BIOS teletyping(int 0x10, AH = 0x0E). */
extern void printStr();
extern void print_char();
extern void print_word_hex();

static void print(uint8 *str)
{
    while(*str)
    {
        __asm__("mov al, %0" : : "dN"(*str));
        print_char();
        str++;
    }
}

#endif
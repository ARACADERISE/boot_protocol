#ifndef protocol_print
#define protocol_print

/* Print a character via BIOS teletyping(int 0x10, AH = 0x0E). */
extern void printStr();
extern void print_char();
extern void move_cursor(uint8 x_pos, uint8 y_pos);
extern void print_word_hex();

typedef struct cursor_pos
{
    uint16      pos_x;
    uint16      pos_y;
} _cursor_pos;

static _cursor_pos cursor_pos = {.pos_x = 0, .pos_y = 0};

static void print(uint8 *str)
{
    while(*str)
    {
        if(*str == '\n')
        {
            cursor_pos.pos_y++;
            cursor_pos.pos_x = 0;
            str++;
            move_cursor((uint8)cursor_pos.pos_x, (uint8)cursor_pos.pos_y);
            continue;
        }
        cursor_pos.pos_x++;
        __asm__("mov al, %0" : : "dN"(*str));
        print_char();
        str++;
    }
    move_cursor((uint8)cursor_pos.pos_x, (uint8)cursor_pos.pos_y);
}

#endif
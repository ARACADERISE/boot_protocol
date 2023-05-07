#ifndef protocol_printing
#define protocol_printing

/* Declare this in a header file somewhere. */
static uint16 *vid_mem = (uint16 *)0xB8000;
static uint8 *vid_mem2 = (uint8 *)0xB8000;

/* Color attributes for `print` */
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
    white           = 0x0F,
    wwhite          = 0xF0,
    rred            = 0xF4,
    cyan_white      = 0x3F,
    cyan_red        = 0x34,
};

const uint8 *color_names[] = {
    "black", "@bblue@w", "@ggreen@w", "@ccyan@w",
    "@rred@w", "@mmagenta@w", "@brbrown@w", "@lglight grey@w",
    "@dgdark grey@w", "@lblight blue@w", "@lglime green@w",
    "@lclight cyan@w", "@lrlight red@w", "@lmlight magenta@w",
    "@yyellow@w", "white"
};

static uint8 color;

/* The default color will be whatever value `color` has originally when the screen is initialized. */
static uint8 default_color;

uint8 *memcpy(uint8 *dest, const uint8 *src, lsize count)
{
    int8 *dstPtr = (int8 *)dest;
    const int8* srcPtr = (const int8 *)src;

    for(; count != 0; count--) *dstPtr++ = *srcPtr++;

    return dest;
}

uint16 *memsetww(uint16 *dest, uint16 val, lsize count)
{
    uint16 *dstPtr = (uint16 *)dest;

    for(; count != 0; count--) *dstPtr++ = val;

    return dest;
}

static uint8 offscreen_chars[cols * 2] = {0};
static uint16 index = 0;

void scroll()
{
    if(c_info.pos_y >= rows - 1)
    {
        uint16 offset = c_info.pos_y - rows + 1;
        memcpy(vid_mem2, vid_mem2 + (offset * cols * 2), (rows - offset) * cols * 2);
        memsetww(vid_mem + ((rows - offset) * cols), get_text_value(' ', (color >> 4) & 0xFF, (color >> 0) & 0xFF), cols);
        c_info.pos_y = rows - 1;
        c_info.pos_x = 0;
    }
}

void put_char(uint8 character)
{
    static uint8 t = 0;
    switch(character)
    {
        case '\n':
        {
            c_info.pos_y++;
            last_cursor_x[last_cursor_x_index] = c_info.pos_x;
            last_cursor_x_index++;
            c_info.pos_x = 0;

            /* Scroll if the y position is >= 25. */
            if(c_info.pos_y >= rows - 1)
                scroll();

            goto end;
        }
        case '\t':
        {
            c_info.pos_x+=4;
            goto end;
        }
        case 0x0E:
        {
            if(c_info.pos_x >= 1)
            {
                c_info.pos_x--;
                vid_mem[c_info.pos_x + (c_info.pos_y * 80)] = (color << 8) | (0xFF & ' ');
                goto end;
            }

            if(c_info.pos_y > 0)
            {
                /* TODO: Make it to where the characters that are "off" the screen
                 * get saves, and when we backspace enough they reappear and the
                 * characters already on the screen get shifted down.
                 * Some of this will have to be done in `scroll`(saving the character going "off" the screen).
                 */
                c_info.pos_y--;
                last_cursor_x_index--;
                c_info.pos_x = last_cursor_x[last_cursor_x_index];
                goto end;
            }
            return;
        }
        /* Do nothing if the above does not run. */
        default: break;
    }
    vid_mem[c_info.pos_x + (c_info.pos_y * 80)] =  (color << 8) | (0xFF & character);
    c_info.pos_x++;

    end:
    update_cursor_pos();
}

void print(const uint8 *str)
{
    lsize i = 0;

    while(str[i] != '\0')
    {
        /* Check the current character to see if it is any of the given
         * "special" characters.
         */
        redo:
        switch(str[i])
        {
            case '\n':
            {
                c_info.pos_y++;
                last_cursor_x[last_cursor_x_index] = c_info.pos_x;
                last_cursor_x_index++;
                c_info.pos_x = 0;
                i++;
                goto redo;
            }
            case '\t':c_info.pos_x += 4;i++;goto redo;
            case '@':
            {
                i++;
                
                switch(str[i])
                {
                    case 'l':
                    {
                        if(str[i+1] == 'g') { i+=2; color = light_grey; goto redo; }
                        if(str[i+1] == 'b') { i+=2; color = light_blue; goto redo; }
                        if(str[i+1] == 'G') { i+=2; color = lime_green; goto redo; }
                        if(str[i+1] == 'c') { i+=2; color = light_cyan; goto redo; }
                        if(str[i+1] == 'r') { i+=2; color = light_red; goto redo; }
                        if(str[i+1] == 'm') { i+=2; color = light_magenta; goto redo; }

                        /* If it is not of the above just skip and make sure the color is the default color. */
                        i++;
                        color = default_color;
                        goto redo;
                    }
                    case 'd':
                    {
                        if(str[i+1] == 'g') { i+=2; color = dark_grey; goto redo; }

                        i++;
                        color = default_color;
                        goto redo;
                    }
                    case 'y':
                    {
                        color = yellow;
                        i++;
                        goto redo;
                    }
                    case ':':
                    {
                        c_info.pos_x = 0;
                        c_info.pos_y = 0;
                        i++;
                        goto redo;
                    }
                    case 'w':
                    {
                        color = white;
                        i++;
                        goto redo;
                    }
                    case 'b':
                    {
                        if(str[i+1] == 'r') { i++; color = brown; }
                        else color = blue;
                        i++;
                        goto redo;
                    }
                    case 'B':
                    {
                        color = black;
                        i++;
                        goto redo;
                    }
                    case 'r':
                    {
                        if(str[i+1] == 'e') { i++;color = default_color; }
                        else color = red;
                        i++;
                        goto redo;
                    }
                    case 'c':
                    {
                        color = cyan;
                        i++;
                        goto redo;
                    }
                    case 'm':
                    {
                        color = magenta;
                        i++;
                        goto redo;
                    }
                    case 'g':
                    {
                        color = green;
                        i++;
                        goto redo;
                    }
                    default: i++;goto redo;
                }
            }
            case '\0':goto end;
            default: break;
        }

        /* Write character to according index in the video memory buffer. */
        vid_mem[c_info.pos_x + (c_info.pos_y * 80)] = (color << 8) | (0xFF & str[i]);

        /* Increment index/cursor x position. */
        i++;
        c_info.pos_x++;

        /* Check if the x position surpasses or is directly equal to
         * the width of the screen. If it is, add newline.
         */
        if(c_info.pos_x >= 80)
        {
            c_info.pos_x = 0;
            c_info.pos_y++;
        }

        update_cursor_pos();
    }

    end:
    return;
}

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

/* Address used for arrays. */
static uint8 *addr = (uint8 *)0x1000;
uint8 *init_new_char_array(uint8 *arr, lsize elements, uint8 *string)
{
    uint8 *arrPtr;

    if(arr == NULL) arrPtr = (uint8 *)addr;
    else arrPtr = arr;

    memsetb(arr, 0, elements);

    /* -1 for `\0` at end. */
    if(string != NULL)
        for(uint16 i = 0; i < elements - 1; i++)
            arrPtr[i] = string[i];
    
    arrPtr[elements - 1] = '\0';
    addr += elements;
    
    return arrPtr;
}

bool strcmp(uint8 *string1, uint8 *string2)
{
    uint16 length1 = 0;
    uint16 length2 = 0;

    while(string1[length1] != '\0') length1++;
    while(string2[length2] != '\0') length2++;

    if(length1 > length2 || length1 < length2) return false;

    length1 = length2 = 0;
    while(string1[length1] != '\0')
    {
        if(string1[length1] != string2[length1]) return false;
        length1++;
    }

    return true;
}

void clear()
{
    uint16 i = 0;
    while(i < rows*cols)
    {
        memset((uint16 *)0xB8000 + (i * cols), get_text_value(' ', 0x00, 0x0F), cols);
        i++;
    }
    zero_out_cursor_pos();
}

uint8 get_raw_color_value()
{
    redo_new_color:
    input(NULL, false);

    if(strcmp(input_buffer, init_new_char_array(NULL, 6, "black")) == true)            return black;
    if(strcmp(input_buffer, init_new_char_array(NULL, 5, "blue")) == true)             return blue;
    if(strcmp(input_buffer, init_new_char_array(NULL, 6, "green")) == true)            return green;
    if(strcmp(input_buffer, init_new_char_array(NULL, 5, "cyan")) == true)             return cyan;
    if(strcmp(input_buffer, init_new_char_array(NULL, 4, "red")) == true)              return red;
    if(strcmp(input_buffer, init_new_char_array(NULL, 8, "magenta")) == true)          return magenta;
    if(strcmp(input_buffer, init_new_char_array(NULL, 6, "brown")) == true)            return brown;
    if(strcmp(input_buffer, init_new_char_array(NULL, 11, "light grey")) == true)      return light_grey;
    if(strcmp(input_buffer, init_new_char_array(NULL, 10, "dark grey")) == true)       return dark_grey;
    if(strcmp(input_buffer, init_new_char_array(NULL, 11, "light blue")) == true)      return light_blue;
    if(strcmp(input_buffer, init_new_char_array(NULL, 11, "lime green")) == true)      return lime_green;
    if(strcmp(input_buffer, init_new_char_array(NULL, 11, "light cyan")) == true)      return light_cyan;
    if(strcmp(input_buffer, init_new_char_array(NULL, 10, "light red")) == true)       return light_red;
    if(strcmp(input_buffer, init_new_char_array(NULL, 14, "light magenta")) == true)   return light_magenta;
    if(strcmp(input_buffer, init_new_char_array(NULL, 7, "yellow")) == true)           return yellow;
    if(strcmp(input_buffer, init_new_char_array(NULL, 6, "white")) == true)            return white;

    clear();
    print("\nOops.. I don't thinks `@y");
    print(input_buffer);
    print("` @wis a valid color. Try again :)\n");

    /* Reprint for convenience. */
    print("For reference, here is a list of all the available colors!(P.S you have to type the numbers out in lowercase)\n\t1. Black\n\t2. @bBlue@w\n\t3. @gGreen@w\n\t4. @cCyan@w\n\t5. @rRed@w\n\t6. @mMagenta@w\n\t7. @bBrown@w\n\t8. @lgLight Grey@w\n\t9. @dgDark Grey@w\n\t10. @lbLight Blue@w\n\t11. @lgLime Green@w\n\t12. @lcLight Cyan@w\n\t13. @lrLight Red@w\n\t14. @lmLight Magenta@w\n\t15. @yYellow@w\n\t16. White@w\n> ");
    
    goto redo_new_color;
}

void clear_screen(uint8 bgcolor, uint8 fgcolor)
{
    if(get_text_attribute(bgcolor, fgcolor) == 0)
    {
        begin_reset_colors:

        clear();
        
        /* Go ahead and setup a temporary color/default color so users can see the messages being printed. */
        color = get_text_attribute(0x00, 0x0F);
        default_color = color;

        print("You might have made a @rmistake!@w You set your foreground and your background to \nthe same color. Your foreground is ");
        print(color_names[fgcolor]);
        print(" and you background color is ");
        print(color_names[bgcolor]);
        
        redo:
        input("\nWhich would you like to change?\n\t1. Foreground\n\t2. Background\n\t3. It wasn't a mistake\n> ", true);

        clear();
        
        /* Print this before we continue onward. */
        print("For reference, here is a list of all the available colors!(P.S you have to type the numbers out in lowercase)\n\t1. Black\n\t2. @bBlue@w\n\t3. @gGreen@w\n\t4. @cCyan@w\n\t5. @rRed@w\n\t6. @mMagenta@w\n\t7. @brBrown@w\n\t8. @lgLight Grey@w\n\t9. @dgDark Grey@w\n\t10. @lbLight Blue@w\n\t11. @lGLime Green@w\n\t12. @lcLight Cyan@w\n\t13. @lrLight Red@w\n\t14. @lmLight Magenta@w\n\t15. @yYellow@w\n\t16. White@w\n> ");

        if(user_input == '1') goto fg;
        if(user_input == '2') goto bg;
        if(user_input == '3') goto endo;

        fg:

        /* Get the new foreground color. */
        fgcolor = get_raw_color_value();

        /* Update the color. */
        color = get_text_attribute(bgcolor, fgcolor);

        goto check_colors;

        bg:

        /* Get the new background color. */
        bgcolor = get_raw_color_value();

        /* Update the color. */
        color = get_text_attribute(bgcolor, fgcolor);

        check_colors:
        /* Make sure that the background doesn't match the foreground still. We only care about the least significant 4-bits here. */
        if(fgcolor == bgcolor || (fgcolor & 0x0F) == bgcolor || (fgcolor & 0x0F) == (bgcolor & 0x0F))
        {
            color = default_color;

            /* Set `user_input` to zero so we are ready to get new input. Also clear the screen of all the junk. */
            user_input = 0;
            clear();

            /* Ask the user if they want FAMP to go ahead and just initialize the background color to be white. */
            input("Do you want FAMP to go ahead and initialize the background to be white?\n[y/n] > ", true);
            
            put_char(user_input);
            put_char('\n');

            if(user_input == 'y') bgcolor = white;
            else goto begin_reset_colors;
        }
    } else {
        endo:
        color = get_text_attribute(bgcolor, fgcolor);
    }

    uint16 i = 0;
    while(i < rows*cols)
    {
        memset((uint16 *)0xB8000 + (i * cols), get_text_value(' ', (color >> 4) & 0x0F, color & 0x0F), cols);
        i++;
    }
    zero_out_cursor_pos();

    default_color = color;
}

#endif
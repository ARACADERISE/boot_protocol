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
        memsetww(vid_mem + ((rows - offset) * cols), get_text_value(' ', cyan, white), cols);
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
                vid_mem[c_info.pos_x + (c_info.pos_y * 80)] = (cyan_white << 8) | (0xFF & ' ');
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

    vid_mem[c_info.pos_x + (c_info.pos_y * 80)] =  (cyan_white << 8) | (0xFF & character);
    c_info.pos_x++;

    end:
    update_cursor_pos();
}

void print(const uint8 *str)
{
    lsize i = 0;
    uint8 color = cyan_white;

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
                    case 'b':
                    {
                        if(str[i+1] == 'r')
                        {
                            i++;
                            color = brown;
                        }
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
                        if(str[i+1] == 'e')
                        {
                            i++;
                            color = cyan_white;
                        }
                        else color = cyan_red;
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

void clear_screen(uint8 color)
{
    uint16 i = 0;
    while(i < rows*cols)
    {
        memset((uint16 *)0xB8000 + (i * cols), get_text_value(' ', cyan, white), cols);
        i++;
    }
    //vid_mem = (uint16 *) 0xB8000;
}

#endif
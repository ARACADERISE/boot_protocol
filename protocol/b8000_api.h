#ifndef protocol_b8000_api
#define protocol_b8000_api

#ifdef __cplusplus
extern "C" {
#endif

#define cols        25
#define rows        80

/* Declare this in a header file somewhere. */
static uint16 *vid_mem = (uint16 *)0xB8000;

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
    rred            = 0xF4
};

typedef struct cursor_info
{
    uint32      pos_x;
    uint32      pos_y;
} _cursor_info;

static _cursor_info c_info = {.pos_x = 0, .pos_y = 0};

void print(const uint8 *str)
{
    lsize i = 0;
    uint8 color = white;

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
                            color = white;
                        }
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
    }

    end:
    return;
}

void clear_screen(uint8 color)
{
    uint16 i = 0;
    while(i < (rows * cols))
    {
        vid_mem[i] = (color << 8) | (0xFF & ' ');
        i++;
    }
    //vid_mem = (uint16 *) 0xB8000;
}

#ifdef __cplusplus
}
#endif

#endif
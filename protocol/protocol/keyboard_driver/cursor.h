#ifndef protocol_cursor
#define protocol_cursor

/* Command ports for cursor. */
#define reg_screen_ctr  0x3D4
#define reg_screen_data 0x3D5

/* X/Y information over where the cursor is at on the screen. */
typedef struct cursor_info
{
    uint32      pos_x;
    uint32      pos_y;
} _cursor_info;

/* Static reference to cursors x/y position. */
static _cursor_info c_info = {.pos_x = 0, .pos_y = 0};

/* Store the last X value if the cursor enters a newline. */
static uint32 last_cursor_x[cols * 4] = {0};
static uint8  last_cursor_x_index = 0;

/* Update the cursor based on c_info.pos_x and c_info.pos_y. */
void update_cursor_pos()
{
    uint16 pos = (c_info.pos_y * 80) + c_info.pos_x;
    __outp(reg_screen_ctr, 14);
    __outp(reg_screen_data, pos >> 8);
    __outp(reg_screen_ctr, 15);
    __outp(reg_screen_data, pos & 0xFF);
}

/* Initialize the cursor. Set c_info.pos_x/c_info.pos_y to zero and update. */
void init_cursor_pos()
{
    /* Set `pos_x`/`pos_y` to zero to initialize. */
    c_info.pos_x = c_info.pos_y = 0;

    update_cursor_pos();
}

/* Just a naming convention. Quite literally just `init_cursor_pos`, but I like my code to not be confusing :) */
void zero_out_cursor_pos()
{
    init_cursor_pos();
}

#endif
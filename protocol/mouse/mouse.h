#ifndef protocol_mouse
#define protocol_mouse

#ifdef __cplusplus
extern "C" {
#endif

/* mouseX/mouseY memory address. */
#define MOUSE_X_ADDR(uint16 * ) 0xC00A
#define MOUSE_Y_ADDR(uint16 * ) 0xC00C
#define MOUSE_STATUS(uint8 * ) 0xC00E

/* States of cursor. */
#define cursor_right 0x08
#define cursor_up 0x08
#define cursor_left 0x18
#define cursor_down 0x18
#define cursor_vertical_right_down 0x28
#define curosr_vertical_left_down 0x38

/* Struct storing all information about the cursor. */
typedef struct cursor_data {
    uint16 mouseX;
    uint16 mouseY;

    uint8 mouse_status;
}
_cursor_data;

/* Static reference to mouse data. */
static _cursor_data cursor_data;

/* Poll the mouse. */
extern void poll_mouse();

/* Enable the mouse. */
extern void enable_mouse();

/* Disable the mouse. */
extern void disable_mouse();

/* Go until a specific x or y value is met. */
extern void get_mouse_until(uint16, uint16);

/* Go until both x and y values are met. */
extern void get_mouse_until_both(uint16, uint16);

/* Poll the mouse and save data. */
static void save_mouse() {
    cursor_data.mouseX = (uint16) * MOUSE_X_ADDR;
    cursor_data.mouseY = (uint16) * MOUSE_Y_ADDR;
    cursor_data.mouse_status = (uint8) * MOUSE_STATUS;
}

/* TODO: Get this shit working, ugh. */

/* Continue to poll mouse until X/Y value is met. */
static void poll_mouse_until(uint16 x, uint16 y) {
    get_mouse_until(x, y);
    save_mouse();
    __asm__("mov ax, %0":: "dN"((uint16) cursor_data.mouseX));
    print_word_hex();
}

/* Init the mouse data by doing a poll. */
static void init_mouse_data() {
    cursor_data.mouseX = 0;
    cursor_data.mouseY = 0;
    cursor_data.mouse_status = 0x8;
}

#ifdef __cplusplus
}
#endif

#endif
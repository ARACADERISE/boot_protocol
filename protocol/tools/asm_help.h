#ifndef protocol_asm_help
#define protocol_asm_help

#ifdef __cplusplus
extern "C" {
#endif

/* All registers that this API allows users to work with.
 * Registers like `ebp` and `esp` etc are all kinda important.
 */
enum registers
{
    ax, ah, al,
    bx, bh, bl,
    cx, ch, cl,
    dx, dh, dl,
    es, ds, sp,
    fs, gs
};

uint16 grab_val_from_ah()
{
    uint8 val = 0;

    __asm__("mov %0, ah" : "=r"(val));
    return val;
}

/* Grab the value from a given parent register(`reg`). */
uint16 grab_val_from_parent(enum registers reg)
{
    uint16 val = 0;

    switch(reg)
    {
        case ax:
        case ah:
        case al: {
            __asm__("mov %0, ax" : "=r"(val));
            break;
        }
        case bx:
        case bh:
        case bl: {
            __asm__("mov %0, bx" : "=r"(val));
            break;
        }
        case cx:
        case ch:
        case cl: {
            __asm__("mov %0, cx" : "=r"(val));
            break;
        }
        case dx:
        case dh:
        case dl: {
            __asm__("mov %0, dx" : "=r"(val));
            break;
        }
        default: break;
    }

    return val;
}

/* Grab the value from a given child register(`reg`). */
uint8 grab_val_from_child(enum registers reg)
{
    uint8 val = 0;

    switch(reg)
    {
        case ah: __asm__("mov %0, ah" : "=r"(val));break;
        case al: __asm__("mov %0, al" : "=r"(val));break;
        case bh: __asm__("mov %0, bh" : "=r"(val));break;
        case bl: __asm__("mov %0, bl" : "=r"(val));break;
        case ch: __asm__("mov %0, ch" : "=r"(val));break;
        case cl: __asm__("mov %0, cl" : "=r"(val));break;
        case dh: __asm__("mov %0, dh" : "=r"(val));break;
        case dl: __asm__("mov %0, dl" : "=r"(val));break;
        default: break;
    }

    return val;
}

#ifdef __cplusplus
}
#endif

#endif
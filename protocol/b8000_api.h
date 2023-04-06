#ifndef protocol_b8000_api
#define protocol_b8000_api

#ifdef __cplusplus
extern "C" {
#endif

#ifndef protocol_util
#include "../util.h"
#endif

#ifndef protocol_types
#include "../types.h"
#endif

#define rows        25
#define cols        80

#ifndef protocol_cursor
#include "keyboard_driver/cursor.h"
#endif

#ifndef protocol_printing
#include "keyboard_driver/printing.h"
#endif

#ifndef protocol_keyboard
#include "keyboard_driver/keyboard.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
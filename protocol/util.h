#ifndef protocol_util
#define protocol_util

#ifdef __cplusplus
extern "C" {
#endif

#ifndef protocol_types
#include "types.h"
#endif

//**********************************
// *      Common Functionality     *
// *********************************
#define halt            __asm__("cli;hlt");

uint8 __char_set_bit(uint8 c, uint8 pos)
{
    if(pos >= 8)
        pos = 7;
    
    uint8 mask = 1 << pos;
    return (c & ~mask) | (1 << pos);
}
uint8 __char_unset_bit(uint8 c, uint8 pos)
{
    if(pos >= 8)
        pos = 7;
    
    uint8 mask = 1 << pos;
    return (c & ~mask) | (0 << pos);
}

bool __char_check_bit_is_set(uint8 c, uint8 pos)
{
    if(pos >= 8)
        pos = 7;
    
    return (c & (1 << pos));
}

bool __char_check_bit_not_set(uint8 c, uint8 pos)
{
    if(pos >= 8)
        pos = 7;
    
    return (c & (1 << pos)) == false ? true : false;
}

uint8 grab_byte_s(uint16 bytes, uint8 pos)
{
    /* Be safe. */
    if(pos-1 < 0)
        pos = 1;
    if(pos > 2)
        pos = 2;

    return (bytes >> (8 * (pos - 1)) & 0xFF);
}

uint8 grab_byte_i(uint32 bytes, uint8 pos)
{
    /* Be safe. */
    if(pos-1 < 0)
        pos = 1;
    if(pos > 4)
        pos = 4;
    
    return (bytes >> (8 * (pos - 1)) & 0xFF);
}

/* Get length of string. */
size strlen(uint8 *string)
{
    size string_size = 0;

    while(*(string++) != '\0' && *(string++)) string_size++;
    return string_size;
}

/* The variable passed to `array` has to be declared as:
 * uint8 array[size]. */
void memsetb(uint8 *array, uint8 value, size count)
{
    size i = 0;

    while(count > 0)
    {
        array[i] = value;
        i++;
        count--;
    }
}

/* The variable passed to `array` has to be declared as:
 * uint16 array[size]
 * */
void memsetw(uint16 *array, uint16 value, size count)
{
    size i = 0;

    /* If the count is greater than the array length, do nothing. */
    if(sizeof(array)/sizeof(array[0]) < count)
        return;
    
    while(count > 0)
    {
        array[++i] = value;
        count--; 
    }
}

/* The variable passed to `array` has to be declared as:
 * uint32 array[size]
 * */
void memsetd(uint32 *array, uint32 value, size count)
{
    size i = 0;

    /* If the count is greater than the array length, do nothing. */
    if(sizeof(array)/sizeof(array[0]) < count)
        return;

    while(count > 0)
    {
        array[++i] = value;
        count--; 
    }
}

// **********END OF COMMON FUNCTIONALITY**********

#ifdef __cplusplus
}
#endif

#endif
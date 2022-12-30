#ifndef protocol_util
#define protocol_util

#ifndef protocol_types
#include "types.h"
#endif

//**********************************
// *      Common Functionality     *
// *********************************
#define halt            __asm__("cli;hlt");

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

    while(*(string++) != '\0') string_size++;
    return string_size;
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

#endif
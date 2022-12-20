#ifndef protocol_util
#define protocol_util

#ifndef protocol_types
#include "types.h"
#endif

//**********************************
// *      Common Functionality     *
// *********************************
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
void memsetw(uint16 *array, uint16 value)
{
    for(size i = 0; i < sizeof(array)/sizeof(array[0]); i++)
        array[i] = value;
}
void memsetd(uint32 *array, uint32 value)
{
    for(size i = 0; i < sizeof(array)/sizeof(array[0]); i++)
        array[i] = value;
}

// **********END OF COMMON FUNCTIONALITY**********

#endif
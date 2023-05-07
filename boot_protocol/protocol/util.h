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

/* Same as `memsetw`, just a different name. */
uint16 *memset(uint16 *dest, uint16 val, lsize count){
	uint16* dstPtr = (uint16 *)dest;

    for (; count != 0; count--) *dstPtr++ = val;
	
	return dest;
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

/*
 *  __inp: back-end function
 *
 *  Read-in a value from a port
 *
 *  Input: uint16 port
 *  Output: uint8 rv(return-value from the port)
 *  On Error: This function does not error
 */
uint8 __inp(uint16 port) {
    uint8 rv;
    
    __asm__ __volatile__("in %0, %1": "=a"(rv): "dN"(port));
    return rv;
}
/*
 *  __outp: back-end function
 *
 *  Write a value to a port
 *
 *  Input: uint16 port, uint8 data
 *  Output: None
 *  On Error: This function does not error
 */
void __outp(uint16 port, uint8 data) {
    __asm__ __volatile__("outb %0, %1":: "dN"(port), "a"(data));
}

/* Get the byte value for the foreground color/background color of the text. */
uint16 get_text_attribute(uint8 background, uint8 foreground)
{
    /* If the highest 4-bits are set then we will just go ahead and use that value. */
    if(((background >> 4) & 0x0F) > 0) background = (background >> 4);
    if(((foreground >> 4) & 0x0F) > 0) foreground = (foreground >> 4);

    /* Check and see if the background is the same as the foreground. */
    if(foreground == background) return 0;

    /* `background << 4` allows for 4 more bits. `foreground & 0x0F` fills those 4 bits to make up the entire byte. */
    return (background << 4) | (foreground & 0x0F);
}

/* Get the overall value of text. First byte will represent the character, second byte will represent fg/bg. */
uint16 get_text_value(uint8 character, uint8 foreground, uint8 background){
    uint16 t_attr = get_text_attribute(foreground, background);
    if(t_attr == 0) return 0;

    return character | (t_attr << 8);
}

// **********END OF COMMON FUNCTIONALITY**********

#ifdef __cplusplus
}
#endif

#endif
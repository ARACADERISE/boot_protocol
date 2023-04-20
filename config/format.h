#ifndef format
#define format

#ifndef _STDIO_H_
#include <stdio.h>
#endif
#ifndef _STDLIB_H_
#include <stdlib.h>
#endif
#ifndef _STRING_H_
#include <string.h>
#endif
#ifndef _STDBOOL_H_
#include <stdbool.h>
#endif
#ifndef	_UNISTD_H
#include <unistd.h>
#endif

#define bytes_per_sector (uint16)512
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

/* Ids. */
#define boot_id_1 0x0A
#define boot_id_2 0x0B
#define user_id 0x1A
#define kernel_id 0x2A

/* "Magic numbers". */
static const uint8 memory_stamp_magic_number_id[] = {0x2B, 0x84, 0x83, 0x82, 0x81};

/* Access bytes. */
#define access_level_one 0b00001011
#define access_level_two 0b00001001
#define access_level_three 0b00001000

typedef struct memory_stamp {
    /* "Magic Number" referencing the start to the memory stamp. */
    uint8 memory_stamp_magic_number[5];

    /* "user", "kernel", "boot" */
    uint8 memory_id;

    /* Binary size in sectors. */
    uint8 sectors;

    /* Estimate size(in bytes). */
    size_t estimate_size_in_bytes;

    /* Has the memory been released/overwritten? */
    bool is_overwritten;

    /* Information over access.
     *
     * bits 0-1: 
     *  Locked(1), Unlocked(0). 
     *  If this is set, the protocol is not willing to give up the memory.
     * bits 1-2:
     *  Primary/Secondary
     *  If set, the memory is primary to the OS working
     *  If not set, the memory is just additional data/code that is not
     *  critical for the OS to run
     * bits 2-3:
     *  Preferred type.
     *  If set, the memory is perferrably just data
     *  If not set, the memory is perferrably just code
     * bits 3-4:
     *  Flexibility.
     *  If bits 2-3 is not set, the flexibility bit can be set.
     *  The flexibility bit tells the protocol the memory is both code and data.
     *  If bits 2-3 is not set, and the flexibility bit is not set, the protocol defaults to expecting mainly code
     *  If bits 2-3 is set, and the flexibility bit is set, the protocol will expect data
     * bits 4-5:
     *  Reserved for protocol.
     *  If this bit is set, something went wrong with the memory
     */
    uint8 access;

    /* Binary name.
     * A static allocated size of 30-bytes should be good enough, right? */
    //uint8       bin_name[30];

    /* Beginning address. */
    uint16 beginning_address;

    /* Ending address. */
    uint16 ending_address;

    /* Name. */
    uint8  name[20];
} _memory_stamp;

static const size_t memory_stamp_size = sizeof(_memory_stamp);

#endif

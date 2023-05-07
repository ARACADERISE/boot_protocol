#ifndef protocol_memory_stamp
#define protocol_memory_stamp

#ifdef __cplusplus
extern "C" {
#endif

/* "Magic numbers". */
#define memory_stamp_magic_number_id        (uint8[]){0x2B, 0x84, 0x83, 0x82, 0x81}
#define memory_stamp_magic_id_not_found     (uint8[]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

/* Types of memory stamps. */
enum memory_stamp_type
{
    kernel,
    second_stage,
    user
};

/* Macros for above enum. */
#define kernel_memory_id                    0x2A
#define second_stage_memory_id              0x0B
#define user_memory_id                      0x1A

/* Access bytes. */
#define access_level_one                    0b00001011
#define access_level_two                    0b00001001
#define access_level_three                  0b00001000

typedef struct memory_stamp {
    /* Memory stamps "magic number" referencing the start to the memory stamp. */
    uint8 memory_stamp_magic_number[5];

    /* "user", "kernel", "boot" */
    uint8 memory_id;

    /* Binary size in sectors. */
    uint8 sectors;

    /* Estimate size(in bytes). */
    lsize estimate_size_in_bytes;

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

    /* Beginning address. */
    uint16 beginning_address;

    /* Ending address. */
    uint16 ending_address;

    /* Name. */
    uint8 name[10];
} _memory_stamp;

/* TODO: Do we need this? This should probably get removed. */
const uint16 _memory_stamp_size = sizeof(_memory_stamp);

/*
 *  __obtain_memory_stamp: back-end function
 *
 *  Grab the memory stamp from memory
 *
 *  Input: enum memory_stamp_type mem_stamp_type(what memory id are we looking for?)
 *  Output: _memory_stamp ptr
 *  On Error: Return NULL _memory_stamp ptr
 */
static _memory_stamp *__obtain_memory_stamp(uint8 *addr, enum memory_stamp_type mem_stamp_type) {
    /* Start from speculated end of second-stage according to linker. */
    //uint8 *addr = (uint8 *) &second_stage_end;

    /* How many bytes of the memory stamps "magic number" have we found? */
    uint8 total = 0;

    /* Store the memory stamp data. */
    _memory_stamp *mem_stamp = NULL;

    /* Loop until we find `0x2B 0x84 0x83`, which are the first three
     * bytes of the memory stamps "magic number".
     */
    redo:
    while(*addr != 0x2B)
        addr++;
    
    /* The byte right after `0x2B` should be `0x84` if its the "magic number" for the memory stamp. */
    if(!(*(addr+1) == 0x84)) { addr++; goto redo; }

    /* And, after `0x84` it should be `0x83`. With this, we know that we have the correct place in memory. */
    if(!(*(addr+2)) == 0x83) { addr++; goto redo; }

    /* If we reach here, `0x2B 0x84 0x83` was found. 
     * Assign `mem_stamp` to the according address(`addr`). 
     */
    mem_stamp = (_memory_stamp *) addr;

    /* First, make sure the 5 bytes representing the start to the memory stamp
     * are correct.
     */
    for(uint8 i = 0; i < 5; i++)
        if(mem_stamp->memory_stamp_magic_number[i] != memory_stamp_magic_number_id[i]) goto redo;

    /* Make sure the memory id of the memory stamp that was found is the same as the memory stamp we are expecting to obtain. 
     * If it does not match, we return NULL.
     */
    switch(mem_stamp_type)
    {
        case second_stage: {
            if(!(mem_stamp->memory_id == second_stage_memory_id))
                return NULL;
            break;
        }
        case kernel: {
            if(!(mem_stamp->memory_id == kernel_memory_id))
                return NULL;
            break;
        }
        case user: {
            if(!(mem_stamp->memory_id == user_memory_id))
                return NULL;
            break;
        }
        default: break;
    }

    return mem_stamp;
}

#ifdef __cplusplus
}
#endif

#endif

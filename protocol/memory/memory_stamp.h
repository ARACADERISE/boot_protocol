#ifndef protocol_memory_stamp
#define protocol_memory_stamp

/* "Magic numbers". */
#define memory_stamp_magic_number_id(uint8[]) {0x2B, 0xFF, 0x2F, 0xDF, 0x88}
#define memory_stamp_magic_id_not_found(uint8[]) {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

typedef struct memory_stamp {
    /* Memory stamps "magic number" referencing the start to the memory stamp. */
    uint8 memory_stamp_magic_number[5];

    /* "user", "kernel", "boot" */
    uint8 memory_id;

    /* Binary size in sectors. */
    uint16 sectors;

    /* Estimate size(in bytes). */
    size estimate_size_in_bytes;

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
}
_memory_stamp;

const uint16 _memory_stamp_size = sizeof(_memory_stamp);

/* Find where the memory stamp resides. */
static _memory_stamp *find_memory_stamp_offset_address() {
    /* Start at given end of the second stage. 
     * `addr` will be used to keep track of the address(it can't go over `0x9000`)
     * `ptr_addr` will be used to obtain bytes from the binary.
     * `bytes_iterated` is the total amount of bytes that have been checked so far
     * `byte` is the dereferenced value from `ptr_addr`
     * `total` is a accumulator that accumulates up to 5. The "magic id" that signified the start of the memory
     *  stamp has 5 bytes to it 
     */
    uint8 addr = (uint8) second_stage_end;
    uint8 *ptr_addr = (uint8 *) & second_stage_end;
    uint16 bytes_iterated = 0;
    uint8 byte = 0;
    uint8 total = 0;

    /* Create instance of `_memory_stamp`. Set to NULL. */
    _memory_stamp *mem_stamp = NULL;

    redo:
    /* If `addr` surpasses, or is strictly at, `0x9000`, fill out the memory stamp with default values. */
    if (addr >= 0x9000)
        goto default_values;

    /* Dereference pointer, get value. */
    byte = *ptr_addr;

    /* Check byte to current index(`total`) in `memory_stamp_magic_number_id` which stores a series
     * of bytes that reference the start of the memory stamp.
     */
    if (byte == memory_stamp_magic_number_id[total]) {
        /* Increment total bytes we've encountered. */
        total++;

        /* Increment `ptr_addr` pointer. Increment `addr` accumulator. */
        ptr_addr++;
        addr++;

        /* Check if `total` is equal to 5. If yes, assign values accordingly. */
        if (total == 5) goto assign_accordingly;

        /* Redo if `total` is not five.
         * The reason we do this is so the if statement beneath doesn't run.
         * No point wasting computation for useless checks.
         */
        goto redo;
    }

    /* If the byte was not what was expected, reset the `total`. */
    if (!(byte == memory_stamp_magic_number_id[total - 1]) && total > 0) total = 0;

    /* Increment address. Increment `addr` accumulator. Increment the total amount of bytes iterated.  */
    ptr_addr++;
    addr++;
    bytes_iterated++;

    /* Redo. */
    goto redo;

    /* If `addr` does not surpass `0x9000`(kernel memory), then the following code will run.*/
    assign_accordingly:

    /* Decrement the `ptr_addr` pointer so we are accessing the correct data. */
    ptr_addr--;

    /* Assign a `_memory_stamp` pointer to the current address(`addr`). */
    mem_stamp = (_memory_stamp * ) ptr_addr;

    /* Return :D */
    goto finish;

    /* The variable `addr` passed `0x9000`(kernel memory). Assign default values. */
    default_values:

    /* Assign address at an offset from the last address of `addr`. */
    mem_stamp = (_memory_stamp *)(addr - sizeof(_memory_stamp));

    /* Assign memory stamp magic number id that specified the memory stamp was not found. */
    for (uint8 i = 0; i < 5; i++) {
        mem_stamp -> memory_stamp_magic_number[i] = memory_stamp_magic_id_not_found[i];
    }

    /* Everything else is zero. */
    mem_stamp -> memory_id = 0;
    mem_stamp -> sectors = 0;
    mem_stamp -> estimate_size_in_bytes = 0;
    mem_stamp -> is_overwritten = 0;
    mem_stamp -> access = 0;
    mem_stamp -> beginning_address = 0;
    mem_stamp -> ending_address = 0;

    finish:
        return mem_stamp;
}

#endif

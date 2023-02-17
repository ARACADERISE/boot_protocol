#ifndef mbr_partition_table
#define mbr_partition_table

#ifdef __cplusplus
extern "C" {
#endif

asm(".code16gcc\n");

#ifndef protocol_MBR_data
#include "MBR_data.h"
#endif

extern void test_read();
extern void load_program_for_higher_half_kernel(uint16 addr, uint8 sector);
extern void read_in_program();

/* Read memory for second stage and kernel.
 * `addr` - the address where whatever we are loading is located
 * `sector_count` - amount of sectors the program uses
 * `curr_sector` - From what sector do we start reading from?
 */
void read_in_memory(uint16 addr, uint8 start_sector, uint8 sector_amount);

#ifdef __cplusplus
}
#endif

#endif
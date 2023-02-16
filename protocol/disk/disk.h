#ifndef protocol_disk
#define protocol_disk

#ifdef __cplusplus
extern "C" {
#endif

#ifndef protocol_types
#include "../types.h"
#endif

/* Current sector count. */
uint16 *curr_sector_count = (uint16 *)0x5000;

/* Information for reading from disk. */
typedef struct disk_read_info
{
    uint16          address;
    uint8           sector_count;
} _disk_read_info;

/* All of the read-in memory. */
typedef struct sectors
{

} _sectors;

extern void __read_disk(uint16 sectors, uint16 addr);

/* Read the sectors :D (screw assembly, it's being a bitch today for some reason)*/
void read_sectors(uint16 addr, uint8 sector_count)
{
    __read_disk(sector_count, addr);
    //__asm__("cli;hlt");

    //*curr_sector_count += sector_count;
}

/* Jump to a location, depending on if it's been read or not. */

/* Read from disk in 16-bit mode :D */
void load_needed_memory(_disk_read_info *dri)
{
    if(dri != NULL)
    {
        for(uint8 i = 0; i < sizeof(dri)/sizeof(dri[0]); i++)
        {
            read_sectors(dri[i].address, dri[i].sector_count);
        }

        if(*curr_sector_count == 0x0F)
        {
            print("Yu");
            halt
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif
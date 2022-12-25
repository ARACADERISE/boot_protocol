#ifndef protocol_disk
#define protocol_disk

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

/* Read the sectors :D (fuck assembly, it's being a bitch today for some reason)*/
void read_sectors(uint16 addr, uint8 sector_count)
{
    uint8 sector = *curr_sector_count & 0xFF;
    sector = 0x0A;
    __asm__("mov ax, %0" : : "r"(addr));
    __asm__("mov es, ax\nxor bx, bx\n");
    __asm__ __volatile__("mov ah, 0x02\n");
    __asm__ __volatile__("mov al, %0\n" : : "dN"(sector_count));
    __asm__ __volatile__("mov ch, 0x00\n");
    __asm__ __volatile__("mov cl, %0\n" : : "dN"(sector));
    __asm__ __volatile__("mov dh, 0x00\n");
    __asm__ __volatile__("mov dl, 0x80\n");
    __asm__ __volatile__("int 0x13\n");
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
            print_str("Yu");
            halt
        }
    }
}

#endif
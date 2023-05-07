#include "format.h"

size_t get_file_size(char *filename) {
    /* Open up the file, check if it exists. If not, error. */
    FILE *f = fopen(filename, "rb");

    if (!(f)) {
        fprintf(stderr, "Error!");
        exit(EXIT_FAILURE);
    }

    /* Get the size. */
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* If the size is zero, error. */
    if (file_size == 0) {
        fprintf(stderr, "File has zero bytes.\n");
        exit(EXIT_FAILURE);
    }
    fclose(f);

    return file_size;
}

static inline void write_padding(_memory_stamp *mem_info, uint16 padding_needed, uint8 *filename) {
    FILE *f = fopen(filename, "a");
    uint8 padding[padding_needed];
    memset(padding, 0, padding_needed);

    if (!(f)) {
        fprintf(stderr, "Error!");
        exit(EXIT_FAILURE);
    }

    fwrite(padding, padding_needed, sizeof(uint8), f);

    /* If `mem_info` is `NULL`, we're writing just padding. */
    if (mem_info != NULL)
        fwrite(mem_info, 1, memory_stamp_size, f);

    fclose(f);
}

int main(int args, char * argv[]) {
    if (!(args >= 2)) {
        fprintf(stderr, "Expected file as argument.\n");
        exit(EXIT_FAILURE);
    }

    /* Variables. */
    size_t file_size = 0;

    /* Memory stamp. This is information that is written in the last X amount of bytes of all binary files used for the OS. */
    _memory_stamp mem_info;

    /* If `argv[2]` is `--jpad`, we just write padding to the file. */
    if (strcmp(argv[2], "--jpad") == 0) {
        file_size = get_file_size(argv[1]);

        /* Get the correct amount of sectors. */
        mem_info.sectors = (file_size / bytes_per_sector);

        if(mem_info.sectors * bytes_per_sector < file_size)
            mem_info.sectors++;

        /* Calculate the amount of padding we need to pad out binary to multiples of 512-byte blocks(sectors). */
        uint16 padding_needed = ((mem_info.sectors * bytes_per_sector) - file_size);
                
        write_padding(NULL, padding_needed, argv[1]);

        return 0;
    }

    /* "Magic number" referencing that the memory stamp is starting. */
    for (uint8 i = 0; i < 5; i++)
        mem_info.memory_stamp_magic_number[i] = memory_stamp_magic_number_id[i];

    /* Decipher what type of binary we're generating a memory stamp for. 
     * `--kernel` deciphers the memory stamp is describing memory over the kernel
     * `--second_stage` deciphers the memory stamp is describing memory over the second-stage bootloader. 
     * I decided to just check if `--kernel` is passed, and if it isn't we default to stapling the `boot_id_2` ID to the memory id. */
    if (strcmp(argv[2], "--kernel") == 0)
    {
        uint8 kernel_name[10] = "kernel";
        for(char i = 0; i < 10; i++)
            memset(&mem_info.name[i], kernel_name[i], 1);
        
        mem_info.memory_id = kernel_id;
    }
    else
    {
        uint8 second_stage_name[20] = "second_stage";
        for(char i = 0; i < 20; i++)
            memset(&mem_info.name[i], second_stage_name[i], 1);

        mem_info.memory_id = boot_id_2;
    }
    
    mem_info.is_overwritten = false; // The memory is, by default, not overwritten. The protocol treats all memory as critical unless otherwise specified
    if (mem_info.memory_id == kernel_id)
        mem_info.beginning_address = 0xA000;
    else
        mem_info.beginning_address = 0x7E00; // The second-stage bootloader is located 256-bytes after the MBR. That is because there is critical information being stored in between 0x7E00 and 0x7F00

    mem_info.access = access_level_one; // The access-level can be changed via the user or the protocol itself. Normally the protocol will change the access-level

    file_size = get_file_size(argv[1]);

    /* Get the correct amount of sectors. */
    mem_info.sectors = file_size / bytes_per_sector;

    /* Check if we need to add a sector. If the overall sector count
     * does not match the file size we must pad it with an extra sector.
     */
    if((mem_info.sectors * bytes_per_sector) < (file_size))
        mem_info.sectors++;

    /* Calculate the amount of padding we need to pad out binary to multiples of 512-byte blocks(sectors). */
    uint16 padding_needed = (mem_info.sectors * bytes_per_sector) - file_size;
    padding_needed -= memory_stamp_size;

    /* Get the estimate size(in bytes) of the binary, and calculate the ending address. */
    mem_info.estimate_size_in_bytes = (file_size + padding_needed) + memory_stamp_size;
    mem_info.ending_address = mem_info.beginning_address + mem_info.estimate_size_in_bytes;

    printf("\n\nmem_info.beginning_address: %X\nmem_info.ending_address: %X\nmem_info.sectors: %d\nmem_info.access: %X\n\n",
        mem_info.beginning_address,
        mem_info.ending_address,
        mem_info.sectors,
        mem_info.access);
    
    /* Write padding to file. */
    write_padding(&mem_info, padding_needed, argv[1]);

    return 0;
}
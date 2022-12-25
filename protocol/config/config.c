#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

typedef struct format
{
    unsigned char       section1;               // `n`
    unsigned char       pad1;                   // 0x0
    unsigned char       bitType;                // 2 = 32bit, 3 = 64bit
    unsigned char       pad2;
    unsigned char       fileDescBegin;          // 'f'
    unsigned char       pad3;                   // 0x0
    unsigned char       type;                   // 's' = second stage, 'k' = kernel
    unsigned char       pad;

    /* File Info. */
    unsigned char      second_stage_address[4];   // Address of second stage
    unsigned char      pad4;
    unsigned char      r_second_stage_addr[4];
    unsigned char      pad5;

    /* File metadata. */
    unsigned char      *second_stage_source_code;
    unsigned char      pad6;
    unsigned char      *second_stage_binary;
    unsigned char      pad7;
    unsigned char      extra_section;           // 'e'
    unsigned char      pad_;
    unsigned char      *kernel_source_code;
    unsigned char      pad8;
    unsigned char      *kernel_binary;
    unsigned char      pad9;

    unsigned char      pad_many[4];
    unsigned char      sum[4];
} _format;

/* Depricated information(additional shit is removed lmao, literally what I am saying). */
typedef struct dep_boot_info
{
    /* Second stage binary/source code file. */
    unsigned char       *second_stage_code;
    unsigned char       *second_stage_binary;

    /* Second stage address. This is for the es:bx memory segmentation. */
    unsigned int        second_stage_addr;

    /* Real second-stage address. Used for `jmp codeseg:second_stage_addr`. */
    unsigned int        r_second_stage_addr;

    /* Kernel binary/source code file. */
    unsigned char       *kernel_code;
    unsigned char       *kernel_binary;

    /* Kernel address. This is for the es:bx memory segmentation. */
    unsigned int        kernel_addr;

    /* Real kernel address. For when the user does `load_kernel`. */  
    unsigned int        r_kernel_addr;
} _dep_boot_info;

int main()
{
    _format boot_info;
    _dep_boot_info dbi;

    //boot_info.filename = calloc(9, sizeof(char*));
    //boot_info.binary_file = calloc(6, sizeof(char*));

    /* Read in the information for bootloader/second stage/kernel info. */
    FILE* bin_info = fopen("config/something.bin", "rb");

    size_t bytes = 0;

    /* Read in once. */
    while(fread(&boot_info, sizeof(struct format), 1, bin_info))
        {
            if(bytes == 1) break;

            bytes++;
            continue;
        }

    fclose(bin_info);

    /* Turn into decimal. */
    dbi.second_stage_addr = atoi(boot_info.second_stage_address);
    dbi.r_second_stage_addr = atoi(boot_info.r_second_stage_addr);

    printf("%x", boot_info.extra_section);

    /*printf("Section 1: %c\n", boot_info.section1);
    printf("bitType: %c\n", boot_info.bitType);
    printf("fileDescBeg: %c\n", boot_info.fileDescBegin);
    printf("Type: %c\n", boot_info.type);
    printf("Filename Size: %c\n", boot_info.filename_size);
    printf("Filename: %s\n", boot_info.filename);
    printf("Binary file: %s\n", boot_info.binary_file);*/

    return 0;
}
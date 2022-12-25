#ifndef protocol_gdt
#define protocol_gdt


/* Constants for `init_bootloader` function. */
/* Helps with configuring certain aspects of the protocol such as what type of GDT. */
//#define gdt_bit32_bit16         0x3216
//#define gdt_bit32_only          0x32

/* What is the status of GDT?
 * 0 - GDT/GDT Descriptor needs to be loaded to memory.
 * 1 - There is an already-working GDT/GDT Descriptor loaded in memory. 
 * */
static unsigned char *gdt_status = (unsigned char *)0xC000;

/* Enum of different types of GDT. */
enum GDT_TYPE {
    clean_gdt           =   0x1,
    bit32_bit16_gdt     =   0x2,
    bit32_only_gdt      =   0x3,
};

/* Video mode? */
enum VID_MODE
{
    B8000               =   0x4,          // Default
    VESA_VID_MODE       =   0x5,
};

/* GDT information, such as what type of GDT, whether or not the user want
 * a default GDT etc. */
// TODO: Do we want this?
// Seems just doing `#define`s is a lot easier, to me anyway. I don't know
//typedef struct gdt_info
//{
    /* What type of GDT? */
//    uint8       gdt_type;

    /* Does the user what a default-gdt to be filled out if there is a clean slate
     * loaded to memory? */
//    bool        default_gdt: 1;
//} _gdt_info;

/* Constants for above enums. */
#define DEFAULT_ALL                 (uint8)clean_gdt | B8000
#define CLEAN_GDT_DEF_VID_MODE      (uint8)clean_gdt | B8000
#define CLEAN_GDT_VESA_VID_MODE     (uint8)clean_gdt | VESA_VID_MODE
#define BIT32_BIT16_DEF_VID_MODE    (uint8)bit32_bit16_gdt | B8000
#define BIT32_BIT16_VESA_VID_MODE   (uint8)bit32_bit16_gdt | VESA_VID_MODE
#define BIT32_ONLY_DEF_VID_MODE     (uint8)bit32_only_gdt | B8000
#define BIT32_ONLY_VESA_VID_MODE    (uint8)bit32_only_gdt | VESA_VID_MODE

/* Descriptiong of GDT. */
/* This will be overwritten(by default) if you inititalize the bootloader with anything but `clean_gdt`. */
typedef struct gdt_desc
{
    uint16      size;
    uint32      address;
} packed _gdt_desc;

#ifdef bit32_bit16_GDT

    /* If the GDT has 16-bit segments, we want to make sure we know this. */
    #define has_rmode_access

    typedef struct GDT
    {
        /* 32-bit: null descriptor. */
        unsigned long long  null_desc;

        /* 32-bit: code segment. */
        uint16              code32_limit;          // 0xFFFFF for full 32-bit range
        uint16              code32_base;
        uint8               code32_base2;
        uint8               code32_access;         // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code32_gran;           // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code32_base_high;

        /* 32-bit: data segment. */
        uint16              data32_limit;         // 0xFFFFF for full 32-bit range
        uint16              data32_base;
        uint8               data32_base2;
        uint8               data32_access;        // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data32_gran;          // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data32_base_high;

        /* 16-bit: code segment. */
        uint16              code16_limit;         // should be 0xFFFFF(same as 32-bit code/data segments)
        uint16              code16_base;
        uint8               code16_base2;
        uint8               code16_access;       // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code16_gran;         // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code16_base_high;

        /* 16-bit: data segment. */
        uint16              data16_limit;         // should be 0xFFFFF(same as 32-bit code/data segments)
        uint16              data16_base;
        uint8               data16_base2;
        uint8               data16_access;       // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data16_gran;         // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data16_base_high;
    } _GDT;

#else
    /* 32-bit only GDT. */
    typedef struct GDT
    {
        /* 32-bit: null descriptor. */
        unsigned long long  null_desc;

        /* 32-bit: code segment. */
        uint16              code32_limit;          // 0xFFFFF for full 32-bit range
        uint16              code32_base;
        uint8               code32_base2;
        uint8               code32_access;         // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code32_gran;           // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code32_base_high;

        /* 32-bit: data segment. */
        uint16              data32_limit;         // 0xFFFFF for full 32-bit range
        uint16              data32_base;
        uint8               data32_base2;
        uint8               data32_access;        // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data32_gran;          // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data32_base_high;
    } _GDT;
#endif

/*
 * With `init_bootloader`, the addresses `0xA000` and `0xAA00` will 
 * reference a already-working GDT/GDT descriptor or it will reference a
 * clean slate of a GDT/GDT descriptor.
 *
 * TODO: Does it really matter if we set the `gdt` and `gdtDesc` to the according addresses?
 *       Perhaps set each to NULL, and if there is a clean slate of a GDT/GDT descriptor
 *       in memory, we are safe to say that the protocol will load the gdt
 *       to the according address(`0xA000` for the actual GDT and `0xAA00` for the GDT descriptor).
 *       Referencing the addresses in the C program doesn't have a huge impact, right?
 * */

/* 0xA000 - Memory address of GDT. */
static _GDT *gdt = (_GDT *)0xA000;

/* 0xAA00 - Memory address of GDT description. */
static _gdt_desc *gdtDesc = (_gdt_desc *)0xAA00;

/* Function call to either load in a new GDT into memory or reference an
 * already-existing GDT that has been previously loaded into memory. */
extern void save_gdt_and_load(_gdt_desc gdtDesc, _GDT gdt);

/* Load the GDT. */
void load_32bit()
{
    /* Yes, I know. This is very, very excessive checking.*/
    if(gdt == NULL || gdtDesc == NULL || (gdt == NULL && gdtDesc == NULL))
    {
        print_str("Error from `load_32bit`:\nThe gdt or gdt description(or both) is NULL :(");
        halt
    }

    save_gdt_and_load(*gdtDesc, *gdt);
}

/* Setting up the description of the GDT. */
static inline void setup_gdt_and_gdt_desc()
{
#ifdef default_gdt
    /* If `gdt_status` is 1, that means there is a already-working GDT/GDT descriptor loaded into memory. */
    /* If `gdt_status` is 0, that means there needs to be a working GDT/GDT descriptor loaded into memory */
    if(*gdt_status == 0)
    {
        gdt->null_desc          = 0,

        /* 32-bit code segment. */
        gdt->code32_limit       = 0xFFFF;
        gdt->code32_base        = 0x0;
        gdt->code32_base2       = 0x0;
        gdt->code32_access      = 0b10011010;
        gdt->code32_gran        = 0b11001111;
        gdt->code32_base_high   = 0x0;

        /* 32-bit data segment. */
        gdt->data32_limit       = 0xFFFF;
        gdt->data32_base        = 0x0;
        gdt->data32_base2       = 0x0;
        gdt->data32_access      = 0b10010010;
        gdt->data32_gran        = 0b11001111;
        gdt->data32_base_high   = 0x0;

        /* 16-bit code segment. */
        gdt->code16_limit       = 0xFFFF;
        gdt->code16_base        = 0x0;
        gdt->code16_base2       = 0x0;
        gdt->code16_access      = 0b10011010;
        gdt->code16_gran        = 0b00001111;
        gdt->code16_base_high    = 0x0;

        /* 16-bit data segment. */
        gdt->data16_limit       = 0xFFFF;
        gdt->data16_base        = 0x0;
        gdt->data16_base2       = 0x0;
        gdt->data16_access      = 0b10010010;
        gdt->data16_gran        = 0b00001111;
        gdt->data16_base_high   = 0x0;

        /* Setup the description for the GDT. */
        gdtDesc->size = (uint16)sizeof(*gdt);
        gdtDesc->address = (uint32)gdt;
    }
#else
    if(*gdt_status == 0)
    {
        print_str("Error in `setup_gdt_desc`:\nDid you forget to `init_bootloader`?");
        __asm__("cli;hlt");
    }
#endif
}

#endif
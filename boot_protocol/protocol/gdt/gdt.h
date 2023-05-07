#ifndef protocol_gdt
#define protocol_gdt

#ifdef __cplusplus
extern "C" {
#endif

/* What is the status of GDT?
 * 0 - GDT/GDT Descriptor needs to be loaded to memory.
 * 1 - There is an already-working GDT/GDT Descriptor loaded in memory. 
 * */
extern uint8 g_gdt_status_address[];
static unsigned char *gdt_status = (unsigned char *)g_gdt_status_address;

/* Enum of different types of GDT. */
enum GDT_TYPE {
    clean_gdt           =   0x1,
    bit32_bit16_gdt     =   0x10,
    bit32_only_gdt      =   0x2,
};

/* Video mode? */
enum VID_MODE
{
    B8000               =   0x20,          // Default
    VESA_VID_MODE       =   0x3,
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
/* Not needed for now. FAMP will only support 32-bit/16-bit GDTs and will offer functionaly for things like vesa to be setup. */
/*#define DEFAULT_ALL                 (uint8)clean_gdt | B8000
#define CLEAN_GDT_DEF_VID_MODE      (uint8)clean_gdt | B8000
#define CLEAN_GDT_VESA_VID_MODE     (uint8)clean_gdt | VESA_VID_MODE
#define BIT32_BIT16_DEF_VID_MODE    (uint8)bit32_bit16_gdt | B8000
#define BIT32_BIT16_VESA_VID_MODE   (uint8)bit32_bit16_gdt | VESA_VID_MODE
#define BIT32_ONLY_DEF_VID_MODE     (uint8)bit32_only_gdt | B8000
#define BIT32_ONLY_VESA_VID_MODE    (uint8)bit32_only_gdt | VESA_VID_MODE*/

/* Descriptiong of GDT. */
/* This will be overwritten(by default) if you inititalize the bootloader with anything but `clean_gdt`. */
typedef struct gdt_desc
{
    uint16      size;
    uint32      address;
} packed _gdt_desc;

/* 32-bit/16-bit GDT. */
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

/* GDT in memory. */
extern _GDT g_gdt_address[];
//static _GDT *gdt = (_GDT *)0xA000;

/* 0xAA00 - Memory address of GDT description. */
extern _gdt_desc g_gdt_desc_address[];
//static _gdt_desc *gdtDesc = (_gdt_desc *)0xAA00;

/* Function call to either load in a new GDT into memory or reference an
 * already-existing GDT that has been previously loaded into memory. */
extern void __save_gdt_and_load(_gdt_desc gdtDesc, _GDT gdt);

/* GDT status settings for `set_gdt_status`.
 * TODO: Possibly remove this. Odds are, we aren't going to support differen types of GDTs. Just 32-bit/16-bit ones.
 */
enum gdt_status_settings
{
    NO_GDT              = 0x00,
    BIT16_BIT32_GDT     = 0x01,
    BIT32_ONLY_GDT      = 0x02
};

/* Manually set the GDT status with a setting(`NO_GDT`, `BIT16_BIT32_GDT` or `BIT32_ONLY_GDT`).
 * 
 * Side Note: `set_gdt_status(NO_GDT)` is quite useless, especially if you load a clean GDT/GDT description into memory.
 *            `set_gdt_status(NO_GDT)` can also screw you up if you load a already-working GDT/GDT description into memory -
 *             this will force you to either make the protocol fill out the GDT, or you yourself.
 * TODO: Possibly remove this for same reason given above.
 */
static void set_gdt_status(enum gdt_status_settings setting)
{
    *gdt_status = setting;
}

/* 
 *  __load_32bit: back-end function
 *
 *  Load the GDT into memory, and jump to 32-bit mode.
 *
 *  Input: None
 *  Output: None
 *  On Error: This function does not error
 */
void __load_32bit()
{
    /* Make sure there is a valid GDT/GDT description in memory.
     * Yes, this is somewhat "excessive checking".. better safe then sorry.
     */
    if(g_gdt_address->null_desc == 1 || g_gdt_desc_address->size == 1 || *gdt_status == 0)
    {
        print("\n  Error from `load_32bit`:\n  There is not a valid GDT/GDT description loaded into memory :(\n      -> Did you forget to `init_bootloader`?\n      -> Perhaps you forgot to setup your GDT?\n\n  For future reference, put `#define default_gdt` if you want FAMP to \n  fill out your GDT.\n\n  Do Note: `default_gdt` only works with the following settings:\n      1. `DEFAULT_ALL`\n      2. `CLEAN_GDT_DEF_VID_MODE`\n      3. `CLEAN_GDT_VESA_VID_MODE`\0");
        halt
    }
    
    /* Lets get into 32-bit mode fast!! */
    __save_gdt_and_load(*g_gdt_desc_address, *g_gdt_address);
}

/*
 *  __setup_gdt_and_gdt_desc: back-end function
 *
 *  Assign default values to each field of the GDT struct
 *
 *  Input: None
 *  Output: None
 *  On Error: This function does not error
 */
static inline void __setup_gdt_and_gdt_desc()
{
    /* `gdt_status` is, most likely, not needed anymore. Just keeping it here for time being. */
    if(*gdt_status == 0)
    {
        g_gdt_address->null_desc          = 0,

        /* 32-bit code segment. */
        g_gdt_address->code32_limit       = 0xFFFF;
        g_gdt_address->code32_base        = 0x0;
        g_gdt_address->code32_base2       = 0x0;
        g_gdt_address->code32_access      = 0b10011010;
        g_gdt_address->code32_gran        = 0b11001111;
        g_gdt_address->code32_base_high   = 0x0;

        /* 32-bit data segment. */
        g_gdt_address->data32_limit       = 0xFFFF;
        g_gdt_address->data32_base        = 0x0;
        g_gdt_address->data32_base2       = 0x0;
        g_gdt_address->data32_access      = 0b10010010;
        g_gdt_address->data32_gran        = 0b11001111;
        g_gdt_address->data32_base_high   = 0x0;

        /* 16-bit code segment. */
        g_gdt_address->code16_limit       = 0xFFFF;
        g_gdt_address->code16_base        = 0x0;
        g_gdt_address->code16_base2       = 0x0;
        g_gdt_address->code16_access      = 0b10011010;
        g_gdt_address->code16_gran        = 0b00001111;
        g_gdt_address->code16_base_high    = 0x0;

        /* 16-bit data segment. */
        g_gdt_address->data16_limit       = 0xFFFF;
        g_gdt_address->data16_base        = 0x0;
        g_gdt_address->data16_base2       = 0x0;
        g_gdt_address->data16_access      = 0b10010010;
        g_gdt_address->data16_gran        = 0b00001111;
        g_gdt_address->data16_base_high   = 0x0;

        /* Setup the description for the GDT. */
        g_gdt_desc_address->size = (uint16)sizeof(*g_gdt_address);
        g_gdt_desc_address->address = (uint32)g_gdt_address;

        set_gdt_status(BIT16_BIT32_GDT);
    }
}

#ifdef __cplusplus
}
#endif

#endif
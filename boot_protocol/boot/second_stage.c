#define bit32_bit16_GDT
#define default_gdt

#include "../protocol/boot_header.h"

void starting_point main()
{
    init_bootloader(CLEAN_GDT_DEF_VID_MODE);

    _memory_stamp *mem_stamp = __obtain_memory_stamp((uint8 *)&second_stage_end, second_stage);

    load_kernel();
}
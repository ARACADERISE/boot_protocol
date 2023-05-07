#ifndef protocol_boot_header
#define protocol_boot_header

#ifdef __cplusplus
extern "C" {
#endif

#ifndef protocol_types
#include "types.h"
#endif

/* The C code has to be 16-bit for second stage bootloader. */
/* The boot sector program loads the second stage bootloader
 * which, to the user, is the initial boot of the OS. The second stage
 * bootloader is where the user can load in the memory map, setup a video mode,
 * and other bare minimum ideals that a OS bootloader needs. */
asm(".code16gcc\n");

/* Beginning address of second stage bootloader(`main.c`). */
extern uint8 second_stage_start[];
extern uint8 second_stage_end[];

/* Memory stamp address for second stage bootloader. */
extern uint8 ss_mem_stamp_addr[];

/* Kernel size/kernel address. */
extern uint16 kernel_size;
extern uint8 kernel_address[];

/* Memory stamp address for kernel. */
extern uint8 kernel_mem_stamp_addr[];

// OLD: extern void init_bootloader(int8);
extern void init_bootloader();
extern void __test_address(uint16);

/* Other ideals for the protocol*/
#ifndef protocol_util
#include "util.h"
#endif

#ifndef protocol_asm_help
#include "tools/asm_help.h"
#endif

#ifndef protocol_print
#include "tools/print.h"
#endif

#ifndef protocol_memory_stamp
#include "memory/memory_stamp.h"
#endif

#ifndef protocol_disk
#include "disk/disk.h"
#endif

#ifndef protocol_gdt_api
#include "gdt.h"
#endif

/* For user-convenience. */
#define starting_point __attribute__((section("__start")))

/* Memory addresses used throughout the protocol. 
 * TODO: Might delete this comment. Memory addresses critical to the protocol will be defined within the given
 *       header file that works with the given feature where the address is critical.
 */

#ifdef __cplusplus
}
#endif

#endif

#ifndef protocol_kernel_header
#define protocol_kernel_header

#ifdef __cplusplus
extern "C" {
#endif

#ifndef protocol_types
#include "types.h"
#endif

#ifndef protocol_util
#include "util.h"
#endif

#ifdef using_vesa
//#include "vesa_api.h"
#elif defined(using_b8000)
#include "b8000_api.h"
#else
#include "b8000_api.h"
#endif

/* Kernel starting/ending points. */
extern uint16 kernel_start;
extern uint16 kernel_end;

/* For user-convenience. */
#define starting_point __attribute__((section("__start")))

/* Memory address that are, without a doubt, able to be released to memory map ASAP. */
const uint8 *instant_free_addresses[] = {
    (const uint8 *)0x0500, 
    (const uint8 *)0x9000,
    (const uint8 *)0xA000 // Kernel would of been relocated by the program in `0x9000`
};

#ifdef __cplusplus
}
#endif

#endif
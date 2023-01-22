#ifndef protocol_kernel_header
#define protocol_kernel_header

#ifndef protocol_types
#include "types.h"
#endif

/* Kernel starting/ending points. */
extern uint16 kernel_start;
extern uint16 kernel_end;

/* For user-convenience. */
#define starting_point __attribute__((section("__start")))

#endif
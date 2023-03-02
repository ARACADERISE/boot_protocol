#ifndef protocol_types
#define protocol_types

#ifdef __cplusplus
extern "C" {
#endif

/* Normal data types. */
typedef char                int8;
typedef unsigned char       uint8;
typedef signed char         sint8;
typedef short               int16;
typedef unsigned short      uint16;
typedef signed short        sint16;
typedef int                 int32;
typedef unsigned int        uint32;
typedef signed int          sint32;

/* Sizes(size_t). */
typedef unsigned long       size;
typedef unsigned long long  lsize;

/* Booleans. */
typedef uint8               bool;
#define true                1
#define false               0
#define NULL                (void *)0

/* Other useful stuff. */
#define packed              __attribute__((packed))

#ifdef __cplusplus
}
#endif

#endif
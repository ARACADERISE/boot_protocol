#ifndef yaml_common
#define yaml_common
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* My own types because.. why not? */
typedef unsigned char		uint8;
typedef signed char		int8;
typedef unsigned short		uint16;
typedef signed short		int16;
typedef unsigned int		uint32;
typedef signed int		int32;
typedef unsigned long		size;

/* 8-bytes. */
typedef unsigned long long	lsize;

typedef uint8			bool;
#define true			1
#define false			0

/* Printing. */
#define yaml_print(msg, ...)				\
	fprintf(stdout, msg, ##__VA_ARGS__);	
#define yaml_error(msg, ...)				\
	yaml_print(msg, ##__VA_ARGS__);			\
	exit(EXIT_FAILURE);

/* Checks. */
#define yaml_assert(cond, err_msg, ...)			\
	if(!(cond)) {					\
		yaml_error(err_msg, ##__VA_ARGS__);	\
	}

#endif

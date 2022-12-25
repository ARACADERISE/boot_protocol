#ifndef yaml_data
#define yaml_data

#ifndef yaml_common
#include "common.h"
#endif

/* Types of data. */
enum data_types
{
	hex,
	dec,
	str
};

/* Information about the yaml file, such as user-defined variables/variable data, arrays etc. */
typedef struct data
{
	/* User-defined value. */
	uint8		*user_defined_val;

	/* Type of information we're storing. */
	enum data_types	data_type;

	/* The value will either be a character, string or decimal/hex. */
	uint32		*val_data;
	
	/* Next user-defined value. */
	struct data	*next;
	
	/* Previous data. */
	struct data	*previous;
} _data;

/* Static reference to yaml file data. */
static _data		*yaml_file_data		= NULL;

/* How much data? */
static size		yaml_file_data_size 	= 0;

/* Initialize memory for yaml file data. */
void init_yaml_data();

#endif

#ifndef yaml_data
#define yaml_data

#ifndef yaml_common
#include "common.h"
#endif

/* What type of OS are we running? 32-bit, 64-bit? */
enum os_type
{
	bit32,
	bit64
};

/* Outline of what the yaml file should look like, for our use case only. */
typedef struct yaml_os_data
{
	uint8			type;					// 2 = 32bit, 3 = 64bit

	bool			has_second_stage;

	/* General OS information. */
	uint8			*OS_name;
	uint8			*OS_version;
	uint8			FS_type;

	/* Disk image name(binary file storing the FS). */
	uint8			*disk_name;

	/* Does the user want FAMP to auto format the disk image? By auto format, I am referring to FAMP formatting the disk image according to `FS_type`. */
	bool			auto_format;

	/* Binary folder. */
	uint8			*bin_folder;

	// Description of kernel
	uint8			*kern_entry_point;
	size			kern_addr;
	uint16			kern_filename_size;		// kernel source code file size
	uint8			*kern_filename;			// kernel source code file name
	uint16			kern_filename_bin_o_size;	// kernel binary file name size
	uint8			*kern_filename_bin_o_name;// kernel binary file name
	size			kern_filename_bin_size;
	uint8			*kern_filename_bin_name;
	size			kern_bin_size;			// size(in bytes) of binary file
} _yaml_os_data;

/* All of the things the yaml file needs to have. Everything else is extra. */
static const char * const needed_names[] = {
	// Type of os. 32bit or 64bit
	"os_type",

	// Name of OS
	"os_name",

	// OS version
	"os_vers",

	// Preferred FS type(supported: FAT32, ext2, FAMPs custom FS)
	"pref_FS",

	// Binary folder where all binaries will be kept
	"bin_folder",

	// Kernel information
	"kernel_o_binary",
	"kernel_bin_binary",
	//"kernel_addr",
	"kernel_source_code_file"
};

/* Types of data. */
enum data_types
{
	Chr,
	Hex,
	Dec,
	Str
};

/* Information about the yaml file, such as user-defined variables/variable data, arrays etc. */
typedef struct data
{
	/* User-defined value. */
	uint8		*user_defined;

	/* Type of information we're storing. */
	enum data_types	data_type;

	/* The value will either be a character, string or decimal/hex. */
	uint16		*val_data;
	
	/* Next user-defined value. */
	struct data	*next;
	
	/* Previous data. */
	struct data	*previous;
} _data;

/* Static reference to yaml file data. */
static _data		*yaml_file_data		= NULL;

/* Reference to first index of yaml_file_data. */
static _data		*all_yaml_data		= NULL;

/* How much data? */
static size		yaml_file_data_size 	= 0;

/* Useful macros :) */
#define _next yaml_file_data = yaml_file_data->next;
#define _back yaml_file_data = yaml_file_data->previous;

/* Initialize memory for yaml file data. */
void init_yaml_data();

/* Assign new data. */
void new_yaml_data(uint8 *user_def, uint16 *data, enum data_types type);

/* Write all the data as binary. */
_yaml_os_data get_yaml_os_info();

#endif

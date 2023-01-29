#include "data.h"

void init_yaml_data()
{
    yaml_file_data      = calloc(1, sizeof(*yaml_file_data));
    
    yaml_file_data->user_defined	= NULL;
    yaml_file_data->val_data    	= NULL;
    yaml_file_data->next        	= NULL;
    yaml_file_data->previous    	= NULL;	

    all_yaml_data 			= yaml_file_data;
}

void new_yaml_data(unsigned char *user_def, unsigned short *data, enum data_types type)
{
    /* Store current yaml_file_data struct. */
    _data *prev = yaml_file_data;
    
    /* Allocate new memory for next reference. */
    yaml_file_data->next         = calloc(1, sizeof(*yaml_file_data->next));
    
    /* Assign the user-defined value. */
    yaml_file_data->user_defined = calloc(strlen(user_def)+1, sizeof(*yaml_file_data->user_defined));
    yaml_file_data->user_defined = user_def;
    
    /* Assign the data type. */
    yaml_file_data->data_type = type;
    
    /* Check the type, and assign accordingly. */
    switch(type)
    {
        case Chr: yaml_file_data->val_data = calloc(1, sizeof(*yaml_file_data->val_data));yaml_file_data->val_data = data;break;
        case Dec:
        case Hex: yaml_file_data->val_data = calloc(2, sizeof(*yaml_file_data->val_data));yaml_file_data->val_data = data;break;
        case Str: yaml_file_data->val_data = calloc(2, sizeof(*yaml_file_data->val_data));yaml_file_data->val_data = data;break;
        default: break;
    }
    
    /* Assign current yaml_file_data struct to the next _data reference. */
    yaml_file_data = yaml_file_data->next;

	/* Make the `next` NULL because there might not be another `next`. */
	yaml_file_data->next = NULL;
    
    /* Allocate memory for previous _data struct and assign to `prev`. */
    yaml_file_data->previous     = calloc(1, sizeof(*yaml_file_data->previous));
    yaml_file_data->previous     = prev;

    /* Increment the size of `yaml_file_data`. */
    yaml_file_data_size++;
}

static inline size determine_size(enum data_types type)
{
    switch(type)
    {
        case Chr: return sizeof(unsigned short);break;
        case Dec:
        case Hex: 
        case Str: return sizeof(unsigned short) * 2;break;
        default: break;
    }

    /* If this isn't then right size(2 bytes), then the compiler
     * can go fuck itself because everything should check out. */
    return sizeof(unsigned short);
}

static inline lsize convert_hex_to_dec(uint8 *hex)
{
	lsize dec = 0;
	lsize base = 1;
	for(uint32 i = strlen(hex)-1; i > 0; i--)
    	{
		if(hex[i] >= '0' && hex[i] <= '9')
		{
		    dec += (hex[i] - 48) * base;
		    base *= 16;
		}
		else if(hex[i] >= 'A' && hex[i] <= 'F')
		{
		    dec += (hex[i] - 55) * base;
		    base *= 16;
		}
		else if(hex[i] >= 'a' && hex[i] <= 'f')
		{
		    dec += (hex[i] - 87) * base;
		    base *= 16;
		}
   	}

	return dec;
}

_yaml_os_data get_yaml_os_info()
{
	/* Init _yaml_os_data. */
	_yaml_os_data os_data;
	os_data.has_second_stage = false;

	/* Point to the first instance of `yaml_file_data`. */
	yaml_file_data = all_yaml_data;

	/* Index of `needed_names`. */
	size ind = 0;

	/* Check and make sure the amount of data we obtained from the yaml file
	 * is equal to(or greater than) the amount of data we require. 
	 * */
	yaml_assert(yaml_file_data_size >= sizeof(needed_names)/sizeof(needed_names[0]), "\n\nError:\n\tMissing some information in `boot.yaml`.\n")

	/* Check the type of OS. */
	if(strcmp((uint8*)yaml_file_data->val_data, "32bit") == 0) os_data.type = 0x02;
	else if(strcmp((uint8*)yaml_file_data->val_data, "64bit") == 0) os_data.type = 0x03;
	else os_data.type = 0x02;

	_next
	_next

	/* Do we have a second stage? */
	if(strcmp((uint8 *)yaml_file_data->val_data, "yes")==0)
	{
		os_data.has_second_stage = true;
		_next


		/* Second stage binary object file info. */
		os_data.ss_filename_bin_o_size = (uint16)strlen((const uint8 *)yaml_file_data->val_data);
		os_data.ss_filename_bin_o_name = (uint8 *)yaml_file_data->val_data;
		_next

		/* Get size of second-stage binary. */
		uint8 ss_bin_name[50] = "../../";
		FILE* ss_bin = fopen((const uint8 *)strcat(ss_bin_name, (uint8 *)yaml_file_data->val_data), "rb");
		yaml_assert(ss_bin, "Error opening second-stage binary file `%s`.\n", ss_bin_name)

		fseek(ss_bin, 0, SEEK_END);
		os_data.ss_filename_bin_size = ftell(ss_bin);
		fseek(ss_bin, 0, SEEK_SET);

		fclose(ss_bin);
		os_data.ss_filename_bin_name = (uint8 *)yaml_file_data->val_data;
		_next

		/* Second stage source code file info. */
		os_data.ss_size = (uint16)strlen((const uint8 *)yaml_file_data->val_data);
		os_data.ss_filename = (uint8 *)yaml_file_data->val_data;
	}
	_next

	/* Kernel binary file info. */
	os_data.kern_filename_bin_o_size = (uint16)strlen((const uint8 *)yaml_file_data->val_data);
	os_data.kern_filename_bin_o_name = (uint8 *)yaml_file_data->val_data;
	_next

	/* Get kernel binary size. */
	uint8 kern_bin_name[50] = "../../";
	FILE* kern_bin = fopen((const uint8 *)strcat(kern_bin_name, (uint8 *)yaml_file_data->val_data), "rb");
	yaml_assert(kern_bin, "Error opening kernel binary file`%s`.\n", kern_bin_name)

	fseek(kern_bin, 0, SEEK_END);
	os_data.kern_filename_bin_size = ftell(kern_bin);
	fseek(kern_bin, 0, SEEK_SET);

	fclose(kern_bin);
	os_data.kern_filename_bin_name = (uint8 *)yaml_file_data->val_data;
	_next

	/* Kernel source code info. */
	os_data.kern_filename_size = (uint16)strlen((const uint8 *)yaml_file_data->val_data);
	os_data.kern_filename = (uint8 *)yaml_file_data->val_data;

	/* Free `yaml_file_data`. We don't need it anymore. */
	free(yaml_file_data);

	return os_data;
}
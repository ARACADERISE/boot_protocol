#include "data.h"

void init_yaml_data()
{
	yaml_file_data = calloc(1, sizeof(*yaml_file_data));

	yaml_file_data->user_defined_val 	= NULL;
	yaml_file_data->next				= NULL;
	yaml_file_data->previous			= NULL;
}

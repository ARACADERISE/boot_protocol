#include "parser.h" 

int main(int args, char *argv[])
{
	_yaml_os_data yod = open_and_parse_yaml("yaml_tests/test1.yaml");
	
	return 0;
}
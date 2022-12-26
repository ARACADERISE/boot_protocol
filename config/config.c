#include "../YamlParser/src/parser.h" 

int main(int args, char *argv[])
{
	_yaml_os_data yod = open_and_parse_yaml("../boot.yaml");

	return 0;
}
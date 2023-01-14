#ifndef config
#define config
#include "../YamlParser/parser.h" 

int32 main(int args, char *argv[]);

#define config_assert(cond, err_msg, ...)           \
    if(!(cond)) {                                   \
        fprintf(stderr, err_msg, ##__VA_ARGS__);    \
        exit(EXIT_FAILURE);                         \
    }

#endif
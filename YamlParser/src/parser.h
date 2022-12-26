#ifndef yaml_parser
#define yaml_parser

#ifndef yaml_common
#include "common.h"
#endif

#ifndef yaml_data
#include "data.h"
#endif

/* This is useless, but I get into a habit of getting into trouble with */
/* including libraries too much. */
#ifndef yaml_lexer
#include "lexer.h"
#endif

/* Parser data :D */
typedef struct parser
{
	/* We need to reference the lexer. */
	_lexer		*lex;

	/* The parser is the placeholder for the token reference. */
	_token		*token;
} _parser;

/* Init the parser, lexer, token reference and start parsing all at once. */
_yaml_os_data open_and_parse_yaml(const uint8 *filename);

#endif
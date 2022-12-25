#include "parser.h"

static void parser_begin(_parser *p);
static void parse_user_defined(_parser *p);

uint8 start(const uint8 *filename)
{
	_parser *p 	= calloc(1, sizeof(*p));

	/* Init lexer. */
	p->lex		= init_lexer(filename);
	
	/* Init token reference. */
	p->token	= init_token_reference();

	/* Lets begin. */
	get_token(p->token, p->lex);
	parser_begin(p);

	/* Everything was a success. */
	return 0;
}

static void parser_advance(_parser *p)
{
	get_token(p->token, p->lex);
}

static void parse_user_defined(_parser *p)
{
	uint8 *user_def = p->token->token_value;
	parser_advance(p);

	/* Since it's a user defined, after the user-defined value `:` is expected.  */
	yaml_assert(p->token->id == colon, "Expected `:` after `%s`.\n", user_def);
	parser_advance(p);

	printf("%s", p->token->token_value);
	exit(0);
}

static void parser_begin(_parser *p)
{
	switch(p->token->id)
	{
		case user_def: parse_user_defined(p);break;
		default: yaml_error("Uh Oh :(")
	}

	exit(0);
}

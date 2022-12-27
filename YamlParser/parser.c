#include "parser.h"

static void parser_begin(_parser *p);
static void parse_user_defined(_parser *p);

_yaml_os_data open_and_parse_yaml(const uint8 *filename)
{
	_parser *p 	= calloc(1, sizeof(*p));

	/* Init lexer. */
	p->lex		= init_lexer(filename);
	
	/* Init token reference. */
	p->token	= init_token_reference();

	/* Initialize the yaml data. */
	init_yaml_data();

	/* Lets begin. */
	get_token(p->token, p->lex);
	parser_begin(p);

	/* Return the os information. */
	return get_yaml_os_info();
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

	/* Assign new yaml data. */
	switch(p->token->id)
	{
		case character: new_yaml_data(user_def, (uint16 *)p->token->token_value, Chr);break;
		case string: new_yaml_data(user_def, (uint16 *)p->token->token_value, Str);break;
		case number: new_yaml_data(user_def, (uint16 *)p->token->token_value, Dec);break;
		case hex: new_yaml_data(user_def, (uint16 *)p->token->token_value, Hex);break;
		default: yaml_error("Error on line %ld:\n\tExpected a string, decimal or hex value.\n", p->lex->line)
	}

	parser_advance(p);
}

static void parser_begin(_parser *p)
{
	redo:
	switch(p->token->id)
	{
		case user_def: parse_user_defined(p);goto redo;break;
		case eof: break;
		default: yaml_error("Error on line %ld.\n", p->lex->line)
	}
}

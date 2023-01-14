#include "lexer.h"

_token *init_token_reference()
{
	_token *t = calloc(1, sizeof(*t));

	return t;
}

/* Read file, return data. */
uint8 *read_file(const uint8 *filename)
{
	FILE* file = fopen(filename, "rb");

	yaml_assert(file, "The file `%s` does not exist.\n", filename)
	
	fseek(file, 0, SEEK_END);
	size file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	yaml_assert(file_size > 0, "Nothing found in the file `%s` :(.\n", filename);

	uint8 *data = calloc(file_size, sizeof(uint8));

	/* Read in all the data :D */
	fread(data, sizeof(uint8), file_size, file);

	fclose(file);
	
	return data;	
}

_lexer *init_lexer(const uint8 *filename)
{
	_lexer *lex = calloc(1, sizeof(*lex));

	lex->src = read_file(filename);
	lex->index = 0;
	lex->curr_char = lex->src[lex->index];
	lex->line = 1;

	return lex;
}

static void advance_lexer(_lexer *lex)
{
	lex->index++;
	lex->curr_char = lex->src[lex->index];
}

static bool lexer_peek(_lexer *lex, uint8 against)
{
	bool is_valid = false;
	advance_lexer(lex);

	if(lex->curr_char == against) is_valid = true;

	lex->index--;
	lex->curr_char = lex->src[lex->index];

	return is_valid;
}

typedef struct number_info
{
	uint8		*number;
	bool		is_hex;
} _number_info;

static _number_info get_number(_lexer *lex)
{
	size index = 0;
	_number_info ni = {.is_hex = false, .number = calloc(1, sizeof(*ni.number))};

	/* Check and see if the number is hex. If it is, skip over the `0x` */
	if(lexer_peek(lex, 'x')) {
		ni.is_hex = true;

		/* Skip `0x`. */
		ni.number[index] = '0';
		index++;
		ni.number = realloc(ni.number, (index + 1) * sizeof(*ni.number));
		advance_lexer(lex);		// 0
		advance_lexer(lex);		// x
	}

	while(is_number(lex->curr_char) || is_hex_dec(lex->curr_char))
	{
		ni.number[index] = lex->curr_char;

		advance_lexer(lex);
		
		/* Reallocate memory for next character. */
		index++;
		ni.number = realloc(ni.number, (index + 1) * sizeof(*ni.number));
	}

	/* Something went wrong. */
	yaml_assert(ni.number, "Error gathering information for number.\n")

	return ni;
}

static uint8 *get_word(_lexer *lex)
{
	uint8 *word = calloc(1, sizeof(*word));
	size index = 0;

	while(lex->curr_char != '"' && lex->curr_char != ':' && lex->curr_char != '\n' && lex->curr_char != '\t' && lex->curr_char != ' ')
	{
		word[index] = lex->curr_char;

		advance_lexer(lex);

		/* Reallocate memory for next character. */
		index++;
		word = realloc(word, (index + 1) * sizeof(*word));
	}

	/* Something went wrong. */
	yaml_assert(word, "Error gathering information for user-defined word.\n")

	return word;
}

static void init_token(_token *t, enum tokens id, uint8 *token_value)
{
	t->id		= id;
	t->token_value 	= token_value;
}

void get_token(_token *input_token, _lexer *lex)
{
begin:
	if(lex->index >= strlen(lex->src))
	{
		init_token(input_token, eof, "\0");
		return;
	}

	if(is_number(lex->curr_char))
	{
		_number_info ni = get_number(lex);

		init_token(input_token, ni.is_hex ? hex : number, ni.number);
		goto end;
	}

	switch(lex->curr_char)
	{
		case '#': {
			while(lex->curr_char != '\n') advance_lexer(lex);
			advance_lexer(lex);
			goto begin;
		}
		case '\n': {
			while(lex->curr_char == '\n')
			{
				lex->line++;
				advance_lexer(lex);
			}
			goto begin;
		}
		case '\t': {
			while(lex->curr_char == '\t') advance_lexer(lex);
			goto begin;
		}
		case ' ': {
			while(lex->curr_char == ' ') advance_lexer(lex);
			goto begin;
		}
		case ':': {
			init_token(input_token, colon, ":");
			advance_lexer(lex);
			goto end;	
		}
		case '"': {
			advance_lexer(lex);		// "
			uint8 *str = get_word(lex);	// string
			advance_lexer(lex);		// "
			
			init_token(input_token, string, str);
			
			advance_lexer(lex);
			goto end;
		}
		case '\'': {
			advance_lexer(lex);						// '
			uint8 c[2] = {lex->curr_char, '\0'};	// char
			advance_lexer(lex);						// '

			init_token(input_token, character, c);

			advance_lexer(lex);
			goto end;
		}
		case '[': {
			init_token(input_token, lsqrbr, "[");
			advance_lexer(lex);
			goto end;
		}
		case ']': {
			init_token(input_token, rsqrbr, "]");
			advance_lexer(lex);
			goto end;
		}
		case ',': {
			init_token(input_token, comma, ",");
			advance_lexer(lex);
			goto end;
		}
		default: break;
	}

	yaml_assert(is_ascii(lex->curr_char), "Error on line %ld:\n\tInvalid value `%c`(%x).\n", lex->line, lex->curr_char,lex->curr_char);

	/* Get user defined value. */
	uint8 *user_def_ = get_word(lex);
	init_token(input_token, user_def, user_def_);

end:
	return;	
}

#ifndef yaml_lexer
#define yaml_lexer

#ifndef yaml_common
#include "common.h"
#endif

/* Yaml lexer, storing the current character, index, size, whole file. */
typedef struct lexer
{
	/* All the source code. */
	uint8		*src;

	/* Current index we're at in `src`. */
	size		index;

	/* Current character. */
	uint8		curr_char;

	/* Extra, but keep track of the line for possible errors in parser. */
	size		line;
} _lexer;

/* All possible tokens. */
enum tokens {
	/* Very common, lol. */
	user_def,
	colon,
	lsqrbr,
	rsqrbr,
	string,
	number,
	hex,
	character,
	comma,
	eof
};

/* Token information. */

typedef struct token
{
	enum tokens	id;
	uint8		*token_value;
} _token;

/* Init a single instance of a _token that we reference. */
_token *init_token_reference();

/* Initialize the lexer. */
_lexer *init_lexer(const uint8 *filename);

/* Get token. */
void get_token(_token *input_token, _lexer *lex);

/* Some thigs that'll help us. */
#define is_ascii(x) (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')
#define is_number(x) (x >= '0' && x <= '9')
#define is_hex_dec(x) ((x >= 'A' && x <= 'F') || (x >= 'a' && x <= 'f'))

#endif

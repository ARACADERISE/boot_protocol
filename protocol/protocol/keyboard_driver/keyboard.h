#ifndef protocol_keyboard
#define protocol_keyboard

/* All possible scancodes to expect. */
enum kb_scancodes {
	nothing			= 0x0,
	SC_escape 		= 0x1,
	SC_one			= 0x2,
	SC_two			= 0x3,
	SC_three		= 0x4,
	SC_four			= 0x5,
	SC_five			= 0x6,
	SC_six			= 0x7,
	SC_seven		= 0x8,
	SC_eight		= 0x9,
	SC_nine			= 0xA,
	SC_zero			= 0xB,
	SC_minus		= 0xC,
	SC_equal		= 0xD,
	SC_backspace	= 0xE,
	SC_tab			= 0xF,
	SC_Q			= 0x10,
	SC_W			= 0x11,
	SC_E			= 0x12,
	SC_R			= 0x13,
	SC_T			= 0x14,
	SC_Y			= 0x15,
	SC_U			= 0x16,
	SC_I			= 0x17,
	SC_O			= 0x18,
	SC_P			= 0x19,
	SC_LSB			= 0x1A, // Left Square Bracket(LSB)
	SC_RSB			= 0x1B, // Right Square Bracket(RSB)
	SC_enter		= 0x1C,
	SC_lctr			= 0x1D, // Left Control(lctr)
	SC_A 			= 0x1E,
	SC_S			= 0x1F,
	SC_D			= 0x20,
	SC_F			= 0x21,
	SC_G			= 0x22,
	SC_H			= 0x23,
	SC_J			= 0x24,
	SC_K			= 0x25,
	SC_L			= 0x26,
	SC_semi			= 0x27, // Semicolon(;)
	SC_sqt			= 0x28, // Single Quote(sqt)
	SC_btck			= 0x29, // Backtick(btck)
	SC_lshft		= 0x2A, // Left Shift(lshft)
	SC_fslsh		= 0x2B, // Foward Slash(fslsh)
	SC_Z			= 0x2C,
	SC_X			= 0x2D,
	SC_C 			= 0x2E,
	SC_V 			= 0x2F,
	SC_B			= 0x30,
	SC_N 			= 0x31,
	SC_M 			= 0x32,
	SC_comma		= 0x33,
	SC_period		= 0x34,
	SC_bslsh		= 0x35, // Back Slash(bslsh)
	SC_rshft		= 0x36, // Right Shift(rshft)
	SC_kpd_ast		= 0x37, // Key Pad Asterisk(kpd_ast)
	SC_lalt			= 0x38, // Left Alt(lalt)
	SC_space 		= 0x39,
	SC_capslock		= 0x3A,
	SC_F1 			= 0x3B,
	SC_F2			= 0x3C,
	SC_F3			= 0x3D,
	SC_F4			= 0x3E,
	SC_F5			= 0x3F,
	SC_F6			= 0x40,
	SC_F7			= 0x41,
	SC_F8			= 0x42,
	SC_F9			= 0x43,
	SC_F10			= 0x44,
	SC_nlck			= 0x45, // NumberLock(nlck)
	SC_slck			= 0x46, // Scroll Lock(slck)
	SC_kpd_seven	= 0x47, // Keypad seven(kpd_seven)
	SC_kpd_eight	= 0x48, // Keypad eight(kpd_eight)
	SC_kpg_nine		= 0x49, // Keypad nine(kpd_nine)
	SC_kpd_minus	= 0x4A, // Keypad minus(kpd_minus)
	SC_kpd_four		= 0x4B, // Keypad four(kpd_four)
	SC_kpd_five		= 0x4C, // Keypad five(kpd_five)
	SC_kpd_six		= 0x4D, // Keypad six(kpd_six)
	SC_kpd_plus		= 0x4E, // Keypad plus(kpd_plus)
	SC_kpd_one		= 0x4F, // Keypad one(kpd_one)
	SC_kpd_two		= 0x50, // Keypad two(kpd_two)
	SC_kpd_three	= 0x51, // Keypad three(kpd_three)
	SC_kpd_zero		= 0x52, // Keypad zero(kpd_zero)
	SC_kpd_period	= 0x53, // Keypad period(kpd_period)
	SC_F11			= 0x57
};

/* All possible scancode values.
 * Any index with 0 means the scancode has no functionality yet.
 */
const uint8 scancode_keys[] = {
	0, // Nothing
	0, // ESC
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=',
	0, // backspace
	0, // tab
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i',
	'o', 'p', '[', ']',
	0, // enter
	0, // left control
	'a', 's', 'd', 'f',
	'g', 'h', 'j', 'k',
	'l', ';', '\'', '`',
	0, // left shift
	'\\', 'z', 'x', 'c',
	'v', 'b', 'n', 'm',
	',', '.', '/',
	0, // right shift
	'*', // on keypad
	0, // left alt
	' ', // space
	0, // capslock
	0, // F1
	0, // F2
	0, // F3
	0, // F4
	0, // F5
	0, // F6
	0, // F7
	0, // F8
	0, // F9
	0, // F10
	0, // numberlock
	0, // scroll lock
	0, // 7 on keypad
	0, // 8 on keypad
	0, // 9 on keypad
	0, // - on keypad
	0, // 4 on keypad
	0, // 5 on keypad
	0, // 6 on keypad
	0, // + on keypad
	0, // 1 on keypad
	0, // 2 on keypad
	0, // 3 on keypad
	0, // 0 on keypad
	0, // . on keypad
	0, // F11
};

/* All possible release scancodes to expect. */
const uint8 release_scancodes[] =
{
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92,
	0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
	0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
	0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD,
	0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6,
	0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBE, 0xBF, 0xC0,
	0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9,
	0xCA, 0xCB, 0xCC, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3,
	0xC4, 0xD5, 0xD6, 0xD7
};

/* Variable to keep track on whether `shift` is still being pressed or not. */
static bool shift_pressed = false;

/* Check if the scancode is a release scancode. */
uint8 check_key_is_released(uint8 scancode)
{
	uint8 ret = scancode;
	for(uint8 i = 0; i < sizeof(release_scancodes)/sizeof(release_scancodes[0]); i++)
	{
		if(scancode == release_scancodes[i]) ret = 0;
	}
	return ret;
}

void clear_tb()
{
	uint8 tb = 0;

	while(1)
	{
		tb = __inp(0x64);
		if(tb & 1) break;
	}
}

/* Get a scancode from the keyboard, return appropriate ASCII/numeral value. */
uint8 get_key() 
{
	uint8 scancode	= 0;
	uint8 char_val 	= 0;
	uint8 *shifts   = (uint8 *)")!@#$%^&*(";

	redo:
	clear_tb();

	scancode = __inp(0x60);

    /* Check the scancode and perform accordingly. */
	check_scancode:
	switch(scancode)
	{
        case SC_enter: return '\n';
		case SC_rshft:
		{
			if(shift_pressed == true) return 0;
			goto get_shift_key;
		}
		case SC_space: return ' ';
		case SC_lshft:
		{
			get_shift_key:

			/* The shift key is pressed until the release scancode is read. */
			shift_pressed = true;

			/* Clear the TB. */
			clear_tb();

			/* Get the scancode. */
			scancode = __inp(0x60);

			/* Basic checks. */
			if(scancode == 0xAA)
			{
				shift_pressed = false;
				return 0;
			}
			if(check_key_is_released(scancode) == 0) return 0;

			/* Get the ASCII value. */
			char_val = (uint8)scancode_keys[scancode];

			/* Check ASCII value to be a number. */
			if(char_val >= '0' && char_val <= '9')
				return shifts[char_val-0x30];
			
			/* Make sure it is a valid ASCII value. Otherwise, check again the scancode. */
			if(!(char_val >= 'A' && char_val <= 'Z'))
				goto check_scancode;

			return (uint8)scancode_keys[scancode] - 0x20;
		}
		/* Backslash. If shift is pressed, return question mark. */
		case 0x35:
		{
			if(shift_pressed) return '?';
			return '/';
		}
		/* `;`. If shift is pressed, return `:`. */
		case 0x27:
		{
			if(shift_pressed == true) return ':';
			return ';';
		}
		case 0x29:
		{
			if(shift_pressed == true) return '~';
			else return '`';
		}
		/* Keypad number 8 pressed. No support for keypad yet. */
		case 0x48: return 0;
		/* Right shift key released. */
		case 0xB6:shift_pressed = false;return 0;
		/* Left shift key released. */
		case 0xAA:shift_pressed = false;return 0;
		case 0x0: return '\0';
		default: break;
	}

	if(scancode == 0) return '\0';
    
	if(check_key_is_released(scancode) == 0) return 0;

	if(shift_pressed == true)
	{
		if(scancode_keys[scancode] >= '0' && scancode_keys[scancode] <= '9')
		{
			switch(scancode_keys[scancode])
			{
				case '1': return '!';
				case '2': return '@';
				case '3': return '#';
				case '4': return '$';
				case '5': return '%';
				case '6': return '^';
				case '7': return '&';
				case '8': return '*';
				case '9': return '(';
				case '0': return ')';
				default: return 0;
			}
		}

		/* Return the scancode if the value at the given index is not a numeral value, not an
		 * ASCII value and if it is zero.
		 */
		if(!(scancode_keys[scancode] >= '0' && scancode_keys[scancode] <= '9') && 
		   !(scancode_keys[scancode] >= 'a' && scancode_keys[scancode] <= 'z') &&
		   scancode_keys[scancode] == 0)
			return scancode;

		return (uint8)scancode_keys[scancode] - 0x20;
	}

	/* Return the scancode if the value at the given index is not a numeral value, not an
	 * ASCII value and if it is zero.
	 */
	if(!(scancode_keys[scancode] >= '0' && scancode_keys[scancode] <= '9') && 
	   !(scancode_keys[scancode] >= 'a' && scancode_keys[scancode] <= 'z') &&
	   scancode_keys[scancode] == 0)
		return scancode;

	return (uint8)scancode_keys[scancode];
}

static uint8 user_input = 0;
static uint8 input_buffer[80];

void input(uint8 *display_message, bool just_get_character)
{
	if(display_message != NULL)
		print(display_message);
	
	/* Just make sure it's zero. */
	user_input = 0;

	memsetb(input_buffer, 0, 80);

	if(just_get_character == false)
	{
		uint16 ind = 0;
		while(user_input != '\n')
		{
			user_input = get_key();
			if(user_input == 0) continue;
			put_char(user_input);

			input_buffer[ind] = user_input;
			ind++;
		}
		input_buffer[ind - 1] = '\0';
		return;
	}

	while(user_input == 0) user_input = get_key();
}

#endif
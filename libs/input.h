/*
 *	FILE: input.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Creates multiple console input reading functions.
 *		Also gives a function capable of detecting special
 *		keyboard input which can then call user-specified
 *		handlers.
 */

#include <unistd.h>
#include <termios.h>

#define BCKSPC 		127
#define SHFT_BCKSPC 8
#define CH_UP		65
#define CH_DOWN		66
#define CH_RIGHT	67
#define CH_LEFT		68
#define CH_DELETE   '~'
#define CH_TAB		'\t'
#define CH_RETURN	'\n'
#define ESCAPE		27
#define ESC_SEQ_1	ESCAPE
#define ESC_SEQ_2	91

/*
 *  Reads a single character from stdin in a nonblocking fashion.  
 *  The character does not appear in stdout.
 *
 *  PARAM:
 *      read - Pointer to place the read character at.
 *
 *  RETURN:
 *     -1 if no character is read
 *      1 if a special character was read
 *      0 otherwise
 */
char getch_spec(char* read);
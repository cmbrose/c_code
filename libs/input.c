/*
 *	FILE: input.c
 *	AUTHOR: Caleb Brose
 *
 *	USE: Creates multiple console input reading functions.
 *		Also gives a function capable of detecting special
 *		keyboard input which can then call user-specified
 *		handlers.
 */

#include "input.h"

//----------------------------------------------------
char getch_spec(char* c)
{
    char buf = 0, is_spec = 0;
    struct termios old = {0};
    tcgetattr(0, &old);
    
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    
    tcsetattr(0, TCSANOW, &old);

    if (read(0, &buf, 1) < 0)
        return -1;

	// Try to read a special character
    if ( buf == ESCAPE )
    {
        is_spec = 1;

        old.c_cc[VMIN] = 0;

        tcsetattr(0, TCSANOW, &old);
        while ( read(0, &buf, 1 ) > 0 );
    }
    
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    
    tcsetattr(0, TCSADRAIN, &old);
    
    *c = buf;
    return is_spec;
}
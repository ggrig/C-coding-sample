#include "helpers.h"

bool rand_from_range(unsigned int * random, unsigned int from, unsigned int to)
{
	if (from >= to)
		return false;

	unsigned int generated;
	if (0 != rand_s(&generated)) //error
		return false;

	*random = generated % (to - from + 1) + from;
	return true;
}

COORD getCursorPos()
{
    COORD pos;
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hcon,&con)) 
    {
        pos.X = con.dwCursorPosition.X;
        pos.Y = con.dwCursorPosition.Y;
    }
    return pos;
}

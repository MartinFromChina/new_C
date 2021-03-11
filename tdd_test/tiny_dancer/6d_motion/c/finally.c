#include <stdio.h>

static unsigned char i = 0;
unsigned char TestFunction(void)
{
    i = i + 2;
	return i;
}



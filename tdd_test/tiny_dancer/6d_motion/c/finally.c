#include <stdio.h>

static unsigned char i = 0;
unsigned char TestFunction(void)
{
    i = i + 1;
	//printf("hello \r\n");
	return i;
}



#include <stdio.h>

static unsigned char i = 0;
unsigned char TestFunction(void)
{
    i = i + 1;
	//printf("hello \r\n");
	return i;
}

unsigned char GetX_Xita(void)
{
	return (i%360);
}
unsigned char GetY_Xita(void)
{
	return (i%360);
}
unsigned char GetZ_Xita(void)
{
	return (i%360);
}




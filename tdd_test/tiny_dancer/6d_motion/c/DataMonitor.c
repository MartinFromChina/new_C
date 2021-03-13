#include "DataMonitor.h"
#include <stdio.h>

static char float_char_buf[33];
char * IntConvertFloatChar(FixedPointNumber src)
{
	uint8_t i;
	double t;
	for(i=0;i<33;i++)
	{
		float_char_buf[i] = ' ';
	}
	t = src;
	sprintf(float_char_buf,"%.2f",t);
	return float_char_buf;
}

/*
double AnotherIntConvertFloatChar(FixedPointNumber src)
{
	double t = *(double*)src;
	return t;
}
*/

#include <stdio.h>
#include <math.h>

#define PI 3.141592653

int main(void)
{
	double angle = 30.00;
	printf("sin(%.2lf)=%.2lf \r\n",angle,sin(angle*PI/180));
	return 0;
}

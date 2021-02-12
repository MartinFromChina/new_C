#include "normal_distribution.h"
#include "uniform_distribution.h"

X_Float64 GaussGenerator(X_Float64 mean,X_Float64 sigma,int *seed)
{
	uint8_t i;
	X_Float64 x,y;
	for(i=0,x=0;i<12;i++)
	{
		x += UniformGenerater(0.0,1.0,seed);
	}
	x = x - 6.0;
	y = mean + (x*sigma);
	return y;
}

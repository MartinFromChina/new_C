#include "uniform_distribution.h"

X_Float64 UniformGenerater(X_Float64 low_limit,X_Float64 up_limit,int *seed)
{
	X_Float64 t;
 	*seed = 2045 * (*seed) + 1;
	*seed = (*seed) - ((*seed) / 1048576) * 1048576;
	t = (*seed) / 1048576.0;
	t = low_limit + (up_limit - low_limit) * t;
	return t;
}

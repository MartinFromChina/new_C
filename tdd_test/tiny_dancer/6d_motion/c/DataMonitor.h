#ifndef __DATAMONITOR_H
#define __DATAMONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../module/common/x_cross_platform.h"
#include "DataConvert.h"

char * IntConvertFloatChar(FixedPointNumber src);
double AnotherIntConvertFloatChar(FixedPointNumber src);

#ifdef __cplusplus
	}
#endif

#endif

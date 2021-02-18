#ifndef __PENTIP_PRESSURE_H
#define __PENTIP_PRESSURE_H

#ifdef __cplusplus
	extern "C"{
#endif

#define SAMPLE_CNT   (0xff)


#include "../../../module/common/x_cross_platform.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"

typedef uint16_t (*GetRawPressure)(X_Void);

X_Boolean PentipInit(GetRawPressure p_get_raw);


#ifdef __cplusplus
		}
#endif


#endif

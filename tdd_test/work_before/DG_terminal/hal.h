#ifndef __HAL_H
#define __HAL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_common.h"

X_Void HAL_BasicSet(X_Void);
X_Void HAL_Run(X_Void);
uint32_t GetSysTime(X_Void);


#ifdef __cplusplus
		}
#endif


#endif



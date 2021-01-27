#ifndef __HAL_H
#define __HAL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_common.h"
#include "../node_interconnect/node_interconnect.h" 

X_Void HAL_BasicSet(X_Void);
X_Void HAL_Run(X_Void);
uint32_t GetSysTime(X_Void);
X_Boolean SendWaveSetForTestModule(uint8_t node_num,uint32_t sent_time,uint8_t *p_buf,uint8_t length,e_direction direct,uint16_t distance);


#ifdef __cplusplus
		}
#endif


#endif



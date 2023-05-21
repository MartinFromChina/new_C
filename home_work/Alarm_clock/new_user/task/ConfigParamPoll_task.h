#ifndef __CONFIGPARAMPOLL_TASK_H
#define __CONFIGPARAMPOLL_TASK_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "LogDebugSwitch.h"
        
X_Void time_up_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void time_down_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);

#ifdef __cplusplus
		}
#endif

#endif

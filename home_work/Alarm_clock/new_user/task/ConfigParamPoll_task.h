#ifndef __CONFIGPARAMPOLL_TASK_H
#define __CONFIGPARAMPOLL_TASK_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "LogDebugSwitch.h"
        
X_Void distance_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void line_num_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void device_num_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void air_speed_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void channel_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void frame_length_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);

#ifdef __cplusplus
		}
#endif

#endif

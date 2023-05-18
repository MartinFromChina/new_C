#ifndef __DELAYEXTTASK_H
#define __DELAYEXTTASK_H

#ifdef __cplusplus
	extern "C"{
#endif

#if (USE_TDD_MOCK != 0)
	#include "../../../test/tdd_test_common.h"
#else
	#include "LogDebugSwitch.h"
#endif
        
typedef X_Void (*delay_todo)(X_Void);
        
X_Void delay_todo_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Void DelayTodoReg(delay_todo todo_func,uint32_t delay_ms);

#ifdef __cplusplus
		}
#endif

#endif

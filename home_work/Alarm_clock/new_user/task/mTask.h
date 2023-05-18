#ifndef __MTASK_H
#define __MTASK_H

#ifdef __cplusplus
	extern "C"{
#endif
        
#include "../../../../module/component/const_loop_scheduler/const_loop_scheduler.h"
#include "LogDebugSwitch.h"
        
#if (USE_TDD_MOCK != 0)
	#include "../../../test/tdd_test_common.h"
#else
//////	#include "LogDebugSwitch.h"
#endif
        
#define ENABLE_REMOTE_TEST      1  // relay lora 
#define ENABLE_LOCAL_TEST       0  // near lora 
        
#if (ENABLE_REMOTE_TEST != 0 && ENABLE_LOCAL_TEST != 0)
   #error [cannot support two mode test at the same time ]
#endif


#define LOCAL_TERMINAL_NUM    0xFFFFEEEE
        
typedef enum
{
    TID_MainTask = 1,
    TID_ButtonTask,
    TID_MaxTaskID_Plus_1,
}e_TaskID;

X_Void TaskInit(X_Void);
X_Void TaskLoopExe(X_Void);

X_Boolean common_task_freeze(uint16_t id);
X_Boolean common_task_restart(uint16_t id);

#ifdef __cplusplus
		}
#endif

#endif


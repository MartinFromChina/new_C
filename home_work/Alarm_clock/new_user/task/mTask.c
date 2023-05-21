#include "mTask.h"
#include "ButtonTask.h"
#include "mApp_MainTask.h"
#include "TestModeTask.h"

extern uint32_t mFunc_SoftTimerGetSysTickCnt(void);
extern X_Boolean ClassInit(X_Void);
extern X_Void ClassRun(uint32_t ms);
extern X_Void LogDebugInit(X_Void);

static X_Boolean isClassInitOK = X_False;
uint32_t TerminalNumberGet(X_Void)
{
    return LOCAL_TERMINAL_NUM;
}

static const s_X_TaskList task_list[] = {
  {TID_MainTask,            main_task},
  {TID_ButtonTask,          button_task},
  {TID_TestMode,            testmode_task},
};

X_TASK_SCHEDULER_DEF(task_manager,sizeof(task_list)/sizeof(task_list[0]),task_list);

X_Void TaskInit(X_Void)
{
    LogDebugInit();
    INSERT(LogDebug)(1,(" hello world \r\n"));
    SysLogDebug(1,(" this is an alarm clock  \r\n"));
	if(x_task_init(task_manager) != xte_ok)
	{
		SysLogDebug(1,(" !!! fatal error , task init failed \r\n"));
	}
    
////////    for(uint8_t i = TID_MainTask ;i < TID_MaxTaskID_Plus_1;i++)
////////    {
////////        common_task_freeze(i);
////////    }
////////    common_task_restart(TID_MainTask);
//////    common_task_restart(TID_ButtonTask);
    
    isClassInitOK = ClassInit();
}


X_Void TaskLoopExe(X_Void)
{
    uint32_t ms = mFunc_SoftTimerGetSysTickCnt();
	x_task_scheduler(task_manager,ms);
    if(isClassInitOK == X_True)
    {
        ClassRun(ms);
    }
}

X_Boolean common_task_freeze(uint16_t id)
{
	return (freeze_x_task(task_manager,id) == xte_ok);
}
X_Boolean common_task_restart(uint16_t id)
{
	return (restart_x_task(task_manager,id)== xte_ok);
}

#include "sys.h"
X_Void mDri_MCU_Reset(X_Void)
{
        __disable_fault_irq();      // STM32 ииик?бф??  
        NVIC_SystemReset();
}

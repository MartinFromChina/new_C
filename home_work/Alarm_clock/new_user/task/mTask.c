#include "mTask.h"
#include "ButtonTask.h"
////////#include "mApp_MainTask.h"
////////#include "loraTask.h"
////////#include "DelayExtTask.h"
////////#include "ButtonTask.h"
////////#include "ConfigParamPoll_task.h"

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
  {TID_MainTask,            X_Null},
////////////  {TID_MainTask,            main_task},
////////////  {TID_LoraTask,            lora_task},
  {TID_ButtonTask,          button_task},
////////////  {TID_DelayTask,           delay_todo_task},
////////////  {TID_TestDestTask,        addr_task},
////////////  
////////////  {TID_TestPeriodTask,      speed_task},
//////////  {TID_DistanceTask,        distance_task},
//////////  {TID_LineNumTask,         line_num_task},
//////////  {TID_DeviceNumTask,       device_num_task},
//////////  {TID_AirSpeed,            air_speed_task},
//////////  {TID_channel,             channel_task},
//////////  {TID_frame_length,        frame_length_task},
};

X_TASK_SCHEDULER_DEF(task_manager,sizeof(task_list)/sizeof(task_list[0]),task_list);

X_Void TaskInit(X_Void)
{
    uint8_t i;
    LogDebugInit();
    INSERT(LogDebug)(1,(" hello world \r\n"));
	if(x_task_init(task_manager) != xte_ok)
	{
		SysLogDebug(1,(" !!! fatal error , task init failed \r\n"));
	}
    
    for(i = TID_MainTask ;i < TID_MaxTaskID_Plus_1;i++)
    {
        common_task_freeze(i);
    }
    common_task_restart(TID_MainTask);
    
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

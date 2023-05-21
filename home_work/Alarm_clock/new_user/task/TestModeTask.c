#include "TestModeTask.h"
#include "mTask.h"
#include "const_loop_scheduler.h"

#include "audioplay.h"	
#include "../../../../module/customize/TestModeConsole/mTestMode.h"

static const sTestModeParam* p_test;
    
#define ACTION_FREQ_IN_MS                   10
/*************************************************************************/
#include "refresh_task_tag.h"

static X_Void testmode_task_init(uint32_t ms)
{
    if(mTestModeInit(p_test) != X_True)
	{
		SysLogDebug(1,("@ test mode module init failed  %d \r\n",ms));
	}
    mDri_TestModeReceiveBufClear();
}

TASK_BODY_BEGIN(testmode_task,testmode_task_init,0,0){
   SeggerRttLoopRead();
   mTestModeLoopReceive(p_test);
   X_delay(ACTION_FREQ_IN_MS);
   X_do(mTestModeHandle(p_test));
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/









static const sTmCommandAnalysis new_command_analysis[] = {
/***************************test rountine begin*******************************/

/***************************test rountine end*******************************/
	{X_True,   "RUNALL",					6,	6,	TM_RUNALL_todo},
	{X_False,  "ENTER",					    5,	5,	TM_ENTER_todo},
	{X_False,  "EXIT",						4,	4,	TM_EXIT_todo},
////	{X_False,  "PWRON",           TM_LENGTH_DONT_CARE,  5,  PWRON_todo},
};

/***********************************************************************************************
***********************************************************************************************
***********************************************************************************************/
static X_Void EnterTestModeCb(X_Void){}
static X_Void ClearTestModeCb(X_Void){}
static X_Void KillAllCb(X_Void){}

APP_TEST_MODE_MODULE_DEFINE(p_test,ACTION_FREQ_IN_MS,
								mDri_TestModeGetByteInterface,mDri_TestModeSentBufInterface,//mDri_TestModeSentBufInterface,
								new_command_analysis,(sizeof(new_command_analysis)/sizeof(new_command_analysis[0])),
								460,3,
								EnterTestModeCb,ClearTestModeCb,1200000,
								KillAllCb);

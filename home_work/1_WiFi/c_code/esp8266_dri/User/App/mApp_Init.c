#include "mApp_Init.h"
#include "mApp_TestMode.h"

#include "../Drive/mDrive.h"
#include "../Func/mFunc.h"
#include "../Module/mModule.h"
#include  "mModule_button.h"

X_Void mAppInit(X_Void)
{
	mDriInit();
	mFuncInit();
	mModuleInit();
	mApp_TM_Init();
}

//////INSERT(LOG_COUNTER_ENTRY_DEF)(p_cnt,0);

X_Void CommonHandle(X_Void)
{
//////	INSERT(LogDebugCounter)(SYS_INFO_DEBUG,p_cnt,3000,("-- main loop alive \r\n"));
	mModule_ButtonActionHandle();
	mApp_TM_Handle();
}
X_Void FastHandle(X_Void)
{
	mApp_TM_LoopRecv();
	SeggerRttLoopRead();
}

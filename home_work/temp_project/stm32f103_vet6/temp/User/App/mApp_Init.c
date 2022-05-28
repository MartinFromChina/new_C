#include "mApp_Init.h"

#include "../Drive/mDrive.h"
#include "../Func/mFunc.h"
#include "../Module/mModule.h"

X_Void mAppInit(X_Void)
{
	mDriInit();
	mFuncInit();
	mModuleInit();
}

INSERT(LOG_COUNTER_ENTRY_DEF)(p_cnt,0);

X_Void CommonHandle(X_Void)
{
	INSERT(LogDebugCounter)(SYS_INFO_DEBUG,p_cnt,100,("-- main loop alive \r\n"));
}
X_Void FastHandle(X_Void)
{

}

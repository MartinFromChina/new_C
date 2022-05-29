#include "mFunc.h"
#include "mFunc_SoftTimer.h"
#include "mFunc_Button.h"
#include "mFunc_TM_Interface.h"

X_Void mFuncInit(X_Void)
{
	mFun_TM_Init();
	mFunc_SoftTimerInit();
	mFunc_ButtonInit();
}

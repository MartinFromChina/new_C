#include "mFunc.h"
#include "mFunc_SoftTimer.h"
#include "mFunc_Button.h"

X_Void mFuncInit(X_Void)
{
	mFunc_SoftTimerInit();
	mFunc_ButtonInit();
}

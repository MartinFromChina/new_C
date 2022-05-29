#ifndef __MAPP_TESTMODE_H
#define __MAPP_TESTMODE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"

X_Void mApp_TM_Init(X_Void);
X_Void mApp_TM_Handle(X_Void);
X_Void mApp_TM_LoopRecv(X_Void);

#ifdef __cplusplus
		}
#endif

#endif

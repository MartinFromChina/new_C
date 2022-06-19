#ifndef __MMODULE_H
#define __MMODULE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"
typedef enum
{
	MEC_InProcess,
	MEC_Finished,
	MEC_Timeout,
	MEC_ErrorOccur,
}eModuleErrorCode;

typedef enum
{
	MID_RTC = 0,
	MID_ETH,
}eModuleID;

typedef X_Void (*evt_reg)(eModuleErrorCode err_code,uint8_t cur_module_or_func,uint8_t current_state);
X_Void mModuleInit(X_Void);
X_Boolean mModule_DoesInTestMode(X_Void);

X_Void mModule_SetTestMode(X_Void);
X_Void mModule_ClearTestMode(X_Void(*special_clear)(X_Void));

#ifdef __cplusplus
		}
#endif

#endif

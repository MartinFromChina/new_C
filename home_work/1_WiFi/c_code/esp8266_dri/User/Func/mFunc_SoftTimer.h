#ifndef __MFUNC_SOFTTIMER_H
#define __MFUNC_SOFTTIMER_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"
		
typedef enum
{
	STID_Rtc = 0,
	STID_Bc95 , // for bc 95 module
	STID_Bc95_normal,
	STID_Bc95_Scan , // bc 95 scan data from cloud every 500 ms 
	STID_Bc95_Recv , // for bc95 scan receive func
	STID_Bc95_Response,
	STID_Gps ,
	STID_ETH , 
	STID_ETH_Recv,   // for eth specific receive
	STID_ETH_Config, // for wify module config process
	STID_ExtFlashDataLoad,
////////	STID_MAX,
}eSoftTimerID;

/************************************************************
code below is meanningless , because the enum type value is set in compiled period
but the marco check is done in precompiled period
#if ((STID_MAX) >= SOFT_TIMER_NUMBER)    
	#error [SoftTimerID beyond scope ]
#endif
************************************************************/
X_Void mFunc_SoftTimerInit(X_Void);
uint32_t mFunc_SoftTimerGetSysRunTime(X_Void);
uint32_t mFunc_SoftTimerGetSysTickCnt(X_Void);
X_Boolean mFunc_SoftTimerSetValue(uint8_t id,uint32_t value);
uint32_t 	mFunc_SoftTimerGetValue(uint8_t id);
X_Void mFunc_SoftTimerDelayMs(uint32_t ms);

#ifdef __cplusplus
		}
#endif

#endif

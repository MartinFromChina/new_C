#ifndef __MDRI_SOFTTIMER_H
#define __MDRI_SOFTTIMER_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"
		
X_Void mDri_SoftTimerInit( X_Void (*systic_cb)(X_Void));
X_Void mDri_SoftTimerHandle(X_Void);
X_Void mDri_SoftTimerDelayMs(uint32_t ms);
X_Void mDri_SoftTimerDelayUs(uint32_t Us);
#ifdef __cplusplus
		}
#endif

#endif

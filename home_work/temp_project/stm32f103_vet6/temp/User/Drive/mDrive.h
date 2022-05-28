#ifndef __MDRIVE_H
#define __MDRIVE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"
#include "stm32f1xx_hal.h"
		
#define ENABLE_ISR          __set_PRIMASK(0)  
#define DISABLE_ISR 				__set_PRIMASK(1)
X_Void mDriInit(X_Void);
		
#ifdef __cplusplus
		}
#endif

#endif

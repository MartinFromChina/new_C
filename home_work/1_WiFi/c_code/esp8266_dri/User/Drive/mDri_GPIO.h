#ifndef __MDRI_GPIO_H
#define __MDRI_GPIO_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"

X_Void mDri_IO_ClockEnable(X_Void);
X_Void mDri_ButtonIO_Init(X_Void);
uint8_t mDri_GetButtonsValue(X_Void);

#ifdef __cplusplus
		}
#endif

#endif

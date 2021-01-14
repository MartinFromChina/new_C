#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"

#define BATTERY_MONITOR_CALL_FREQUENCT_IN_MS       (8)   // every 8 ms , the mModule_BatteryStrengthMonitor is called once

uint8_t mModule_GetBatteryStrength(X_Void);
X_Void mModule_BatteryStrengthMonitor(X_Void);


#if (USE_TDD_MOCK != 0)
uint16_t ForTest(X_Void);
#endif

#ifdef __cplusplus
	}
#endif

#endif
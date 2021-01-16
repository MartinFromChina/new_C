#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"

#define BATTERY_MONITOR_CALL_FREQUENCT_IN_MS       (8)   // every 8 ms , the mModule_BatteryStrengthMonitor is called once
typedef uint16_t (*onBatteryAdc)(X_Void);

X_Void mModule_BatteryInit(onBatteryAdc get_adc);
uint8_t mModule_GetBatteryStrength(X_Void);
X_Boolean mModule_BatteryStrengthMonitor(X_Boolean isInCharge);

#if (USE_TDD_MOCK != 0)
	X_Void mModule_BatteryDeInit(X_Void);
	uint8_t BatterySmooth_ForTest(uint8_t current,uint8_t back_up,uint8_t threshold);
	uint8_t ConvAdcToPercentage_ForTest(uint16_t adc);
#endif

#ifdef __cplusplus
	}
#endif

#endif
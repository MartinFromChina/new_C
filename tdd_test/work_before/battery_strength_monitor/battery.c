#include "battery.h"

#if (USE_TDD_MOCK != 0)
#include "./test/test_main.h"

extern Adc_Mock adcm;
uint16_t ForTest(X_Void)
{
	return MOCKABLE(GetBatteryAdcValue)();
}

extern uint32_t MOCKABLE(GetCurrentTime)(X_Void);

#endif


static uint8_t CurrentBatteryStrength = 0;

static uint16_t GetBatteryAdcValue(X_Void)
{
	return 0;
}

X_Void mModule_BatteryStrengthMonitor(X_Void)
{
	uint32_t time =  MOCKABLE(GetCurrentTime)();
	if(time <= CONV_MS_TO_TICKS(10000))
	{
		if((time % CONV_MS_TO_TICKS(100)) == 0) 
		{
			MOCKABLE(GetBatteryAdcValue)();
		}
	}
	else
	{
		if((time % CONV_MS_TO_TICKS(30000)) == 0)	
		{
			MOCKABLE(GetBatteryAdcValue)();
		}
	}
	
}

uint8_t mModule_GetBatteryStrength(X_Void)
{
	return CurrentBatteryStrength;
}
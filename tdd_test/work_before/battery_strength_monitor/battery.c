#include "battery.h"

static uint8_t CurrentBatteryStrength = 0;
static onBatteryAdc adc_value_get = (onBatteryAdc)0;

#if (USE_TDD_MOCK != 0)
#include "./test/test_main.h"

extern Adc_Mock adcm;
X_Void mModule_BatteryDeInit(X_Void)
{
	adc_value_get = (onBatteryAdc)0;
}
#endif

static uint16_t GetBatteryAdcValue(X_Void)
{
	return 0;
}
X_Void mModule_BatteryInit(onBatteryAdc getadc)
{
	adc_value_get = getadc;
}
X_Boolean mModule_BatteryStrengthMonitor(X_Void)
{
	X_Boolean isBatteryStateUpdate = X_False;
	uint32_t time =  MOCKABLE(GetCurrentTime)();
	if(adc_value_get == X_Null) {return isBatteryStateUpdate;}
	if(time <= CONV_MS_TO_TICKS(10000))
	{
		if((time % CONV_MS_TO_TICKS(100)) == 0) 
		{
			adc_value_get();
		}
	}
	else
	{
		if((time % CONV_MS_TO_TICKS(30000)) == 0)	
		{
			adc_value_get();
		}
	}

	return isBatteryStateUpdate;
}

uint8_t mModule_GetBatteryStrength(X_Void)
{
	return CurrentBatteryStrength;
}
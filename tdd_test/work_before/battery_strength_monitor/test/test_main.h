#ifndef  __TEST_MAIN_H
#define  __TEST_MAIN_H

#include "tdd_common.h"
#include "../battery.h"

class Adc_Mock{
	public:	
	MOCK_METHOD0(ExpectCurrentIndex,	uint8_t());
};

uint16_t mockable_GetBatteryAdcValue(X_Void);

#endif
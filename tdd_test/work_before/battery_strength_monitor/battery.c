#include "battery.h"
#include "../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../google_test/include/include/gmock/gmock.h"
#include "./test/test_main.h"

extern Adc_Mock adcm;

static uint16_t GetBatteryAdcValue(X_Void)
{
	return 0;
}
uint16_t ForTest(X_Void)
{
	return MOCKABLE(GetBatteryAdcValue)();
}


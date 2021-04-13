#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../LedDirectDisplay.h"


#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

/*

#if (SEGGER_RTT_DEBUG == 0)
	#ifdef DEBUG 
		#error [will take the risk that marco "DEBUG" cause hardfault in product !!!]
	#endif
#else 
	#ifndef DEBUG 
		#warning [will miss more detail when fatal error occur if disable marco "DEBUG"]
	#endif
#endif

*/
typedef X_Boolean(*does_power_on)(X_Void);

static uint16_t init_cnt = 0,draw_cnt = 0,off_cnt = 0;
static X_Boolean current_led_on = X_False;
static uint32_t CurrentColor = 0,global_timer = 0;
static uint8_t debug_flag = 0;


static X_Void test_init(X_Void)
{
	init_cnt = 0;
	draw_cnt = 0; 
	off_cnt = 0;
	current_led_on = X_False;
	CurrentColor = 0;
	debug_flag = 0;
	global_timer = 0;
}
X_Void mockable_LedInit(X_Void)
{
	init_cnt ++; 
}

X_Void mockable_LedOn(X_Void)
{

}

X_Void mockable_LedOff(X_Void)
{
	
}
X_Boolean mockable_DoesLedOn(X_Void)
{
	return current_led_on;
}


TEST(Led,init)
{
	APP_LED_DIRECT_DISPLAY_MODULE_DEF(p_led,MOCKABLE(LedInit),MOCKABLE(LedOn),MOCKABLE(LedOff),MOCKABLE(DoesLedOn),10);
	//X_Boolean isOK;
	test_init();
	
	//isOK = LedDisplayEventRegister(p_led,&led_event);
	//EXPECT_EQ(isOK,X_False);
	
	LedDirectDisplayInit(p_led);
	/*

	EXPECT_EQ(init_cnt,1);

	isOK = LedDisplayEventRegister(p_led,&led_event);
	EXPECT_EQ(isOK,X_True);
	*/

}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------led_display_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "Protocol_recv.find_headers_0";
  
  return RUN_ALL_TESTS();
}

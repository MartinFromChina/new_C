#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../LedDisplay.h"


#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
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
static uint16_t init_cnt = 0,draw_cnt = 0,off_cnt = 0;
static X_Boolean current_led_on = X_False;

static X_Void test_init(X_Void)
{
	init_cnt = 0;
	draw_cnt = 0; 
	off_cnt = 0;
	current_led_on = X_False;
}
X_Void mockable_LedInit(X_Void)
{
	init_cnt ++; 
}
X_Void mockable_LedDraw(uint32_t color)
{
	draw_cnt ++;
	color = color;
	current_led_on = X_True;
}

X_Void mockable_LedOff(X_Void)
{
	off_cnt++;
	current_led_on = X_False;
}

X_Void mockable_PowerApply(X_Void)
{

}


#define HANDLE_FREQUENCY  20
APP_LED_DISPLAY_MODULE_DEF(p_led,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),10,HANDLE_FREQUENCY);

static sLedDisplayEvent led_event;
TEST(Led,init)
{
	X_Boolean isOK;
	test_init();
	
	isOK = LedDisplayEventRegister(p_led,&led_event);
	EXPECT_EQ(isOK,X_False);
	
	LedDisplayInit(p_led);

	EXPECT_EQ(init_cnt,1);

	isOK = LedDisplayEventRegister(p_led,&led_event);
	EXPECT_EQ(isOK,X_True);
}

static sLedDisplayEvent blink_event1 = {
	LedBlink,
	{
		COLOR_RGB_Blue,
		100,
		100,
		1
	},
};
TEST(Led,sample_blink)
{
	uint32_t timer_cnt = 0;
	led_event.event_mode 	= LedBlink;
	led_event.param.color   = COLOR_RGB_Blue;
	led_event.param.led_off_time   = 300;
	led_event.param.led_on_time    = 500;
	led_event.param.on_off_cycle   = 5;


	LedDisplayEventRegister(p_led,&led_event);

	while(timer_cnt < 5000)
	{
		LedDisplayHandle(p_led);
		timer_cnt = timer_cnt + HANDLE_FREQUENCY;
		switch(timer_cnt)
		{
			case 0:
				//EXPECT_EQ(isOK,X_True);
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
	}

	
}


TEST(Led,disable_enable)
{

}

TEST(Led,disable_enable_immediately)
{

}



TEST(Led,on_off)
{

}

TEST(Led,mul_entry)
{

}



GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------led_display_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "Protocol_recv.find_headers_0";
  return RUN_ALL_TESTS();
}

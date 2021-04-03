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
static uint32_t CurrentColor = COLOR_RGB_Black,global_timer = 0;
static uint8_t debug_flag = 0;


static X_Void test_init(X_Void)
{
	init_cnt = 0;
	draw_cnt = 0; 
	off_cnt = 0;
	current_led_on = X_False;
	CurrentColor = COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black);
	debug_flag = 0;
	global_timer = 0;
}
X_Void mockable_LedInit(X_Void)
{
	init_cnt ++; 
}
X_Void mockable_LedDraw(uint32_t color)
{
	draw_cnt ++;
	CurrentColor = COLOR_WITH_FULL_TRANSPORT(color);
	current_led_on = X_True;
}

X_Void mockable_LedOff(X_Void)
{
	off_cnt++;
	current_led_on = X_False;
	CurrentColor = COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black);
}

X_Void mockable_PowerApply(uint32_t time_in_ms)
{
	time_in_ms = time_in_ms;
}
X_Void mockable_PowerRelease(X_Void){}
X_Boolean mockable_DoesPowerOn(X_Void)
{
	return X_True;
}

uint8_t mockable_GetDebugFlag(X_Void)
{
	return debug_flag;
}
uint32_t mockable_GetCurrentTime(X_Void)
{
	return global_timer;
}

#define HANDLE_FREQUENCY  20
#define POWER_SETUP_IN_MS   0



static sLedDisplayEvent led_event = {
	LedBlink,
	{
		COLOR_RGB_Blue,
		100,
		200,
		100,
	},
};
TEST(Led,init)
{
	APP_LED_DISPLAY_MODULE_DEF(p_led,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),MOCKABLE(DoesPowerOn),10,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
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
		
TEST(Led,simple_blink)
{
	APP_LED_DISPLAY_MODULE_DEF(p_led1,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),(does_power_on)0,10,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
	uint32_t timer_cnt = 0;
	X_Boolean isOK;

	test_init();
	led_event.event_mode 	= LedBlink;
	led_event.param.color   = COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Blue);
	led_event.param.led_off_time   = 300;
	led_event.param.led_on_time    = 500;
	led_event.param.on_off_cycle   = 5;

	isOK = LedDisplayEventRegister(p_led1,&blink_event1);
	EXPECT_EQ(isOK,X_False);
		
	LedDisplayInit(p_led1);
	isOK = LedDisplayEventRegister(p_led1,&led_event);
	EXPECT_EQ(isOK,X_True);

	while(timer_cnt < 5000)   
	{
		LedDisplayHandle(p_led1);
		timer_cnt = timer_cnt + HANDLE_FREQUENCY;
		switch(timer_cnt)
		{
			case HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			case (HANDLE_FREQUENCY*10):
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Blue));
				break;
			case (HANDLE_FREQUENCY*6) + 500 :
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			default:
				break;
		}
	}
	EXPECT_EQ(draw_cnt,5);
	EXPECT_EQ(off_cnt,5);

	
}

TEST(Led,event_param)
{
	sLedDisplayEvent temp_event = {
		LedBlink,
		{
			COLOR_RGB_Blue,
			100,
			100,
			1
		},
	};
	APP_LED_DISPLAY_MODULE_DEF(p_led4,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),(does_power_on)0,5,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
	X_Boolean isOK;
	test_init();
	LedDisplayInit(p_led4);

	temp_event.event_mode = LedBlink;
	temp_event.param.color = COLOR_RGB_Red;
	temp_event.param.on_off_cycle = 0;
	isOK = LedDisplayEventRegister(p_led4,&temp_event);
	EXPECT_EQ(isOK,X_False);

	temp_event.event_mode = LedBlink;
	temp_event.param.color = COLOR_RGB_Red;
	temp_event.param.led_on_time = 0;
	temp_event.param.on_off_cycle = 100;
	isOK = LedDisplayEventRegister(p_led4,&temp_event);
	EXPECT_EQ(isOK,X_False);

	temp_event.event_mode = LedBlink;
	temp_event.param.color = LD_COLOR_OFF;
	temp_event.param.led_on_time = 50;
	isOK = LedDisplayEventRegister(p_led4,&temp_event);
	EXPECT_EQ(isOK,X_False);

	temp_event.event_mode = LedBlink;
	temp_event.param.color = COLOR_RGB_Red;
	temp_event.param.led_on_time = 2;
	temp_event.param.led_off_time = 50;
	temp_event.param.on_off_cycle = 1;
	isOK = LedDisplayEventRegister(p_led4,&temp_event);
	EXPECT_EQ(isOK,X_False);

	temp_event.event_mode = LedBlink;
	temp_event.param.color = COLOR_RGB_Black;
	temp_event.param.led_on_time = 20;
	temp_event.param.led_off_time = 50;
	temp_event.param.on_off_cycle = 1;
	isOK = LedDisplayEventRegister(p_led4,&temp_event);
	EXPECT_EQ(isOK,X_False);

}

TEST(Led,mul_blink)
{
    sLedDisplayEvent temp_event;
	APP_LED_DISPLAY_MODULE_DEF(p_led3,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),(does_power_on)0,5,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
	X_Boolean isOK;
	test_init();
	debug_flag = 0;
	LedDisplayInit(p_led3);

	temp_event.event_mode  = LedBlink;

	while(global_timer < 15000)   
	{
		LedDisplayHandle(p_led3);
		global_timer = global_timer + HANDLE_FREQUENCY;

		switch(global_timer)
		{
			case HANDLE_FREQUENCY:
				temp_event.param.color = COLOR_RGB_Red;
				temp_event.param.led_on_time = 78;
				temp_event.param.led_off_time = 64;
				temp_event.param.on_off_cycle = 1;
				isOK = LedDisplayEventRegister(p_led3,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			case 500:
				temp_event.param.color = COLOR_RGB_DarkRed;
				temp_event.param.led_on_time = 126;
				temp_event.param.led_off_time = 398;
				temp_event.param.on_off_cycle = 5;
				isOK = LedDisplayEventRegister(p_led3,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			case 500 + HANDLE_FREQUENCY:
				temp_event.param.color = COLOR_RGB_Yellow;
				temp_event.param.led_on_time = 2000;
				temp_event.param.led_off_time = 50;
				temp_event.param.on_off_cycle = 3;
				isOK = LedDisplayEventRegister(p_led3,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;				
			case 2000 :
				temp_event.param.color = COLOR_RGB_YellowGreen;
				temp_event.param.led_on_time = 250;
				temp_event.param.led_off_time = 1800;
				temp_event.param.on_off_cycle = 1;
				isOK = LedDisplayEventRegister(p_led3,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			default:
				break;
		}
		
		switch(global_timer)
		{
			case 60 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Red));
				break;
			case 140 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			case 2160 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_DarkRed));
				break;
			case 5400 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Yellow));
				break;
			case 7420 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			default:
				break;
		}
		
	}
	EXPECT_EQ(draw_cnt,10);
	EXPECT_EQ(off_cnt,10);

}


TEST(Led,mul_blink_full)
{
	APP_LED_DISPLAY_MODULE_DEF(p_led2,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),(does_power_on)0,5,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
	X_Boolean isOK;

	LedDisplayInit(p_led2);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_True);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_True);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_True);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_True);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_True);
	isOK = LedDisplayEventRegister(p_led2,&blink_event1);
	EXPECT_EQ(isOK,X_False);
}
TEST(Led,fast_mul_blink)
{
	

}

TEST(Led,disable_enable_immediately)
{

}

TEST(Led,on_off)
{
	sLedDisplayEvent temp_event;
	APP_LED_DISPLAY_MODULE_DEF(p_led4,MOCKABLE(LedInit),MOCKABLE(LedDraw),MOCKABLE(LedOff),MOCKABLE(PowerApply),MOCKABLE(PowerRelease),(does_power_on)0,5,HANDLE_FREQUENCY,POWER_SETUP_IN_MS);
	X_Boolean isOK;
	test_init();
	debug_flag = 0;
	LedDisplayInit(p_led4);

	while(global_timer < 15000)   
	{
		LedDisplayHandle(p_led4);
		global_timer = global_timer + HANDLE_FREQUENCY;

		switch(global_timer)
		{
			case HANDLE_FREQUENCY:
				temp_event.event_mode  = LedBlink;
				temp_event.param.color = COLOR_RGB_Red;
				temp_event.param.led_on_time = 78;
				temp_event.param.led_off_time = 64;
				temp_event.param.on_off_cycle = 1;
				isOK = LedDisplayEventRegister(p_led4,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			case 500:
				temp_event.event_mode  = LedHoldOn;
				temp_event.param.color = COLOR_RGB_DarkRed;
				temp_event.param.led_on_time = 126;
				temp_event.param.led_off_time = 398;
				temp_event.param.on_off_cycle = 5;
				isOK = LedDisplayEventRegister(p_led4,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			case 500 + HANDLE_FREQUENCY:
				temp_event.event_mode  = LedBlink;
				temp_event.param.color = COLOR_RGB_Yellow;
				temp_event.param.led_on_time = 2000;
				temp_event.param.led_off_time = 50;
				temp_event.param.on_off_cycle = 3;
				isOK = LedDisplayEventRegister(p_led4,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;				
			case 2000 :
				temp_event.event_mode  = LedHoldOn;
				temp_event.param.color = COLOR_RGB_YellowGreen;
				temp_event.param.led_on_time = 250;
				temp_event.param.led_off_time = 1800;
				temp_event.param.on_off_cycle = 1;
				isOK = LedDisplayEventRegister(p_led4,&temp_event);
				EXPECT_EQ(isOK,X_True);
				break;
			default:
				break;
		}
		/*
		switch(global_timer)
		{
			case 60 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Red));
				break;
			case 140 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			case 2160 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_DarkRed));
				break;
			case 5400 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Yellow));
				break;
			case 7420 + HANDLE_FREQUENCY:
				EXPECT_EQ(CurrentColor,COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black));
				break;
			default:
				break;
		}
		*/
		
	}
	//EXPECT_EQ(draw_cnt,10);
	//EXPECT_EQ(off_cnt,10);
}

TEST(Led,on_off_recover)
{

}

TEST(Led,power_apply)
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

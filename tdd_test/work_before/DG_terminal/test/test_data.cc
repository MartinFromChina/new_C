#include "test_data.h"
#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "../DG_protocol.h"
#include "test_common.h"


#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include "../hal.h"

//#define DG_TERMINAL_LOST_TYPE 		0x88

static X_Boolean LoadCheckSum(uint8_t *p_buf,uint8_t length)
{
	uint8_t i;
	uint8_t sum = 0;
	if(length == 0 || length == 255) {return X_False;}
	for(i=0;i<length - 1;i++)
	{
		sum += p_buf[i];
	}
	p_buf[length - 1] = sum;
	return X_True;
}
uint8_t GenerateBasicInfSet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t terminal,
								uint8_t forward,uint8_t double_forward,uint8_t backward,uint8_t double_backward)
{
	static s_DG_basic_info_set bis,*p;
	p = &bis;
	
	bis.common.header = 0x55aa;
	bis.common.length = sizeof(bis)/sizeof(uint8_t);
	bis.common.src    = src;
	bis.common.dest   = dest;
	bis.common.type   = BASIC_INFO_SET_TYPE;
	
	bis.terminal	  				= terminal;
	bis.double_forward_terminal 	= double_forward;
	bis.forward_terminal			= forward;
	bis.backward_terminal			= backward;
	bis.double_backward_terminal	= double_backward;
	bis.reserve						= 0;
	
	LoadCheckSum((uint8_t *)p,sizeof(bis)/sizeof(uint8_t));

	
	*p_buf =  (uint8_t *)(p);
	return sizeof(bis)/sizeof(uint8_t);
}

uint8_t GenerateBasicInfGet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t terminal)
{
	static s_DG_basic_info_get big,*p;
	p = &big;
	big.common.header = 0x55aa;
	big.common.length = sizeof(big)/sizeof(uint8_t);
	big.common.src    = src;
	big.common.dest   = dest;
	big.common.type   = BASIC_INFO_GET_TYPE;
	big.terminal      = terminal;
	LoadCheckSum((uint8_t *)p,sizeof(big)/sizeof(uint8_t));
	*p_buf = (uint8_t *)(p);
	return sizeof(big)/sizeof(uint8_t);
}

uint8_t GenerateRTC_Config(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t   start_terminal,
									uint16_t  year,uint8_t   mounth,uint8_t   day,uint8_t   hour,uint8_t   minute,uint8_t   second)
{
	static s_DG_RTC_config rtc,*p;
	p = &rtc;
	
	rtc.common.header = 0x55aa;
	rtc.common.length = sizeof(rtc)/sizeof(uint8_t);
	rtc.common.src    = src;
	rtc.common.dest   = dest;
	rtc.common.type   = RTC_CONFIG_TYPE;
	
	rtc.start_terminal= start_terminal;
	rtc.year		  = year;
	rtc.mounth		  = mounth;
	rtc.day			  = day;
	rtc.hour		  = hour;
	rtc.minute		  = minute;
	rtc.second		  = second;
	LoadCheckSum((uint8_t *)p,sizeof(rtc)/sizeof(uint8_t));
	*p_buf = (uint8_t *)(p);
	return sizeof(rtc)/sizeof(uint8_t);
}

uint8_t GenerateInfoMulGet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t   start_terminal)
{
	static s_DG_info_mul_get mget,*p;
	p = &mget;
	mget.common.header = 0x55aa;
	mget.common.length = sizeof(mget)/sizeof(uint8_t);
	mget.common.src    = src;
	mget.common.dest   = dest;
	mget.common.type   = DG_INFO_MUL_GET_TYPE;
	mget.start_terminal =  start_terminal;
	mget.reserve        = 0;
	LoadCheckSum((uint8_t *)p,sizeof(mget)/sizeof(uint8_t));
	*p_buf = (uint8_t *)(p);
	return sizeof(mget)/sizeof(uint8_t);
}
/*
typedef struct
{
	s_DG_data_common common;
	uint8_t   lost_terminal;
	uint8_t   check_sum;
}s_DG_terminal_lost;
*/



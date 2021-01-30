#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "test_data.h"

#include <iostream>
#include "../hal.h"
#include "test_common.h"

using namespace std;
static uint8_t table_index = 0;
static const s_monitor_table table1 []=  {
/***********************************************/
	{
		X_True,
		2,
		{0xaa,0x55, 9, 1, 9,0x55, 4, 0,0x6b}, // used string compare maybe more suitable
		9,
		11,
	},
	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0x55,0x91}, // used string compare maybe more suitable
		9,
		11,
	},
	{
		X_False,
		2,
		{0xaa,0x55, 9, 2, 9,0x55, 4, 0,0x6c}, // used string compare maybe more suitable
		9,
		11,
	},
	/***********************************************/
	{X_False,0xff,{0}, 0,0,}, // ignore
	/***********************************************/
	{
		X_True,
		3,
		{0xaa,0x55, 9, 2, 9,0x55, 4, 0,0x6c}, // used string compare maybe more suitable
		9,
		20,
	},
	{
		X_False,
		3,
		{0xcc,0x66, 9, 0, 3, 2,0xfe,0x55,0x93}, // used string compare maybe more suitable
		9,
		20,
	},
	{
		X_False,
		3,
		{0xaa,0x55, 9, 3, 9,0x55, 4, 0,0x6d}, // used string compare maybe more suitable
		9,
		20,
	},
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0x55,0x91}, // used string compare maybe more suitable
		9,
		22,
	},
	/***********************************************/
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	/***********************************************/
	{
		X_True,
		4,
		{0xaa,0x55, 9, 3, 9,0x55, 4, 0,0x6d}, // used string compare maybe more suitable
		9,
		25,
	},
	{
		X_False,
		4,
		{0xcc,0x66, 9, 0, 4, 3,0xfe,0x55,0x95}, // used string compare maybe more suitable
		9,
		25,
	},
	{
		X_False,
		4,
		{0xaa,0x55, 9, 4, 9,0x55, 4, 0,0x6e}, // used string compare maybe more suitable
		9,
		25,
	},
	/***********************************************/
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	/***********************************************/
	{
		X_False,
		9,
		{0xcc,0x66, 0xc, 0, 9, 8,0x55, 4, 0, 0, 0,0xa8,}, // used string compare maybe more suitable
		12,
		62,
	},
	/***********************************************/
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	/***********************************************/
	{
		X_False,
		4,
		{0xcc,0x66,0x1b, 0, 4, 3,0x55, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xad}, // used string compare maybe more suitable
		27,
		99,
	},
	/***********************************************/	
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66,0x1b, 0, 2, 1,0x55, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xa9}, // used string compare maybe more suitable
		27,
		124,
	},
	/***********************************************/
	{X_False,0xff,{0}, 0,0,}, // ignore
	{X_False,0xff,{0}, 0,0,}, // ignore
};

static X_Void data_monitor_1(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if(table1[table_index].current_node_num != 0xff)
	{
	
		EXPECT_EQ(table1[table_index].isRecv, isRecv);
		EXPECT_EQ(table1[table_index].current_node_num, current_node_num);
		EXPECT_EQ(table1[table_index].time, time);
		EXPECT_EQ(table1[table_index].length, length);

		for(i=0;i<length;i++)
		{
			EXPECT_EQ(table1[table_index].data[i], p_data[i]);
		}
	}
	else 
	{
		INSERT(LogDebug)(0,("ignore it !!!\r\n"));
	}
	if((table_index + 1) < (  (uint8_t)(  sizeof(table1)/sizeof(table1[0])      )     )) {table_index ++;}
	else {table_index = 0;}
	
}


TEST(multicast,start_1_get_from_4_9)
{
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet(1);
	TestCommonInit(data_monitor_1);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,1,9,4);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

TEST(multicast,start_8_get_from_5_2)
{
}

TEST(multicast,start_8_get_from_8_2)
{
}

TEST(multicast,start_8_get_from_2_2)
{
}

TEST(multicast,start_5_get_from_6_8)
{
}





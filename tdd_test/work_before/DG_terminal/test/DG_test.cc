#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "test_data.h"
#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include <iostream>
#include "../hal.h"
#include "test_common.h"


using namespace std;

TEST(DG,init)
{	
	uint8_t *p_data,length,error_data[6] = {0xaa,0x55,06,0,0,05};
	X_Boolean isOK;
	HAL_BasicSet();
	DisableLogDebug();
	length = GenerateBasicInfSet(&p_data,0xff,1,1,0,0,2,3);
	isOK = SendWaveSetForTestModule(5,0,p_data,length,ED_bidirection,50);
	EXPECT_EQ(isOK, X_True);
	
	length = GenerateBasicInfGet(&p_data,0xff,1,1);
	isOK = SendWaveSetForTestModule(5,200,p_data,length,ED_bidirection,9);
	EXPECT_EQ(isOK, X_True);

	isOK = SendWaveSetForTestModule(5,250,error_data,6,ED_bidirection,9); // length bigger than 5 , if not the fun_find_other will not take it
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
}

static uint8_t table_index= 0;
typedef struct 
{
	X_Boolean isRecv;
	uint8_t current_node_num;
	uint8_t data[15];
	uint16_t length;
	uint32_t time;
}s_monitor_table;

static const s_monitor_table table1 []=  {
	{
		X_True,
		2,
		{0xaa,0x55, 0xd, 0x1, 0x2,0xf0, 2, 0, 1, 3, 4, 0, 9,0,0},
		13,
		11,
	},
	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0xf0,0x2c,0,0,0,0,0,0},
		9,
		11,
	},
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0xf0,0x2c,0,0,0,0,0,0},
		9,
		22,
	},

};
X_Void data_monitor_1(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if(current_node_num != 3 && current_node_num != 4)
	{
	
	EXPECT_EQ(table1[table_index].isRecv, isRecv);
	EXPECT_EQ(table1[table_index].current_node_num, current_node_num);
	EXPECT_EQ(table1[table_index].time, time);
	EXPECT_EQ(table1[table_index].length, length);

	for(i=0;i<length;i++)
	{
		EXPECT_EQ(table1[table_index].data[i], p_data[i]);
	}
	
	if((table_index + 1) < (  (uint8_t)(  sizeof(table1)/sizeof(table1[0])      )     )) {table_index ++;}
	else {table_index = 0;}
	}
}

TEST(DG,1_to_2_basic_set)
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet();
	TestCommonInit(data_monitor_1);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,1,2,2,1,0,3,4);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,20);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static const s_monitor_table table2 []=  {
/***********************************************/
	{
		X_True,
		2,
		{0xaa,0x55, 0xd, 0x1, 0x3,0xf0, 3, 1, 2, 4, 5, 0, 15,0,0},
		13,
		11,
	},
	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0xf0,0x2c,0,0,0,0,0,0},
		9,
		11,
	},
	{
		X_False,
		2,
		{0xaa,0x55, 0xd, 0x2, 0x3,0xf0, 3, 1, 2, 4, 5, 0, 16,0,0},
		13,
		11,
	},
	/***********************************************/
	{
		X_True,
		3,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0xf0,0x2c,0,0,0,0,0,0},
		9,
		20,
	},
	{
		X_True,
		3,
		{0xaa,0x55, 0xd, 0x2, 0x3,0xf0, 3, 1, 2, 4, 5, 0, 16,0,0},
		13,
		20,
	},
	{
		X_False,
		3,
		{0xcc,0x66, 9, 0, 3, 2,0xfe,0xf0,0x2e,0,0,0,0,0,0},
		9,
		20,
	},
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 2, 1,0xfe,0xf0,0x2c,0,0,0,0,0,0},
		9,
		22,
	},
	{
		X_True,
		1,
		{0xaa,0x55, 0xd, 0x2, 0x3,0xf0, 3, 1, 2, 4, 5, 0, 16,0,0},
		13,
		22,
	},
	/***********************************************/
	{
		X_True,
		2,
		{0xcc,0x66, 9, 0, 3, 2,0xfe,0xf0,0x2e,0,0,0,0,0,0},
		9,
		29,
	},
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 3, 2,0xfe,0xf0,0x2e,0,0,0,0,0,0},
		9,
		40,
	},
};

X_Void data_monitor_2(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if( current_node_num != 4 && current_node_num != 5 && current_node_num != 6)
	{
	
	EXPECT_EQ(table2[table_index].isRecv, isRecv);
	EXPECT_EQ(table2[table_index].current_node_num, current_node_num);
	EXPECT_EQ(table2[table_index].time, time);
	EXPECT_EQ(table2[table_index].length, length);

	for(i=0;i<length;i++)
	{
		EXPECT_EQ(table2[table_index].data[i], p_data[i]);
	}
	
	if((table_index + 1) < (  (uint8_t)(  sizeof(table2)/sizeof(table2[0])      )     )) {table_index ++;}
	else {table_index = 0;}
	}
}

TEST(DG,1_to_3_terminal_2_trans_down)
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet();
	TestCommonInit(data_monitor_2);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,1,3,3,2,1,4,5);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);

	//SetTemporaryDistance(8); // so that the terminal 2's ack can not reach terminal 1; which cause an ack timeout for terminal one
	
	HAL_Run();
	TestCommonDeInit();
}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------DG_mul_terminal_test from DG_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "trans.init";
  return RUN_ALL_TESTS();
}




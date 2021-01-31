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
	/*******************************************************/
	{	
		X_False,
		2,
		{0xaa,0x55, 0xd, 0x2, 0x3,0xf0, 3, 1, 2, 4, 5, 0, 16,0,0},
		13,
		34,
	},
	/*******************************************************/
	{	
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 1,0x88, 3,0xc9},
		9,
		57,
	},
};

static X_Void data_monitor_1(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if( current_node_num != 4 && current_node_num != 5 && current_node_num != 6)
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
	if(isRecv == X_False && current_node_num == 3 && p_data[0] == 0xcc)
	{
		SetTemporaryDistance(8); // so that the terminal 3's ack can not reach terminal 2; which cause an ack timeout for terminal 1
	}
}

TEST(DG_ack,3_to_2_ack_timeout1)
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet(1);
	TestCommonInit(data_monitor_1,(p_monitor_list_init)0);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,1,3,3,2,1,4,5);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}


static const s_monitor_table table2 []=  {
	/***********************************************/
	{
		X_True,
		3,
		{0xaa,0x55, 0xd, 0x4, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 7,0,0},
		13,
		5,
	},
	{
		X_False,
		3,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		5,
	},
	{
		X_False,
		3,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		5,
	},
	/***********************************************/
	{
		X_True,
		4,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		10,
	},
	{
		X_True,
		4,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		10,
	},
	/***********************************************/
	{
		X_True,
		2,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		14,
	},
	{
		X_True,
		2,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		14,
	},
	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		14,
	},
	{
		X_False,
		2,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		14,
	},
	/***********************************************/
	{
		X_True,
		3,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		23,
	},
	{
		X_True,
		3,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		23,
	},
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		25,
	},
	{
		X_True,
		1,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		25,
	},
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		25,
	},
	{
		X_True,
		1,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		25,
	},
	/***********************************************/
	{
		X_False,
		1,
		{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},
		9,
		25,
	},
	/***********************************************/

		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
};

static X_Void data_monitor_2(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if( current_node_num != 7 && current_node_num != 5 && current_node_num != 6 && table2[table_index].current_node_num != 0xff)
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

TEST(DG_ack,4_to_1_ack)
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet(4);
	TestCommonInit(data_monitor_2,(p_monitor_list_init)0);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,4,1,1,0,0,2,3);
	isOK = SendWaveSetForTestModule(4,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static const s_monitor_table table3 []=  {
	/***********************************************/
	{
		X_True,
		3,
		{0xaa,0x55, 0xd, 0x4, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 7,0,0},
		13,
		5,
	},
	{
		X_False,
		3,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		5,
	},
	{
		X_False,
		3,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		5,
	},
	/***********************************************/
	{
		X_True,
		4,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		10,
	},
	{
		X_True,
		4,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		10,
	},
	/***********************************************/
	{
		X_True,
		2,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		14,
	},
	{
		X_True,
		2,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		14,
	},
	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		14,
	},
	{
		X_False,
		2,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		14,
	},
	/***********************************************/
	{
		X_True,
		3,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		23,
	},
	{
		X_True,
		3,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		23,
	},
	/***********************************************/
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 3, 4,0xfe,0xf0,0x30,},
		9,
		25,
	},
	{
		X_True,
		1,
		{0xaa,0x55, 0xd, 0x3, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 6,0,0},
		13,
		25,
	},
	{
		X_True,
		1,
		{0xcc,0x66, 9, 0, 2, 3,0xfe,0xf0,0x2e,},
		9,
		25,
	},
	{
		X_True,
		1,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		25,
	},
	/***********************************************/
	{
		X_False,
		1,
		{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},
		9,
		25,
	},
	/***********************************************/

		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
	/***********************************************/
	{
		X_False,
		2,
		{0xaa,0x55, 0xd, 0x2, 0x1,0xf0, 1, 0, 0, 2, 3, 0, 5,0,0},
		13,
		37,
	},
	/***********************************************/
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
	/***********************************************/

	{
		X_False,
		2,
		{0xcc,0x66, 9, 0, 2, 3,0x88,1,0xc9,},
		9,
		60,
	},
	/***********************************************/
	{
		X_True,
		3,
		{0xcc,0x66, 9, 0, 2, 3,0x88,1,0xc9,},
		9,
		69,
	},
	{
		X_False,
		3,
		{0xcc,0x66, 9, 0, 3, 4,0x88,1,0xcb,},
		9,
		69,
	},
	
	/***********************************************/
	{
		X_True,
		4,
		{0xcc,0x66, 9, 0, 3, 4,0x88,1,0xcb,},
		9,
		74,
	},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},
		{X_False,0xff,{0xcc,0x66, 9, 0, 1, 2,0xfe,0xf0,0x2c,},9,25,},			

};

static X_Void data_monitor_3(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if( current_node_num != 7 && current_node_num != 5 && current_node_num != 6 && table3[table_index].current_node_num != 0xff)
	{
	
		EXPECT_EQ(table3[table_index].isRecv, isRecv);
		EXPECT_EQ(table3[table_index].current_node_num, current_node_num);
		EXPECT_EQ(table3[table_index].time, time);
		EXPECT_EQ(table3[table_index].length, length);

		for(i=0;i<length;i++)
		{
			EXPECT_EQ(table3[table_index].data[i], p_data[i]);
		}

		if(isRecv == X_True && current_node_num == 1 && p_data[0] == 0xaa && p_data[3] == 2 && p_data[4] == 1)
		{
			SetTemporaryDistance(10); // so that the terminal 1's ack can not reach terminal 2; which cause an ack timeout for terminal 2
		}
	}

	if(table3[table_index].current_node_num == 0xff)
	{
		INSERT(LogDebug)(0,("nore it !!!\r\n"));
	}

	if(current_node_num != 7 && current_node_num != 5 && current_node_num != 6 )
	{
		if((table_index + 1) < (  (uint8_t)(  sizeof(table3)/sizeof(table3[0])      )     )) {table_index ++;}
		else {table_index = 0;}
	}
	
}

TEST(DG_ack,4_to_1_ack_terminal1_lost)
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet(4);
	TestCommonInit(data_monitor_3,(p_monitor_list_init)0);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,4,1,1,0,0,2,3);
	isOK = SendWaveSetForTestModule(4,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}



static X_Void data_monitor_4(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;

	if( current_node_num != 7 && current_node_num != 5 && current_node_num != 6 && table3[table_index].current_node_num != 0xff)
	{
	
		//PECT_EQ(table3[table_index].isRecv, isRecv);
		//PECT_EQ(table3[table_index].current_node_num, current_node_num);
		//PECT_EQ(table3[table_index].time, time);
		//PECT_EQ(table3[table_index].length, length);

		for(i=0;i<length;i++)
		{
			//PECT_EQ(table3[table_index].data[i], p_data[i]);
		}

		if(isRecv == X_True && current_node_num == 4 && p_data[0] == 0xaa && p_data[3] == 3 && p_data[4] == 4)
		{
			SetTemporaryDistance(2); // so that the terminal 1's ack can not reach terminal 2; which cause an ack timeout for terminal 2
		}
		else
		{
			if(time >= 26) {SetTemporaryDistance(11);}
		}
	}

	if(table3[table_index].current_node_num == 0xff)
	{
		INSERT(LogDebug)(0,("nore it !!!\r\n"));
	}

	if(current_node_num != 7 && current_node_num != 5 && current_node_num != 6 )
	{
		if((table_index + 1) < (  (uint8_t)(  sizeof(table3)/sizeof(table3[0])      )     )) {table_index ++;}
		else {table_index = 0;}
	}
	
}

TEST(DG_ack,1_to_4_ack_terminal4_lost) // no check ,just see the log by real person
{	
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	HAL_BasicSet(1);
	TestCommonInit(data_monitor_4,(p_monitor_list_init)0);
	DisableLogDebug();// called it after TestCommonInit
	
	length = GenerateBasicInfSet(&p_data,1,4,4,3,2,5,6);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}




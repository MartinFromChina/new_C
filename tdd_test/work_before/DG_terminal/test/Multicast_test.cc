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

TEST(multicast,init)
{


}



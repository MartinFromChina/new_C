#include "node_test.h"
#include <iostream>
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

using namespace std;

static uint16_t node_handle_called_cnt = 0;

static X_Boolean NodeHandle(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	UNUSED_PARAMETER(p_manager);UNUSED_PARAMETER(current_node_num);UNUSED_PARAMETER(p_data);UNUSED_PARAMETER(length);
	node_handle_called_cnt++;
	//EXPECT_EQ(message.wave_num, 8);
	return X_True;
}

static s_node_manager m_node1,m_node2,m_node3;
static s_node         node1,node2,node3;

TEST(node,add1)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit(NodeHandle);

	m_node1.p_node = &node1;
	m_node1.p_node->node_number  			= 1;
	node1.forware_distance                  = 1;

	
	NodeAdd(p_manager,&m_node1);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 0);
	EXPECT_EQ(GetNodeNum(), 1);
	WaveTransDeInit();
}

TEST(node,add2)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit(NodeHandle);

	m_node1.p_node = &node1;
	m_node1.p_node->node_number  			= 1;
	node1.forware_distance					= 1;

	m_node2.p_node = &node2;
	m_node2.p_node->node_number  			= 2;
	node2.forware_distance					= 1;


	
	NodeAdd(p_manager,&m_node1);
	NodeAdd(p_manager,&m_node2);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 0);
	EXPECT_EQ(GetNodeNum(), 2);
	WaveTransDeInit();
}

TEST(node,add3)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit(NodeHandle);

	m_node1.p_node = &node1;
	m_node1.p_node->node_number  			= 1;
	node1.forware_distance					= 1;

	m_node2.p_node = &node2;
	m_node2.p_node->node_number  			= 2;
	node2.forware_distance					= 1;


	m_node3.p_node = &node3;
	m_node3.p_node->node_number  			= 2;
	node3.forware_distance					= 1;

	
	NodeAdd(p_manager,&m_node1);
	NodeAdd(p_manager,&m_node2);
	NodeAdd(p_manager,&m_node3);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt,0);
	EXPECT_EQ(GetNodeNum(), 3);
	WaveTransDeInit();
}


TEST(node,add15)
{
	uint8_t i;
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit(NodeHandle);

	static s_node_manager node_1,node_2,node_3,node_4,node_5,node_6,node_7,node_8,node_9,node_10,node_11,node_12,node_13,node_14,node_15;
	static s_node_manager *node_array[15] = {&node_1,&node_2,&node_3,&node_4,&node_5,&node_6,&node_7,&node_8
											,&node_9,&node_10,&node_11,&node_12,&node_13,&node_14,&node_15};
	node_1.p_node = &node1;
	node1.node_number  			= 1;
	node1.forware_distance      = 1;
	
	node_2.p_node =  &node1;	
	node_3.p_node =  &node1;	
	node_4.p_node =  &node1;	
	node_5.p_node =  &node1;	
	node_6.p_node =  &node1;	
	node_7.p_node =  &node1;	
	node_8.p_node =  &node1;	
	node_9.p_node =  &node1;	
	node_10.p_node =  &node1;	
	node_11.p_node =  &node1;	
	node_12.p_node =  &node1;	
	node_13.p_node =  &node1;	
	node_14.p_node =  &node1;	
	node_15.p_node =  &node1;	
	
	

	for(i=0;i<15;i++)
	{
		NodeAdd(p_manager,node_array[i]);
		//NodeAdd(p_manager,&node_1);
	}

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 0);
	EXPECT_EQ(GetNodeNum(), 15);
	WaveTransDeInit();
}
/*
TEST(node,add15_same)  // can not add the same node_manager 
{
	uint8_t i;
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit(NodeHandle);

	static s_node_manager node_1;

	node_1.p_node = &node1;
	//m_node1.p_node->node_name	  			= "node1"; 
	node_1.p_node->node_number  			= 1;
	node_1.p_node->forware_node		 	= INVALID_NODE_NUM;
	node_1.p_node->backward_node 			= 2;	

	for(i=0;i<15;i++)
	{
		NodeAdd(p_manager,&node_1);
	}

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 15);
	EXPECT_EQ(GetNodeNum(), 15);
	WaveTransDeInit();
}
*/

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------note interconnect_test from node_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "trans.init";
  //testing::FLAGS_gtest_filter = "node.add15";
  return RUN_ALL_TESTS();
}




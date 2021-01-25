#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;

#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include "../node_interconnect.h"

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
static uint16_t node_handle_called_cnt = 0;
static X_Boolean NodeHandle(s_node_handler message)
{
	node_handle_called_cnt++;
	EXPECT_EQ(message.wave_num, 8);
	return X_True;
}

static s_node_manager m_node1,m_node2,m_node3;
static s_node         node1,node2,node3;

TEST(node,add1)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit();

	m_node1.p_node = &node1;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node1.p_node->node_number  			= 1;
	m_node1.p_node->forware_node		 	= INVALID_NODE_NUM;
	m_node1.p_node->backward_node 			= 2;
	m_node1.p_node->node_handle   			= NodeHandle;
	m_node1.p_node->node_message_num 		= 0;
	m_node1.p_node->node_message.wave_num   = 8;

	
	NodeAdd(p_manager,&m_node1);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 1);
	EXPECT_EQ(GetNodeNum(), 1);

}

TEST(node,add2)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit();

	m_node1.p_node = &node1;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node1.p_node->node_number  			= 1;
	m_node1.p_node->forware_node		 	= INVALID_NODE_NUM;
	m_node1.p_node->backward_node 			= 2;
	m_node1.p_node->node_handle   			= NodeHandle;
	m_node1.p_node->node_message_num 		= 0;
	m_node1.p_node->node_message.wave_num   = 8;

	m_node2.p_node = &node2;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node2.p_node->node_number  			= 2;
	m_node2.p_node->forware_node		 	= 1;
	m_node2.p_node->backward_node 			= INVALID_NODE_NUM;
	m_node2.p_node->node_handle   			= NodeHandle;
	m_node2.p_node->node_message_num 		= 0;
	m_node2.p_node->node_message.wave_num   = 8;

	
	NodeAdd(p_manager,&m_node1);
	NodeAdd(p_manager,&m_node2);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 2);
	EXPECT_EQ(GetNodeNum(), 2);

}

TEST(node,add3)
{
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	node_handle_called_cnt = 0;
	p_manager = WaveTransInit();

	m_node1.p_node = &node1;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node1.p_node->node_number  			= 1;
	m_node1.p_node->forware_node		 	= INVALID_NODE_NUM;
	m_node1.p_node->backward_node 			= 2;
	m_node1.p_node->node_handle   			= NodeHandle;
	m_node1.p_node->node_message_num 		= 0;
	m_node1.p_node->node_message.wave_num   = 8;

	m_node2.p_node = &node2;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node2.p_node->node_number  			= 2;
	m_node2.p_node->forware_node		 	= 1;
	m_node2.p_node->backward_node 			= INVALID_NODE_NUM;
	m_node2.p_node->node_handle   			= NodeHandle;
	m_node2.p_node->node_message_num 		= 0;
	m_node2.p_node->node_message.wave_num   = 8;

	m_node3.p_node = &node3;
	//m_node1.p_node->node_name	  			= "node1"; 
	m_node3.p_node->node_number  			= 2;
	m_node3.p_node->forware_node		 	= 1;
	m_node3.p_node->backward_node 			= INVALID_NODE_NUM;
	m_node3.p_node->node_handle   			= NodeHandle;
	m_node3.p_node->node_message_num 		= 0;
	m_node3.p_node->node_message.wave_num   = 8;

	
	NodeAdd(p_manager,&m_node1);
	NodeAdd(p_manager,&m_node2);
	NodeAdd(p_manager,&m_node3);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(node_handle_called_cnt, 3);
	EXPECT_EQ(GetNodeNum(), 3);

}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------note interconnect_test from node_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "Protocol_recv.find_headers_0";
  return RUN_ALL_TESTS();
}




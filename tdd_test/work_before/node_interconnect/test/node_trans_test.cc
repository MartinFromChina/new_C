#include "node_test.h"

static X_Boolean NodeRecvHandle(_s_node *p_node,s_node_handler message)
{

}

TEST(trans,init)
{
	uint8_t i;
	X_Boolean isRun = X_True;
	s_node_manager *p_manager;
	p_manager = WaveTransInit();

	s_node_manager node_1,node_2,node_3;
	s_node         node1,node2,node3;
	node_1.p_node = &node1;
	
	node_1.p_node->node_number  			= 1;
	node_1.p_node->forware_node		 	= INVALID_NODE_NUM;
	node_1.p_node->backward_node 			= 2;
	node_1.p_node->handle   			= NodeRecvHandle;
	node_1.p_node->node_message_num 		= 0;

	s_wave wave ;

	NodeAdd(p_manager,&node_1);

	SendWave(p_manager,GetTime(),1,&wave);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	//EXPECT_EQ(node_handle_called_cnt, 15);
	//EXPECT_EQ(GetNodeNum(), 15);
	WaveTransDeInit();
}


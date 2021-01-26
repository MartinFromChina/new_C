#include "node_test.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

#define USE_DOTTED_LINE_TO_CUT     0
#define NODE_RECV_DEBUG     0

typedef struct
{
	uint8_t node_num;
	uint32_t time;
}s_node_rec_table;

static s_node_rec_table node_recv_table[2] =
	{
		{2,7},
		{3,17},
	};

static uint16_t recv_times = 0;
static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,uint8_t current_node_num)
{
	UNUSED_PARAMETER(p_manager);
	INSERT(LogDebug)(NODE_RECV_DEBUG,("0: node %d receive at time %d \r\n",current_node_num,GetTime()));
	EXPECT_EQ(current_node_num,node_recv_table[recv_times].node_num);
	EXPECT_EQ(GetTime(),node_recv_table[recv_times].time);
	recv_times ++;
	return X_True;
}

TEST(trans,init)
{
	X_Boolean isRun = X_True,isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle);

	 s_node_manager node_1,node_2,node_3; // if used static ,the value will be init with value 0
	 s_node         node1,node2,node3;
	
	 node1.node_number  			= 1;
	 node1.forware_distance			= INVALID_NODE_DISTANCE;
	 node_1.p_node = &node1;
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node2.node_number  			= 2;
	node2.forware_distance			= 7;
	node_2.p_node = &node2;
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node3.node_number  			= 3;
	node3.forware_distance			= 10;
	node_3.p_node = &node3;
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);
	
	//SendWave(p_manager,GetTime(),1,&wave);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	
	EXPECT_EQ(GetDistanceBetweenNode(0,0), INVALID_NODE_DISTANCE);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(0,1), INVALID_NODE_DISTANCE);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(2,4), INVALID_NODE_DISTANCE);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(3,4), INVALID_NODE_DISTANCE);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));

	EXPECT_EQ(GetDistanceBetweenNode(1,1), 0)<<"oppos \r\n";
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(2,2), 0);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(3,3), 0);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	
	EXPECT_EQ(GetDistanceBetweenNode(1,2), 7);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(2,3), 10);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(1,3), 17);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));

	EXPECT_EQ(GetDistanceBetweenNode(2,1), 7);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(3,2), 10);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	EXPECT_EQ(GetDistanceBetweenNode(3,1), 17);
	INSERT(LogDebug)(USE_DOTTED_LINE_TO_CUT,(" --------------------------------------------- \r\n"));
	
	WaveTransDeInit();
}

TEST(trans,node1_send_wave)
{
	X_Boolean isRun = X_True,isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle);
	recv_times = 0;
	 s_node_manager node_1,node_2,node_3; // if used static ,the value will be init with value 0
	 s_node         node1,node2,node3;
	
	 node1.node_number  			= 1;
	 node1.forware_distance			= INVALID_NODE_DISTANCE;
	 node_1.p_node = &node1;
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node2.node_number  			= 2;
	node2.forware_distance			= 7;
	node_2.p_node = &node2;
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node3.node_number  			= 3;
	node3.forware_distance			= 10;
	node_3.p_node = &node3;
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);

	node1.current_hold_wave  				= 1;
	node1.wave_trans[0].content_length 		= 2;
	node1.wave_trans[0].context[0] 			= 0x55;
	node1.wave_trans[0].context[1] 			= 0xaa;
	node1.wave_trans[0].isDisapper 			= X_False;
	node1.wave_trans[0].max_trans_distance 	= 20;
	node1.wave_trans[0].direct  			= ED_bidirection;
	
	SendWave(p_manager,GetTime(),1,&node1.wave_trans[0]);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
}

static X_Boolean NodeRecvHandle1(_s_node_manager *p_manager,uint8_t current_node_num)
{
	UNUSED_PARAMETER(p_manager);
	EXPECT_EQ(recv_times,0);
	EXPECT_EQ(current_node_num,2);
	INSERT(LogDebug)(NODE_RECV_DEBUG,("1: node %d receive at time %d \r\n",current_node_num,GetTime()));
	if(recv_times != 0) {return X_False;}
	EXPECT_EQ(GetTime(),7);
	recv_times ++;
	return X_True;
}

TEST(trans,node1_send_wave1)
{
	X_Boolean isRun = X_True,isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle1);
	recv_times = 0;
	 s_node_manager node_1,node_2,node_3; // if used static ,the value will be init with value 0
	 s_node         node1,node2,node3;
	
	 node1.node_number  			= 1;
	 node1.forware_distance			= INVALID_NODE_DISTANCE;
	 node_1.p_node = &node1;
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node2.node_number  			= 2;
	node2.forware_distance			= 7;
	node_2.p_node = &node2;
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node3.node_number  			= 3;
	node3.forware_distance			= 10;
	node_3.p_node = &node3;
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);

	node1.current_hold_wave  				= 1;
	node1.wave_trans[0].content_length 		= 2;
	node1.wave_trans[0].context[0] 			= 0x55;
	node1.wave_trans[0].context[1] 			= 0xaa;
	node1.wave_trans[0].isDisapper 			= X_False;
	node1.wave_trans[0].max_trans_distance 	= 12;
	node1.wave_trans[0].direct  			= ED_bidirection;
	
	SendWave(p_manager,GetTime(),1,&node1.wave_trans[0]);

	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
}


#include "node_test.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

#define USE_DOTTED_LINE_TO_CUT     0

static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,s_node *p_cur_node)
{
	return X_True;
}

TEST(trans,init)
{
	uint8_t i;
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


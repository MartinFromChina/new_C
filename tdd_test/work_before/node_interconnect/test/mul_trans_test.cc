#include "node_test.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

static s_node_manager node_1,node_2,node_3,node_4,node_5;
static s_node         node1,node2,node3,node4,node5;

#define DISTANCE_BETWEEN_NODE_1_2   2
#define DISTANCE_BETWEEN_NODE_2_3   3
#define DISTANCE_BETWEEN_NODE_3_4   5
#define DISTANCE_BETWEEN_NODE_4_5   8  

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
static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	UNUSED_PARAMETER(p_manager);UNUSED_PARAMETER(current_node_num);UNUSED_PARAMETER(p_data);UNUSED_PARAMETER(length);
	//INSERT(LogDebug)(NODE_RECV_DEBUG,("0: node %d receive at time %d \r\n",current_node_num,GetTime()));
	//EXPECT_EQ(current_node_num,node_recv_table[recv_times].node_num);
	//EXPECT_EQ(GetTime(),node_recv_table[recv_times].time);
	//recv_times ++;
	return X_True;
}

static X_Void BasicSet(X_Void)
{
	X_Boolean isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle);
	
	 node1.node_number  				= 1;
	 node1.forware_distance				= INVALID_NODE_DISTANCE;
	 node_1.p_node = &node1;
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node2.node_number  					= 2;
	node2.forware_distance				= DISTANCE_BETWEEN_NODE_1_2;
	node_2.p_node = &node2;
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node3.node_number  					= 3;
	node3.forware_distance				= DISTANCE_BETWEEN_NODE_2_3;
	node_3.p_node = &node3;
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);

	node4.node_number  					= 4;
	node4.forware_distance				= DISTANCE_BETWEEN_NODE_3_4;
	node_4.p_node = &node4;
	isOK = NodeAdd(p_manager,&node_4);
	EXPECT_EQ(isOK, X_True);

	node5.node_number  					= 5;
	node5.forware_distance				= DISTANCE_BETWEEN_NODE_4_5;
	node_5.p_node = &node5;
	isOK = NodeAdd(p_manager,&node_5);
	EXPECT_EQ(isOK, X_True);
	
	//SendWave(p_manager,GetTime(),1,&wave);

}

TEST(mul_trans,same_time)
{
	X_Boolean isRun = X_True;
	BasicSet();
	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	WaveTransDeInit();
}
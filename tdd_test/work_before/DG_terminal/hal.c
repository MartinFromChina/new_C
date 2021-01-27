#include "DG_common.h"
#include "hal.h"
#include "terminal1.h"
#include "terminal2.h"

typedef struct
{
	X_Void (*main_loop)(X_Void);
}s_main_loop;

static s_main_loop const Mains[] ={
	main_loop_1,
	main_loop_2,
}; 



static X_Void Hal_Main_Loop(X_Void)
{
	uint8_t i;
	for(i=0;i<sizeof(Mains)/sizeof(Mains[0]);i++)
	{
		Mains[i].main_loop();
	}
}

static X_Boolean Hal_Irq_And_Wireless(X_Void)
{
	X_Boolean isRun = X_True;
	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	return X_False;
}


/**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************/
static s_node_manager * p_node_manager = (s_node_manager *)0;
static X_Boolean isInit = X_False;
static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	 INSERT(LogDebug)(1,("node %d receive data %2x at time %d \r\n",current_node_num,p_data[0],(GetSysTime())));
}

#define DISTANCE_BETWEEN_NODE_1_2   2
#define DISTANCE_BETWEEN_NODE_2_3   3
#define DISTANCE_BETWEEN_NODE_3_4   5
#define DISTANCE_BETWEEN_NODE_4_5   8 
#define DISTANCE_BETWEEN_NODE_5_6   2
#define DISTANCE_BETWEEN_NODE_6_7   3
#define DISTANCE_BETWEEN_NODE_7_8   5
#define DISTANCE_BETWEEN_NODE_8_9   8 
#define DISTANCE_BETWEEN_NODE_9_10  7

#include "../../google_test/include/gtest/include/gtest/gtest.h"

static s_node_manager node_1,node_2,node_3,node_4,node_5,node_6,node_7,node_8,node_9,node_10;
static s_node         node1,node2,node3,node4,node5,node6,node7,node8,node9,node10;

static s_node * NodeBasicInit(s_node *p_node,uint8_t mode_num,uint16_t forward_distance)
{
	if(p_node == X_Null) {return (s_node *)0;}
	 p_node->node_number  				= mode_num;
	 p_node->forware_distance			= forward_distance;
	 p_node->current_hold_wave          = 0;

	 for(uint8_t i = 0;i<MAX_WAVE_ONE_NODE_HOLD;i++)
	 {
		p_node ->wave_trans[i].isDisapper = X_True;
	 }
	 return p_node;
}

static s_wave * NodeWaveAdd(s_node *p_node,uint8_t *p_buf,uint16_t length,e_direction direc,uint16_t max_distance)
{
	uint8_t i,j;
	s_wave *p_wave = (s_wave *)0;

	for(i=0;i<MAX_WAVE_ONE_NODE_HOLD;i++)
	{
		if(p_node->wave_trans[i].isDisapper == X_True)
		{
			p_wave = &(p_node->wave_trans[i]);
			p_wave ->content_length = length;
			p_wave ->direct         = direc;
			p_wave ->isDisapper     = X_False;
			p_wave ->passed_node_cnt    = 0;
			p_wave ->max_trans_distance = max_distance;
			for(j=0;j<length;j++)
			{
				p_wave ->context[j] = p_buf[j];
			}
			p_node->current_hold_wave ++;
			break;
		}
	
	}
	return p_wave;
}
X_Void HAL_BasicSet(X_Void)
{
	X_Boolean isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle);
	
	node_1.p_node = NodeBasicInit(&node1,1,INVALID_NODE_DISTANCE);
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node_2.p_node = NodeBasicInit(&node2,2,DISTANCE_BETWEEN_NODE_1_2);
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node_3.p_node = NodeBasicInit(&node3,3,DISTANCE_BETWEEN_NODE_2_3);
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);

	node_4.p_node = NodeBasicInit(&node4,4,DISTANCE_BETWEEN_NODE_3_4);
	isOK = NodeAdd(p_manager,&node_4);
	EXPECT_EQ(isOK, X_True);

	node_5.p_node = NodeBasicInit(&node5,5,DISTANCE_BETWEEN_NODE_4_5);
	isOK = NodeAdd(p_manager,&node_5);
	EXPECT_EQ(isOK, X_True);

	node_6.p_node = NodeBasicInit(&node6,6,DISTANCE_BETWEEN_NODE_5_6);
	isOK = NodeAdd(p_manager,&node_6);
	EXPECT_EQ(isOK, X_True);

	node_7.p_node = NodeBasicInit(&node7,7,DISTANCE_BETWEEN_NODE_6_7);
	isOK = NodeAdd(p_manager,&node_7);
	EXPECT_EQ(isOK, X_True);
	
	node_8.p_node = NodeBasicInit(&node8,8,DISTANCE_BETWEEN_NODE_7_8);
	isOK = NodeAdd(p_manager,&node_8);
	EXPECT_EQ(isOK, X_True);

	node_9.p_node = NodeBasicInit(&node9,9,DISTANCE_BETWEEN_NODE_8_9);
	isOK = NodeAdd(p_manager,&node_9);
	EXPECT_EQ(isOK, X_True);

	node_10.p_node = NodeBasicInit(&node10,10,DISTANCE_BETWEEN_NODE_9_10);
	isOK = NodeAdd(p_manager,&node_10);
	EXPECT_EQ(isOK, X_True);

	p_node_manager = p_manager;
	isInit = X_True;
	
}
X_Void HAL_Run(X_Void)
{
	uint8_t i;
	X_Boolean isRun = X_True;
	while(isRun == X_True)
	{		
		Hal_Main_Loop();
		isRun = Hal_Irq_And_Wireless();
	}
	for(i=0;i<100;i++)
	{
		Hal_Main_Loop();
	}
	WaveTransDeInit();
}

uint32_t GetSysTime(X_Void)
{
	return GetTime();
}

static s_node_manager *GetNodePointer(s_node_manager *p_manager,uint8_t curr_node_num)
{
	uint16_t i;
	s_node_manager *p_current;
	// no need pointer null check ,because the caller is me
	s_node_manager *p_curr = (s_node_manager *)0;
	p_current = p_manager;
	for(i=0;i<100;i++)
	{
		if(p_current ->p_node ->node_number == curr_node_num)
		{
			p_curr = p_current;
			break;
		}
		if(p_current ->p_next == X_Null) {break;}
		p_current = p_current ->p_next;
	}
	return p_curr;
}
X_Boolean SendWaveSetForTestModule(uint8_t node_num,uint32_t sent_time,uint8_t *p_buf,uint8_t length,e_direction direct,uint16_t distance)
{
	s_wave * p_wave;
	s_node_manager * p_current;
	if(isInit == X_False) {return X_False;}
	p_current = GetNodePointer(p_node_manager,node_num);
	if(p_current == X_Null) {return X_False;}
	p_wave = NodeWaveAdd(p_current ->p_node,p_buf,length,direct,distance);
	if(p_wave != X_Null)
	{
		SendWave(p_node_manager,sent_time,p_current -> p_node ->node_number,p_wave);
		return X_True;
	}
	else{return X_False;}
}
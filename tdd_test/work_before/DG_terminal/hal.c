#include "DG_common.h"
#include "hal.h"
#include "terminal1.h"
#include "terminal2.h"
#include "terminal3.h"
#include "./test/test_common.h"
#include "DG_protocol.h"

#define HAL_DEDBUG         0
#define HAL_RUN_DEBUG      0

typedef struct
{
	X_Void (*init)(X_Boolean isStartPoint);
}s_init;

typedef struct
{
	X_Void (*main_loop)(uint32_t current_time);
}s_main_loop;

typedef struct
{
	uint8_t node_num;
	X_Void (*irq)(uint8_t data);
}s_irq_handle;

static s_init const Inits[] ={
	Terminal1Init,
	Terminal2Init,
	Terminal3Init,
	Terminal4Init,
	Terminal5Init,
	Terminal6Init,
	Terminal7Init,
	Terminal8Init,
	Terminal9Init,
	Terminal10Init,

}; 

static s_main_loop const Mains[] ={
	main_loop_1,
	main_loop_2,
	main_loop_3,
	main_loop_4,
	main_loop_5,
	main_loop_6,
	main_loop_7,
	main_loop_8,
	main_loop_9,
	main_loop_10,

}; 

static s_irq_handle const irqs[] ={
	{1,UartIrqCallBack_1},
	{2,UartIrqCallBack_2},
	{3,UartIrqCallBack_3},
	{4,UartIrqCallBack_4},
	{5,UartIrqCallBack_5},
	{6,UartIrqCallBack_6},
	{7,UartIrqCallBack_7},
	{8,UartIrqCallBack_8},
	{9,UartIrqCallBack_9},
	{10,UartIrqCallBack_10},
}; 

static X_Void Hal_Main_Loop(X_Void)
{
	uint8_t i;
	uint32_t time = GetSysTime();
	for(i=0;i<sizeof(Mains)/sizeof(Mains[0]);i++)
	{
		if(Mains[i].main_loop != X_Null){Mains[i].main_loop(time);}
	}
}


/**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************
**************************************************************************************************************/
X_Void Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{UNUSED_PARAMETER(current_node_num);UNUSED_PARAMETER(p_data);UNUSED_PARAMETER(length);UNUSED_PARAMETER(time);}
X_Void Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{UNUSED_PARAMETER(current_node_num);UNUSED_PARAMETER(p_data);UNUSED_PARAMETER(length);UNUSED_PARAMETER(time);}

uint16_t GetDistance(X_Void)
{
	return COMMON_WIRELESS_DISTANCE;
}
static s_node_manager * p_node_manager = (s_node_manager *)0;
static X_Boolean isInit = X_False,iStillRun = X_True;
static uint32_t time_backup = 0;
static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	 UNUSED_PARAMETER(p_manager);
	 INSERT(LogDebug)(HAL_DEDBUG,("node %d receive data %2x at time %d \r\n",current_node_num,p_data[0],(GetSysTime())));
	 MOCKABLE(Recv_Monitor)(current_node_num,p_data,length,GetSysTime());
	 uint8_t i,j;
		for(i=0;i<sizeof(irqs)/sizeof(irqs[0]);i++)
		{
			if(current_node_num == irqs[i].node_num)
			{
				if(irqs[i].irq != X_Null) 
				{
					for(j=0;j<length;j++)
					irqs[i].irq(p_data[j]);

				}
				break;
			}
		}
		
		return X_True;
}

#define DISTANCE_BETWEEN_NODE_1_2   11
#define DISTANCE_BETWEEN_NODE_2_3   9
#define DISTANCE_BETWEEN_NODE_3_4   5
#define DISTANCE_BETWEEN_NODE_4_5   8 
#define DISTANCE_BETWEEN_NODE_5_6   7
#define DISTANCE_BETWEEN_NODE_6_7   9
#define DISTANCE_BETWEEN_NODE_7_8   5
#define DISTANCE_BETWEEN_NODE_8_9   8 
#define DISTANCE_BETWEEN_NODE_9_10  7

#include "../../google_test/include/gtest/include/gtest/gtest.h"

static s_node_manager node_1,node_2,node_3,node_4,node_5,node_6,node_7,node_8,node_9,node_10;
static s_node         node1,node2,node3,node4,node5,node6,node7,node8,node9,node10;

typedef struct
{
	s_node_manager *p_manager;
	s_node         *p_node;
	uint16_t       forward_distance;
}s_terminal_node_list;
static  s_terminal_node_list List[] = {
	{&node_1,&node1,INVALID_NODE_DISTANCE},
	{&node_2,&node2,DISTANCE_BETWEEN_NODE_1_2},
	{&node_3,&node3,DISTANCE_BETWEEN_NODE_2_3},
	{&node_4,&node4,DISTANCE_BETWEEN_NODE_3_4},
	{&node_5,&node5,DISTANCE_BETWEEN_NODE_4_5},
	{&node_6,&node6,DISTANCE_BETWEEN_NODE_5_6},
	{&node_7,&node7,DISTANCE_BETWEEN_NODE_6_7},
	{&node_8,&node8,DISTANCE_BETWEEN_NODE_7_8},
	{&node_9,&node9,DISTANCE_BETWEEN_NODE_8_9},
	{&node_10,&node10,DISTANCE_BETWEEN_NODE_9_10},

};
	
	
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
static X_Boolean send(uint8_t node_num,uint32_t sent_time,uint8_t *p_buf,uint8_t length)
{
	uint16_t distance; 
	LoadCheckSum(p_buf,length);
	MOCKABLE(Send_Monitor)(node_num,p_buf,length,sent_time);
	distance = MOCKABLE(GetDistance)();
	return SendWaveSetForTestModule(node_num,sent_time,p_buf,length,COMMON_WIRELESS_DIRECTION,distance);
}

X_Void HAL_BasicSet(uint8_t start_point)
{
	uint8_t i;
	X_Boolean isOK,isStart;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(NodeRecvHandle);

	for(i=0;i<(sizeof(List)/sizeof(List[0]));i++)
	{
		List[i].p_manager ->p_node = NodeBasicInit(List[i].p_node,i+1,List[i].forward_distance);
		isOK = NodeAdd(p_manager,List[i].p_manager);
		EXPECT_EQ(isOK, X_True);

	}
	p_node_manager = p_manager;
	for(i=0;i<(sizeof(Inits)/sizeof(Inits[0]));i++)
	{
		if((i+1) == start_point) {isStart = X_True;}
		else {isStart = X_False;}
		Inits[i].init(isStart);
	}
	DG_TerminalInit(send);
	
	isInit = X_True;
	iStillRun = X_True;
	time_backup = 0;
}
X_Void HAL_Run(X_Void)
{
	uint8_t i;
	uint16_t cnt = 0;
	X_Boolean isRun = X_True;
	while(isRun == X_True)
	{		
		Hal_Main_Loop();
		isRun = RunNodeCommunicationProcess();
		cnt ++;
	}
	for(i=0;i<100;i++)
	{
		Hal_Main_Loop();
	}
	INSERT(LogDebug)(HAL_RUN_DEBUG,("HAL stop  run %d cycle \r\n",cnt));
	WaveTransDeInit();
}

uint32_t GetSysTime(X_Void)
{
	if(iStillRun == X_False)
	{
		return time_backup ++;
	}
	time_backup = GetTime();
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
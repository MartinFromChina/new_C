#include "node_interconnect.h"

#include "../../../module/common/StateMachine/StateMachine.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"
#include "../../../module/common/priorityqueue/priority_queues.h"


#define MAX_NODE_NUM  100
#define NODE_NUM_DEBUG 0


typedef struct
{
	s_StateMachineParam 		base;
	X_Boolean 					isStateRun;
	s_node_manager *            p_manager;
	uint16_t 					node_num;
}sParamExtern;

static sParamExtern sPE;

typedef enum
{
	CS_Idle,
	CS_transmation,
	CS_node_receive,
	CS_node_send,
	CS_end,
}communication_state;


static uint32_t GetTime(X_Void);

/**********************************************************************************************************************************
			0
*********************************************************************************************************************************/
static StateNumber CS_IdleAction(s_StateMachineParam *p_this)
{
	uint16_t i;
	s_node_manager *p_next;
	sParamExtern * p_ext = (sParamExtern *)p_this;
	p_ext ->node_num = 0;
	if( p_ext ->p_manager ->flag == NF_idle)
	{
		p_ext ->node_num = 0;
		INSERT(LogDebug)(NODE_NUM_DEBUG,("no node\r\n"));
		return  CS_end;
	}
	p_ext ->node_num = 1;
	if(p_ext ->p_manager ->p_next == X_Null) 
	{	
		INSERT(LogDebug)(NODE_NUM_DEBUG,("one node\r\n"));
		return CS_transmation;
	}
	p_next = p_ext ->p_manager ->p_next;
	
	for(i=0 ;i < MAX_NODE_NUM  ;i++)
	{	
		p_ext ->node_num ++;
		INSERT(LogDebug)(NODE_NUM_DEBUG,("another node found ; total node num %d\r\n",p_ext ->node_num));
		if(p_next ->p_next == X_Null) {break;}
		p_next = p_next ->p_next;
	}
	return CS_transmation;
}
static StateNumber CS_transmationAction(s_StateMachineParam *p_this)
{
	uint16_t i;
	p_node_handle p_handle[100];
	s_node_manager *p_next;
	sParamExtern * p_ext = (sParamExtern *)p_this;
	INSERT(LogDebug)(NODE_NUM_DEBUG,("node num is %d\r\n",p_ext ->node_num));

	p_next = p_ext ->p_manager;
	for(i=0;i<p_ext ->node_num;i++)
	{
		p_handle[i] = p_next ->p_node ->node_handle;
		p_next = p_next ->p_next;
	}
	
	for(i=0;i<p_ext ->node_num;i++)
	{
		if( p_handle[i] != X_Null) {p_handle[i](p_ext ->p_manager ->p_node->node_info);}
	}
	
	return CS_end;
}
static StateNumber CS_node_receiveAction(s_StateMachineParam *p_this)
{
	sParamExtern * p_ext = (sParamExtern *)p_this;
	UNUSED_PARAMETER(p_ext);
	return p_this->current_state;
}
static StateNumber CS_node_sendAction(s_StateMachineParam *p_this)
{
	sParamExtern * p_ext = (sParamExtern *)p_this;
	UNUSED_PARAMETER(p_ext);
	return p_this->current_state;
}
static StateNumber CS_endAction(s_StateMachineParam *p_this)
{
	sParamExtern * p_ext = (sParamExtern *)p_this;
	p_ext ->isStateRun = X_False;
	return p_this->current_state;
}


static const StateAction NodeStateAction[] = {
		{CS_IdleAction},
		{CS_transmationAction},
		{CS_node_receiveAction},
		{CS_node_sendAction},
		{CS_endAction},
};

APP_STATE_MACHINE_DEF(p_state
								,sizeof(NodeStateAction)/sizeof(NodeStateAction[0])
								,&NodeStateAction[0]);

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	UNUSED_PARAMETER(state_going_to_leave);
	UNUSED_PARAMETER(state_going_to_enter);
}



static uint32_t time_cnt = 0;
static X_PriorityQueue *p_queue  = (X_PriorityQueue *)0;

X_Boolean RunNodeCommunicationProcess(X_Void)
{
	if(sPE.p_manager == X_Null) {return X_False;}
	if(sPE.isStateRun != X_True)  {return X_False;}
	mStateMachineRun(p_state,&sPE.base,StateJumpRecorder);
	time_cnt ++;
	return X_True;
}

static s_node_manager manager
{
	NF_idle,
	(s_node*)0,
	(s_node_manager*)0,
};

s_node_manager *WaveTransInit(X_Void)
{
	time_cnt = 0;
	manager.flag  = NF_idle;
	sPE.p_manager = &manager;
	sPE.isStateRun = X_True;
	mStateMachineStateSet(p_state,CS_Idle);
	p_queue = BH_PriorityQueueInit(200);
	return sPE.p_manager;
}
X_Void WaveTransDeInit(X_Void)
{
	BH_PriorityQueueDestory(&p_queue);
}

static uint32_t GetTime(X_Void)
{
	return time_cnt;
}

X_Boolean NodeAdd(s_node_manager *p_manager,s_node_manager *p_new_node)
{
	uint16_t i;
	s_node_manager *p_next;
	if(p_manager == X_Null || p_new_node == X_Null) {return X_False;}
	p_new_node ->flag = NF_end_node;
	p_new_node ->p_next = (s_node_manager*)0;
	
	if(p_manager ->flag == NF_idle)
	{
		p_manager ->flag = NF_end_node;
		p_manager ->p_node = p_new_node ->p_node;
		p_manager ->p_next = (s_node_manager*)0;
		INSERT(LogDebug)(NODE_NUM_DEBUG,("add header node \r\n"));
		return X_True;
	}
	else
	{
		i = 1;
		p_next = p_manager;
		while(1 && i< MAX_NODE_NUM)
		{
			if(p_next ->flag == NF_end_node)
			{
				p_next ->flag = NF_inter_node;
				p_next ->p_next = p_new_node;
				INSERT(LogDebug)(NODE_NUM_DEBUG,("add new node\r\n"));
				return X_True;
			}
			if(p_next ->p_next == X_Null) {return X_False;}// inter_node must have the next node
			p_next = p_next ->p_next;//get next
			i++;
		}

	}
}

uint16_t GetNodeNum(X_Void)
{
	return sPE.node_num;
}
X_Boolean SendWave(uint32_t sys_time,uint8_t node_num,s_wave p_wave)
{
	return X_True;
}

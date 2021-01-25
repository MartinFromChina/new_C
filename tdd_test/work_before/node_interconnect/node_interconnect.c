#include "node_interconnect.h"

#include "../../../module/common/StateMachine/StateMachine.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"
#include "../../../module/common/priorityqueue/priority_queues.h"


#define MAX_NODE_NUM  100

#define NODE_NUM_DEBUG 			    0
#define NODE_ADD_DEBUG 			    0
#define WAVE_TRANS_DEBUG 			1
#define NODE_DISTANCE_DEBUG 		0



static uint32_t time_cnt = 0;
static X_PriorityQueue *p_queue  = (X_PriorityQueue *)0;
typedef struct 
{
	s_element_base base;
	uint16_t other_info;
}s_element_extern;

static s_element_extern s_ee[MAX_NODE_NUM*2];



typedef struct
{
	s_StateMachineParam 		base;
	X_Boolean 					isStateRun;
	s_node_manager *            p_manager;
	uint16_t 					node_num;
	uint16_t 					wait_time;
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

/**********************************************************************************************************************************
			0
*********************************************************************************************************************************/
static StateNumber CS_IdleAction(s_StateMachineParam *p_this)
{
	uint16_t i;
	s_node_manager *p_next,*p_previous;
	
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
	
	#if (NODE_ADD_DEBUG != 0)
	p_next = p_ext ->p_manager;
	p_previous = p_next ->p_previous;
	for(i=0 ;i < p_ext ->node_num  ;i++)
	{
		
		if(p_next != X_Null)
		{
			if(p_next ->p_node == X_Null) {break;}
			INSERT(LogDebug)(NODE_ADD_DEBUG,(" --------node num : %2x ; previous node %2x , distance %d ;next node :%2x , distance %d\r\n"
										,p_next ->p_node ->node_number
										,p_next ->p_node ->forware_node,p_next ->p_node ->forware_distance
										,p_next ->p_node ->backward_node,p_next ->p_node ->backward_distance));
		}
		
		if(p_previous != X_Null)
		{
			//if(p_previous ->p_node == X_Null) {break;}
			INSERT(LogDebug)(NODE_ADD_DEBUG,(" !!!previous node %2x , distance %d ;\r\n"
								,p_previous ->p_node ->node_number,p_previous ->p_node ->backward_distance));
		}
		
		if(p_next ->p_next != X_Null)
		{
			//if(p_next->p_next->p_node == X_Null) {break;}
			INSERT(LogDebug)(NODE_ADD_DEBUG,(" !!!next node :%2x , distance %d\r\n"
								,p_next ->p_next ->p_node ->node_number,p_next ->p_next ->p_node ->forware_distance));
		}
		
		INSERT(LogDebug)(NODE_ADD_DEBUG,("\r\n"));
		if(p_next != X_Null) {p_next = p_next ->p_next;}
		if(p_next != X_Null) {p_previous = p_next ->p_previous;}
	}
	#endif
	
	return CS_transmation;
}
static StateNumber CS_transmationAction(s_StateMachineParam *p_this)
{
	uint16_t i,node_priority;
	p_node_handle p_handle[100];
	s_node_manager *p_next;
	s_element_base *p_base;
	
	sParamExtern * p_ext = (sParamExtern *)p_this;
	INSERT(LogDebug)(NODE_NUM_DEBUG,("node num is %d\r\n",p_ext ->node_num));

	p_next = p_ext ->p_manager;
	for(i=0;i<p_ext ->node_num;i++)
	{
		p_handle[i] = p_next ->handle;
		p_next = p_next ->p_next;
	}
	
	for(i=0;i<p_ext ->node_num;i++)
	{
		if( p_handle[i] != X_Null) {p_handle[i](p_ext ->p_manager,0);}
	}
	
	node_priority = BH_PriorityQueueReleaseMin(p_queue,&p_base);

	if(node_priority != INVALID_PRIOQUEUE_PRIORITY)
	{
		p_ext ->wait_time = node_priority;
		s_element_extern * p_elem_ext = (s_element_extern *)p_base;
		p_ext ->node_num  = p_elem_ext ->other_info;
		return CS_node_receive;
	}
	else {INSERT(LogDebug)(WAVE_TRANS_DEBUG,("no node in priority queue\r\n"));	}
	
	return CS_end;
}
static StateNumber CS_node_receiveAction(s_StateMachineParam *p_this)
{
	uint16_t sys_time;
	sParamExtern * p_ext = (sParamExtern *)p_this;
	sys_time = GetTime();
	if(sys_time >= p_ext ->wait_time)
	{
		p_ext ->p_manager ->handle(p_ext ->p_manager,p_ext ->node_num);
		return CS_transmation;
	}
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
	(p_node_handle)0,
	(s_node_manager*)0,
	(s_node_manager*)0,
};

s_node_manager *WaveTransInit(p_node_handle handle)
{
	time_cnt = 0;
	manager.flag  = NF_idle;
	manager.handle = handle;
	sPE.p_manager = &manager;
	sPE.isStateRun = X_True;
	mStateMachineStateSet(p_state,CS_Idle);
	p_queue = BH_PriorityQueueInit(MAX_NODE_NUM*2);
	return sPE.p_manager;
}
X_Void WaveTransDeInit(X_Void)
{
	uint16_t i;
	s_node_manager *p_next;
	BH_PriorityQueueDestory(&p_queue);
	
	p_next = &manager;
	for(i=0 ;i < MAX_NODE_NUM  ;i++)
	{	
		p_next ->flag = NF_idle;
		if(p_next ->p_next == X_Null) {break;}
		p_next = p_next ->p_next;
	}
}

uint32_t GetTime(X_Void)
{
	return time_cnt;
}

X_Boolean NodeAdd(s_node_manager *p_manager,s_node_manager *p_new_node)
{
	uint16_t i;
	s_node_manager *p_next;
	if(p_manager == X_Null || p_new_node == X_Null) {return X_False;}
	if(p_new_node ->p_node ->forware_distance == 0 ){return X_False;}
	p_new_node ->flag = NF_end_node;
	p_new_node ->handle = p_manager ->handle;
	p_new_node ->p_next = (s_node_manager*)0;

	//p_new_node ->p_node ->forware_node 		= 
	//p_new_node ->p_node ->forware_distance 	= 
	p_new_node ->p_node ->backward_node 	= INVALID_NODE_NUM;
	p_new_node ->p_node ->backward_distance = INVALID_NODE_DISTANCE;
	p_new_node ->p_node ->current_hold_wave = 0;
	
	if(p_manager ->flag == NF_idle)
	{
		p_manager ->flag = NF_end_node;
		p_manager ->p_previous = (s_node_manager*)0;
		p_manager ->p_next = (s_node_manager*)0;
		p_manager ->p_node = p_new_node ->p_node;
		p_manager ->p_node ->forware_node 			= INVALID_NODE_NUM;
		p_manager ->p_node ->backward_node 			= INVALID_NODE_NUM;
		p_manager ->p_node ->forware_distance		= INVALID_NODE_DISTANCE;
		p_manager ->p_node ->backward_distance 		= INVALID_NODE_DISTANCE;
		p_manager ->p_node ->current_hold_wave      = 0;
		INSERT(LogDebug)(NODE_NUM_DEBUG | NODE_ADD_DEBUG,("add header node %d\r\n",p_new_node ->p_node ->node_number));
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
				//p_next ->p_previous // unchanged
				//p_next ->p_node ->forware_node 			= unchange;
				//p_next ->p_node ->forware_distance		= unchange;
				p_next ->p_node ->backward_distance		= p_new_node ->p_node ->forware_distance;
				p_next ->p_node ->backward_node         = p_new_node ->p_node ->node_number;
				if(p_next ->p_node ->backward_node <= p_next ->p_node ->node_number ){return X_False;}
				p_new_node ->p_previous = p_next;
				p_new_node ->p_node ->forware_node 			= p_next ->p_node ->node_number;
				//p_new_node ->p_node ->forware_distance    = unchange;
				
				INSERT(LogDebug)(NODE_NUM_DEBUG |NODE_ADD_DEBUG,("add new node %d\r\n",p_new_node ->p_node ->node_number));
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
uint16_t GetDistanceBetweenNode(uint8_t node_num1,uint8_t node_num2)
{
	X_Boolean isSmallMatched = X_False;
	uint8_t num_small,num_big,num;
	uint16_t i,distance;
	s_node_manager *p1 = (s_node_manager *)0,*p2 = (s_node_manager *)0,*p_next;

	if(node_num1 <= node_num2)
	{
		num_small = node_num1;
		num_big   = node_num2;
	}
	else
	{
		num_small = node_num2;
		num_big   = node_num1;
	}
	p_next = &manager;
	for(i=0 ;i < MAX_NODE_NUM  ;i++)
	{	
		num = (isSmallMatched == X_False) ? num_small : num_big;
		//////INSERT(LogDebug)(NODE_DISTANCE_DEBUG,(" num %d ; num_small %d ; num_big %d; \r\n",num,num_small,num_big));
		if(p_next ->p_node ->node_number == num)
		{
			if(isSmallMatched == X_False) 
			{
				p1 = p_next;
				isSmallMatched = X_True;
				if(node_num1 == node_num2) {return 0;}
				INSERT(LogDebug)(NODE_DISTANCE_DEBUG,(" ----found P1 %d \r\n",p1 ->p_node ->node_number));

			}
			else
			{
				p2 = p_next;
				INSERT(LogDebug)(NODE_DISTANCE_DEBUG,(" ----found P2 %d\r\n",p2 ->p_node ->node_number));
				break;
			}
			
		}
		if(p_next ->p_next == X_Null ) {break;}
		p_next = p_next ->p_next;
	}
	if(p1 == X_Null || p2 == X_Null ) {return INVALID_NODE_DISTANCE;}

	p_next = p1;
	distance = 0;
	for(i=0 ;i < MAX_NODE_NUM  ;i++)
	{
		distance += p_next ->p_node ->backward_distance;
		if(p_next ->p_next == X_Null ) {break;}
		p_next = p_next ->p_next;
		if(p_next == p2) {break;}
	}
	return distance;
	
}

X_Boolean SendWave(s_node_manager *p_manager,uint32_t sys_time,uint8_t node_num,s_wave *p_wave)
{
	s_ee[node_num].base.priority = 7;
	s_ee[node_num].other_info    = 2;
	if(BH_PriorityQueueInsert(p_queue,&s_ee[node_num].base) != 7){INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" insert failed\r\n"));}
	s_ee[node_num + 1].base.priority = 17;
	s_ee[node_num + 1].other_info    = 3;
	if(BH_PriorityQueueInsert(p_queue,&s_ee[node_num + 1].base) != 17){INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" insert failed again\r\n"));}

	return X_True;
}

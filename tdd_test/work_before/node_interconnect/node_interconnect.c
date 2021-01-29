#include "node_interconnect.h"

#include "../../../module/common/StateMachine/StateMachine.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"
#include "../../../module/common/priorityqueue/priority_queues.h"
/*#include "../../../module/common/loopqueue/loop_queues.h" */



#define MAX_NODE_NUM  100

#define NODE_NUM_DEBUG 			    0
#define NODE_ADD_DEBUG 			    0
#define WAVE_TRANS_DEBUG 			0
#define NODE_DISTANCE_DEBUG 		0
#define WAVE_SEND_DEBUG 			0
#define WAVE_RELEASE_DEBUG 			0

#define POWER  100



static uint8_t power_adjust = 0;
static X_Boolean isLockClock = X_True;
static uint32_t time_cnt = 0,time_out_threshold = 0xffff;
static X_PriorityQueue *p_queue  = (X_PriorityQueue *)0;

/*APP_LOOPQUEUE_DEF(p_loop,MAX_NODE_NUM *2);*/



typedef struct 
{
	s_element_base base;
	uint16_t other_info;
	s_wave  *p_wave;
}s_element_extern;

static uint16_t element_index = 0;
static s_element_extern s_ee[MAX_NODE_NUM*2];



typedef struct
{
	s_StateMachineParam 		base;
	X_Boolean 					isStateRun;
	s_node_manager *            p_manager;
	uint16_t                    total_node_num;
	uint16_t 					node_num;
	uint16_t 					wait_time;
	s_wave  		*			p_wave;
	uint16_t                    end_delay_time;
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
	sParamExtern * p_ext = (sParamExtern *)p_this;
	isLockClock = X_True;
	GetNodeNum();
	if(p_ext ->total_node_num == 0) {return CS_end;}
	
	#if (NODE_ADD_DEBUG != 0)
	uint16_t i;
	s_node_manager *p_next;
	s_node_manager *p_previous;
	p_next = p_ext ->p_manager;
	p_previous = p_next ->p_previous;
	for(i=0 ;i < p_ext ->total_node_num  ;i++)
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
	uint16_t node_priority;
	s_element_base *p_base;
	
	sParamExtern * p_ext = (sParamExtern *)p_this;
	INSERT(LogDebug)(NODE_NUM_DEBUG,("total node num is %d\r\n",p_ext ->total_node_num));
	isLockClock = X_True;
	node_priority = BH_PriorityQueueReleaseMin(p_queue,&p_base);

	if(node_priority != INVALID_PRIOQUEUE_PRIORITY)
	{
		p_ext ->wait_time = node_priority/POWER;
		s_element_extern * p_elem_ext = (s_element_extern *)p_base;
		p_ext ->node_num  = p_elem_ext ->other_info;
		p_ext ->p_wave    = p_elem_ext ->p_wave;
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
		p_ext ->p_manager ->handle(p_ext ->p_manager,p_ext ->node_num,p_ext ->p_wave->context,p_ext->p_wave->content_length);
		if(p_ext ->p_wave ->passed_node_cnt > 0) 
		{
			p_ext ->p_wave ->passed_node_cnt --;
			INSERT(LogDebug)(WAVE_RELEASE_DEBUG,("**************************node  %d wave passed cnt %d\r\n",p_ext ->p_wave ->source_node_num,p_ext ->p_wave ->passed_node_cnt));
			if(p_ext ->p_wave ->passed_node_cnt == 0)
			{
				p_ext ->p_wave ->isDisapper = X_True;
				INSERT(LogDebug)(WAVE_RELEASE_DEBUG,("**************************node  %d wave release\r\n",p_ext ->p_wave->source_node_num));
			}
		} 
		isLockClock = X_True;
		return CS_transmation;
	}
	isLockClock = X_False;
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
	if(p_ext ->end_delay_time > 0)
	{
		p_ext ->end_delay_time --;
		return CS_transmation;
	}
	else
	{
		p_ext ->isStateRun = X_False;
	}
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

typedef X_Void (*state_record)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);
X_Boolean RunNodeCommunicationProcess(X_Void)
{
	if(sPE.p_manager == X_Null) {return X_False;}
	if(sPE.isStateRun != X_True)  {return X_False;}
	mStateMachineRun(p_state,&sPE.base,(state_record)0);
	if(isLockClock == X_False) {time_cnt ++;}
	if(time_cnt >= time_out_threshold){return X_False;}
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
	isLockClock = X_True;
	time_cnt = 0;time_out_threshold = 0xffff;
	element_index = 0;
	power_adjust = 0;
	manager.flag  = NF_idle;
	manager.handle = handle;
	sPE.p_manager = &manager;
	sPE.isStateRun = X_True;
	sPE.node_num   = INVALID_NODE_NUM;
	sPE.total_node_num = INVALID_NODE_NUM;
	sPE.end_delay_time = 1000;
	mStateMachineStateSet(p_state,CS_Idle);
	p_queue = BH_PriorityQueueInit(MAX_NODE_NUM*2);
	//LoopQueueInitialize(p_loop);
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
X_Void SetTimeThresHold(uint32_t threshold)
{
	time_out_threshold = threshold;
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
	return X_False;
}

uint16_t GetNodeNum(X_Void)
{
	uint16_t i;
	s_node_manager *p_next;
	
	if(sPE.total_node_num != INVALID_NODE_NUM) {return sPE.total_node_num;}

	sPE.total_node_num = 0;
	if( manager.flag == NF_idle)
	{
		sPE.total_node_num = 0;
		INSERT(LogDebug)(NODE_NUM_DEBUG,("no node\r\n"));
		return  0;
	}
	sPE.total_node_num = 1;
	if(manager.p_next == X_Null) 
	{	
		INSERT(LogDebug)(NODE_NUM_DEBUG,("one node\r\n"));
		return 1;
	}
	p_next = manager.p_next;
	
	for(i=0 ;i < MAX_NODE_NUM  ;i++)
	{	
		sPE.total_node_num ++;
		INSERT(LogDebug)(NODE_NUM_DEBUG,("another node found ; total node num %d\r\n",sPE.total_node_num));
		if(p_next ->p_next == X_Null) {break;}
		p_next = p_next ->p_next;
	}
	return sPE.total_node_num;
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

static s_node_manager *GetNodePointer(s_node_manager *p_manager,uint8_t curr_node_num)
{
	uint16_t i;
	s_node_manager *p_current;
	// no need pointer null check ,because the caller is me
	s_node_manager *p_curr = (s_node_manager *)0;
	p_current = p_manager;
	for(i=0;i<MAX_NODE_NUM;i++)
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

X_Boolean SendWave(s_node_manager *p_manager,uint32_t sys_time,uint8_t node_num,s_wave *p_wave)
{
	X_Boolean isForward = X_False,isBackward = X_False;
	uint16_t total_node_num,i,distance;
	s_node_manager *p_current,*p_current_backup;

	if(p_manager == X_Null)  {return X_False;}
	total_node_num = GetNodeNum();
	///////////////INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" total_node_num %d\r\n",total_node_num));
	p_current = GetNodePointer(p_manager,node_num);
	p_current_backup = p_current;
	if(p_current == X_Null) {return X_False;}

	if(p_wave ->direct == ED_bidirection || p_wave ->direct == ED_forward) {isForward = X_True;}
	if(p_wave ->direct == ED_bidirection || p_wave ->direct == ED_backward) {isBackward = X_True;}

	p_wave ->passed_node_cnt = 0;
	p_wave ->isDisapper               = X_False;
	p_wave ->source_node_num          = node_num;
	
	distance = 0;
	if(isBackward == X_True)
	{
		for(i=0;i<total_node_num;i++)
		{
			if(p_current ->p_next == X_Null) {break;}
			distance += p_current ->p_node ->backward_distance;
			if(distance <= p_wave ->max_trans_distance)
			{
				s_ee[element_index].base.priority = distance + (uint16_t)sys_time;
				s_ee[element_index].base.priority = s_ee[element_index].base.priority * POWER;
				s_ee[element_index].base.priority += power_adjust;
				if(power_adjust >= POWER) {power_adjust = 0;}
				else {power_adjust ++;}
				
				s_ee[element_index].other_info    = p_current ->p_node->backward_node;
				s_ee[element_index].p_wave        = p_wave;
				
				if(BH_PriorityQueueInsert(p_queue,&s_ee[element_index].base) != INVALID_PRIOQUEUE_PRIORITY)
				{
					INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" -----insert successed node % d will receive it at time %d ; data[0]: %2x; distance %d, max distance %d\r\n"
								,s_ee[element_index].other_info,s_ee[element_index].base.priority/POWER,p_wave ->context[0],distance,p_wave ->max_trans_distance));
					p_wave ->passed_node_cnt ++;
				}
				else
				{
					INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" insert failed\r\n"));
				}
				if((element_index + 1)<(MAX_NODE_NUM * 2)) {element_index ++;}
			}
			p_current = p_current ->p_next;
		}
	}

	distance = 0;
	p_current = p_current_backup;
	if(isForward == X_True)
	{
		for(i=0;i<total_node_num;i++)
		{
			if(p_current ->p_previous == X_Null) {break;}
			distance += p_current ->p_node ->forware_distance;
			if(distance <= p_wave ->max_trans_distance)
			{
				s_ee[element_index].base.priority = distance + (uint16_t)sys_time;
				s_ee[element_index].base.priority = s_ee[element_index].base.priority * POWER;
				s_ee[element_index].base.priority += power_adjust;
				if(power_adjust >= POWER) {power_adjust = 0;}
				else {power_adjust ++;}
				
				s_ee[element_index].other_info    = p_current ->p_node->forware_node;
				s_ee[element_index].p_wave        = p_wave;
				
				if(BH_PriorityQueueInsert(p_queue,&s_ee[element_index].base) != INVALID_PRIOQUEUE_PRIORITY)
				{
					INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" -----insert successed node % d will receive it at time %d data[0]: %2x\r\n"
								,s_ee[element_index].other_info,s_ee[element_index].base.priority/POWER,p_wave ->context[0]));
					p_wave ->passed_node_cnt ++;
				}
				else
				{
					INSERT(LogDebug)(WAVE_TRANS_DEBUG,(" insert failed\r\n"));
				}
				if((element_index + 1)<(MAX_NODE_NUM * 2)) {element_index ++;}
			}
			p_current = p_current ->p_previous;
		}
	}
	
	return X_True;
}

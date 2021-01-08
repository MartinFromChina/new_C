#include "priority_queues.h"
#include "../Math/bit_operation.h"
#include <stdlib.h> 


#define BUF_FREE   0
#define BUF_USED   1

#define USE_INLINE  

//#include <stdio.h> // for test

#if (USE_TDD_PRIORITY_QUEUE_INTERNAL_TERST == 1)

uint32_t InsertPrioTable(uint16_t priority,uint16_t *table_index)
{

	uint16_t priority_convert,index;
	uint32_t bit,bit_number;
		

	// don't forget prio_to_insert boundary check
	priority_convert = priority;
	index = priority_convert/BIT_COUNT_IN_UINT32;
	*table_index = index ;

	//printf("------------------index = %d\r\n",index);
	bit_number = (uint32_t)priority_convert & (BIT_COUNT_IN_UINT32 - 1u);
	bit = 1u;
	bit <<= (BIT_COUNT_IN_UINT32 - 1u) - bit_number;
	//printf("------------------bit_number = %2x ;bit = %2x\r\n",bit_number,bit);
	return bit;
}

#endif

X_Void 				BT_PriorityQueueInit(const sPrioListManager *p_manager)
{
	uint16_t i;
	if(p_manager == X_Null) {return;}
	if(p_manager ->max_priority > MAX_PRIOQUEUE_PRIORITY )  {return;}

	for(i=0;i<p_manager->table_size ;i++)
	{
		p_manager->p_bit_table[i] = 0;
	}
	
	p_manager ->p_PLP->current_used_bit_cnt = 0;
	p_manager ->p_PLP ->isInit = X_True;
}

CURRENT_PRIORITY 	BT_PriorityQueueInsert(const sPrioListManager *p_manager,uint16_t prio_to_insert)
{
	uint16_t priority_convert,index;
	uint32_t bit,bit_number;
		

	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	if(prio_to_insert > p_manager ->max_priority) {return INVALID_PRIOQUEUE_PRIORITY;}
	priority_convert = prio_to_insert;
	index = priority_convert/BIT_COUNT_IN_UINT32;
	if(index > p_manager ->table_size) {return INVALID_PRIOQUEUE_PRIORITY;}
	bit_number = (uint32_t)priority_convert & (BIT_COUNT_IN_UINT32 - 1u);
	bit = 1u;
	bit <<= (BIT_COUNT_IN_UINT32 - 1u) - bit_number;

	if((p_manager ->p_bit_table[index] & bit) == 0) {p_manager ->p_PLP ->current_used_bit_cnt ++;}
	p_manager ->p_bit_table[index] |= bit;

	
	
	return prio_to_insert;
}
CURRENT_PRIORITY 	BT_PriorityQueueFindMin(const sPrioListManager *p_manager)
{
	X_Boolean isAllZero = X_True;
	uint16_t i,prio = 0;

	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	for(i=0;i<p_manager->table_size;i++)
	{
		if(p_manager-> p_bit_table[i] != 0) {isAllZero = X_False;break;}
		else {prio += BIT_COUNT_IN_UINT32;}
	}

	if( isAllZero == X_True )  {return INVALID_PRIOQUEUE_PRIORITY;}

	prio +=(uint16_t)GetLeadZeroCount(p_manager-> p_bit_table[i]);
	if(prio > p_manager ->max_priority) {return INVALID_PRIOQUEUE_PRIORITY;}
	
	return prio;
}

CURRENT_PRIORITY 	BT_PriorityQueueReleaseMin(const sPrioListManager *p_manager)
{
	X_Boolean isAllZero = X_True;
	uint16_t i,prio = 0,index;
	uint32_t bit,bit_number;

	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	for(i=0;i<p_manager->table_size;i++)
	{
		if(p_manager-> p_bit_table[i] != 0) {isAllZero = X_False;break;}
		else {prio += BIT_COUNT_IN_UINT32;}
	}

	if( isAllZero == X_True )  {return INVALID_PRIOQUEUE_PRIORITY;}

	prio +=(uint16_t)GetLeadZeroCount(p_manager-> p_bit_table[i]);
	if(prio > p_manager ->max_priority) {return INVALID_PRIOQUEUE_PRIORITY;}


	index = prio/BIT_COUNT_IN_UINT32;
	if(index > p_manager ->table_size) {return INVALID_PRIOQUEUE_PRIORITY;}
	bit_number = (uint32_t)prio & (BIT_COUNT_IN_UINT32 - 1u);
	bit = 1u;
	bit <<= (BIT_COUNT_IN_UINT32 - 1u) - bit_number;
	p_manager-> p_bit_table[index] &= ~bit;

	if(p_manager ->p_PLP ->current_used_bit_cnt > 0) {p_manager ->p_PLP ->current_used_bit_cnt --;}
	
	return prio;
}

X_Void 				BT_PriorityQueueClear(const sPrioListManager *p_manager)
{
	BT_PriorityQueueInit(p_manager);
}
X_Boolean 			BT_DoesPriorityQueueEmpty(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return X_True;}
	if(p_manager ->p_PLP -> isInit != X_True) {return X_True;}
	return (p_manager ->p_PLP ->current_used_bit_cnt == 0);
}
uint16_t 			BT_GetPriorityQueueUsedNodeNum(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	return p_manager ->p_PLP ->current_used_bit_cnt;
}


/***********************************************
****************************************************/

X_PriorityQueue  *		BH_PriorityQueueInit(uint16_t max_elements)
{
	X_PriorityQueue entry;
    X_PriorityQueue * H;

	if(max_elements > MAX_BH_QUEUE_NODE_NUM || max_elements == 0) {return 0;}

	H = (X_PriorityQueue *)malloc (sizeof(entry));
	if(H == NULL) {return 0;}

	H ->current_size = 0;
	H ->max_node = max_elements;
	return H;
}
X_Void 					BH_PriorityQueueDestory(X_PriorityQueue * H)
{

}
X_Void 					BH_PriorityQueueClear(X_PriorityQueue *            H)
{

}
CURRENT_PRIORITY 		BH_PriorityQueueInsert(X_PriorityQueue * H,s_element_base * p_base)
{
    if(H == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}

	if(H ->current_size >= H ->max_node) {return INVALID_PRIOQUEUE_PRIORITY;}
	H ->current_size ++;
	return p_base ->priority;
}
CURRENT_PRIORITY		BH_PriorityQueueFindMin(X_PriorityQueue * H,s_element_base * p_base)
{
	if(H == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}

	return 30;
}
CURRENT_PRIORITY 		BH_PriorityQueueReleaseMin(X_PriorityQueue * H,s_element_base * p_base)
{
	if(H == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}

	if(H ->current_size > 0) {H ->current_size --;}
	return 30;
}
X_Boolean 				BH_DoesPriorityQueueEmpty(X_PriorityQueue * H)
{
	if(H == X_Null) {return X_True;}
	return X_True;
}
uint16_t 				BH_GetPriorityQueueUsedNodeNum(X_PriorityQueue * H)
{
	if(H == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	return H ->current_size;
}





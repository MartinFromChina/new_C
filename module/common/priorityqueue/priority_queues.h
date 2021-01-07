#ifndef __PRIORITY_QUEUES_H
#define __PRIORITY_QUEUES_H

#ifdef __cplusplus
 extern "C"{
#endif
/*
#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
*/
#include "../x_cross_platform.h"
#include "../CommonMarco.h"

#define INVALID_PRIOQUEUE_PRIORITY  (0xFFFF) 
#define MAX_PRIOQUEUE_PRIORITY ((uint16_t)(INVALID_PRIOQUEUE_PRIORITY - 1))

#define CURRENT_PRIORITY uint16_t

/*
 * scope : 0 ~ max_priority
 * table size = (max_priority % 32 == 0) ? scope / 32 + 1: scope / 32 + 1
 */
#define BIT_COUNT_IN_UINT32   32
#define GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(scope)    (uint16_t)((scope/BIT_COUNT_IN_UINT32) + 1)


typedef struct
{
 X_Boolean isInit;
 uint16_t current_used_bit_cnt;
}sPrioListparam;


typedef struct
{
	uint16_t  	max_priority;// 0 ~ max_priority
	uint16_t   table_size;
	uint32_t   *p_bit_table;
	sPrioListparam *p_PLP;
}sPrioListManager;


#define APP_BIT_TABLE_PRIORITYQUEUE_DEF_WITHOUT_POINTER() 


#define APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_manager,max_priority_value)   \
		static uint32_t  CONCAT_2(p_manager,_bit_table)[GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value)]; 	\
		static sPrioListparam CONCAT_2(p_manager,_prio_param) = {X_False,0};				\
		static const sPrioListManager CONCAT_2(p_manager,_prioqueue_entry)= {			\
			max_priority_value,												\
			GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value),	\
			CONCAT_2(p_manager,_bit_table),									\
			&CONCAT_2(p_manager,_prio_param),								\
		};																		\
		static const sPrioListManager *p_manager = &CONCAT_2(p_manager,_prioqueue_entry);


X_Void 				BT_PriorityQueueInit(const sPrioListManager *p_manager);
CURRENT_PRIORITY 	BT_PriorityQueueInsert(const sPrioListManager *p_manager,uint16_t prio_to_insert);
CURRENT_PRIORITY 	BT_PriorityQueueFindMin(const sPrioListManager *p_manager);
CURRENT_PRIORITY 	BT_PriorityQueueReleaseMin(const sPrioListManager *p_manager);
X_Void 				BT_PriorityQueueClear(const sPrioListManager *p_manager);
X_Boolean 			BT_DoesPriorityQueueEmpty(const sPrioListManager *p_manager);
uint16_t 			BT_GetPriorityQueueUsedNodeNum(const sPrioListManager *p_manager);


/**********************
*************************/

struct X_BinaryHeapStruct;
typedef struct X_BinaryHeapStruct *X_PriorityQueue;

X_PriorityQueue 		BH_PriorityQueueInit(uint16_t max_elements);
X_Void 					BH_PriorityQueueDestory(X_PriorityQueue H);
X_Void 					BH_PriorityQueueClear(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueInsert(X_PriorityQueue H,uint16_t priority);
CURRENT_PRIORITY 		BH_PriorityQueueFindMin(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueReleaseMin(X_PriorityQueue H);
X_Boolean 				BH_DoesPriorityQueueEmpty(X_PriorityQueue H);
uint16_t 				BH_GetPriorityQueueUsedNodeNum(X_PriorityQueue H);








#ifdef __cplusplus
}
#endif

#endif

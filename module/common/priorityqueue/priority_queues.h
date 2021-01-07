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
#define CURRENT_PRIORITY uint16_t

typedef struct
{
	CURRENT_PRIORITY     prio;
}sPrioListManager;


#define APP_BIT_TABLE_PRIORITYQUEUE_DEF_WITHOUT_POINTER() 
#define APP_BIT_TABLE_PRIORITYQUEUE_DEF(x,max_num)   static const sPrioListManager *x;


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

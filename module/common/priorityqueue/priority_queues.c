#include "priority_queues.h"

#define BUF_FREE   0
#define BUF_USED   1

#define USE_INLINE  

X_Void 				BT_PriorityQueueInit(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return;}



	p_manager ->p_PLP ->isInit = X_True;
}
CURRENT_PRIORITY 	BT_PriorityQueueInsert(const sPrioListManager *p_manager,uint16_t prio_to_insert)
{
	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}
	return 30;
}
CURRENT_PRIORITY 	BT_PriorityQueueFindMin(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	return 30;
}

CURRENT_PRIORITY 	BT_PriorityQueueReleaseMin(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	return 30;
}

X_Void 				BT_PriorityQueueClear(const sPrioListManager *p_manager)
{

}
X_Boolean 			BT_DoesPriorityQueueEmpty(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return X_True;}
	if(p_manager ->p_PLP -> isInit != X_True) {return X_True;}

	return X_False;
}
uint16_t 			BT_GetPriorityQueueUsedNodeNum(const sPrioListManager *p_manager)
{
	if(p_manager == X_Null) {return INVALID_PRIOQUEUE_PRIORITY;}
	if(p_manager ->p_PLP -> isInit != X_True) {return INVALID_PRIOQUEUE_PRIORITY;}

	return 0;
}


/***********************************************
X_PriorityQueue 		BH_PriorityQueueInit(uint16_t max_elements)
{
	return 0;
}
X_Void 					BH_PriorityQueueDestory(X_PriorityQueue H)
{

}
X_Void 					BH_PriorityQueueClear(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueInsert(X_PriorityQueue H,uint16_t priority);
CURRENT_PRIORITY 		BH_PriorityQueueFindMin(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueReleaseMin(X_PriorityQueue H);
X_Boolean 				BH_DoesPriorityQueueEmpty(X_PriorityQueue H);
uint16_t 				BH_GetPriorityQueueUsedNodeNum(X_PriorityQueue H);

****************************************************/



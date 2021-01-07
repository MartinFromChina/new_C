#include "priority_queues.h"

#define BUF_FREE   0
#define BUF_USED   1

#define USE_INLINE  

X_Void 				BT_PriorityQueueInit(const sPrioListManager *p_manager)
{

}
CURRENT_PRIORITY 	BT_PriorityQueueInsert(const sPrioListManager *p_manager,uint16_t prio_to_insert)
{
	return 0;
}
CURRENT_PRIORITY 	BT_PriorityQueueFindMin(const sPrioListManager *p_manager)
{
	return 0;
}

CURRENT_PRIORITY 	BT_PriorityQueueReleaseMin(const sPrioListManager *p_manager)
{
	return 0;
}

X_Void 				BT_PriorityQueueClear(const sPrioListManager *p_manager)
{

}
X_Boolean 			BT_DoesPriorityQueueEmpty(const sPrioListManager *p_manager)
{
	return X_True;
}
uint16_t 			BT_GetPriorityQueueUsedNodeNum(const sPrioListManager *p_manager)
{
	return 0;
}



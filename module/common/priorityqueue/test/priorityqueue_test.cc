#include <iostream>
#include "tdd_common.h"

#include "../priority_queues.h"

using namespace std;

/***********************
X_Void 				BT_PriorityQueueInit(const sPrioListManager *p_manager);
CURRENT_PRIORITY 	BT_PriorityQueueInsert(const sPrioListManager *p_manager,uint16_t prio_to_insert);
CURRENT_PRIORITY 	BT_PriorityQueueFindMin(const sPrioListManager *p_manager);
CURRENT_PRIORITY 	BT_PriorityQueueReleaseMin(const sPrioListManager *p_manager);
X_Void 				BT_PriorityQueueClear(const sPrioListManager *p_manager);
X_Boolean 			BT_DoesPriorityQueueEmpty(const sPrioListManager *p_manager);
uint16_t 			BT_GetPriorityQueueUsedNodeNum(const sPrioListManager *p_manager);

*************************/

static  uint16_t buf_number = 0;
#define OPERATION_TIMES 0xfffe
#define NORMAL_PRIORITY_SCOPE  500
APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_prio_queue,NORMAL_PRIORITY_SCOPE);

TEST(queue_test,init)
{
	X_Boolean isOK = X_False;

	do{
		buf_number = BT_PriorityQueueInsert(p_prio_queue,30);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		buf_number = BT_PriorityQueueFindMin(p_prio_queue);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		buf_number = BT_GetPriorityQueueUsedNodeNum(p_prio_queue);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		isOK = BT_DoesPriorityQueueEmpty(p_prio_queue);
		EXPECT_EQ(isOK ,X_True);
		//UNUSED_VARIABLE(data_buf);
		
		BT_PriorityQueueInit(p_prio_queue);
		buf_number = BT_PriorityQueueInsert(p_prio_queue,30);
		EXPECT_EQ(buf_number ,30);

		buf_number = BT_PriorityQueueFindMin(p_prio_queue);
		EXPECT_EQ(buf_number ,30);

		buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
		EXPECT_EQ(buf_number ,30);

		buf_number = BT_GetPriorityQueueUsedNodeNum(p_prio_queue);
		EXPECT_EQ(buf_number ,0);

		isOK = BT_DoesPriorityQueueEmpty(p_prio_queue);
		
	}while(0);
	
};

/************************
X_PriorityQueue 		BH_PriorityQueueInit(uint16_t max_elements);
X_Void 					BH_PriorityQueueDestory(X_PriorityQueue H);
X_Void 					BH_PriorityQueueClear(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueInsert(X_PriorityQueue H,uint16_t priority);
CURRENT_PRIORITY 		BH_PriorityQueueFindMin(X_PriorityQueue H);
CURRENT_PRIORITY 		BH_PriorityQueueReleaseMin(X_PriorityQueue H);
X_boolean 				BH_DoesPriorityQueueEmpty(X_PriorityQueue H);
uint16_t 				BH_GetPriorityQueueUsedNodeNum(X_PriorityQueue H);

*************************/

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from priorityqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
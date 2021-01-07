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

static  uint16_t buf_number;
#define OPERATION_TIMES 0xfffe
APP_PRIORITYQUEUE_DEF_WITHOUT_POINTER(p_prio_queue,5000);

TEST(queue_test,normal)
{
	uint16_t i = 200;
	static 	uint8_t data_buf[200];
	PriorityQueueInit(p_prio_queue);

	do{
		buf_number = PriorityQueuePush(p_queue,X_False);
		EXPECT_LT(buf_number,200);
		
		data_buf[buf_number] = i;
			//printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
			UNUSED_VARIABLE(data_buf);
		
		buf_number = SimpleQueueFirstOut(p_queue);
		EXPECT_LT(buf_number,200);
		EXPECT_EQ(200-i,buf_number);
		
	}while(i-- > 1);
	
};

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from loopqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
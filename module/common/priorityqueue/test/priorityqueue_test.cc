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


#if (USE_TDD_PRIORITY_QUEUE_INTERNAL_TERST == 1)
TEST(queue_test,prio_bit_table_insert)
{
	uint32_t table_value;
	uint16_t table_index = 0;
	table_value = InsertPrioTable(1,&table_index);
	EXPECT_EQ(table_value,0x40000000);
	EXPECT_EQ(table_index,0);

	table_value = InsertPrioTable(100,&table_index);
	EXPECT_EQ(table_value,0x08000000); 
	EXPECT_EQ(table_index,3);
}

#else

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

TEST(queue_test,prio_scope)
{
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_1,MAX_PRIOQUEUE_PRIORITY);
	BT_PriorityQueueInit(p_1);
	EXPECT_EQ(0,BT_GetPriorityQueueUsedNodeNum(p_1));
	
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_2,MAX_PRIOQUEUE_PRIORITY + 1);
	BT_PriorityQueueInit(p_2);
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BT_GetPriorityQueueUsedNodeNum(p_2));

	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_3,MAX_PRIOQUEUE_PRIORITY - 1);
	BT_PriorityQueueInit(p_3);
	EXPECT_EQ(0,BT_GetPriorityQueueUsedNodeNum(p_3));
	


}

TEST(queue_test,normal_insert)
{
	BT_PriorityQueueInit(p_prio_queue);
	
	buf_number = BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(buf_number,100);
	EXPECT_EQ(100,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,101);
	EXPECT_EQ(buf_number,101);
	EXPECT_EQ(100,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,3);
	EXPECT_EQ(buf_number,3);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,98);
	EXPECT_EQ(buf_number,98);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,499);
	EXPECT_EQ(buf_number,499);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,0);
	EXPECT_EQ(buf_number,0);
	EXPECT_EQ(0,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,17);
	EXPECT_EQ(buf_number,17);
	EXPECT_EQ(0,BT_PriorityQueueFindMin(p_prio_queue));
}
#endif
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

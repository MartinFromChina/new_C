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
	
}

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

	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_4,1);
	BT_PriorityQueueInit(p_4);
	EXPECT_EQ(0,BT_GetPriorityQueueUsedNodeNum(p_4));
	EXPECT_EQ(1,BT_PriorityQueueInsert(p_4,1));
	EXPECT_EQ(0,BT_PriorityQueueInsert(p_4,0));
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p_4));

	
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p_5,0);
	BT_PriorityQueueInit(p_5);
	EXPECT_EQ(0,BT_GetPriorityQueueUsedNodeNum(p_5));
	EXPECT_EQ(0,BT_PriorityQueueInsert(p_5,0));
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BT_PriorityQueueInsert(p_5,1));
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_5));

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
TEST(queue_test,same_priority_insert)
{
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p1,65534);
	BT_PriorityQueueInit(p1);

	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	EXPECT_EQ(32767,BT_PriorityQueueFindMin(p1));
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p1));

	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	EXPECT_EQ(255,BT_PriorityQueueFindMin(p1));
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p1));
}

TEST(queue_test,normal_delete_min)
{
	BT_PriorityQueueInit(p_prio_queue);
	
	buf_number = BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(buf_number,100);
	EXPECT_EQ(100,BT_PriorityQueueReleaseMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,101);
	EXPECT_EQ(buf_number,101);
	EXPECT_EQ(101,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,3);
	EXPECT_EQ(buf_number,3);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,98);
	EXPECT_EQ(buf_number,98);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,499);
	EXPECT_EQ(buf_number,499);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));


	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,3);
	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,98);
	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,101);

	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,499);
	
}
TEST(queue_test,clear)
{
	BT_PriorityQueueInit(p_prio_queue);

	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,INVALID_PRIOQUEUE_PRIORITY);

	BT_PriorityQueueInsert(p_prio_queue,100);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,100);

	BT_PriorityQueueInsert(p_prio_queue,99);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,99);

	BT_PriorityQueueClear(p_prio_queue);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,INVALID_PRIOQUEUE_PRIORITY);

}

TEST(queue_test,get_uesd_node_mum)
{
	uint16_t i;
	BT_PriorityQueueInit(p_prio_queue);
	buf_number = BT_GetPriorityQueueUsedNodeNum(p_prio_queue);
	EXPECT_EQ(buf_number,0);

	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueInsert(p_prio_queue,99);
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,98);
	EXPECT_EQ(3,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueInsert(p_prio_queue,i%NORMAL_PRIORITY_SCOPE);
	}
	EXPECT_EQ(NORMAL_PRIORITY_SCOPE,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(NORMAL_PRIORITY_SCOPE-1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	
}

TEST(queue_test,does_empty)
{
	uint16_t i;
	BT_PriorityQueueInit(p_prio_queue);

	EXPECT_EQ(X_True, BT_DoesPriorityQueueEmpty(p_prio_queue));

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueInsert(p_prio_queue,i%NORMAL_PRIORITY_SCOPE);
	}

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueReleaseMin(p_prio_queue);
		if(BT_DoesPriorityQueueEmpty(p_prio_queue) == X_True) {break;}
	}
	EXPECT_EQ(i, 499);

	
}

TEST(queue_test,boundary)
{

}

TEST(queue_test,mul_entry)
{
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p2,500);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p3,1000);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p4,2680);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p5,779);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p6,456);

	BT_PriorityQueueInit(p2);
	BT_PriorityQueueInit(p3);
	BT_PriorityQueueInit(p4);
	BT_PriorityQueueInit(p5);
	BT_PriorityQueueInit(p6);

	BT_PriorityQueueInsert(p2,288);
	BT_PriorityQueueInsert(p6,288);
	BT_PriorityQueueInsert(p5,288);
	BT_PriorityQueueInsert(p3,288);
	BT_PriorityQueueInsert(p4,288);

	
	BT_PriorityQueueInsert(p2,100);
	BT_PriorityQueueInsert(p6,200);
	BT_PriorityQueueInsert(p5,98);
	BT_PriorityQueueInsert(p3,128);
	BT_PriorityQueueInsert(p4,256);

	BT_PriorityQueueReleaseMin(p3);
	BT_PriorityQueueReleaseMin(p5);
	
	EXPECT_EQ(100, BT_PriorityQueueFindMin(p2));
	EXPECT_EQ(288, BT_PriorityQueueFindMin(p3));
	EXPECT_EQ(256, BT_PriorityQueueFindMin(p4));
	EXPECT_EQ(288, BT_PriorityQueueFindMin(p5));
	EXPECT_EQ(200, BT_PriorityQueueFindMin(p6));

	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p2));
	EXPECT_EQ(1, BT_GetPriorityQueueUsedNodeNum(p3));
	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p4));
	EXPECT_EQ(1, BT_GetPriorityQueueUsedNodeNum(p5));
	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p6));
	
	
}
TEST(queue_test,operation_speed)
{

}




#endif
/************************
X_PriorityQueue *		BH_PriorityQueueInit(uint16_t max_elements);
X_Void 					BH_PriorityQueueDestory(X_PriorityQueue * H);
X_Void 					BH_PriorityQueueClear(X_PriorityQueue * H);
CURRENT_PRIORITY 		BH_PriorityQueueInsert(X_PriorityQueue * H,s_element_base * p_base);
CURRENT_PRIORITY		BH_PriorityQueueFindMin(X_PriorityQueue * H,s_element_base * p_base);
CURRENT_PRIORITY 		BH_PriorityQueueReleaseMin(X_PriorityQueue * H,s_element_base * p_base);
X_Boolean 				BH_DoesPriorityQueueEmpty(X_PriorityQueue * H);
uint16_t 				BH_GetPriorityQueueUsedNodeNum(X_PriorityQueue * H);
*************************/
#define NORMAL_NODE_SCOPE   500
typedef struct 
{
	s_element_base base;
	uint16_t other_info;
}s_element_extern;

static s_element_extern s_ee[NORMAL_NODE_SCOPE];

TEST(BH_prio_queue,init)
{
	X_PriorityQueue *p_s1 = 0;
	X_Boolean isOK = X_False;

	do{		
		buf_number = BH_PriorityQueueFindMin(p_s1,&s_ee[499].base);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		buf_number = BH_PriorityQueueReleaseMin(p_s1,&s_ee[499].base);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		buf_number = BH_GetPriorityQueueUsedNodeNum(p_s1);
		EXPECT_EQ(buf_number ,INVALID_PRIOQUEUE_PRIORITY);

		isOK = BH_DoesPriorityQueueEmpty(p_s1);
		EXPECT_EQ(isOK ,X_True);
		//UNUSED_VARIABLE(data_buf);

		s_ee[0].base.priority = 30;
		
		p_s1 = BH_PriorityQueueInit(NORMAL_NODE_SCOPE);
		buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[0].base);
		EXPECT_EQ(buf_number ,30);

		buf_number = BH_PriorityQueueFindMin(p_s1,&s_ee[499].base);
		EXPECT_EQ(buf_number ,30);

		buf_number = BH_PriorityQueueReleaseMin(p_s1,&s_ee[499].base);
		EXPECT_EQ(buf_number ,30);

		buf_number = BH_GetPriorityQueueUsedNodeNum(p_s1);
		EXPECT_EQ(buf_number ,0);

		isOK = BH_DoesPriorityQueueEmpty(p_s1);
		EXPECT_EQ(isOK ,X_True);
	}while(0);
	
}

TEST(BH_prio_queue,node_scope)
{
	X_PriorityQueue *p_s1 = 0,*p_s2 = 0,*p_s3 = 0,*p_s4 = 0,*p_s5 = 0;

	p_s1 = BH_PriorityQueueInit(MAX_BH_QUEUE_NODE_NUM);
	EXPECT_EQ(0,BH_GetPriorityQueueUsedNodeNum(p_s1));
	
	p_s2 = BH_PriorityQueueInit(MAX_BH_QUEUE_NODE_NUM + 1);
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_GetPriorityQueueUsedNodeNum(p_s2));

	p_s3 = BH_PriorityQueueInit(MAX_BH_QUEUE_NODE_NUM - 1);
	EXPECT_EQ(0,BH_GetPriorityQueueUsedNodeNum(p_s3));


	s_ee[0].base.priority = 1;
	s_ee[1].base.priority = 0;

	p_s4 = BH_PriorityQueueInit(1);
	EXPECT_EQ(0,BH_GetPriorityQueueUsedNodeNum(p_s4));
	EXPECT_EQ(1,BH_PriorityQueueInsert(p_s4,&s_ee[0].base));
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_PriorityQueueInsert(p_s4,&s_ee[1].base));
	EXPECT_EQ(1,BH_GetPriorityQueueUsedNodeNum(p_s4));

	
	p_s5 = BH_PriorityQueueInit(0);
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_GetPriorityQueueUsedNodeNum(p_s5));
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_PriorityQueueInsert(p_s5,&s_ee[1].base));
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_PriorityQueueInsert(p_s5,&s_ee[0].base));
	EXPECT_EQ(INVALID_PRIOQUEUE_PRIORITY,BH_GetPriorityQueueUsedNodeNum(p_s5));

}

TEST(BH_prio_queue,normal_insert)
{
	X_PriorityQueue *p_s1;

	s_ee[0].base.priority = 100;
	s_ee[1].base.priority = 101;
	s_ee[2].base.priority = 3;
	s_ee[3].base.priority = 98;
	s_ee[4].base.priority = 499;
	s_ee[5].base.priority = 0;
	s_ee[6].base.priority = 17;

	p_s1 = BH_PriorityQueueInit(NORMAL_NODE_SCOPE);
	
	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[0].base);
	EXPECT_EQ(buf_number,100);
	EXPECT_EQ(100,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(100,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[1].base);
	EXPECT_EQ(buf_number,101);
	EXPECT_EQ(100,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(100,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[2].base);
	EXPECT_EQ(buf_number,3);
	EXPECT_EQ(3,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(3,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[3].base);
	EXPECT_EQ(buf_number,98);
	EXPECT_EQ(3,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(3,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[4].base);
	EXPECT_EQ(buf_number,499);
	EXPECT_EQ(3,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(3,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[5].base);
	EXPECT_EQ(buf_number,0);
	EXPECT_EQ(0,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(0,s_ee[499].base.priority);

	buf_number = BH_PriorityQueueInsert(p_s1,&s_ee[6].base);
	EXPECT_EQ(buf_number,17);
	EXPECT_EQ(0,BH_PriorityQueueFindMin(p_s1,&s_ee[499].base));
	EXPECT_EQ(17,s_ee[499].base.priority);
}
/*

TEST(BH_prio_queue,same_priority_insert)
{
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p1,65534);
	BT_PriorityQueueInit(p1);

	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	buf_number = BT_PriorityQueueInsert(p1,32767);
	EXPECT_EQ(32767,BT_PriorityQueueFindMin(p1));
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p1));

	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	buf_number = BT_PriorityQueueInsert(p1,255);
	EXPECT_EQ(255,BT_PriorityQueueFindMin(p1));
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p1));
}

TEST(BH_prio_queue,normal_delete_min)
{
	BT_PriorityQueueInit(p_prio_queue);
	
	buf_number = BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(buf_number,100);
	EXPECT_EQ(100,BT_PriorityQueueReleaseMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,101);
	EXPECT_EQ(buf_number,101);
	EXPECT_EQ(101,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,3);
	EXPECT_EQ(buf_number,3);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,98);
	EXPECT_EQ(buf_number,98);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));

	buf_number = BT_PriorityQueueInsert(p_prio_queue,499);
	EXPECT_EQ(buf_number,499);
	EXPECT_EQ(3,BT_PriorityQueueFindMin(p_prio_queue));


	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,3);
	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,98);
	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,101);

	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,499);
	
}
TEST(BH_prio_queue,clear)
{
	BT_PriorityQueueInit(p_prio_queue);

	buf_number = BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(buf_number,INVALID_PRIOQUEUE_PRIORITY);

	BT_PriorityQueueInsert(p_prio_queue,100);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,100);

	BT_PriorityQueueInsert(p_prio_queue,99);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,99);

	BT_PriorityQueueClear(p_prio_queue);
	buf_number = BT_PriorityQueueFindMin(p_prio_queue);
	EXPECT_EQ(buf_number,INVALID_PRIOQUEUE_PRIORITY);

}

TEST(BH_prio_queue,get_uesd_node_mum)
{
	uint16_t i;
	BT_PriorityQueueInit(p_prio_queue);
	buf_number = BT_GetPriorityQueueUsedNodeNum(p_prio_queue);
	EXPECT_EQ(buf_number,0);

	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,100);
	EXPECT_EQ(1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueInsert(p_prio_queue,99);
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	BT_PriorityQueueInsert(p_prio_queue,98);
	EXPECT_EQ(3,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(2,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueInsert(p_prio_queue,i%NORMAL_PRIORITY_SCOPE);
	}
	EXPECT_EQ(NORMAL_PRIORITY_SCOPE,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));

	BT_PriorityQueueReleaseMin(p_prio_queue);
	EXPECT_EQ(NORMAL_PRIORITY_SCOPE-1,BT_GetPriorityQueueUsedNodeNum(p_prio_queue));
	
}

TEST(BH_prio_queue,does_empty)
{
	uint16_t i;
	BT_PriorityQueueInit(p_prio_queue);

	EXPECT_EQ(X_True, BT_DoesPriorityQueueEmpty(p_prio_queue));

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueInsert(p_prio_queue,i%NORMAL_PRIORITY_SCOPE);
	}

	for(i=0;i<2000;i++)
	{
		BT_PriorityQueueReleaseMin(p_prio_queue);
		if(BT_DoesPriorityQueueEmpty(p_prio_queue) == X_True) {break;}
	}
	EXPECT_EQ(i, 499);

	
}

TEST(BH_prio_queue,boundary)
{

}

TEST(BH_prio_queue,mul_entry)
{
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p2,500);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p3,1000);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p4,2680);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p5,779);
	APP_BIT_TABLE_PRIORITYQUEUE_DEF(p6,456);

	BT_PriorityQueueInit(p2);
	BT_PriorityQueueInit(p3);
	BT_PriorityQueueInit(p4);
	BT_PriorityQueueInit(p5);
	BT_PriorityQueueInit(p6);

	BT_PriorityQueueInsert(p2,288);
	BT_PriorityQueueInsert(p6,288);
	BT_PriorityQueueInsert(p5,288);
	BT_PriorityQueueInsert(p3,288);
	BT_PriorityQueueInsert(p4,288);

	
	BT_PriorityQueueInsert(p2,100);
	BT_PriorityQueueInsert(p6,200);
	BT_PriorityQueueInsert(p5,98);
	BT_PriorityQueueInsert(p3,128);
	BT_PriorityQueueInsert(p4,256);

	BT_PriorityQueueReleaseMin(p3);
	BT_PriorityQueueReleaseMin(p5);
	
	EXPECT_EQ(100, BT_PriorityQueueFindMin(p2));
	EXPECT_EQ(288, BT_PriorityQueueFindMin(p3));
	EXPECT_EQ(256, BT_PriorityQueueFindMin(p4));
	EXPECT_EQ(288, BT_PriorityQueueFindMin(p5));
	EXPECT_EQ(200, BT_PriorityQueueFindMin(p6));

	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p2));
	EXPECT_EQ(1, BT_GetPriorityQueueUsedNodeNum(p3));
	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p4));
	EXPECT_EQ(1, BT_GetPriorityQueueUsedNodeNum(p5));
	EXPECT_EQ(2, BT_GetPriorityQueueUsedNodeNum(p6));
	
	
}
TEST(BH_prio_queue,operation_speed)
{

}
*/



GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from priorityqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

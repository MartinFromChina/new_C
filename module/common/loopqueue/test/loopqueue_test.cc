#include <iostream>
#include "tdd_common.h"

#include "../loop_queues.h"

using namespace std;

static  uint16_t buf_number;
SIMPLE_LOOPQUEUE_DEF(p_queue,200);
#define OPERATION_TIMES 0xfffe

TEST(queue_test,normal)
{
	uint16_t i = 200;
	static 	uint8_t data_buf[200];
	SimpleQueueInitialize(p_queue);

	do{
		buf_number = SimpleQueueFirstIn(p_queue,X_False);
		EXPECT_LT(buf_number,200);
		
		data_buf[buf_number] = i;
			//printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
			UNUSED_VARIABLE(data_buf);
		
		buf_number = SimpleQueueFirstOut(p_queue);
		EXPECT_LT(buf_number,200);
		EXPECT_EQ(200-i,buf_number);
		
	}while(i-- > 1);
	
};


TEST(queue_test,big_amount_push_pop)
{
	uint16_t i = OPERATION_TIMES;
	SimpleQueueInitialize(p_queue);


	do{
		buf_number = SimpleQueueFirstIn(p_queue,X_True);
		EXPECT_LT(buf_number,OPERATION_TIMES);
		
		buf_number = SimpleQueueFirstOut(p_queue);
		EXPECT_EQ((OPERATION_TIMES-i)%200,buf_number);
		
	}while(i-- > 1);
	
};



TEST(queue_test,node_occupy_test)
{
	SIMPLE_LOOPQUEUE_DEF(p_big_queue,10000);
	uint16_t i = 0;
	
	SimpleQueueInitialize(p_big_queue);

	do{
		buf_number = SimpleQueueFirstIn(p_big_queue,X_False);
		if(i < 10000)
		{
			EXPECT_EQ(buf_number,i);
		}
		else
		{
			EXPECT_EQ(buf_number,INVALID_NODE_NUM);
		}
		
	}while((i++) < (OPERATION_TIMES - 1));
	
	EXPECT_EQ(10000,GetSimpleQueueUsedNodeNumber(p_big_queue));
	
	i = 0;
	do{
		buf_number = SimpleQueueFirstOut(p_big_queue);
		EXPECT_EQ(i,buf_number);
		
	}while(i++ < 9999);
	
};

#define BIG_QUEUE_SIZE (10000)
TEST(queue_test,time_test)
{
	uint16_t i = OPERATION_TIMES,j;
	SIMPLE_LOOPQUEUE_DEF(p_big_queue,BIG_QUEUE_SIZE);
	
	for(j=0;j<10;j++)
	{
		do{
		buf_number = SimpleQueueFirstIn(p_big_queue,X_True);
		EXPECT_LT(buf_number,OPERATION_TIMES);
		
		buf_number = SimpleQueueFirstOut(p_big_queue);
		EXPECT_EQ((OPERATION_TIMES-i)%BIG_QUEUE_SIZE,buf_number);
		
		}while(i-- > 1);
		
		i = OPERATION_TIMES;
		ClearSimpleQueue(p_big_queue);
		
	}
	
	
	
	
	
};

TEST(queue_test,mul_entry)
{
	uint16_t i = 10000;
	uint16_t buf_number1,buf_number2,buf_number3,buf_number4,buf_number5;
	SIMPLE_LOOPQUEUE_DEF(p_queue_1,BIG_QUEUE_SIZE);
	SIMPLE_LOOPQUEUE_DEF(p_queue_2,BIG_QUEUE_SIZE/2);
	SIMPLE_LOOPQUEUE_DEF(p_queue_3,BIG_QUEUE_SIZE/4);
	SIMPLE_LOOPQUEUE_DEF(p_queue_4,BIG_QUEUE_SIZE -255);
	SIMPLE_LOOPQUEUE_DEF(p_queue_5,BIG_QUEUE_SIZE * 2);
	
	do{
	buf_number1 = SimpleQueueFirstIn(p_queue_1,X_False);
	
	buf_number2 = SimpleQueueFirstIn(p_queue_2,X_True);
	buf_number2 = SimpleQueueFirstIn(p_queue_2,X_True);
	
	buf_number5 = SimpleQueueFirstIn(p_queue_5,X_False);
	buf_number5 = SimpleQueueFirstIn(p_queue_5,X_False);
	buf_number5 = SimpleQueueFirstIn(p_queue_5,X_False);
	
	}while(i-- > 1);
	
	
	EXPECT_EQ(buf_number1,9999);
	EXPECT_EQ(buf_number2,4999);
	EXPECT_EQ(buf_number5,INVALID_NODE_NUM);
	
	EXPECT_EQ(10000,GetSimpleQueueUsedNodeNumber(p_queue_1));
	EXPECT_EQ(5000,GetSimpleQueueUsedNodeNumber(p_queue_2));
	EXPECT_EQ(20000,GetSimpleQueueUsedNodeNumber(p_queue_5));
	EXPECT_EQ(0,GetSimpleQueueUsedNodeNumber(p_queue_4));
	
	buf_number5 = SimpleQueueFirstOut(p_queue_5);
	EXPECT_EQ(19999,GetSimpleQueueUsedNodeNumber(p_queue_5));
	EXPECT_EQ(buf_number5,0);
	
	UNUSED_VARIABLE(p_queue_3);
	UNUSED_VARIABLE(buf_number3);
	UNUSED_VARIABLE(buf_number4);
};


GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from loopqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
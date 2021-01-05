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


GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from loopqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include <iostream>
#include "tdd_common.h"

#include "../mStack.h"
using namespace std;
/*
#define INVALID_LOOP_QUEUE_NODE_NUM  (0xFFFF)

X_Void 		LoopQueueInitialize(const sListManager *p_manager);
uint16_t    LoopQueueFirstIn(const sListManager *p_manager,X_Boolean is_OccupyPermit);
uint16_t    LoopQueueFirstOut(const sListManager *p_manager);
X_Void      ClearLoopQueue(const sListManager *p_manager);
X_Void      RealseLoopQueueBuf(const sListManager *p_manager,uint8_t buf_num);
uint16_t    GetLoopQueueUsedNodeNumber(const sListManager *p_manager);
X_Boolean   DoesLoopQueueEmpty(const sListManager *p_manager);
*/
/***********************


static  uint16_t buf_number;
APP_LOOPQUEUE_DEF(p_queue,200);
#define OPERATION_TIMES 0xfffe

TEST(queue_test,init)
{
	X_Boolean isEmpty;
	buf_number = LoopQueueFirstIn(p_queue,X_True);
	EXPECT_EQ(buf_number,INVALID_LOOP_QUEUE_NODE_NUM);
	
    buf_number = LoopQueueFirstOut(p_queue);
	EXPECT_EQ(buf_number,INVALID_LOOP_QUEUE_NODE_NUM);
	
    buf_number = GetLoopQueueUsedNodeNumber(p_queue);
	EXPECT_EQ(buf_number,0);
	
    isEmpty = DoesLoopQueueEmpty(p_queue);
	EXPECT_EQ(isEmpty,X_True);
	
	
	LoopQueueInitialize(p_queue);
	buf_number = LoopQueueFirstIn(p_queue,X_True);
	EXPECT_EQ(buf_number,0);
	
    buf_number = LoopQueueFirstOut(p_queue);
	EXPECT_EQ(buf_number,0);
	
    buf_number = GetLoopQueueUsedNodeNumber(p_queue);
	EXPECT_EQ(buf_number,0);
	
    isEmpty = DoesLoopQueueEmpty(p_queue);
	EXPECT_EQ(isEmpty,X_True);
};


TEST(queue_test,normal)
{
	uint16_t i = 200;
	static 	uint8_t data_buf[200];
	LoopQueueInitialize(p_queue);

	do{
		buf_number = LoopQueueFirstIn(p_queue,X_False);
		EXPECT_LT(buf_number,200);
		
		data_buf[buf_number] = i;
			//printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
			UNUSED_VARIABLE(data_buf);
		
		buf_number = LoopQueueFirstOut(p_queue);
		EXPECT_LT(buf_number,200);
		EXPECT_EQ(200-i,buf_number);
		
	}while(i-- > 1);
	
};


TEST(queue_test,big_amount_push_pop)
{
	uint16_t i = OPERATION_TIMES;
	LoopQueueInitialize(p_queue);


	do{
		buf_number = LoopQueueFirstIn(p_queue,X_True);
		EXPECT_LT(buf_number,OPERATION_TIMES);
		
		buf_number = LoopQueueFirstOut(p_queue);
		EXPECT_EQ((OPERATION_TIMES-i)%200,buf_number);
		
	}while(i-- > 1);
	
};



TEST(queue_test,node_occupy_test)
{
	APP_LOOPQUEUE_DEF(p_big_queue,10000);
	uint16_t i = 0;
	
	LoopQueueInitialize(p_big_queue);

	do{
		buf_number = LoopQueueFirstIn(p_big_queue,X_False);
		if(i < 10000)
		{
			EXPECT_EQ(buf_number,i);
		}
		else
		{
			EXPECT_EQ(buf_number,INVALID_LOOP_QUEUE_NODE_NUM);
		}
		
	}while((i++) < (OPERATION_TIMES - 1));
	
	EXPECT_EQ(10000,GetLoopQueueUsedNodeNumber(p_big_queue));
	
	i = 0;
	do{
		buf_number = LoopQueueFirstOut(p_big_queue);
		EXPECT_EQ(i,buf_number);
		
	}while(i++ < 9999);
	
};

#define BIG_QUEUE_SIZE (10000)
TEST(queue_test,time_test)
{
	uint16_t i = OPERATION_TIMES,j;
	APP_LOOPQUEUE_DEF(p_big_queue,BIG_QUEUE_SIZE);
	LoopQueueInitialize(p_big_queue);
	
	for(j=0;j<10;j++)
	{
		do{
		buf_number = LoopQueueFirstIn(p_big_queue,X_True);
		EXPECT_LT(buf_number,OPERATION_TIMES);
		
		buf_number = LoopQueueFirstOut(p_big_queue);
		EXPECT_EQ((OPERATION_TIMES-i)%BIG_QUEUE_SIZE,buf_number);
		
		}while(i-- > 1);
		
		i = OPERATION_TIMES;
		ClearLoopQueue(p_big_queue);
		
	}
	
	
	
	
	
};

TEST(queue_test,mul_entry)
{
	uint16_t i = 10000;
	uint16_t buf_number1,buf_number2,buf_number3,buf_number4,buf_number5;
	APP_LOOPQUEUE_DEF(p_queue_1,BIG_QUEUE_SIZE);
	APP_LOOPQUEUE_DEF(p_queue_2,BIG_QUEUE_SIZE/2);
	APP_LOOPQUEUE_DEF(p_queue_3,BIG_QUEUE_SIZE/4);
	APP_LOOPQUEUE_DEF(p_queue_4,BIG_QUEUE_SIZE -255);
	APP_LOOPQUEUE_DEF(p_queue_5,BIG_QUEUE_SIZE * 2);
	
	LoopQueueInitialize(p_queue_1);
	LoopQueueInitialize(p_queue_2);
	LoopQueueInitialize(p_queue_3);
	LoopQueueInitialize(p_queue_4);
	LoopQueueInitialize(p_queue_5);
	
	do{
	buf_number1 = LoopQueueFirstIn(p_queue_1,X_False);
	
	buf_number2 = LoopQueueFirstIn(p_queue_2,X_True);
	buf_number2 = LoopQueueFirstIn(p_queue_2,X_True);
	
	buf_number5 = LoopQueueFirstIn(p_queue_5,X_False);
	buf_number5 = LoopQueueFirstIn(p_queue_5,X_False);
	buf_number5 = LoopQueueFirstIn(p_queue_5,X_False);
	
	}while(i-- > 1);
	
	
	EXPECT_EQ(buf_number1,9999);
	EXPECT_EQ(buf_number2,4999);
	EXPECT_EQ(buf_number5,INVALID_LOOP_QUEUE_NODE_NUM);
	
	EXPECT_EQ(10000,GetLoopQueueUsedNodeNumber(p_queue_1));
	EXPECT_EQ(5000,GetLoopQueueUsedNodeNumber(p_queue_2));
	EXPECT_EQ(20000,GetLoopQueueUsedNodeNumber(p_queue_5));
	EXPECT_EQ(0,GetLoopQueueUsedNodeNumber(p_queue_4));
	
	buf_number5 = LoopQueueFirstOut(p_queue_5);
	EXPECT_EQ(19999,GetLoopQueueUsedNodeNumber(p_queue_5));
	EXPECT_EQ(buf_number5,0);
	
	UNUSED_VARIABLE(p_queue_3);
	UNUSED_VARIABLE(buf_number3);
	UNUSED_VARIABLE(buf_number4);
};

***************************/
TEST(empty,test)
{

}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from stack_test.cc\n";
  // ::testing::GTEST_FLAG(filter) = "*class*:*mode*";

  testing::InitGoogleTest(&argc, argv);
  #if (WINDOWS_64_SYSTEM == 0)
  	return
  #else
	int value = 
  #endif
  RUN_ALL_TESTS();

  while(1)
  {
	
  }

#if (WINDOWS_64_SYSTEM!=0)
  value = value;
  while(1)
  {

  }
#endif
}
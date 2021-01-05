#include <iostream>
#include "tdd_common.h"

#include "../loop_queues.h"

#define MAX_LONG_QUEUE_LENGTH   270

static  uint16_t buf_number;
static 	uint8_t data_buf[MAX_LONG_QUEUE_LENGTH];

//using namespace std;
SIMPLE_LOOPQUEUE_DEF(p_queue,200);

TEST(queue_test,normal)
{
	uint16_t i = 200;
	SimpleQueueInitialize(p_queue);


	do{
		buf_number = SimpleQueueFirstIn(p_queue,X_False);
		EXPECT_LT(buf_number,200);
		
		data_buf[buf_number] = i;
			//printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
		
		buf_number = SimpleQueueFirstOut(p_queue);
		EXPECT_LT(buf_number,200);
		EXPECT_EQ(200-i,buf_number);
		
	}while(i-- > 1);
	
};


GTEST_API_ int main(int argc, char **argv) {
  //cout<<"Running main() from loopqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
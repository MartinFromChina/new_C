#include <iostream>
#include "tdd_common.h"

#include "loop_queues.h"
#define MAX_LONG_QUEUE_LENGTH   270

static  uint16_t buf_number;
static 	uint8_t data_buf[MAX_LONG_QUEUE_LENGTH];

//using namespace std;
SIMPLE_LOOPQUEUE_DEF(p_queue,200);

TEST(sort_test,from_small_to_big)
{
	uint16_t i = 200;
	X_Boolean isOK;
	SimpleQueueInitialize(p_queue);


	do{
		buf_number = SimpleQueueFirstIn(p_queue,&isOK,X_False);
		EXPECT_EQ(X_True,isOK);
		{
			data_buf[buf_number] = i;
			//printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
		}
	buf_number = SimpleQueueFirstOut(p_queue_manager,&isOK);
	EXPECT_EQ(X_True,isOK);
		
	}while(i-- > 1)
	
}

GTEST_API_ int main(int argc, char **argv) {
  //cout<<"Running main() from loopqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
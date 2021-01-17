#include "thread_test.h"
#include <iostream>
#include <thread> 
using namespace std;

static uint8_t globle_buf[10000];
static uint16_t j = 0;
void mainloop(void) 
{
	uint32_t i;
	for(i=0;i<1000;i++)
	{
		globle_buf[j] = 0x55;
		if(j<9999){j++;}
		usleep(2);
	}
}
void irq_handler(void) 
{
	uint32_t i;
	for(i=0;i<100;i++)
	{
		globle_buf[j] = 0xaa;
		if(j<9999){j++;}
		usleep(1);
	}
}

TEST(MultiThread, Test) {
   uint16_t i;
   uint8_t temp;
   std::thread irq(irq_handler);
   mainloop();
   irq.join();
   for(i=0;i<j;i++)
   {
   		temp = globle_buf[i];
		cout<<"globle_buf["<<dec<<i<<"]"<<"is "<<hex<<(int*)temp<<endl;
   }
	   
}


GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running test from thread_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


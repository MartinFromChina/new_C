#include "uart_test.h"
#include <iostream>
#include <thread> 
using namespace std;

static uint8_t globle_buf[10000];
static uint16_t j = 0;

#include <fstream>
#include <string>
static uint32_t 	linenum1[2000];
static uint8_t 		buf1[2000];

int WriteTxT_by_line(const string &filename, uint16_t total_line,uint32_t *p_line_num,uint8_t *p_value)
{
	uint16_t i;
	FILE *fp = fopen(filename.c_str(), "w");
	if (!fp) 
	{ 
		cout << "File open error!\n"; 
		return 0; 
	}
	for(i = 0;i<total_line;i++)
	{
		fprintf(fp,"%d  %d \r\n",p_line_num[i],p_value[i]);
	}
	
	fclose(fp);
	return 1;

}

void mainloop(void) 
{
	uint32_t i;
	for(i=0;i<60;i++)
	{
		globle_buf[j] = 1;
		if(j<9999){j++;}
		usleep(2);
	}
}
#include <cstdlib> // Header file needed to use srand and rand
#include <ctime>

void irq_handler(void) 
{
	uint32_t i,sleeptime;
	unsigned seed;  // Random generator seed
    // Use the time function to get a "seed” value for srand
    seed = time(0);
	srand(seed);

	for(i=0;i<50;i++)
	{
		globle_buf[j] = 2;
		if(j<9999){j++;}

		sleeptime = rand()%5;
		usleep(sleeptime);
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
		linenum1[i] = i;
		buf1[i] = temp;
   }

   if(j > 0 &&j< 1500)
	{
		string file ="./py_diagram/data.txt";
		temp = WriteTxT_by_line(file,j,linenum1,buf1);
		cout<<"------------temp ="<<dec<<(int*)temp<<" ;j= "<<dec<<j<<endl;
	}
	   
}


GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running test from thread_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


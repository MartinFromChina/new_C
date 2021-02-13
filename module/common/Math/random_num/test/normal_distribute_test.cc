#include "../../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;

#include "../normal_distribution.h"
#include "../../../FileOperation/rw_by_line.h"
#include "../../../InsertLog/InsertLogDebug.h"

#include <stdlib.h>

int compare(const void *value1, const void *value2) {

	X_Float64 result ;
	result = (*(X_Float64*)value1 - *(X_Float64*)value2);
	if( result < 0.00000001 && result > -0.00000001  ) // not  good way
	{
		return 0;
	}
	else if(result > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
	
}

#define BASIC_MOUNT 500
#define SCAN  (200)

TEST(normal,distribution)
{
	uint16_t i;
	X_Float64 result = 0,result_sum = 0,result_buf[BASIC_MOUNT];
	int seed =  13579;
	char temp[320];
	char buf1[MAX_LENGTH_OF_FILE_NAME],buf2[MAX_LENGTH_OF_FILE_NAME];
	char *filename = ConvFileStrToChar("./text/display.txt",buf1);
	char *gauss_file = ConvFileStrToChar("./text/gauss_sort.txt",buf2);

	filename = filename;
	
	for(i=0;i<BASIC_MOUNT;i++)
	{
		result = GaussGenerator(0.0,1.0,&seed);
		/////////////insert_LogDebug(1,("result is %.7f \r\n",result));
		result_sum += result;
		result_buf[i] = result;
	}
	result_sum = result_sum/i;
	insert_LogDebug(1,("average is %.7f \r\n",result_sum));

	//qsort(result_buf, BASIC_MOUNT, sizeof(X_Float64)/sizeof(uint8_t), compare);
	//insert_LogDebug(1,("min is %.7f ; max is %.7f\r\n",result_buf[0],result_buf[BASIC_MOUNT - 1]));

	FileClear(gauss_file);
	for(i=0;i<BASIC_MOUNT;i++)
	{
		snprintf(temp,320,"%.7f",result_buf[i]);
		WriteFileByLine(gauss_file,i,temp);
	}
/*
	X_Float64 min = result_buf[0] ,max = result_buf[BASIC_MOUNT - 1],seg;
    uint8_t mount_buf[SCAN],cnt;
	X_Float64 temp_max;

	seg  = (max - min)/SCAN;
	temp_max = min + seg;
	for(j=0;j<SCAN;j++)
	{
		cnt = 0;
		for(i=0;i<100;i++)
		{
			
			if(compare(&result_buf[i],&min) >= 0 && compare(&result_buf[i],&temp_max) <= 0)
			{
				cnt++;
			}
		}
		mount_buf[j] = cnt;
		min = temp_max;
		temp_max = min + seg;
	}
	
	FileClear(filename);
	for(i=0;i<SCAN;i++)
	{
		snprintf(temp,320,"%2x",mount_buf[i]);
		WriteFileByLine(filename,i,temp);
	}
	*/
}
 

 
 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------ramdon_test from normal_distribution_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}


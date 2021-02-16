#include "test_common.h"
#include <iostream>
using namespace std;

#include "../z_math.h"
#include "../../../../module/common/FileOperation/rw_by_line.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

X_Void SleepMilliseconds(uint32_t ms) {
	uint32_t end = clock() + ms;
	while(clock() < end) 
	{

	}
}


TEST(pentip,init)
{
	uint16_t i,j,size;
	z_fpoint_array *p_array;
	z_point        point;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	char *p_file_name1,buf1[MAX_LENGTH_OF_FILE_NAME];
	char *p_file_name2,buf2[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./text/handwriting.txt",buf);
	p_file_name1 = ConvFileStrToChar("./text/handwriting1.txt",buf1);
	p_file_name2 = ConvFileStrToChar("./text/handwriting2.txt",buf2);
	
	p_array = z_new_fpoint_array(200,100.0,100.0);
	
	point.x = 20 ; point.y = 20;
	z_insert_point(p_array,point);

	SleepMilliseconds(2000);
	point.x = 60 ; point.y = 60;
	z_insert_point(p_array,point);

	SleepMilliseconds(8000);
	point.x = 200 ; point.y = 200;
	z_insert_point(p_array,point);

	size = min(p_array ->cap,p_array->len);
	INSERT(LogDebug)(1,("size is %d\r\n",size));

	char temp[500];
	uint16_t param;
	FileClear(p_file_name);
	FileClear(p_file_name1);
	FileClear(p_file_name2);
	for(i=0;i<size;i++)
	{
		//INSERT(LogDebug)(1,("point %d ; x %f ;y %f ;width %f \r\n",i,p_array->point[i].p.x,p_array->point[i].p.y,p_array->point[i].w));
		//INSERT(LogDebug)(1,("point %d ; x %d y %d width %d\r\n",i,p_array[i].last_point.x,p_array[i].last_point.y,p_array[i].last_width));
		param = p_array->point[i].p.x * 2;
		snprintf(&temp[0],100,"%d",param);
		param = p_array->point[i].p.y * 2;
		snprintf(&temp[100],100,"%d",param);
		param = p_array->point[i].w * 50;
		snprintf(&temp[200],100,"%d",param);
/*
		INSERT(LogDebug)(1,("temp %s \r\n",temp));
		INSERT(LogDebug)(1,("temp %s \r\n",&temp[100]));
		INSERT(LogDebug)(1,("temp %s \r\n",&temp[200]));
*/		
		WriteFileByLine(p_file_name,i,temp);
		WriteFileByLine(p_file_name1,i,&temp[100]);
		WriteFileByLine(p_file_name2,i,&temp[200]);

	}
	
	
	z_drop_fpoint_array(p_array);
	//EXPECT_GT(result_sum, 0.49);
	//EXPECT_LT(result_sum, 0.51);

}

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------handwriting_test from handwriting_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}

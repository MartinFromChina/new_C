#include <iostream>
#include "test_main.h"
using namespace std;
using namespace testing;

Adc_Mock adcm;
///////EXPECT_CALL(adcm, ExpectCurrentIndex()).Times(AnyNumber()).WillRepeatedly(Return((temp_index)));

#define MAX_BATTERY_RAWDATA_SIZE 313
static uint16_t battery_raw_data[MAX_BATTERY_RAWDATA_SIZE];
static uint16_t temp_index = 0,display_data_length = 0;
static uint32_t adc_times = 0;
static X_Boolean isBatteryUpdata = X_False,isInChargeState = X_False;
static uint8_t battery_dtrength = 0,battery_dtrength_backup = 0;


#include <fstream>
#include <string>
#include <vector>
// 功能：将filename 中的数据（共cols列）读取到_vector中，_vector可视为二维数组
int read_scanf(const string &filename, const int &cols, vector<double *> &_vector)
{
	FILE *fp = fopen(filename.c_str(), "r");
	bool flag = true;
	int i = 0;
	if (!fp) 
	{ 
		cout << "File open error!\n"; 
		return 0; 
	}
 
	while (flag)
	{
		double *rowArray = new double[cols]; //new一个double类型的动态数组
 
		for (i = 0; i < cols; i++) //读取数据，存在_vector[cols]中
		{
			if (EOF == fscanf(fp,"%lf", &rowArray[i]))
			{ 
				flag = false; 
				break; 
			}
			//输出rowArray存入的数据
			//cout << rowArray[0] << " " << rowArray[1] << " " << rowArray[2] << " " << rowArray[3] << endl;
		}
		if (cols == i) //将txt文本文件中的一行数据存入rowArray中，并将rowArray存入vector中
			_vector.push_back(rowArray);
	}
	fclose(fp);
	return 1;
}
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
#include <cstdlib> // Header file needed to use srand and rand
#include <ctime>
static X_Void BatteryRawDataLoadFromTXT(X_Void)
{
	string file ="../other/basic_data.txt";
	//txt文件中有4列
	int columns = 1;
	vector<double *> output_vector;
	if (!read_scanf(file, columns, output_vector))
	{
		cout<<"read failed"<<endl;
		return ;
	}
		
	//output_vector可视为二维数组;输出数组元素：
	int rows = output_vector.size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++) 
		{ 
			//cout << output_vector[i][j] << " "; 
			if (i < MAX_BATTERY_RAWDATA_SIZE) {battery_raw_data[i+100] = output_vector[i][0];}
		}
		//cout << endl;
	}
	unsigned seed;  // Random generator seed
    // Use the time function to get a "seed” value for srand
    seed = time(0);
    srand(seed);
	for(int i = 0; i < 100; i++)
	{
		battery_raw_data[i] = 1770 + (rand()%20);
	}
	/*
	uint16_t line_num = 0,i = 0;
	string temp;
	ifstream ReadFile;
	ReadFile.open("../other/basic_data.txt",ios::in);//ios::in 表示以只读的方式读取文件
	
	if(ReadFile.fail()) {cout<<"read failed"<<endl;return;}
	
	while(getline(ReadFile,temp))
    {
	    line_num++;
    }
	cout<<"-------------line_num = "<<line_num<<endl;
	int *tc=new int[line_num];
	char *t=new char[line_num];

	//
	cout<<"ReadFile.eof() return : "<<ReadFile.eof()<<endl;
	while(i < MAX_BATTERY_RAWDATA_SIZE) //读取数据到数组
	{
		ReadFile>>tc[i];
		ReadFile>>t[i];
		cout<<tc[i]<<"\t"<<t[i]<<endl;
		i++;
		cout<<"i = "<<i<<endl;
	}
	
	ReadFile.close();
	*/
		
   
}
static uint32_t current_time = 0;
static X_Boolean isDislayByPython = X_False;
static uint16_t mockable_GetBatteryAdcValue(X_Void)
{	
	adc_times ++;
	 return battery_raw_data[(temp_index ++) % MAX_BATTERY_RAWDATA_SIZE];
}
uint32_t mockable_GetCurrentTime(X_Void)
{
	return current_time;
}
static X_Void TestInit(X_Void)
{
	temp_index = 0;
	adc_times = 0;
	current_time = 0;
	isDislayByPython = X_False;
	isBatteryUpdata = X_False;
	isInChargeState = X_False;
	display_data_length = 0;
	battery_dtrength = 0;battery_dtrength_backup = 50;
	mModule_BatteryInit(mockable_GetBatteryAdcValue);
}

static X_Void mockable_SystemTimeUpdata(X_Void)
{
	current_time ++;
}
/*static X_Void mockable_SystemTimeSet(uint32_t times)
{
	current_time = times;
}*/

static uint8_t buf[300];
static uint32_t linenum[300];
static X_Void mockable_SystemHandler(X_Void)
{
	X_Boolean isBatteryValueCome = X_False,isInCharge = X_False; 
	isInCharge = isInChargeState;

	/***************************************************************/
	mockable_SystemTimeUpdata();
	isBatteryValueCome = mModule_BatteryStrengthMonitor(isInCharge);
	/***************************************************************/

	
	if(isBatteryValueCome == X_True){battery_dtrength = mModule_GetBatteryStrength();}
	if(isBatteryValueCome == X_True && isDislayByPython == X_True)
	{ 
		if(display_data_length < 300)
		{
			linenum[display_data_length] 	= current_time/2;
			buf[display_data_length]		= mModule_GetBatteryStrength();
			display_data_length ++;
		}
		
	}
	isBatteryUpdata = isBatteryValueCome;
}

#define SYSTICK_RUN_TIME_IN_HOURS  		2
#define SYSTICK_RUN_TIME_IN_MS     		(SYSTICK_RUN_TIME_IN_HOURS * 60 * 60 * 1000)
#define SYSTICK_RUN_TIME_IN_TICKS    	((uint32_t)(SYSTICK_RUN_TIME_IN_MS/BATTERY_MONITOR_CALL_FREQUENCT_IN_MS))

TEST(battery_monitor,adc_mock)// must be first test ,because the adc basic data should be load !!!
{
	uint16_t i,value;
	TestInit();
	BatteryRawDataLoadFromTXT();
	
	for(i=0;i< 500 ;i++)
	{	
		value = mockable_GetBatteryAdcValue();
		EXPECT_GT(value,1700);
		EXPECT_LT(value,2300);
	}
}
TEST(battery_monitor,init)
{
	adc_times = 0;
	current_time = 0;
	mModule_BatteryDeInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(10000));
	EXPECT_EQ(adc_times,0);

	TestInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(10000));
	EXPECT_EQ(adc_times,104);
}
TEST(battery_monitor,systick)
{
	TestInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < SYSTICK_RUN_TIME_IN_TICKS);
	EXPECT_EQ(mockable_GetCurrentTime(), 900000);
}

TEST(battery_monitor,Get_adcvalue_100times_during_10seconds_after_wakeup)
{
	EXPECT_EQ(CONV_MS_TO_TICKS(8), 1);
	EXPECT_EQ(CONV_MS_TO_TICKS(3), 0);
	EXPECT_EQ(CONV_MS_TO_TICKS(2000), 250);
	TestInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(10000));
	EXPECT_EQ(adc_times,104);
}

TEST(battery_monitor,Get_adcvalue_105times_during_40seconds_after_wakeup)
{
	TestInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(40000));
	EXPECT_EQ(adc_times,105);
}
TEST(battery_monitor,Get_adcvalue_120times_during_500seconds_after_wakeup)
{
	TestInit();
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(500000));
	EXPECT_EQ(adc_times,120);
}

TEST(battery_monitor,battery_sterngth_display_by_python_in_100minutes_after_reset)
{	
	TestInit();
	isDislayByPython = X_True;
	do{
		mockable_SystemHandler();
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000));
	EXPECT_GT(display_data_length,0);
	if(display_data_length > 0 )
	{
		string file ="../other/display_data.txt";
		WriteTxT_by_line(file,display_data_length,linenum,buf);
	}
}

TEST(battery_monitor,display_battery_in_5_seconds)
{
	TestInit();
	EXPECT_EQ(mModule_GetBatteryStrength(), 50);
	do{
		mockable_SystemHandler();
		if(mockable_GetCurrentTime() > CONV_MS_TO_TICKS(5000) && isBatteryUpdata == X_True)
		{
			EXPECT_GT(mModule_GetBatteryStrength(), 4);
			EXPECT_LT(mModule_GetBatteryStrength(), 25);
		}
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(10000));
}

TEST(battery_monitor,battery_sterngth_no_sudden_change_in_100minutes_after_reset)
{	
	uint8_t mul;
	TestInit();
	do{
		mockable_SystemHandler();
		if(isBatteryUpdata == X_True)
		{	
			mul = (battery_dtrength >= battery_dtrength_backup) ? battery_dtrength - battery_dtrength_backup :battery_dtrength_backup - battery_dtrength;

			if(CONV_TICKS_TO_MS(mockable_GetCurrentTime()) <= 10000){EXPECT_LT(mul,6);}
			else{EXPECT_LT(mul,3);}
			battery_dtrength_backup = battery_dtrength;
		}
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000));
}

TEST(battery_monitor,battery_sterngth_no_drop_when_in_charge0)
{	
	uint8_t mul;
	TestInit();
	isDislayByPython = X_True;
	do{
		if(mockable_GetCurrentTime() > CONV_MS_TO_TICKS(0) && mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000)) {isInChargeState = X_True;}
		else{isInChargeState = X_False;}
		mockable_SystemHandler();
		
		if(isBatteryUpdata == X_True)
		{	
			if(isInChargeState == X_True && mockable_GetCurrentTime() > CONV_MS_TO_TICKS(10000))
			{
			//cout<<"------------dot \r\n";
			EXPECT_GT(battery_dtrength,battery_dtrength_backup);
			}
			battery_dtrength_backup = battery_dtrength;
		}
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000));
		
	EXPECT_GT(display_data_length,0);
	if(display_data_length > 0 )
	{
		string file ="../other/finial_display.txt";
		WriteTxT_by_line(file,display_data_length,linenum,buf);
	}
}

TEST(battery_monitor,battery_sterngth_no_drop_when_10_200_secs_in_charge0)
{	
	uint8_t mul;
	TestInit();
	do{
		if(mockable_GetCurrentTime() > CONV_MS_TO_TICKS(10000) && mockable_GetCurrentTime() < CONV_MS_TO_TICKS(200000)) {isInChargeState = X_True;}
		else{isInChargeState = X_False;}
		mockable_SystemHandler();
		
		if(isBatteryUpdata == X_True)
		{	
			if(isInChargeState == X_True && mockable_GetCurrentTime() > CONV_MS_TO_TICKS(10000))
			{
				//cout<<"------------dot \r\n";
				EXPECT_GT(battery_dtrength,battery_dtrength_backup);
			}
			battery_dtrength_backup = battery_dtrength;
		}
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000));
}

TEST(battery_monitor,battery_sterngth_no_drop_when_200_400_secs_2000_5000_in_charge0)
{	
	uint8_t mul;
	TestInit();
	do{
		if(mockable_GetCurrentTime() > CONV_MS_TO_TICKS(10000) && mockable_GetCurrentTime() < CONV_MS_TO_TICKS(200000)) {isInChargeState = X_True;}
		else{isInChargeState = X_False;}
		if(mockable_GetCurrentTime() > CONV_MS_TO_TICKS(2000000) && mockable_GetCurrentTime() < CONV_MS_TO_TICKS(5000000)) {isInChargeState = X_True;}
		else{isInChargeState = X_False;}
		mockable_SystemHandler();
		
		if(isBatteryUpdata == X_True)
		{	
			if(isInChargeState == X_True && mockable_GetCurrentTime() > CONV_MS_TO_TICKS(10000))
			{
				//cout<<"------------dot \r\n";
				EXPECT_GT(battery_dtrength,battery_dtrength_backup);
			}
			battery_dtrength_backup = battery_dtrength;
		}
	}while(mockable_GetCurrentTime() < CONV_MS_TO_TICKS(6000000));
}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running battery_monitor_test from test_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

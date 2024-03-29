#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "test_data.h"

#include <iostream>
#include "../hal.h"
#include "test_common.h"

using namespace std;
static uint8_t table_index = 0,log_flag = 0;

static s_monitor_list MoniotorList[MAX_MONITOR_LIST_SIZE];

static X_Void MonitorListInit1(X_Void)
{
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	s_monitor_list src_no_matter,src2 = {0,{X_True,2,{0xaa,0x55, 9, 1, 9,0x55, 4, 0,0x6b}, 9,11,}};
	s_monitor_list src22 = {0,{X_False,2,{0xcc,0x66, 9, 0, 2, 1,0xfe,0x55,0x91}, 9,11,}};
	s_monitor_list src222 = {0,{X_False,2,{0xaa,0x55, 9, 2, 9,0x55, 4, 0,0x6c}, 9,11,}};
	MonitorListAdd(MoniotorList,&src2,0);
	MonitorListAdd(MoniotorList,&src22,0);
	MonitorListAdd(MoniotorList,&src222,0);

	MonitorListAdd(MoniotorList,&src_no_matter,1);

	s_monitor_list src3 = {0,{X_True,3,{0xaa,0x55, 9, 2, 9,0x55, 4, 0,0x6c},9,20,},};
	s_monitor_list src33 = {0,{X_False,3,{0xcc,0x66, 9, 0, 3, 2,0xfe,0x55,0x93},9,20,},};
	s_monitor_list src333 = {0,{X_False,3,{0xaa,0x55, 9, 3, 9,0x55, 4, 0,0x6d},9,20,},};
	MonitorListAdd(MoniotorList,&src3,0);
	MonitorListAdd(MoniotorList,&src33,0);
	MonitorListAdd(MoniotorList,&src333,0);

	s_monitor_list src1 = {0,{X_True,1,{0xcc,0x66, 9, 0, 2, 1,0xfe,0x55,0x91},9,22,},};
	MonitorListAdd(MoniotorList,&src1,0);
	MonitorListAdd(MoniotorList,&src_no_matter,2);
		
	s_monitor_list src4 = {0,{X_True,4,{0xaa,0x55, 9, 3, 9,0x55, 4, 0,0x6d},9,25,},};
	s_monitor_list src44 = {0,{X_False,4,{0xcc,0x66, 9, 0, 4, 3,0xfe,0x55,0x95},9,25,},};
	s_monitor_list src444 = {0,{X_False,4,{0xaa,0x55, 9, 4, 9,0x55, 4, 0,0x6e},9,25,},};
	MonitorListAdd(MoniotorList,&src4,0);
	MonitorListAdd(MoniotorList,&src44,0);
	MonitorListAdd(MoniotorList,&src444,0);

	MonitorListAdd(MoniotorList,&src_no_matter,31);

	s_monitor_list src9 = {0,{X_False,9,{0xcc,0x66, 0xc, 0, 9, 8,0x55, 4, 0, 0, 0,0xa8,},12,62,},};
	MonitorListAdd(MoniotorList,&src9,0);

	MonitorListAdd(MoniotorList,&src_no_matter,29);

	s_monitor_list back4 = {0,{X_False,4,{0xcc,0x66,0x1b, 0, 4, 3,0x55, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xad},27,99,},};
	MonitorListAdd(MoniotorList,&back4,0);

	MonitorListAdd(MoniotorList,&src_no_matter,15);

	s_monitor_list back1 = {0,{X_True,1,{0xcc,0x66,0x1b, 0, 2, 1,0x55, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xa9}, 27,124},};
	MonitorListAdd(MoniotorList,&back1,0);
}
static X_Void data_monitor_1(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;
	s_monitor_list *p_table = MoniotorList;
	if(p_table[table_index].ignore_times == 0)
	{
		INSERT(LogDebug)(log_flag,("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:check it !!!~~~~~~~~~~~~~~~~~~~~~~~~\r\n"));
		EXPECT_EQ(p_table[table_index].table.isRecv, isRecv);
		EXPECT_EQ(p_table[table_index].table.current_node_num, current_node_num);
		EXPECT_EQ(p_table[table_index].table.time, time);
		EXPECT_EQ(p_table[table_index].table.length, length);

		for(i=0;i<length;i++)
		{
			EXPECT_EQ(p_table[table_index].table.data[i], p_data[i]);
		}

		if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
		else {table_index = 0;}
	}
	else 
	{
		INSERT(LogDebug)(log_flag,(":ignore it !!!\r\n"));
		p_table[table_index].ignore_times --;
		if(p_table[table_index].ignore_times == 0)
		{
			if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
			else {table_index = 0;}
		}
	}
}


TEST(multicast,start_1_get_from_4_9)
{
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(1);
	TestCommonInit(data_monitor_1,MonitorListInit1);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,1,9,4);
	isOK = SendWaveSetForTestModule(1,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static X_Void MonitorListInit2(X_Void)
{
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	s_monitor_list src_no_matter,src7 = {0,{X_True,7,{0xaa,0x55, 9, 8, 2,0x55, 5, 0,0x6c}, 9,5,}};
	s_monitor_list src77 = {0,{X_False,7,{0xcc,0x66, 9, 0, 7, 8,0xfe,0x55,0x9d}, 9,5,}};
	s_monitor_list src777 = {0,{X_False,7,{0xaa,0x55, 9, 7, 2,0x55, 5, 0,0x6b}, 9,5,}};
	MonitorListAdd(MoniotorList,&src7,0);
	MonitorListAdd(MoniotorList,&src77,0);
	MonitorListAdd(MoniotorList,&src777,0);

	MonitorListAdd(MoniotorList,&src_no_matter,1);

	s_monitor_list src8 = {0,{X_True,8,{0xcc,0x66, 9, 0, 7, 8,0xfe,0x55,0x9d}, 9,10,}};
	MonitorListAdd(MoniotorList,&src8,0);

	MonitorListAdd(MoniotorList,&src_no_matter,28);

	s_monitor_list back2 = {0,{X_False,2,{0xcc,0x66, 0xc, 0, 2, 3,0x55, 5, 0, 0, 0,0x9d}, 12,43,}};
	MonitorListAdd(MoniotorList,&back2,0);

	MonitorListAdd(MoniotorList,&src_no_matter,3);

	s_monitor_list back3 = {0,{X_False,3,{0xcc,0x66, 0xf, 0, 3, 4,0x55, 5, 0, 0, 0,0, 0, 0,0xa2}, 15,52,}};
	MonitorListAdd(MoniotorList,&back3,0);

	MonitorListAdd(MoniotorList,&src_no_matter,5);

	s_monitor_list back4 = {0,{X_False,4,{0xcc,0x66,0x12, 0, 4, 5,0x55, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xa7},18,57}};
	MonitorListAdd(MoniotorList,&back4,0);

	MonitorListAdd(MoniotorList,&src_no_matter,21);

	s_monitor_list back8 = {0,{X_True,8,{0xcc,0x66,0x15, 0, 7, 8,0x55, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xb0},21,86}};
	MonitorListAdd(MoniotorList,&back8,0);
	
}

TEST(multicast,start_8_get_from_5_2)
{
	uint8_t start_point = 8;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_1,MonitorListInit2);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,start_point,2,5);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();

}
static X_Void MonitorListInit3(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,65);
	
	s_monitor_list back8 = {0,{X_True,8,{0xcc,0x66,0x1b, 0, 7, 8,0x55, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0xb8}, 27,86,}};
	MonitorListAdd(MoniotorList,&back8,0);
}

TEST(multicast,start_8_get_from_7_2)
{
	uint8_t start_point = 8;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_1,MonitorListInit3);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,start_point,2,7);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static X_Void MonitorListInit4(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,17);
	
	s_monitor_list back4 = {0,{X_True,4,{0xcc,0x66, 0xc, 0, 3, 4,0x55, 2, 0, 0, 0,0x9c}, 12,28,}};
	MonitorListAdd(MoniotorList,&back4,0);
}


TEST(multicast,start_4_get_from_2_2)
{
	uint8_t start_point = 4;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_1,MonitorListInit4);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,start_point,2,2);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();

}
static X_Void MonitorListInit5(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,21);
	
	s_monitor_list back1 = {0,{X_False,1,{0xcc,0x66, 0xc, 0, 1, 2,0x55, 3, 0, 0, 0,0x99}, 12,33}};
	MonitorListAdd(MoniotorList,&back1,0);

	MonitorListAdd(MoniotorList,&src_no_matter,9);

	s_monitor_list back5 = {0,{X_True,5,{0xcc,0x66, 9, 0, 4, 5,0x88, 1,0xcd}, 9,90}};
	MonitorListAdd(MoniotorList,&back5,0);
}

static X_Void data_monitor_5(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;
	s_monitor_list *p_table = MoniotorList;
	if(p_table[table_index].ignore_times == 0)
	{
		INSERT(LogDebug)(log_flag,("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:check it !!!~~~~~~~~~~~~~~~~~~~~~~~~\r\n"));
		EXPECT_EQ(p_table[table_index].table.isRecv, isRecv);
		EXPECT_EQ(p_table[table_index].table.current_node_num, current_node_num);
		EXPECT_EQ(p_table[table_index].table.time, time);
		EXPECT_EQ(p_table[table_index].table.length, length);

		for(i=0;i<length;i++)
		{
			EXPECT_EQ(p_table[table_index].table.data[i], p_data[i]);
		}

		if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
		else {table_index = 0;}
	}
	else 
	{
		INSERT(LogDebug)(log_flag,(":ignore it !!!\r\n"));
		p_table[table_index].ignore_times --;
		if(p_table[table_index].ignore_times == 0)
		{
			if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
			else {table_index = 0;}
		}
	}

	if(time >= 33)
	{
		SetTemporaryDistance(10); 
	}
}


TEST(multicast,start_5_get_from_3_1_termenical1_lost)
{
	uint8_t start_point = 5;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_5,MonitorListInit5);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,start_point,1,3);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static X_Void MonitorListInit6(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,21);
	
	s_monitor_list back1 = {0,{X_False,1,{0xcc,0x66, 0xc, 0, 1, 2,0x55, 3, 0, 0, 0,0x99}, 12,33}};
	MonitorListAdd(MoniotorList,&back1,0);

	MonitorListAdd(MoniotorList,&src_no_matter,9);

	//s_monitor_list back5 = {0,{X_True,5,{0xcc,0x66, 9, 0, 4, 5,0x88, 1,0xcd}, 9,90}};
	//MonitorListAdd(MoniotorList,&back5,0);
}

static X_Void data_monitor_6(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{	
	uint16_t i;
	s_monitor_list *p_table = MoniotorList;
	if(p_table[table_index].ignore_times == 0)
	{
		INSERT(LogDebug)(log_flag,("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:check it !!!~~~~~~~~~~~~~~~~~~~~~~~~\r\n"));
		EXPECT_EQ(p_table[table_index].table.isRecv, isRecv);
		EXPECT_EQ(p_table[table_index].table.current_node_num, current_node_num);
		EXPECT_EQ(p_table[table_index].table.time, time);
		EXPECT_EQ(p_table[table_index].table.length, length);

		for(i=0;i<length;i++)
		{
			EXPECT_EQ(p_table[table_index].table.data[i], p_data[i]);
		}

		if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
		else {table_index = 0;}
	}
	else 
	{
		INSERT(LogDebug)(log_flag,(":ignore it !!!\r\n"));
		p_table[table_index].ignore_times --;
		if(p_table[table_index].ignore_times == 0)
		{
			if((table_index + 1) < (  (uint8_t)(  sizeof(MoniotorList)/sizeof(MoniotorList[0])      )     )) {table_index ++;}
			else {table_index = 0;}
		}
	}

	if(time == 44)
	{
		/*
		if there is a lost in terminal 2 ,then the terminal 3 will receive nothing ;by the way terminal 3 have nothing to wait at all
		so ,there will not be any response for terminal 5. this case can be handle by host or erery terminal add additional function to 
		wait multicast type response , it will make the program more complex ... maybe I will do it later 
		*/ 
		SetTemporaryDistance(3); 
	}
	else
	{
		SetTemporaryDistance(11); 
	}
}

TEST(multicast,start_5_get_from_3_1_termenical2_lost_in_return_path) 
{
	uint8_t start_point = 5;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_6,MonitorListInit6);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 
	
	length = GenerateInfoMulGet(&p_data,start_point,1,3);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}


static X_Void MonitorListInit7(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,112);
	
	s_monitor_list back1 = {0,{X_True,1,{0xcc,0x66,0x1b, 0, 2, 1,0x55, 3, 0, 0, 0, 0,0x88,0x77, 0,0x88,0x77, 0,0x88,0x77, 0,0x88,0x77, 0, 0, 0,0xa4}, 27,258}};
	MonitorListAdd(MoniotorList,&back1,0);
}

TEST(multicast,start_1_set_4_7_get_3_8) 
{
	uint8_t start_point = 1;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_1,MonitorListInit7);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 

	length = GenerateSpeedMulSet(&p_data,start_point,7,4,0x7788);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	length = GenerateInfoMulGet(&p_data,start_point,8,3);
	isOK = SendWaveSetForTestModule(start_point,150,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}

static X_Void MonitorListInit8(X_Void)
{
	s_monitor_list src_no_matter;
	MonitorListInit(MoniotorList,sizeof(MoniotorList)/sizeof(MoniotorList[0]));

	MonitorListAdd(MoniotorList,&src_no_matter,138);
	
	s_monitor_list back10 = {0,{X_True,10,{0xcc,0x66,0x21, 0, 9, 0xa,0x55, 8, 0, 0, 0, 0, 0, 0, 0,0x66,0x99, 0,0x66,0x99, 0,0x66,0x99, 0,0x66,0x99, 0,0x66,0x99, 0, 0, 0,0xbe,}, 0x21,288}};
	MonitorListAdd(MoniotorList,&back10,0);
}


TEST(multicast,start_10_set_7_3_get_8_1) 
{
	uint8_t start_point = 10;
	uint8_t *p_data,length;
	X_Boolean isOK;
	table_index= 0;
	log_flag = 0;
	HAL_BasicSet(start_point);
	TestCommonInit(data_monitor_1,MonitorListInit8);
	DisableLogDebug();// called it after TestCommonInit
	SetTemporaryDistance(11); 

	length = GenerateSpeedMulSet(&p_data,start_point,3,7,0x9966);
	isOK = SendWaveSetForTestModule(start_point,0,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	length = GenerateInfoMulGet(&p_data,start_point,1,8);
	isOK = SendWaveSetForTestModule(start_point,150,p_data,length,ED_bidirection,12);
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
	TestCommonDeInit();
}



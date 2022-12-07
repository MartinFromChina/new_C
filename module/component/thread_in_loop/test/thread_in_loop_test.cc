#include <iostream>
#include "tdd_common.h"

#include "../thread_in_loop.h"
#include <stdio.h>
#define LOG_DEBUG_METHOD   printf
#include "../../../common/InsertLog/InsertLogDebug.h"

using namespace std;

static uint32_t sys_cnt_ms = 0,sys_cnt_ms1 = 0,sys_cnt_ms2 = 0,sys_cnt_ms3 = 0;
static int todo_1_cnt = 0,todo_2_cnt = 0,todo_2_1_cnt = 0;

static X_Void BeforeTest(X_Void)
{
  sys_cnt_ms = 0 ;
  sys_cnt_ms1 = 0;
  sys_cnt_ms2 = 0;
  sys_cnt_ms3 = 0;
  todo_1_cnt = 0;
  todo_2_cnt = 0;
  todo_2_1_cnt = 0;
}
static X_Void SysTickIRQ(X_Void)
{
  sys_cnt_ms ++;
  if(sys_cnt_ms1 > 0) {sys_cnt_ms1 --;}
  if(sys_cnt_ms2 > 0) {sys_cnt_ms2 --;}
  if(sys_cnt_ms3 > 0) {sys_cnt_ms3 --;}
}

uint32_t GetSysTime1(X_Void)
{
  return sys_cnt_ms1;
}
X_Boolean SetSysTime1(uint32_t time_ms)
{
  sys_cnt_ms1 = time_ms;
  return X_True;
}
uint32_t GetSysTime2(X_Void)
{
  return sys_cnt_ms2;
}
X_Boolean SetSysTime2(uint32_t time_ms)
{
  sys_cnt_ms2 = time_ms;
  return X_True;
}
uint32_t GetSysTime3(X_Void)
{
  return sys_cnt_ms3;
}
X_Boolean SetSysTime3(uint32_t time_ms)
{
  sys_cnt_ms3 = time_ms;
  return X_True;
}
/**************************1****basic delay test***********************************************/
#define BASIC_TEST    0
APP_THREAD_INIT(t_basic1,GetSysTime1,SetSysTime1);
APP_THREAD_INIT(t_basic2,GetSysTime2,SetSysTime2);

static X_Void Todo1(X_Void)
{
  insert_LogDebug(BASIC_TEST,(" %d --- thread 1 todo 1 \r\n",sys_cnt_ms));
  todo_1_cnt ++;
}

static X_Void Todo2(X_Void)
{
  insert_LogDebug(BASIC_TEST,(" %d --- thread 2 todo 2 \r\n",sys_cnt_ms));
  todo_2_cnt ++;
}
static X_Void Todo2_1(X_Void)
{
    insert_LogDebug(BASIC_TEST,(" %d --- thread 2 todo 2.1 \r\n",sys_cnt_ms));
    todo_2_1_cnt ++;
}

static X_Void Thread1(X_Void)
{
  APP_TH_DO(t_basic1,Todo1);
  APP_TH_WAIT(t_basic1,3000);
  APP_TH_END(t_basic1);
}

static X_Void Thread2(X_Void)
{
  APP_TH_DO(t_basic2,Todo2);
  APP_TH_WAIT(t_basic2,1000);
  APP_TH_DO(t_basic2,Todo2_1);
  APP_TH_WAIT(t_basic2,500);
  APP_TH_END(t_basic2);
}

TEST(thread,basic)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1();
    Thread2();
  }
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(7,todo_2_cnt);
  EXPECT_EQ(6,todo_2_1_cnt);
}

/***********************2****Wait until**************************************************/
#define WAIT_UNTIL_TEST    0
APP_THREAD_INIT(t_wu1,GetSysTime1,SetSysTime1);
APP_THREAD_INIT(t_wu2,GetSysTime2,SetSysTime2);

static X_Void Todo1_t_wu1(X_Void)
{
  insert_LogDebug(WAIT_UNTIL_TEST,(" %d --- thread 1 todo 1 \r\n",sys_cnt_ms));
  todo_1_cnt ++;
}

static X_Void Todo2_t_wu2(X_Void)
{
  insert_LogDebug(WAIT_UNTIL_TEST,(" %d --- thread 2 todo 2 \r\n",sys_cnt_ms));
  todo_2_cnt ++;
}
static X_Void Todo2_1_t_wu2(X_Void)
{
  insert_LogDebug(WAIT_UNTIL_TEST,(" %d --- thread 2 todo 2.1 \r\n",sys_cnt_ms));
  todo_2_1_cnt ++;
}
static X_Void Todo2_2_t_wu2(X_Void)
{
  insert_LogDebug(WAIT_UNTIL_TEST,(" %d --- thread 2 todo 2.2 \r\n",sys_cnt_ms));
}

static X_Void Thread1_t_wu1(X_Void)
{
  APP_TH_DO(t_wu1,Todo1_t_wu1);
  APP_TH_WAIT(t_wu1,3000);
  APP_TH_END(t_wu1);
}

static uint32_t sys_cnt_ms_backup = 0;
static X_Void WaitVariableInit(X_Void)
{
  insert_LogDebug(WAIT_UNTIL_TEST,(" %d --- thread 2 WaitVariableInit \r\n",sys_cnt_ms));
  sys_cnt_ms_backup = sys_cnt_ms;
}

static X_Boolean DoesTimeMatched1(X_Void)
{
  if((sys_cnt_ms - sys_cnt_ms_backup) < 5000) {return X_False;}
  return X_True;
}
static X_Boolean DoesTimeMatched2(X_Void)
{
  if((sys_cnt_ms - sys_cnt_ms_backup) < 2000) {return X_False;}
  return X_True;
}
static X_Void Thread2_t_wu2(X_Void)
{
  APP_TH_WAIT(t_wu2,1000);
  APP_TH_DO(t_wu2,Todo2_t_wu2);
  ///////APP_TH_WAIT_UNTIL(t_wu2,(one_step)0,DoesTimeMatched);
  APP_TH_WAIT_UNTIL(t_wu2,WaitVariableInit,DoesTimeMatched1);
  APP_TH_DO(t_wu2,Todo2_1_t_wu2);
  APP_TH_WAIT_UNTIL(t_wu2,WaitVariableInit,DoesTimeMatched2);
  APP_TH_DO(t_wu2,Todo2_2_t_wu2);
  APP_TH_END(t_wu2);
}

TEST(thread,wait_until)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_wu1();
    Thread2_t_wu2();
  }
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(2,todo_2_cnt);
  EXPECT_EQ(1,todo_2_1_cnt);
}

/*****************************************************************************************/
/**************************3***********while loop test****************************************************/
#define WHILE_LOOP_TEST    0
APP_THREAD_INIT(t_wloop1,GetSysTime1,SetSysTime1);
APP_THREAD_INIT(t_wloop2,GetSysTime2,SetSysTime2);

static X_Void Todo1_t_wloop1(X_Void)
{
  insert_LogDebug(WHILE_LOOP_TEST,(" %d --- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ thread 1 todo 1 \r\n",sys_cnt_ms));
  todo_1_cnt ++;
}

static X_Void Thread1_t_wloop1(X_Void)
{
  APP_TH_DO(t_wloop1,Todo1_t_wloop1);
  APP_TH_WAIT(t_wloop1,3000);
  APP_TH_END(t_wloop1);
}



static X_Void Todo2_t_wloop2(X_Void)
{
  insert_LogDebug(WHILE_LOOP_TEST,(" %d --- thread 2 todo 2 \r\n",sys_cnt_ms));
  todo_2_cnt ++;
}
static X_Void Todo2_1_t_wloop2(X_Void)
{
  insert_LogDebug(WHILE_LOOP_TEST,(" %d --- thread 2 todo 2.1 \r\n",sys_cnt_ms));
  todo_2_1_cnt ++;
}
static X_Void Todo2_2_t_wloop2(X_Void)
{
  insert_LogDebug(WHILE_LOOP_TEST,(" %d --- thread 2 todo 2.2 \r\n",sys_cnt_ms));
}

static X_Boolean DoesWhileBreak(X_Void)
{
  if((sys_cnt_ms - sys_cnt_ms_backup) < 5000) {return X_False;}
  return X_True;
}

static X_Void Thread2_t_wloop2(X_Void)
{
  APP_TH_WAIT(t_wloop2,1000);
  APP_TH_DO(t_wloop2,Todo2_t_wloop2);
  APP_TH_WHILE(t_wloop2,WaitVariableInit,DoesWhileBreak,Todo2_1_t_wloop2,500); // (does_in_conditon)0
  APP_TH_END(t_wloop2);
}


TEST(thread,while_loop)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_wloop1();
    Thread2_t_wloop2();
  }
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(2,todo_2_cnt);
  EXPECT_EQ(16,todo_2_1_cnt);
}

static X_Void Thread2_t_wloop2_special1(X_Void)
{
  APP_TH_WAIT(t_wloop2,1000);
  APP_TH_DO(t_wloop2,Todo2_t_wloop2);


  APP_TH_WHILE(t_wloop2,WaitVariableInit,(does_in_conditon)0,Todo2_1_t_wloop2,500); // 
  APP_TH_END(t_wloop2);
}

TEST(thread,while_loop1)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_wloop1();
    Thread2_t_wloop2_special1();
  }
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(1,todo_2_cnt);
  EXPECT_EQ(18,todo_2_1_cnt);
}

static X_Boolean DoesWhileBreak2(X_Void)
{
  if((sys_cnt_ms - sys_cnt_ms_backup) < 20) {return X_False;}
  return X_True;
}

static X_Void Thread2_t_wloop2_special2(X_Void)
{
  APP_TH_WAIT(t_wloop2,1000);
  APP_TH_DO(t_wloop2,Todo2_t_wloop2);
  APP_TH_WHILE(t_wloop2,WaitVariableInit,DoesWhileBreak2,Todo2_1_t_wloop2,0); // 
  APP_TH_WAIT(t_wloop2,1500);
  APP_TH_WHILE(t_wloop2,WaitVariableInit,DoesWhileBreak2,Todo2_2_t_wloop2,0); //
  APP_TH_WAIT(t_wloop2,500);
  APP_TH_END(t_wloop2);
}

TEST(thread,while_loop2)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_wloop1();
    Thread2_t_wloop2_special2();
  }
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(3,todo_2_cnt);
  EXPECT_EQ(57,todo_2_1_cnt);
}
/*****************************************************************************************/
/************************4***for loop **************************************************/
#define FORLOOP_TEST    0
APP_THREAD_INIT(t_for1,GetSysTime1,SetSysTime1);
APP_THREAD_INIT(t_for2,GetSysTime2,SetSysTime2);

static X_Void Todo1_t_for1(X_Void)
{
  insert_LogDebug(FORLOOP_TEST,(" %d --- ~~~~~~~~~~~~~~~~~~~~~~~~~ thread 1 todo 1 \r\n",sys_cnt_ms));
  todo_1_cnt ++;
}

static X_Void Thread1_t_for1(X_Void)
{
  APP_TH_DO(t_for1,Todo1_t_for1);
  APP_TH_WAIT(t_for1,3000);
  APP_TH_END(t_for1);
}

static X_Void Todo2_t_for2(X_Void)
{
  insert_LogDebug(FORLOOP_TEST,(" %d --- thread 2 todo 2 \r\n",sys_cnt_ms));
  todo_2_cnt ++;
}
static X_Void Todo2_1_t_for2(X_Void)
{
    insert_LogDebug(FORLOOP_TEST,(" %d --- thread 2 todo 2.1 \r\n",sys_cnt_ms));
    todo_2_1_cnt ++;
}

static X_Void Todo2_2_t_for2(X_Void)
{
    insert_LogDebug(FORLOOP_TEST,(" %d --- thread 2 todo 2.2 \r\n",sys_cnt_ms));

}

static X_Void Thread2_t_for2(X_Void)
{
  APP_TH_WAIT(t_for2,1000);
  APP_TH_DO(t_for2,Todo2_t_for2);
  APP_TH_FOR(t_for2,WaitVariableInit,DoesWhileBreak,Todo2_1_t_for2,500,3); // (does_in_conditon)0
  APP_TH_END(t_for2);
}

TEST(thread,for_loop)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_for1();
    Thread2_t_for2();
  }
  
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(5,todo_2_cnt);
  EXPECT_EQ(14,todo_2_1_cnt);
 
}

static X_Void Thread2_t_for2_special1(X_Void)
{
  APP_TH_WAIT(t_for2,1000);
  APP_TH_DO(t_for2,Todo2_t_for2);
  APP_TH_FOR(t_for2,WaitVariableInit,DoesWhileBreak,Todo2_1_t_for2,0,5); // (does_in_conditon)0
  APP_TH_WAIT(t_for2,3000);
  APP_TH_END(t_for2);
}

TEST(thread,for_loop1)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_for1();
    Thread2_t_for2_special1();
  }
  
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(3,todo_2_cnt);
  EXPECT_EQ(15,todo_2_1_cnt);
 
}

static X_Void Thread2_t_for2_special2(X_Void)
{
  APP_TH_WAIT(t_for2,1000);
  APP_TH_DO(t_for2,Todo2_t_for2);
  APP_TH_FOR(t_for2,WaitVariableInit,DoesWhileBreak2,Todo2_1_t_for2,0,15); // 
  APP_TH_WAIT(t_for2,1500);
  APP_TH_FOR(t_for2,WaitVariableInit,DoesWhileBreak2,Todo2_2_t_for2,0,6); //
  APP_TH_WAIT(t_for2,500);
  APP_TH_END(t_for2);
}

TEST(thread,for_loop2)
{
  uint32_t i;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    SysTickIRQ();
    Thread1_t_for1();
    Thread2_t_for2_special2();
  }
  
	EXPECT_EQ(4,todo_1_cnt);
  EXPECT_EQ(3,todo_2_cnt);
  EXPECT_EQ(45,todo_2_1_cnt);
 
}

/***************************combine test **************************************************/
#define COMBINE_TEST    0
#define COMBINE_TH3_TEST    1

APP_THREAD_INIT(th1,GetSysTime1,SetSysTime1);
APP_THREAD_INIT(th2,GetSysTime2,SetSysTime2);
APP_THREAD_INIT(th3,GetSysTime3,SetSysTime3);

static int th_cnt1 = 0,th_cnt2 = 0,th_cnt3 = 0;
static uint32_t th_ms_backup1 = 0,th_ms_backup2 = 0,th_ms_backup3 = 0;

//---------------------1---------------------------
static X_Void th1_todo(X_Void)
{
  th_cnt1 ++;
  insert_LogDebug(COMBINE_TEST,(" %d --- I am thread 1 infinite loop \r\n",sys_cnt_ms));
}

static X_Void CombineThread1(X_Void)
{
  APP_TH_WAIT(th1,99);
  APP_TH_INFINITE_WHILE(th1,th1_todo,999);
  APP_TH_END(th1);
}
//----------------------2--------------------------
static X_Void th2_todo(X_Void)
{
  th_cnt2 ++;
  insert_LogDebug(COMBINE_TEST,(" %d --- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ I am thread 2 for loop \r\n",sys_cnt_ms));
}
static X_Void CombineThread2(X_Void)
{
  APP_TH_SIMPLE_FOR(th2,th2_todo,999,2);
  APP_TH_WAIT(th2,2999);
  APP_TH_END(th2);
}

//----------------------3--------------------------
/*
APP_THREAD_INIT(id,set_time_func,get_time_func)
APP_TH_END(id)
APP_TH_DO(id,todo)
APP_TH_WAIT(id,wait_ms)
APP_TH_WAIT_UNTIL(id,var_init,condition)
APP_TH_WHILE(id,var_init,condition,todo,step_ms)
APP_TH_FOR(id,var_init,condition,todo,step_ms,loop_times)
APP_TH_INFINITE_WHILE(id,todo,step_ms)
APP_TH_SIMPLE_FOR(id,todo,step_ms,loop_times)
*/
static void thread3_init(void)
{
  insert_LogDebug(COMBINE_TH3_TEST,(" %d --- thread 3 init \r\n",sys_cnt_ms));
}
static void thread3_initLora(void)
{
  insert_LogDebug(COMBINE_TH3_TEST,(" %d --- thread 3 init Lora\r\n",sys_cnt_ms));
}

static void SlaveBoardPowerOn(void)
{
  th_ms_backup3 = sys_cnt_ms;
  insert_LogDebug(COMBINE_TH3_TEST,(" %d --- slave board power on \r\n",sys_cnt_ms));
}
static X_Boolean DoesSlaveBoardReady(void)
{
  if((sys_cnt_ms - th_ms_backup3) >= 1500) 
  {
    insert_LogDebug(COMBINE_TH3_TEST,(" %d --- slave board ready  \r\n",sys_cnt_ms));
    return X_True;
  }
  return X_False;
}

static uint8_t retry_cnt = 0;
static void ReadyForLoraCommand(void)
{
  insert_LogDebug(COMBINE_TH3_TEST,(" %d --- ready for recv lora command  \r\n",sys_cnt_ms));
  retry_cnt = 0;
}
static X_Boolean DoesGetLoraCMD(void)
{
  
  if(retry_cnt > 5) 
  {
    insert_LogDebug(COMBINE_TH3_TEST,(" %d --- get lora command  \r\n",sys_cnt_ms));
    return X_True;
  }
  return X_False;
}
static void ScanLoraCommand(void)
{
  retry_cnt ++;
  insert_LogDebug(COMBINE_TH3_TEST,(" %d --- ~~~~~~~~~~~~ scan lora command  %d \r\n",sys_cnt_ms,retry_cnt));
}

static X_Void Thread3_routine(void)
{


}

static X_Void CombineThread3(X_Void)
{
  APP_TH_WAIT(th3,499);
  APP_TH_DO(th3,thread3_init);
  APP_TH_WAIT(th3,499);
  APP_TH_DO(th3,thread3_initLora);
  APP_TH_WAIT(th3,99);
  APP_TH_WAIT_UNTIL(th3,SlaveBoardPowerOn,DoesSlaveBoardReady);

  APP_TH_FOR(th3,ReadyForLoraCommand,DoesGetLoraCMD,ScanLoraCommand,199,10);

  APP_TH_INFINITE_WHILE(th3,Thread3_routine,200);
  APP_TH_END(th3);
}
//------------------------------------------------
TEST(thread,combine)
{
  uint32_t i;
  th_ms_backup2 = th_ms_backup2;
  th_ms_backup1 = th_ms_backup1;
  BeforeTest();

  for(i=0;i<10000;i++)
  {
    CombineThread1();
    CombineThread2();
    CombineThread3();
    SysTickIRQ();
  }

  EXPECT_EQ(10,th_cnt1);
  EXPECT_EQ(6,th_cnt2);
  EXPECT_EQ(0,th_cnt3);
}
/*************************************************************************************/

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from priorityqueue_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

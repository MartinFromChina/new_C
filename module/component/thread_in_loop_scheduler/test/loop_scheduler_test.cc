#include <iostream>
#include "tdd_common.h"

#include "../loop_scheduler.h"
#include <stdio.h>
#define LOG_DEBUG_METHOD   printf
#include "../../../common/InsertLog/InsertLogDebug.h"

using namespace std;

static uint32_t sys_cnt_ms = 0;

static X_Void SysTickIRQ(X_Void)
{
  sys_cnt_ms ++;
}
uint32_t GetSystemTime(X_Void)
{
  return sys_cnt_ms;
}

static X_Void BeforeTest(X_Void)
{
  sys_cnt_ms = 0;
  task_scheduler_init(GetSystemTime);
}
/*****************************scheduler test**********************************
//X_TASK_CREATE(t_do);

#define TASK_SCHEDULER_LOG_DEBUG                    1

static int normal_cnt1 = 0,normal_cnt2 = 0;
static X_Boolean isInit;
static X_Void task1(uint32_t ms)
{
  //insert_LogDebug(TASK_SCHEDULER_LOG_DEBUG,("hello ,ms %d \r\n",ms));
  normal_cnt1 ++;
  if(ms == 50) {freeze_task_in_schedule(2);}
  else if(ms == 80) {unfreeze_task_in_schedule(2);}
}

TEST(scheduler,add_task)
{
  uint32_t i;
  normal_cnt1 = 0;
  BeforeTest();

  add_task_to_schedule(1,task1,&isInit);
  for(i=0;i<100;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }
	EXPECT_EQ(100,normal_cnt1);
}

static X_Void task2(uint32_t ms)
{
  ms = ms;
  normal_cnt2 ++;
}

TEST(scheduler,task_freeze)
{
  uint32_t i;
  normal_cnt1 = 0;
  normal_cnt2 = 0;
  BeforeTest();

  add_task_to_schedule(1,task1,&isInit);
  add_task_to_schedule(2,task2,&isInit);
  for(i=0;i<100;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }
	EXPECT_EQ(100,normal_cnt1);
  EXPECT_EQ(70,normal_cnt2);
}
***************************************************************/

/******************************task create Test*********************************/
#define TASK_CREATE_DEBUG                    0
#undef X_TASK_LOG_DEBUG   
#define X_TASK_LOG_DEBUG              0

static X_Boolean isAckToTask2 = X_False;
static int task1_cnt = 0, task2_cnt = 0;

#include "../refresh_task_tag.h"
static X_Void marco_task1_init(X_Void)
{
  insert_LogDebug(TASK_CREATE_DEBUG,("task 1 init \r\n"));
}
TASK_BODY_BEGIN(marco_task1,marco_task1_init,0,0){
  X_do(insert_LogDebug(TASK_CREATE_DEBUG,("task 1 alive %d \r\n",x_task_param_ms));task1_cnt ++);
  X_delay(3000);
  X_do(isAckToTask2 = X_True);
}TASK_BODY_END

static X_Boolean  TaskOneRegiester(X_Void)               
{
  return X_TASK_REGISTE(1,marco_task1);
}


#include "../refresh_task_tag.h"
static X_Void marco_task2_init(X_Void)
{
  insert_LogDebug(TASK_CREATE_DEBUG,("task 2 init \r\n"));
}
TASK_BODY_BEGIN(marco_task2,marco_task2_init,0,0){
  X_do(insert_LogDebug(TASK_CREATE_DEBUG,("task 2 alive %d \r\n",x_task_param_ms));task2_cnt ++);
  X_delay(2000);
  X_wait_until(isAckToTask2 == X_True);
  //X_do(isAckToTask2 = X_False);
}TASK_BODY_END

static X_Boolean  TaskTwoRegiester(X_Void)               
{
  return X_TASK_REGISTE(2,marco_task2);
}

TEST(scheduler,task_create)
{
  uint32_t i;
  isAckToTask2 = X_False;
  task1_cnt = 0;
  task2_cnt = 0;
  BeforeTest();

  TaskOneRegiester();
  TaskTwoRegiester();
  for(i=0;i<10000;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }
	EXPECT_EQ(4,task1_cnt);
  EXPECT_EQ(5,task2_cnt);
}
/*****************************task if nest test**********************************/
#define TASK_NEST_IF_DEBUG            1
#undef X_TASK_LOG_DEBUG   
#define X_TASK_LOG_DEBUG              1

typedef enum
{
  Idle = 0,
  Send,
  Check,
  Wait,
  Error,
}eState;
static uint8_t cur_state = Idle;
static int send_cnt = 0;
/////static uint8_t child_thread_id = 0;
static int continue_to_cnt = 0,todo_1_cnt = 0,todo_2_cnt = 0,todo_2_1_cnt = 0;

/*
static X_Void NewSend1(X_Void)
{
  X_TASK_LOG(TASK_NEST_IF_DEBUG ,(" %d ------------------------- thread 2 ActionSend1 \r\n",sys_cnt_ms));
  send_cnt ++;
}
*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static X_Void if_task_1_init(X_Void){}
TASK_BODY_BEGIN(if_task1,if_task_1_init,0,0){
  X_delay(50);
  X_do(todo_1_cnt ++;
      X_TASK_LOG(TASK_NEST_IF_DEBUG & 0,(" %d --- I am thread 1 infinite loop \r\n",sys_cnt_ms))
      );
}TASK_BODY_END

static X_Boolean  IfTask1Regiester(X_Void)               
{
  return X_TASK_REGISTE(1,if_task1);
}
/*~~~~~~~~~~~~~~~if ~~~~~~~~~~~~~~~~~~~~
#include "../refresh_task_tag.h"
static X_Void if_task_2_init(X_Void){}
TASK_BODY_BEGIN(if_task2,if_task_2_init,3){
  X_if(child_thread_id == 1)
  {
    X_if(cur_state == Idle)
    {
      X_do(cur_state = Wait);
    }
    X_else_if(cur_state == Wait)
    {
      X_delay(1500);
      X_do(cur_state = Send);
      X_do(X_TASK_LOG(TASK_NEST_IF_DEBUG || 1,(" %d ------------------------- continue to do test OK \r\n",sys_cnt_ms));continue_to_cnt ++ );
    }
    X_else_if(cur_state == Send)
    {
      X_do(NewSend2());
    }
    X_endif 

    X_do(cur_state = Idle);
    X_do(child_thread_id = 2); 
  }
  X_else_if(child_thread_id == 0)
  {
    X_if(cur_state == Idle)
    {
      X_do(cur_state = Wait);
    }
    X_else_if(cur_state == Send)
    {
      X_do(NewSend1());
    }
    X_else_if(cur_state == Wait)
    {
      X_delay(3000);
      X_do(cur_state = Send);
    }
    X_endif 

    X_do(cur_state = Idle);
    X_do(child_thread_id = 1);
  }
  X_else
  {
    X_delay(1000);
    X_do(cur_state = Idle);
    X_do(child_thread_id = 0);
  }
  X_endif 

}TASK_BODY_END

static X_Boolean  IfTask2Regiester(X_Void)               
{
  return X_TASK_REGISTE(2,if_task2);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int act_cnt_1_5 = 0,log_cnt_1_5 = 0;
static X_Void  if_task_1_5_init(X_Void){}
TASK_BODY_BEGIN(if_task1_5,if_task_1_5_init,1,0){
  X_if(cur_state == Idle)
  {
    X_do(act_cnt_1_5 ++/*;printf(" --------- %d ms before condition change  \r\n",sys_cnt_ms)*/);
    X_do(cur_state = (act_cnt_1_5 >= 3) ? Wait:Idle);
    X_do(log_cnt_1_5 ++);
    X_do(/*printf(" --------- %d ms after condition change  \r\n",sys_cnt_ms)*/);
  }X_endif

  X_delay(50);
}TASK_BODY_END

static X_Boolean  IfTask1_5Regiester(X_Void)               
{
  return X_TASK_REGISTE(15,if_task1_5);
}
/*~~~~~~~~~~~~~~~~~nest error ~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int log_cnt_3 = 0;
TASK_BODY_BEGIN(if_task3,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(3 == 3)
  {
    X_if(4 == 4){
        X_do(log_cnt_3 ++);
    }X_endif
    
  }X_endif
}TASK_BODY_END

static X_Boolean  IfTask3Regiester(X_Void)               
{
  return X_TASK_REGISTE(3,if_task3);
}
/*~~~~~~~~~~~~~~~~if else ~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int log_cnt_4 = 0;
static int flag_cnt_4 = 0;
TASK_BODY_BEGIN(if_task4,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(flag_cnt_4 > 0)
  {
    X_delay(1000);
    X_do(/*printf(" --------- %d task 4 if \r\n",sys_cnt_ms);*/log_cnt_4++);
  }
  X_else
  {
    X_do(/*printf(" --------- %d task 4 else \r\n",sys_cnt_ms)*/);
    X_delay(1000);
    X_do(flag_cnt_4 = 1);
  }
  X_endif

  X_delay(100);
  X_do(/*printf(" --------- %d task 4 end \r\n",sys_cnt_ms)*/);

}TASK_BODY_END

static X_Boolean  IfTask4Regiester(X_Void)               
{
  return X_TASK_REGISTE(4,if_task4);
}
/*~~~~~~~~~~~~~~~~~~lack of endif error ~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int act_cnt_5 = 0;
TASK_BODY_BEGIN(if_task5,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(3 == 4)
  {

  }

  X_do(act_cnt_5 ++);

}TASK_BODY_END

static X_Boolean  IfTask5Regiester(X_Void)               
{
  return X_TASK_REGISTE(5,if_task5);
}
/*~~~~~~~~~~~~~~~state machine by if else if ~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static uint8_t state6 = 0,state_still_cnt6 = 0,state6_eq_1_cnt = 0;
static int condition_cnt6_1 = 0,condition_cnt6_2 = 0;
static int hello_buf6[5];
X_Boolean DoesState6_EQ_1(void)
{
  condition_cnt6_1 ++;
  return (state6 == 1);
}
X_Boolean DoesState6_EQ_2(void)
{
  condition_cnt6_2 ++;
  return (state6 == 2);
}
TASK_BODY_BEGIN(if_task6,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(state6 == 0)
  {
    X_delay(100);
    ///////////X_do(printf("--------- %d ms ; TASK8if_line_num_buf %d \r\n",sys_cnt_ms,TASK8if_line_num_buf[0]));
    X_do(state6 = 3);
    X_do(hello_buf6[0] = 'h');//X_do(printf("--------- %d ms h \r\n",sys_cnt_ms));
  }
  X_else_if(DoesState6_EQ_1())
  {
    X_delay(200);
  }
  X_else_if(DoesState6_EQ_2())
  {
    X_do(state_still_cnt6 ++);
    X_do(state6 = (state_still_cnt6 >= 2)? 4 : 2);
    X_do(hello_buf6[state_still_cnt6 + 1] = 'l');//X_do(printf("--------- %d ms l \r\n",sys_cnt_ms));
  }
  X_else_if(state6 == 3)
  {
    X_do(state6 = 2);
    X_do(hello_buf6[1] = 'e');//X_do(printf("--------- %d ms e \r\n",sys_cnt_ms));
  }
   X_else_if(state6 == 4)
  {
    X_do(hello_buf6[4] = 'o';state6 = 1);//X_do(printf("--------- %d ms o \r\n",sys_cnt_ms);state6 = 1);
  }
  X_endif

  X_if(state6 == 1)
  {
    X_do(/*printf("--------- %d ms state6 = 1 \r\n",sys_cnt_ms);*/state6_eq_1_cnt ++);
  }
  X_endif

}TASK_BODY_END

static X_Boolean  IfTask6Regiester(X_Void)               
{
  return X_TASK_REGISTE(6,if_task6);
}
/*~~~~~~~~~~~~~~~~nest if else if ~~~~~~~~~~~~~~~~~~~*/
//  X_do(state7_0 = 100;state7_1 = 100;state7_2 = 100);
#include "../refresh_task_tag.h"
static int next_if_cnt7 = 0;
static uint8_t state7_0 = 0,state7_1 = 0,state7_2 = 0;
static X_Boolean isUsed7 = X_False;

TASK_BODY_BEGIN(if_task7,X_TASK_NULL_INIT_FUNC,2,0){
  X_if(state7_0 == 0)
  {
      X_if(isUsed7 == X_False)
      {
        X_do(printf(" --------- %d ms l \r\n",sys_cnt_ms));
        X_do(state7_1 = 3;isUsed7 = X_True);
      }
      X_endif
      
      X_if(state7_1 == 0)
      {
        X_do(printf(" --------- %d ms d \r\n",sys_cnt_ms););
      }
      X_else_if(state7_1 == 1)
      {
        X_do(printf(" --------- %d ms d \r\n",sys_cnt_ms););
        X_do(state7_0 = 0;state7_1 = 100);
      }
      X_else_if(state7_1 == 2)
      {
          X_do(state7_0 = 1;state7_1 = 1;state7_2 = 1);
          X_do(printf(" --------- %d ms o \r\n",sys_cnt_ms));
      }
      X_else_if(state7_1 == 3)
      {
        X_do(printf(" --------- %d ms l \r\n",sys_cnt_ms));
        X_do(state7_1 = 2);
      }
      X_else_if(state7_1 == 4)
      {

      }
      X_else
      {
        X_do(printf(" --------- %d ms e \r\n",sys_cnt_ms););
        X_do(state7_0 = 0;state7_1 = 4);
        X_do(printf(" --------- %d ms n \r\n",sys_cnt_ms););
        X_delay(30);
        X_do(printf(" --------- %d ms d \r\n",sys_cnt_ms););
      }
      X_endif
  }
  X_else_if(state7_0 == 1)
  {
      X_if(state7_1 == 0)
      {
          X_do(state7_2 = 0;state7_1 = 1);
          X_do(printf(" --------- %d ms w \r\n",sys_cnt_ms));
      }
      X_else_if(state7_1 == 1)
      {
          X_if(state7_2 == 1)
          {
              X_do(printf(" --------- %d ms _ \r\n",sys_cnt_ms));
              X_do(state7_1 = 0);
              X_return;
          }
          X_else_if(state7_2 == 0)
          {
              X_do(state7_0 = 3;state7_1 = 0);
              X_do(printf(" --------- %d ms o \r\n",sys_cnt_ms));
              X_delay(200);
              X_do(printf(" --------- %d ms r \r\n",sys_cnt_ms));
          }
          X_endif
      }
      X_endif
  }
  X_else_if(state7_0 == 2)
  {
      X_if(state7_1 == 0)
      {
        X_do(state7_0 = 0);
        X_do(printf(" --------- %d ms e \r\n",sys_cnt_ms));
      }
      X_else_if(state7_1 == 1)
      {
        X_do(state7_1 = 0);
        X_do(printf(" --------- %d ms h \r\n",sys_cnt_ms));
      }
      X_endif
  }
  X_else_if(state7_0 == 3)
  {
      X_if(state7_1 == 0)
      {
        X_do(state7_0 = 0;state7_1 = 1);
        X_do(printf(" --------- %d ms l \r\n",sys_cnt_ms));
      }
      X_endif
  }
  X_else
  {
    X_return;
  }
  X_endif

  X_if(1 == 1)
  {
    X_delay(100);
    X_do(next_if_cnt7 ++/*;printf("--------- %d reach the task7 end \r\n",sys_cnt_ms)*/);
  }
  X_endif

}TASK_BODY_END

static X_Boolean  IfTask7Regiester(X_Void)               
{
  return X_TASK_REGISTE(7,if_task7);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static X_Void All_cntAboutIfTestClear(X_Void)
{
  uint16_t i;
  isAckToTask2 = X_False;
  task1_cnt = 0;
  task2_cnt = 0;
  cur_state = Idle;
  send_cnt = 0;
  continue_to_cnt = 0;
  todo_1_cnt = 0;
  todo_2_cnt = 0;
  todo_2_1_cnt = 0;

  act_cnt_1_5 = 0;log_cnt_1_5 = 0;
  log_cnt_3 = 0;
  log_cnt_4 = 0;
  act_cnt_5 = 0;
  state6 = 0;
  condition_cnt6_1 = 0;
  condition_cnt6_2 = 0;
  state_still_cnt6 = 0;
  state6_eq_1_cnt  = 0;
  for(i=0;i<5;i++)
  {
    hello_buf6[i] = 0;
  }

  next_if_cnt7 = 0;
  state7_0 = 2;state7_1 = 1;state7_2 = 0;
  isUsed7 = X_False;
}
TEST(nest,conditionif)
{
  uint32_t i;
  All_cntAboutIfTestClear();
  BeforeTest();

  IfTask1Regiester();
  //IfTask2Regiester();
  IfTask1_5Regiester();
  IfTask3Regiester();
  IfTask4Regiester();
  IfTask5Regiester();
  IfTask6Regiester();
  IfTask7Regiester();

  for(i=0;i<5000;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }

  EXPECT_EQ(98,todo_1_cnt);

  EXPECT_EQ(3,log_cnt_1_5);
  EXPECT_EQ(3,act_cnt_1_5);

  EXPECT_EQ(0,log_cnt_3);

  EXPECT_EQ(3,log_cnt_4);

  EXPECT_EQ(1,act_cnt_5);

  EXPECT_EQ(29,condition_cnt6_1);
  EXPECT_EQ(4,condition_cnt6_2);
  EXPECT_EQ('h',hello_buf6[0]);
  EXPECT_EQ('e',hello_buf6[1]);
  EXPECT_EQ('l',hello_buf6[2]);
  EXPECT_EQ('l',hello_buf6[3]);
  EXPECT_EQ('o',hello_buf6[4]);
  EXPECT_EQ(25,state6_eq_1_cnt);

  EXPECT_EQ(47,next_if_cnt7);
}
/***************************************************************/

/****************************task for loop test***********************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int act_cnt10 = 0;
TASK_BODY_BEGIN(loop_task10,X_TASK_NULL_INIT_FUNC,0,0){
  X_while(1==1)
  {
    X_do(act_cnt10 ++);
  }
}TASK_BODY_END

static X_Boolean  LoopTask10Regiester(X_Void)               
{
  return X_TASK_REGISTE(10,loop_task10);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int act_cnt11 = 0;
TASK_BODY_BEGIN(loop_task11,X_TASK_NULL_INIT_FUNC,0,0){
  X_while(1==1)
  {
    X_while(1==1)
    {
      X_do(act_cnt11 ++);
    }X_endloop
    
  }X_endloop

}TASK_BODY_END

static X_Boolean  LoopTask11Regiester(X_Void)               
{
  return X_TASK_REGISTE(11,loop_task11);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int act_cnt12 = 0,act_cnt12_1 = 0;
TASK_BODY_BEGIN(loop_task12,X_TASK_NULL_INIT_FUNC,0,0){
 X_while(2==2)
 {
    //X_do(printf(" cur loop_line_num_backup %d \r\n",loop_line_num_backup));
    X_do(act_cnt12++);
 }X_endloop

  X_do(printf(" ------- cur loop_line_num_backup %d \r\n",loop_line_num_backup));
  X_delay(500);
  X_do(act_cnt12_1++);
}TASK_BODY_END

static X_Boolean  LoopTask12Regiester(X_Void)               
{
  return X_TASK_REGISTE(12,loop_task12);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int cnt13_1 = 0,cnt13_2 = 0,cnt13_3 = 0,cnt13_condition_call = 0;
static X_Boolean DoesCnt13_2_LT_200(X_Void)
{
  //printf("----- %d ms DoesCnt13_2_LT_200\r\n",sys_cnt_ms);
  cnt13_condition_call ++;
  return (cnt13_2 < 200);
}
TASK_BODY_BEGIN(loop_task13,X_TASK_NULL_INIT_FUNC,0,0){
  X_delay(100);
  X_while(cnt13_1 < 200)
  {
    X_do(cnt13_1 ++);
    //X_do(printf("----- %d ms cnt13_1 ++ = %d\r\n",sys_cnt_ms,cnt13_1));
    X_delay(10);
  }X_endloop

  X_while(DoesCnt13_2_LT_200())
  {
    X_do(cnt13_2 ++);
    //X_do(printf("----- %d ms cnt13_2 ++ = %d\r\n",sys_cnt_ms,cnt13_2));
    X_delay(10);
  }X_endloop

  X_delay(10);
  X_do(cnt13_3 ++);
  //X_do(printf("----- %d ms cnt13_3 ++ = %d\r\n",sys_cnt_ms,cnt13_3));

}TASK_BODY_END

static X_Boolean  LoopTask13Regiester(X_Void)               
{
  return X_TASK_REGISTE(13,loop_task13);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int cnt14_1 = 0,cnt14_2 = 0;
TASK_BODY_BEGIN(loop_task14,X_TASK_NULL_INIT_FUNC,0,0){
  X_delay(100);
   /*X_do(printf("----- %d ms  before loop condition %2x , line %d , end line %d cnt14_1 %d cur_line_num %d ; %d\r\n"
                      ,sys_cnt_ms,TASK14loop_condition_buf[0],TASK14loop_line_num_buf[0],TASK14loop_end_line_num_buf[0],
                      cnt14_1,TASK14cur_line_num,loop_line_num_backup));*/
  X_while(1==1)
  {
    X_delay(10);
    ////X_do(printf("----- %d ms before cnt14_1 = %d\r\n",sys_cnt_ms,cnt14_1));
    X_if(cnt14_1 > 150)
    {
      ////X_do(printf("----- %d ms after cnt14_1 = %d\r\n",sys_cnt_ms,cnt14_1));
      X_break;
    }
    X_endif

    X_do(cnt14_1++);
    /*
    X_do(printf("----- %d ms  in loop condition %2x , line %d , end line %d cnt14_1 %d cur_line_num %d ,loop_line_num_backup %d\r\n"
                      ,sys_cnt_ms,TASK14loop_condition_buf[0],TASK14loop_line_num_buf[0],TASK14loop_end_line_num_buf[0],
                      cnt14_1,TASK14cur_line_num,loop_line_num_backup));*/
  }
  /*X_do(printf("----- %d ms  end loop condition %2x , line %d , end line %d cnt14_1 %d cur_line_num %d ,loop_line_num_backup %d\r\n"
                      ,sys_cnt_ms,TASK14loop_condition_buf[0],TASK14loop_line_num_buf[0],TASK14loop_end_line_num_buf[0],
                      cnt14_1,TASK14cur_line_num,loop_line_num_backup));*/
  X_endloop

  X_delay(50);
  X_do(cnt14_2++);
  //X_do(printf("----- %d ms cnt14_2 ++ = %d\r\n",sys_cnt_ms,cnt14_2));
  X_delay(100);

}TASK_BODY_END

static X_Boolean  LoopTask14Regiester(X_Void)               
{
  return X_TASK_REGISTE(14,loop_task14);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int i_15 = 0,j_15 = 0,loop_15_cnt = 0,jump_out_cnt15 = 0;
TASK_BODY_BEGIN(loop_task15,X_TASK_NULL_INIT_FUNC,0,1){
  X_while(i_15 < 100)
  {
      X_do(i_15 ++);
      //X_do(printf(" ---- %d first while loop begin i %d\r\n",sys_cnt_ms,i_15));
      X_while(j_15 < 20)
      {
        X_do(j_15 ++;loop_15_cnt++);
        //X_do(printf(" ---- %d second loop end j %d\r\n",sys_cnt_ms,j_15));
      }X_endloop
      ////X_do(printf(" ---- %d first while loop end i %d j %d\r\n",sys_cnt_ms,i_15,j_15));
      X_do(j_15 = 0);
  }X_endloop

 X_do(jump_out_cnt15++);
  ////X_do(printf(" ---- %d jump out two while loop\r\n",sys_cnt_ms));
  X_delay(5000);
}TASK_BODY_END

static X_Boolean  LoopTask15Regiester(X_Void)               
{
  return X_TASK_REGISTE(15,loop_task15);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int i_16 = 0, j_16 = 0, k_16 = 0,end_cnt16 = 0;
static uint16_t flag_16 = 1;
TASK_BODY_BEGIN(loop_task16,X_TASK_NULL_INIT_FUNC,0,2){
  X_if(flag_16 == 1)
  {
      X_while(i_16 < 5)
      {
          X_while(j_16 < 10)
          {
              X_while(k_16 < 7)
              {
                X_delay(10);
                X_do(k_16 ++);
                //X_do(printf(" ----- %d  k_16 %d  \r\n",sys_cnt_ms,k_16));
              }X_endloop
            X_do(j_16 ++);
            //X_do(printf(" ----- %d  j_16 %d  \r\n",sys_cnt_ms,j_16));
            X_delay(5);
          }X_endloop
        X_do(i_16 ++);
        //X_do(printf(" ----- %d  i_16 %d  \r\n",sys_cnt_ms,i_16));
        X_delay(20);
      }X_endloop
    X_do(flag_16 = 2);
  }X_endif

  X_delay(100);
  X_do(end_cnt16 ++);
  //X_do(printf(" ----- %d reach task16 end \r\n",sys_cnt_ms));
}TASK_BODY_END

static X_Boolean  LoopTask16Regiester(X_Void)               
{
  return X_TASK_REGISTE(16,loop_task16);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static int i_17 = 0, j_17 = 0, k_17 = 0,end_cnt17 = 0;
static uint16_t flag_17 = 1;
TASK_BODY_BEGIN(loop_task17,X_TASK_NULL_INIT_FUNC,1,2){
  X_if(flag_17 == 1)
  {
      X_while(i_17 < 5)
      {
          X_while(j_17 < 10)
          {
              X_while(k_17 < 7)
              {
                X_delay(10);
                X_do(k_17 ++);
                //X_do(printf(" ----- %d  k_17 %d  \r\n",sys_cnt_ms,k_17));
              }X_endloop
            X_do(j_17 ++);
            //X_do(printf(" ----- %d  j_17 %d  \r\n",sys_cnt_ms,j_17));
            X_delay(5);

            X_if(j_17 == 5)
            {
              //X_do(printf(" ----- %d  break  j_17 %d  \r\n",sys_cnt_ms,j_17));
              X_delay(1000);
              X_break;
            }X_endif

          }X_endloop
        X_do(i_17 ++);
        //X_do(printf(" ----- %d  i_17 %d  \r\n",sys_cnt_ms,i_17));
        X_delay(20);
      }X_endloop
    X_do(flag_17 = 2);
  }X_endif

  X_delay(100);
  X_do(end_cnt17 ++);
  //X_do(printf(" ----- %d reach task16 end \r\n",sys_cnt_ms));
}TASK_BODY_END

static X_Boolean  LoopTask17Regiester(X_Void)               
{
  return X_TASK_REGISTE(17,loop_task17);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static X_Void AllVariableClear(X_Void)
{
  act_cnt10 = 0;
  act_cnt11 = 0;
  act_cnt12 = 0;
  act_cnt12_1 = 0;
  cnt13_1 = 0;cnt13_2 = 0;cnt13_3 = 0;cnt13_condition_call = 0;
  cnt14_1 = 0;cnt14_2 = 0;
  i_15 = 0,j_15 = 0,loop_15_cnt = 0;
  jump_out_cnt15 = 0;
  i_16 = 0;j_16 = 0;k_16 = 0;flag_16 = 1;end_cnt16 = 0;
  i_17 = 0; j_17 = 0; k_17 = 0;flag_17 = 1;end_cnt17 = 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TEST(nest,conditionloop)
{
  uint32_t i;
  BeforeTest();
  AllVariableClear();

  LoopTask10Regiester();
  LoopTask11Regiester();
  LoopTask12Regiester();
  LoopTask13Regiester();
  LoopTask14Regiester();
  LoopTask15Regiester();
  LoopTask16Regiester();
  LoopTask17Regiester();

  for(i=0;i<5000;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }

  EXPECT_EQ(1,act_cnt10);
  EXPECT_EQ(0,act_cnt11);
  EXPECT_EQ(4999,act_cnt12);
  EXPECT_EQ(0,act_cnt12_1);

  EXPECT_EQ(200,cnt13_1);
  EXPECT_EQ(200,cnt13_2);
  EXPECT_EQ(5,cnt13_3);
  EXPECT_EQ(205,cnt13_condition_call);

  EXPECT_EQ(151,cnt14_1);
  EXPECT_EQ(13,cnt14_2);

  EXPECT_EQ(2000,loop_15_cnt);
  EXPECT_EQ(1,jump_out_cnt15);

  EXPECT_EQ(5,i_16);
  EXPECT_EQ(10,j_16);
  EXPECT_EQ(7,k_16);
  EXPECT_EQ(47,end_cnt16);

  EXPECT_EQ(5,i_17);
  EXPECT_EQ(10,j_17);
  EXPECT_EQ(7,k_17);
  EXPECT_EQ(37,end_cnt17);
  
}
/****************************task for ms boundary***********************************/
static int synchron_cnt_20 = 0,matched_cnt20 = 0,matched_cnt22 = 0;
static uint32_t task_ms_cnt = 0;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
TASK_BODY_BEGIN(loop_task20,X_TASK_NULL_INIT_FUNC,0,0){
  X_delay(400);
  X_do(synchron_cnt_20 = 1);
}TASK_BODY_END

static X_Boolean  LoopTask20Regiester(X_Void)               
{
  return X_TASK_REGISTE(20,loop_task20);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

TASK_BODY_BEGIN(loop_task21,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(synchron_cnt_20 == 1)
  {
    //X_do(printf(" ----- %d ms synchron_cnt_20 matched\r\n",task_ms_cnt));
    X_do(matched_cnt20 ++);
  }X_endif
}TASK_BODY_END

static X_Boolean  LoopTask21Regiester(X_Void)               
{
  return X_TASK_REGISTE(21,loop_task21);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static X_Boolean isConditionMatched = X_False;
TASK_BODY_BEGIN(loop_task22,X_TASK_NULL_INIT_FUNC,0,0){
  X_time_limited_wait_with_result(300,synchron_cnt_20 == 1,isConditionMatched);
  X_if(isConditionMatched == X_True)
  {
    X_do(printf(" ----- %d ms isConditionMatched true\r\n",task_ms_cnt));
    X_do(matched_cnt22++);
  }
  X_else
  {
    X_do(printf(" ----- %d ms isConditionMatched false\r\n",task_ms_cnt));
  }X_endif
  X_time_limited_wait(300,synchron_cnt_20 == 1);
  X_do(matched_cnt22++);
  X_delay(300);
}TASK_BODY_END

static X_Boolean  LoopTask22Regiester(X_Void)               
{
  return X_TASK_REGISTE(22,loop_task22);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TEST(scheduler,ms_boundary)
{
  uint32_t i;
  BeforeTest();
  sys_cnt_ms = 0xffffffff - 300;
  task_ms_cnt = 0;
  synchron_cnt_20 = 0;
  matched_cnt20 = 0;
  isConditionMatched = X_False;
  matched_cnt22 = 0;

  LoopTask20Regiester();
  LoopTask21Regiester();
  LoopTask22Regiester();

  for(i=0;i<500;i++)
  {
    loop_task_scheduler();
    SysTickIRQ();
    task_ms_cnt ++;
  }

  EXPECT_EQ(99,matched_cnt20);
  EXPECT_EQ(1,matched_cnt22);
  
}
/****************************if while return break nest mix test ***********************************/
static const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
X_Boolean Is_Leap_Year(uint16_t _year)// this func in here OK ? // fix it later
{                     
	if (_year % 4 == 0) /* 必须能被4整除 */
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
			{
				return X_True;	/* 如果以00结尾,还要能被400整除 */
			}
			else 
			{
				return X_False;   
			}

		}
		else 
		{
			return X_True;   
		}
	}
	else 
	{
		return X_False; 
	}
}      


static uint8_t month,day,hour,minute,sec;
static uint16_t year;

static uint8_t x_month,x_day,x_hour,x_minute,x_sec;
static uint16_t x_year;
static uint32_t x_cnt = 0;
static X_Boolean isBreak = X_False;

static X_Void mFunc_CntToRtcData(uint32_t cnt)
{
	uint32_t temp = 0;
	uint16_t temp1 = 0;

	temp = cnt / 86400;   // 得到天数 

	temp1 = 1970;  // 从1970年开始 

	while (temp >= 365)
	{                          
		if (Is_Leap_Year(temp1) == X_True)	// 是闰年 
		{
			if (temp >= 366)
			{
				temp -= 366;		// 闰年的秒钟数 
			}
			else
			{
				//temp1++;		// armfly: 这里闰年处理错误，不能加1 
				break;
			}  
		}
		else 
		{
			temp -= 365;       // 平年 
		}
		temp1++;  
	}   
	year = temp1;	// 得到年份 
	printf("temp %d\r\n",temp);
	temp1 = 0;
	while (temp >= 28)	// 超过了一个月 //
	{
		if((Is_Leap_Year(year) == X_True) && (temp1 == 1))	// 当年是不是闰年/2月份 
		{
      //printf("~~~~~~~~~~~~~ 1\r\n");
			if (temp >= 29)
			{
         //printf("~~~~~~~~~~~~~ 1.1\r\n");
				temp -= 29;	// 闰年的秒钟数 
			}
			else
			{
        //printf("~~~~~~~~~~~~~ 1.2\r\n");
				break; 
			}
    }
    else 
		{
      printf("~~~~~~~~~~~~~ 2\r\n");
			if (temp >= mon_table[temp1])
			{
        printf("~~~~~~~~~~~~~ 2.1\r\n");
				temp -= mon_table[temp1];	// 平年 
			}
			else 
			{
        printf("~~~~~~~~~~~~~ 2.2\r\n");
				break;
			}
		}
		temp1++;  
    printf(" —————————————————————————————————————————————— temp1 %d\r\n",temp1);
	}
	month = temp1 + 1;	// 得到月份 
	day = temp + 1;  // 得到日期 

	temp = cnt % 86400;    // 得到秒钟数 

	hour = temp / 3600;	// 小时 

	minute = (temp % 3600) / 60; // 分钟 

	sec = (temp % 3600) % 60; // 秒钟 

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

static 	uint32_t x_temp = 0;
static	uint16_t x_temp1 = 0;

static X_Boolean DoesDayLeftMoreThanAMonth(X_Void)
{
  printf("DoesDayLeftMoreThanAMonth !!! \r\n");
  return (x_temp >= mon_table[x_temp1]);
}
TASK_BODY_BEGIN(loop_task30,X_TASK_NULL_INIT_FUNC,1,0){

	X_do(x_temp = (x_cnt / 86400));   // 得到天数 

	X_do(x_temp1 = 1970);  // 从1970年开始 

	X_while (x_temp >= 365)
	{                          
		X_if (Is_Leap_Year(x_temp1) == X_True)	// 是闰年 
		{
			X_if (x_temp >= 366)
			{
				X_do(x_temp -= 366);		// 闰年的秒钟数 
			}
			X_else
			{
				//temp1++;		// armfly: 这里闰年处理错误，不能加1 
				X_break;
			}X_endif  
		}
		X_else 
		{
			X_do(x_temp -= 365);       // 平年 
		}X_endif
		X_do(x_temp1++);  
	}X_endloop   
	X_do(x_year = x_temp1);	// 得到年份 
	X_do(printf("task temp %d\r\n",x_temp));
	X_do(x_temp1 = 0);
	X_while (x_temp >= 28)	// 超过了一个月 //
	{
		X_if((Is_Leap_Year(x_year) == X_True) && (x_temp1 == 1))	// 当年是不是闰年/2月份 
		{
      //X_do(printf("~~~~~~~~~~~~~ 1\r\n"));
			X_if (x_temp >= 29)
			{
        //X_do(printf("~~~~~~~~~~~~~ 1.1\r\n"));
				X_do(x_temp -= 29);	// 闰年的秒钟数 
			}
			X_else
			{
        //X_do(printf("~~~~~~~~~~~~~ 1.2\r\n"));
				X_break; 
			}X_endif
    }
    X_else 
		{
      X_do(printf("~~~~~~~~~~~~~ 2   ; line %d ; nest_cnt %d \r\n",TASK21if_line_num_buf[nest_cnt],nest_cnt));
			X_if (DoesDayLeftMoreThanAMonth())//(x_temp >= mon_table[x_temp1])
			{
        X_do(printf("~~~~~~~~~~~~~ 2.1; line %d ; nest_cnt %d \r\n",TASK21if_line_num_buf[nest_cnt - 1],nest_cnt));
        X_delay(100);
        X_do(printf("~~~~~~~~~~~~~ 2.1.1; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
				X_do(x_temp -= mon_table[x_temp1]);	// 平年 
        X_delay(10);
        X_do(printf("~~~~~~~~~~~~~ 2.1.2; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
			}
			X_else 
			{
        X_do(printf("~~~~~~~~~~~~~ 2.2; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
				X_break;
			}X_endif
		}X_endif
		X_do(x_temp1++);  
    X_do(printf(" —————————————————————————————————————————————— task temp1 %d\r\n",x_temp1));
	}X_endloop

	X_do(x_month = x_temp1 + 1);	// 得到月份 
	X_do(x_day = x_temp + 1);  // 得到日期 

	X_do(x_temp = x_cnt % 86400);    // 得到秒钟数 

	X_do(x_hour = x_temp / 3600);	// 小时 

	X_do(x_minute = (x_temp % 3600) / 60); // 分钟 

	X_do(x_sec = (x_temp % 3600) % 60); // 秒钟 

  X_do(freeze_task_in_schedule(30);isBreak = X_True);
}TASK_BODY_END

static X_Boolean  LoopTask30Regiester(X_Void)               
{
  return X_TASK_REGISTE(30,loop_task30);
}

static X_Void monitor(uint32_t cnt_ref)
{
  mFunc_CntToRtcData(cnt_ref);
  printf(" %d year %d month %d day %d hour %d min %d second \r\n",year,month,day,hour,minute,sec);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TEST(nest,if_loop_break_mix)
{
  BeforeTest();
  x_temp = 0;
  x_temp1 = 0;

  LoopTask30Regiester();

  monitor(1667135302);
  EXPECT_EQ(2022,year);
  EXPECT_EQ(10,month);
  EXPECT_EQ(30,day);
  EXPECT_EQ(13,hour);
  EXPECT_EQ(8,minute);
  EXPECT_EQ(22,sec);

  x_cnt = 1667135302;


  while(isBreak == X_False)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }

  printf("~~0~~ %d ms %d year %d month %d day %d hour %d min %d second \r\n",sys_cnt_ms,x_year,x_month,x_day,x_hour,x_minute,x_sec);

  //EXPECT_EQ(99,matched_cnt20);

}

TEST(nest,if_loop_break_mix1)
{
  BeforeTest();
  x_temp = 0;
  x_temp1 = 0;
  isBreak = X_False;

  x_cnt = 1664135302;

  LoopTask30Regiester();

  monitor(x_cnt);
  /*
  EXPECT_EQ(2022,year);
  EXPECT_EQ(10,month);
  EXPECT_EQ(30,day);
  EXPECT_EQ(13,hour);
  EXPECT_EQ(8,minute);
  EXPECT_EQ(22,sec);*/

  while(isBreak == X_False)
  {
    loop_task_scheduler();
    SysTickIRQ();
  }

  printf("~~1~~ %d ms %d year %d month %d day %d hour %d min %d second \r\n",sys_cnt_ms,x_year,x_month,x_day,x_hour,x_minute,x_sec);

  //EXPECT_EQ(99,matched_cnt20);

}

/*************************************************************************************/
GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from thread_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
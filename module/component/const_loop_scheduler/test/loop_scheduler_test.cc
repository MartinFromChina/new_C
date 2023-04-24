#include <iostream>
#include "tdd_common.h"

#include "../const_loop_scheduler.h"
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

static X_Void BeforeTest(const s_X_Tasks_Manager * manager)
{
  sys_cnt_ms = 0;
  x_task_init(manager);
}

/*****************************scheduler test**********************************/

#define TASK_SCHEDULER_LOG_DEBUG                    1

static int normal_cnt1 = 0,normal_cnt2 = 0;
static X_Void t1_freeze(uint16_t task_id);
static X_Void t1_unfreeze(uint16_t task_id);

static X_Void task1(uint32_t ms,uint16_t task_id,uint8_t *p_isInitDone)
{
  //insert_LogDebug(TASK_SCHEDULER_LOG_DEBUG,("hello ,ms %d \r\n",ms));
  UNUSED_VARIABLE(task_id); UNUSED_VARIABLE(p_isInitDone);
  normal_cnt1 ++;
  if(ms == 50) {t1_freeze(2);}
  else if(ms == 80) {t1_unfreeze(2);}
}

static X_Void task2(uint32_t ms,uint16_t task_id,uint8_t *p_isInitDone)
{
  ms = ms;UNUSED_VARIABLE(task_id); UNUSED_VARIABLE(p_isInitDone);
  normal_cnt2 ++;
}

static const s_X_TaskList task1_list[] = {
  {1,task1},
  {2,task2},
};

X_TASK_SCHEDULER_DEF(sch_t1,sizeof(task1_list)/sizeof(task1_list[0]),task1_list);

static X_Void t1_freeze(uint16_t task_id)
{
  freeze_x_task(sch_t1,task_id);
}

static X_Void t1_unfreeze(uint16_t task_id)
{
  unfreeze_x_task(sch_t1,task_id);
}

TEST(scheduler,task_freeze)
{
  uint32_t i;
  normal_cnt1 = 0;
  normal_cnt2 = 0;
  BeforeTest(sch_t1);

  for(i=0;i<100;i++)
  {
    x_task_scheduler(sch_t1,GetSystemTime());
    SysTickIRQ();
  }
	EXPECT_EQ(100,normal_cnt1);
  EXPECT_EQ(70,normal_cnt2);
}
/***************************************************************/

/******************************task create Test*********************************/
#define TASK_CREATE_DEBUG                    0
//#undef X_TASK_LOG_DEBUG   
//#define X_TASK_LOG_DEBUG              0

static X_Boolean isAckToTask2 = X_False;
static int task1_cnt = 0, task2_cnt = 0;

#include "../refresh_task_tag.h"
static X_Void marco_task1_init(uint32_t ms)
{
  insert_LogDebug(TASK_CREATE_DEBUG,("task 1 init %d ms\r\n",ms));
}
TASK_BODY_BEGIN(marco_task1,marco_task1_init,0,0){
  X_do(insert_LogDebug(TASK_CREATE_DEBUG,("task 1 alive %d \r\n",x_task_param_ms));task1_cnt ++);
  X_delay(3000);
  X_do(isAckToTask2 = X_True);
}TASK_BODY_END


#include "../refresh_task_tag.h"
static X_Void marco_task2_init(uint32_t ms)
{
  insert_LogDebug(TASK_CREATE_DEBUG,("task 2 init %d ms\r\n",ms));
}
TASK_BODY_BEGIN(marco_task2,marco_task2_init,0,0){
  X_do(insert_LogDebug(TASK_CREATE_DEBUG,("task 2 alive %d \r\n",x_task_param_ms));task2_cnt ++);
  X_delay(2000);
  X_wait_until(isAckToTask2 == X_True);
  //X_do(isAckToTask2 = X_False);
}TASK_BODY_END

static const s_X_TaskList tc_task_list[] = {
  {1,marco_task1},
  {2,marco_task2},
};

X_TASK_SCHEDULER_DEF(tc,sizeof(tc_task_list)/sizeof(tc_task_list[0]),tc_task_list);

TEST(scheduler,task_create)
{
  uint32_t i;
  isAckToTask2 = X_False;
  task1_cnt = 0;
  task2_cnt = 0;
  BeforeTest(tc);

  for(i=0;i<10000;i++)
  {
    x_task_scheduler(tc,GetSystemTime());
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
static X_Void if_task_1_init(uint32_t ms){ms = ms;}
TASK_BODY_BEGIN(if_task1,if_task_1_init,0,0){
  X_delay(50);
  X_do(todo_1_cnt ++;
      X_TASK_LOG(TASK_NEST_IF_DEBUG & 0,(" %d --- I am thread 1 infinite loop \r\n",sys_cnt_ms))
      );
}TASK_BODY_END
/*~~~~~~~~~~~~~~~if ~~~~~~~~~~~~~~~~~~~~*
#include "../refresh_task_tag.h"
static X_Void if_task_2_init(X_Void){}
TASK_BODY_BEGIN(if_task2,if_task_2_init,3,0){
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
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int act_cnt_1_5 = 0,log_cnt_1_5 = 0;
static X_Void  if_task_1_5_init(uint32_t ms){ms = ms;}
TASK_BODY_BEGIN(if_task1_5,if_task_1_5_init,1,0){
  X_if(cur_state == Idle)
  {
    X_do(act_cnt_1_5 ++);
    //X_do(printf(" --------- %d ms before condition change  \r\n",sys_cnt_ms));
    X_do(cur_state = (act_cnt_1_5 >= 3) ? Wait:Idle);
    X_do(log_cnt_1_5 ++);
    //X_do(printf(" --------- %d ms after condition change  \r\n",sys_cnt_ms));
  }X_endif

  X_delay(50);
}TASK_BODY_END

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
/*~~~~~~~~~~~~~~~~if else ~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int log_cnt_4 = 0;
static int flag_cnt_4 = 0;
TASK_BODY_BEGIN(if_task4,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(flag_cnt_4 > 0)
  {
    X_delay(1000);
    //X_do(printf(" --------- %d task 4 if \r\n",sys_cnt_ms));
    X_do(log_cnt_4++);
  }
  X_else
  {
    //X_do(printf(" --------- %d task 4 else \r\n",sys_cnt_ms));
    X_delay(1000);
    X_do(flag_cnt_4 = 1);
  }
  X_endif

  X_delay(100);
  //X_do(printf(" --------- %d task 4 end \r\n",sys_cnt_ms));

}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~lack of endif error ~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"
static int act_cnt_5 = 0;
TASK_BODY_BEGIN(if_task5,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(3 == 4)
  {

  }

  X_do(act_cnt_5 ++);

}TASK_BODY_END
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
    //X_do(printf("--------- %d ms state6 = 1 \r\n",sys_cnt_ms));
    X_do(state6_eq_1_cnt ++);
  }
  X_endif

}TASK_BODY_END
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
    X_do(next_if_cnt7 ++);
    //X_do(printf("--------- %d reach the task7 end \r\n",sys_cnt_ms));
  }
  X_endif

}TASK_BODY_END

//WITH_JING(#INCLUDE(TO_STR(WITH_POINT_H(temp))))
//#include "temp.h"

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

static const s_X_TaskList ifn_task_list[] = {
  {1,if_task1},
  {15,if_task1_5},
  {3,if_task3},
  {4,if_task4},
  {5,if_task5},
  {6,if_task6},
  {7,if_task7},
};

X_TASK_SCHEDULER_DEF(ifn,sizeof(ifn_task_list)/sizeof(ifn_task_list[0]),ifn_task_list);

TEST(nest,conditionif)
{
  uint32_t i;
  All_cntAboutIfTestClear();
  BeforeTest(ifn);

  for(i=0;i<5000;i++)
  {
    x_task_scheduler(ifn,GetSystemTime());
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

static const s_X_TaskList loop_task_list[] = {
  {10,loop_task10},
  {11,loop_task11},
  {12,loop_task12},
  {13,loop_task13},
  {14,loop_task14},
  {15,loop_task15},
  {16,loop_task16},
  {17,loop_task17},
};

X_TASK_SCHEDULER_DEF(tloop,sizeof(loop_task_list)/sizeof(loop_task_list[0]),loop_task_list);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TEST(nest,conditionloop)
{
  uint32_t i;
  BeforeTest(tloop);
  AllVariableClear();

  for(i=0;i<5000;i++)
  {
    x_task_scheduler(tloop,GetSystemTime());
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
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "../refresh_task_tag.h"

TASK_BODY_BEGIN(loop_task21,X_TASK_NULL_INIT_FUNC,0,0){
  X_if(synchron_cnt_20 == 1)
  {
    //X_do(printf(" ----- %d ms synchron_cnt_20 matched\r\n",task_ms_cnt));
    X_do(matched_cnt20 ++);
  }X_endif
}TASK_BODY_END
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

static const s_X_TaskList bou_task_list[] = {
  {20,loop_task20},
  {21,loop_task21},
  {22,loop_task22},
};

X_TASK_SCHEDULER_DEF(bou,sizeof(bou_task_list)/sizeof(bou_task_list[0]),bou_task_list);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TEST(scheduler,ms_boundary)
{
  uint32_t i;
  BeforeTest(bou);
  sys_cnt_ms = 0xffffffff - 300;
  task_ms_cnt = 0;
  synchron_cnt_20 = 0;
  matched_cnt20 = 0;
  isConditionMatched = X_False;
  matched_cnt22 = 0;

  for(i=0;i<500;i++)
  {
    x_task_scheduler(bou,GetSystemTime());
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
	if (_year % 4 == 0) 
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
			{
				return X_True;	
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
      //printf("~~~~~~~~~~~~~ 2\r\n");
			if (temp >= mon_table[temp1])
			{
       //printf("~~~~~~~~~~~~~ 2.1\r\n");
				temp -= mon_table[temp1];	// 平年 
			}
			else 
			{
        //printf("~~~~~~~~~~~~~ 2.2\r\n");
				break;
			}
		}
		temp1++;  
    //printf(" —————————————————————————————————————————————— temp1 %d\r\n",temp1);
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

static X_Void freeze_task_30(X_Void);

static X_Boolean DoesDayLeftMoreThanAMonth(X_Void)
{
  //printf("DoesDayLeftMoreThanAMonth !!! \r\n");
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
	//X_do(printf("task temp %d\r\n",x_temp));
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
      //X_do(printf("~~~~~~~~~~~~~ 2   ; line %d ; nest_cnt %d \r\n",TASK21if_line_num_buf[nest_cnt],nest_cnt));
			X_if (DoesDayLeftMoreThanAMonth())//(x_temp >= mon_table[x_temp1])
			{
        //X_do(printf("~~~~~~~~~~~~~ 2.1; line %d ; nest_cnt %d \r\n",TASK21if_line_num_buf[nest_cnt - 1],nest_cnt));
        X_delay(100);
        //X_do(printf("~~~~~~~~~~~~~ 2.1.1; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
				X_do(x_temp -= mon_table[x_temp1]);	// 平年 
        X_delay(10);
        //X_do(printf("~~~~~~~~~~~~~ 2.1.2; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
			}
			X_else 
			{
        //X_do(printf("~~~~~~~~~~~~~ 2.2; line %d\r\n",TASK21if_line_num_buf[nest_cnt - 1]));
				X_break;
			}X_endif
		}X_endif
		X_do(x_temp1++);  
    //X_do(printf(" —————————————————————————————————————————————— task temp1 %d\r\n",x_temp1));
	}X_endloop

	X_do(x_month = x_temp1 + 1);	// 得到月份 
	X_do(x_day = x_temp + 1);  // 得到日期 

	X_do(x_temp = x_cnt % 86400);    // 得到秒钟数 

	X_do(x_hour = x_temp / 3600);	// 小时 

	X_do(x_minute = (x_temp % 3600) / 60); // 分钟 

	X_do(x_sec = (x_temp % 3600) % 60); // 秒钟 

  X_do(freeze_task_30();isBreak = X_True);
}TASK_BODY_END


static X_Void monitor(uint32_t cnt_ref)
{
  mFunc_CntToRtcData(cnt_ref);
  printf(" %d year %d month %d day %d hour %d min %d second \r\n",year,month,day,hour,minute,sec);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static const s_X_TaskList rtc_task_list[] = {
  {30,loop_task30},
};

X_TASK_SCHEDULER_DEF(rtc,sizeof(rtc_task_list)/sizeof(rtc_task_list[0]),rtc_task_list);


static X_Void freeze_task_30(X_Void)
{
	freeze_x_task(rtc,30);
}

TEST(nest,if_loop_break_mix)
{
  BeforeTest(rtc);
  x_temp = 0;
  x_temp1 = 0;

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
    x_task_scheduler(rtc,GetSystemTime());
    SysTickIRQ();
  }

  printf("~~0~~ %d ms %d year %d month %d day %d hour %d min %d second \r\n",sys_cnt_ms,x_year,x_month,x_day,x_hour,x_minute,x_sec);

  //EXPECT_EQ(99,matched_cnt20);

}

TEST(nest,if_loop_break_mix1)
{
  BeforeTest(rtc);
  x_temp = 0;
  x_temp1 = 0;
  isBreak = X_False;

  x_cnt = 1664135302;

  monitor(x_cnt);

  while(isBreak == X_False)
  {
    x_task_scheduler(rtc,GetSystemTime());
    SysTickIRQ();
  }

  printf("~~1~~ %d ms %d year %d month %d day %d hour %d min %d second \r\n",sys_cnt_ms,x_year,x_month,x_day,x_hour,x_minute,x_sec);

  //EXPECT_EQ(99,matched_cnt20);

}

/*************************************************************************************/
#include "../TemplateTask/template_task.h"

typedef struct{
  uint8_t state;
  uint16_t on_time;
  uint16_t off_time;
  uint8_t  other;
}s_param_ext;

static X_Void T_Task_Init(uint32_t t_task_ms,uint16_t instance_id,uint8_t const * p_name,X_Void *p_this)
{
  ((s_param_ext*)p_this) ->off_time = 100;
  printf("t_task_ms %d ; instance_id %d name: %s \r\n",t_task_ms,instance_id,p_name);
}
// 
TEMPLATE_TASKS_BODY_BEGIN(led_tasks,T_Task_Init,s_param_ext,6,"amy","penny","horward"){
                            T_do(p_this ->state = 1);
                            T_delay(2000);
                            T_if(p_this ->off_time != 0)
                            {
                                T_while(p_this ->off_time --)
                                {
                                    T_if(p_this ->off_time == 49)
                                    {
                                        T_do(printf("template led_tasks : instacne %d : off_time = 49 at %d ms \r\n",instance_id,t_task_ms));
                                    }T_endif
                                    T_delay(20);
                                }T_endloop
                            }
                            T_else
                            {
                                T_do(p_this ->off_time = 20);
                            }T_endif

                          }TEMPLATE_TASKS_BODY_END


TEST(template,task)
{
  uint16_t i;
  t_task_init(led_tasks);

  for(i=0;i<10000;i++)
  {
    t_task_scheduler(led_tasks,i);
  }


}
/*************************************************************************************/
#include "../HierarchicalSM/h_state_machine.h"

static uint8_t temp_cnt = 0;

HSM_LOW_LEVEL_BODY_BEGIN(level_2)
{
  H_do(printf(" I am level 2 ms %d\r\n",ms));
  H_delay(1000);    
  H_do(printf(" I am level 2 ms %d\r\n",ms));
  H_delay(1000);   

}HSM_BODY_END

HSM_LOW_LEVEL_BODY_BEGIN(level_2_1)
{
  H_do(printf(" I am level 2.1 ms %d\r\n",ms));
  H_delay(500);    
  H_do(printf(" I am level 2.1 ms %d\r\n",ms));
  H_delay(500);   

}HSM_BODY_END


HSM_LOW_LEVEL_BODY_BEGIN(level_1)
{
  H_while(1)
  {
    H_if(temp_cnt == 8)
    {
      H_do(temp_cnt = 9);
      H_do(printf("second: I am level one ms %d\r\n",ms));
      H_delay(1000);  
    }
    H_else_if(temp_cnt == 0)
    {
      H_do(temp_cnt = 8);
      H_do(printf(" first : I am level one ms %d\r\n",ms));
      H_delay(1000);    
    }
    H_else
    {
      H_wait_until(level_2(ms,p_base ->p_level_next) == X_False);
      H_wait_until(level_2_1(ms,p_base ->p_level_next) == X_False);   
      H_delay(500);
      H_break;
    }H_endif
  }H_endloop

}HSM_BODY_END

HSM_TOP_LEVEL_BODY_BEGIN(just_do,2)
{
  H_do(printf(" hello ms %d\r\n",ms));
  H_delay(1000);
  H_wait_until(level_1(ms,p_base ->p_level_next) == X_False);
}HSM_BODY_END


TEST(hsm,task)
{
  uint16_t i;
  e_x_task_error_code err;
  err = hsm_init(just_do);
   printf(" err %d \r\n",err);

  for(i=0;i<10000;i++)
  {
    hsm_run(just_do,i);
  }


}
/*************************************************************************************/

/*************************************************************************************/

/*************************************************************************************/

/*************************************************************************************/
GTEST_API_ int main(int argc, char **argv) {

/*
  uint16_t i;

  for(i=1;i<101;i++)
  {
     printf(" _%d,",i);
     if((i % 10) == 0) {printf(" \r\n");}
     
  }

  printf(" \r\n \r\n");

  for(i=0;i<101;i++)
  {
     printf(" %d,",100 - i);
     if((i % 10) == 0) {printf(" \r\n");}
     
  }
*/
  cout<<"Running main() from thread_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  #if (WINDOWS_64_SYSTEM == 0)
  	return
  #else
	int value = 
  #endif
  RUN_ALL_TESTS();

#if (WINDOWS_64_SYSTEM!=0)
  value = value;
  while(1)
  {

  }
#endif
}

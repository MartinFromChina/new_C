#include <iostream>
#include "tdd_common.h"

#include "../const_loop_scheduler.h"
#include <stdio.h>
#define LOG_DEBUG_METHOD   printf
#include "../../../common/InsertLog/InsertLogDebug.h"
#include "../TemplateTask/template_task.h"

using namespace std;


static uint32_t sys_cnt_ms = 0;

static X_Void SysTickIRQ(X_Void)
{
  sys_cnt_ms ++;
}
static uint32_t GetSystemTime(X_Void)
{
  return sys_cnt_ms;
}

static X_Void BeforeTest(const s_X_Tasks_Manager * manager)
{
  sys_cnt_ms = 0;
  x_task_init(manager);
}
/*****************************scheduler test**********************************/
/*
#define TASK_SCHEDULER_LOG_DEBUG                    1

typedef struct template_task_test
{
  s_T_taskCommonParam base;
  int normal_cnt1; 
}s_sch1_test;


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

TEMPLATE_TASKS_BODY_BEGIN(ch1_task1,(t_one_step)0,s_sch1_test,1,"shceduler1")){
  T_if()

}TEMPLATE_TASKS_BODY_END

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

TEST(T_scheduler,task_freeze)
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

*/

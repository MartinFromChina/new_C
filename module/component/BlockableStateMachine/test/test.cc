#include <iostream>
#include "tdd_common.h"

#include "../b_StateMachine.h"
#include <stdio.h>
#define LOG_DEBUG_METHOD   printf
#include "../../../common/InsertLog/InsertLogDebug.h"

using namespace std;

typedef struct
{
  s_BSM_Param base;
  uint16_t ext_cnt;
}s_param_ext;


StateNumber Action1(s_BSM_Param *p_base,uint32_t ms)
{
  BSM_PARAM_EXTERN(s_param_ext);
  printf("Action1 ms %d\r\n",ms);
  return 2;
}
StateNumber Action2(s_BSM_Param *p_base,uint32_t ms)
{
  X_Boolean isOK;
  BSM_PARAM_EXTERN(s_param_ext);
  S_do(printf("Action2 %d ;  ms %d\r\n",p_this ->base.cur_state,ms));
  // S_wait_until(ms == 328);
  //S_time_limited_wait(100,ms == 439);
  S_time_limited_wait_with_result(100,ms == 348,isOK);
  // S_do(printf("isOK %d ms %d\r\n",isOK,ms));
  S_do(
    (isOK == X_True) ? 
      printf("OK  ms %d\r\n",ms)
    :
      printf("not OK  ms %d\r\n",ms)
  );
  S_return(100);
}
StateNumber Action3(s_BSM_Param *p_base,uint32_t ms)
{
  BSM_PARAM_EXTERN(s_param_ext);
  S_do(printf("Action3 ms %d\r\n",ms));
  S_delay(100);
  S_return(1);
}

BLOCKABLE_SM_DEF(t1,X_False,Action1,Action2,Action3);

static s_param_ext ext_param;

TEST(scheduler,task_freeze)
{
  for(uint16_t i = 0;i<500;i++)
  {
      BSM_Run(t1,&ext_param.base,i);
  }
  //EXPECT_EQ(100,normal_cnt1);
  //EXPECT_EQ(70,normal_cnt2);
}

/*
BLOCKABLE_SM_DEF(t1,X_False
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3,Action1,Action2,Action3,Action1,Action2,Action3,Action1
                ,Action1,Action2,Action3
                );
*/
/***************************************************************/
GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from test.cc\n";

/*
  for(uint16_t i = 0;i<256;i++)
  {
    printf(" _%d,",i);
  }
  printf(" \r\n");

  for(uint16_t i = 0;i<256;i++)
  {
    printf(" %d,",256 - i);
  }
  */
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

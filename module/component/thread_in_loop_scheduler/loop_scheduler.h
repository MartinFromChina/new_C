/****************************************************************************************************
the X_task_marco use line number as state flag to implement state-jump. if you want to be more efficient, use ProtoThread
in the task body:
there must be a X_marco at the beginning of each code line 
only one X_marco in the same code line
********************************************************************************************************/

#ifndef __LOOP_SCHEDULER_H
#define __LOOP_SCHEDULER_H

#ifdef __cplusplus
	extern "C" {

#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"

/**********************************************************************************************************************/
#define MAX_X_TASK_NUMBER      (0xffff)

#ifndef TOTAL_X_TASK_NUMBER
    #define  TOTAL_X_TASK_NUMBER 50
#endif 

#if (TOTAL_X_TASK_NUMBER > MAX_X_TASK_NUMBER)
    #error[total task number beyond scope !!!]
#endif
/**********************************************************************************************************************/
#ifndef X_TASK_LOG_DEBUG_METHOD
    #include <stdio.h>
    #define  X_TASK_LOG_DEBUG_METHOD printf
#endif

#ifndef X_TASK_LOG_DEBUG
    #define  X_TASK_LOG_DEBUG 1
#endif

#ifndef USE_X_TASK_LOG
    #define  USE_X_TASK_LOG 1
#endif

#if (USE_X_TASK_LOG != 0)
    #define X_TASK_LOG(flag,message)   do{ 																\
                                            if((flag) != 0)	\
                                            {	X_TASK_LOG_DEBUG_METHOD	message ;	   	}			\
                                        }while(0)

#else
    #define X_TASK_LOG(flag,message)  do{ }while(0)
#endif

#ifndef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  0
#endif
/**********************************************************************************************************************/
typedef X_Void (*x_task_func)(uint32_t x_task_param_ms,uint16_t task_id);
typedef uint32_t (*x_get_time)(X_Void);
typedef X_Void (*x_one_step)(X_Void);

#define X_TASK_NULL_INIT_FUNC      ((x_one_step)0)
#define X_TASK_FALSE_CONDITION      (1 != 1)
/**********************************************************************************************************************/
X_Boolean task_scheduler_init(x_get_time p_get_time_func);
X_Boolean add_task_to_schedule(uint16_t task_id,x_task_func p_task_func,X_Boolean *p_isInit);
                               
X_Void  loop_task_scheduler(X_Void);

X_Boolean freeze_task_in_schedule(uint16_t task_id);
X_Boolean unfreeze_task_in_schedule(uint16_t task_id);
X_Boolean restart_task_in_schedule(uint16_t task_id);
/**********************************************************************************************************************/
#define X_TASK_INVALID_LINE_NUM        0
#define X_TASK_GOTO_THE_END_LINE_NUM        0xFFFFFFFF
#define X_TASK_INVALID_DEEPTH          0xFFFF
X_Void X_Task_ByteBufClear(uint8_t *p_buf,uint16_t length);
X_Void X_Task_32bitsBufClear(uint32_t *p_buf,uint16_t length);

X_Void X_Task_if_line_push(uint16_t loop_deepth,uint16_t if_deepth,uint8_t *p_line_flag_buf);
X_Void X_Task_if_line_pop_All(uint16_t loop_deepth,uint16_t max_if_deepth,uint8_t *p_line_flag_buf,uint32_t *P_line_buf);
X_Void X_Task_loop_line_pop_all(uint16_t loop_deepth,uint16_t max_loop_deepth,uint32_t *P_end_line_buf,uint32_t *P_line_buf);
/************************************************sth about get_time_func ************************************************************************
 p_get_time_func example:

volatile sys_cnt_ms = 0;

void sys_isr(void)
{
    sys_cnt_ms ++;
}

uint32_t get_sys_time(X_Void)
{
    uint32_t temp;
    ENTER_CRITICAL_REGION;
    temp = sys_cnt_ms;
    EXIT_CRITICAL_REGION;
    return temp;
}

sth about CRITICAL_REGION : 
    if(multi-core CPU)
    {
        not in consideration
    }
    else if(single core CPU)
    {
        if( >= 32 bits) // 32 bits variable , 32 bits register , 32bits bus width
        {
            no need enter CRITICAL_REGION
        }
        else
        {
            need enter CRITICAL_REGION // disable ISR , disable task switch or lock the bus 
        }
    }

**********************************************************************************************************************/

/************************************************sth about X_TASK_LOG**********************************************************************
choose your X_TASK_LOG_DEBUG_METHOD  first ;
#define USE_X_TASK_LOG  1  before release  ;
#define X_TASK_LOG_DEBUG  1  for more detail if there is a fatal error in task ; 
**********************************************************************************************************************/

/************************************************task create sample code *******************************************************************
#include "../refresh_task_tag.h"

TASK_BODY_BEGIN(loop_task10,X_TASK_NULL_INIT_FUNC,0,0){

}TASK_BODY_END

static X_Boolean  LoopTask10Regiester(X_Void)               
{
  return X_TASK_REGISTE(10,loop_task10);
}
*******************************************************************************************************************/

/***************************************************careful about the "while nest if with break" situation !!!*************
there still some bugs I belive ;
try to use it in different occasions , and find the bugs ;

also you could avoid using the code struct below for more security :

~~~~~~~~~~~~~~~~~~~~~bad code struct example ~~~~~~~~~~~~~~~~~~~~~~~~
X_while(condition1)
{
	X_if(condition1.1)
	{
		X_if(condition1.1.1)
		{
			
		}
		X_else
		{
			X_break;
		}X_endif
	}
	X_else
	{
		
	}X_endif
}X_endloop

X_while(condition2)
{
	X_if(condition2.1)
	{
		X_if(condition2.1.1)
		{
			X_do(condition2.1.1 = false);
			X_delay(100);
		}
		X_else
		{
			X_break;
		}X_endif
	}
	X_else
	{
		
	}X_endif
}X_endloop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
the "loop, if ,break" marco combination is not under full test !!!
here is some suggestions when you have to use the code struct above :
1, it is better that condition is an expression than a fucntion (try to keep the function have the same behaviour every time it is called)
2, the code in line 177 "X_do(condition2.1.1 = false);" maybe cause the X_if flase next time ,   call it after "X_delay(100);" is better 
3, it is better to use "X_break" in X_if rather than in X_else 
4, do not nest too much , it seems ugly 
5, try to use only one while loop in a task 
*******************************************************************************************************************/

/***************************************************an example to convert RTC cnt to UTC time  USE X_TASK (maybe have bugs)*************
static const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
X_Boolean Is_Leap_Year(uint16_t _year)
{                     
	if (_year % 4 == 0) // 必须能被4整除 
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
			{
				return X_True;	// 如果以00结尾,还要能被400整除 
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

static uint8_t x_month,x_day,x_hour,x_minute,x_sec;
static uint16_t x_year;
static uint32_t x_cnt = 0;
static X_Boolean isBreak = X_False;

#include "../refresh_task_tag.h"

static 	uint32_t x_temp = 0;
static	uint16_t x_temp1 = 0;

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
	X_do(x_temp1 = 0);
	X_while (x_temp >= 28)	// 超过了一个月 //
	{
		X_if((Is_Leap_Year(x_year) == X_True) && (x_temp1 == 1))	// 当年是不是闰年/2月份 
		{
			X_if (x_temp >= 29)
			{
				X_do(x_temp -= 29);	// 闰年的秒钟数 
			}
			X_else
			{
				X_break; 
			}X_endif
    }
    X_else 
		{
			X_if (x_temp >= mon_table[x_temp1])//(x_temp >= mon_table[x_temp1])
			{
				X_delay(100);
				X_do(x_temp -= mon_table[x_temp1]);	// 平年 
				X_delay(10);
			}
			X_else 
			{
				X_break;
			}X_endif
		}X_endif
		X_do(x_temp1++);  
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
************************************************************************************************************************/
#ifdef __cplusplus
		}
#endif

#endif

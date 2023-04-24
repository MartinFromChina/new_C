/****************************************************************************************************
the X_task_marco use line number as state flag to implement state-jump. if you want to be more efficient, use ProtoThread
in the task body:
there must be a X_marco at the beginning of each code line 
only one X_marco in the same code line
********************************************************************************************************/

#ifndef __CONST_LOOP_SCHEDULER_H
#define __CONST_LOOP_SCHEDULER_H

#ifdef __cplusplus
	extern "C" {

#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "x_task_common.h"

typedef struct 
{
    uint16_t  id;
    x_task_func task_process;
}s_X_TaskList;

typedef struct 
{
	uint16_t     max_task_num;
	uint8_t      * p_is_init_ok;
	uint8_t      * p_init_flag_buf;
	uint8_t      * p_freeze_flag_buf;
	s_X_TaskList const * p_task;
}s_X_Tasks_Manager;


#define X_TASK_SCHEDULER_DEF(scheduler_id,task_number,p_task_list)            \
			static uint8_t CONCAT_2(scheduler_id,is_init_OK) = 0;   			\
			static uint8_t CONCAT_2(scheduler_id,init_flag_buf)[task_number];   \
			static uint8_t CONCAT_2(scheduler_id,freeze_flag_buf)[task_number];  \
			static const s_X_Tasks_Manager CONCAT_2(scheduler_id,Tasks_manager) = {   \
								task_number,                                         \
								&CONCAT_2(scheduler_id,is_init_OK),                  \
								CONCAT_2(scheduler_id,init_flag_buf),               \
								CONCAT_2(scheduler_id,freeze_flag_buf),             \
								p_task_list,                                           \
							};                                                          \
			static const s_X_Tasks_Manager * scheduler_id = &CONCAT_2(scheduler_id,Tasks_manager)
/**********************************************************************************************************************/

/**********************************************************************************************************************/
e_x_task_error_code x_task_init(const s_X_Tasks_Manager *p_manager);                               
e_x_task_error_code x_task_scheduler(const s_X_Tasks_Manager *p_manager,uint32_t ms);

e_x_task_error_code freeze_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id);
e_x_task_error_code unfreeze_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id);
e_x_task_error_code restart_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id);
/**********************************************************************************************************************/

/************************************************sth about get_time_func ************************************************************************
 typedef uint32_t (*x_get_time)(X_Void);
 
 static x_get_time  p_get_time_func;
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

static const s_X_TaskList task_list[] = {
  {10,loop_task10},
};

X_TASK_SCHEDULER_DEF(t1,sizeof(task_list)/sizeof(task_list[0]),task_list);

x_task_init(t1);

x_task_scheduler(t1,get_sys_time());

X_Void freeze_task_10(X_Void)
{
	freeze_x_task(t1,10);
}
X_Void unfreeze_task_10(X_Void)
{
	unfreeze_x_task(t1,10);
}
X_Void restart_task_10(X_Void)
{
	restart_x_task(t1,10);
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

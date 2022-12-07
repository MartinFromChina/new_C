#ifndef __THREAD_IN_LOOP_H
#define __THREAD_IN_LOOP_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"

typedef X_Void (*one_step)(X_Void);
typedef uint32_t (*get_time)(X_Void);
typedef X_Boolean (*set_time)(uint32_t time_ms);
typedef X_Boolean (*does_in_conditon)(X_Void);


typedef struct 
{
    uint32_t *p_cur_line_num;
    uint8_t  *p_state_sw;
    uint32_t *p_for_loop_cnt;
    get_time  const p_get_time;
    set_time  const p_set_time;
}sThreadInLoop;

X_Void AppThreadToDo(const sThreadInLoop *p_manager,one_step todo,uint32_t cur_line_num);
X_Void AppThreadEndHandle(const sThreadInLoop *p_manager);
X_Boolean DoesAppThreadWait(const sThreadInLoop *p_manager,uint32_t wait_time_ms,uint32_t cur_line_num);
X_Boolean DoesAppThreadWaitUntil(const sThreadInLoop *p_manager,uint32_t cur_line_num,one_step init_func,does_in_conditon in_conditon_func);
X_Boolean DoesAppThreadWhileLoop(const sThreadInLoop *p_manager,uint32_t cur_line_num
                                ,one_step init_func,does_in_conditon in_conditon_func,one_step todo,uint32_t step_delay_ms);
X_Boolean DoesAppThreadForLoop(const sThreadInLoop *p_manager,uint32_t cur_line_num
                                ,one_step init_func,does_in_conditon in_conditon_func,one_step todo,uint32_t step_delay_ms,uint32_t loop_times);

#define APP_THREAD_INIT(id,set_time_func,get_time_func)            \
            static X_Boolean CONCAT_2(id, is_blocked) = X_False;    \
            static uint32_t CONCAT_2(id, line_num) = 0;                \
            static uint32_t CONCAT_2(id, temp_line_num) = 0;        \
            static uint8_t CONCAT_2(id, state_sw) = 0;        \
            static uint32_t CONCAT_2(id, for_loop_cnt) = 0;        \
            static const sThreadInLoop CONCAT_2(id, _entry) = {     \
                &CONCAT_2(id, line_num),                               \
                &CONCAT_2(id, state_sw),                            \
                &CONCAT_2(id, for_loop_cnt),                         \
                set_time_func,                                        \
                get_time_func,                                      \
            };                                                          \
            static const sThreadInLoop* id = &CONCAT_2(id, _entry) 

#define APP_TH_END(id)  do{                         \
                                AppThreadEndHandle(id);  \
                            }while(0)

#define APP_TH_DO(id,todo)     do{ 													\
                                        CONCAT_2(id, temp_line_num) = __LINE__ ;    \
                               	        AppThreadToDo(id,todo,CONCAT_2(id, temp_line_num));       \
                                }while(0)

#define APP_TH_WAIT(id,wait_ms)  do{                         \
                                CONCAT_2(id, temp_line_num) = __LINE__ ;    \
                                CONCAT_2(id, is_blocked) = DoesAppThreadWait(id,wait_ms,CONCAT_2(id, temp_line_num));     \
                                if(CONCAT_2(id, is_blocked) == X_True) {return;}                             \
                            }while(0)

#define APP_TH_WAIT_UNTIL(id,var_init,condition)    do{                         \
                                CONCAT_2(id, temp_line_num) = __LINE__ ;    \
                                CONCAT_2(id, is_blocked) = DoesAppThreadWaitUntil(id,CONCAT_2(id, temp_line_num),var_init,condition);     \
                                if(CONCAT_2(id, is_blocked) == X_True) {return;}                             \
                            }while(0)

#define APP_TH_WHILE(id,var_init,condition,todo,step_ms)      do{                         \
                                CONCAT_2(id, temp_line_num) = __LINE__ ;    \
                                CONCAT_2(id, is_blocked) = DoesAppThreadWhileLoop(id,CONCAT_2(id, temp_line_num),var_init,condition,todo,step_ms);     \
                                if(CONCAT_2(id, is_blocked) == X_True) {return;}                             \
                            }while(0)

#define APP_TH_FOR(id,var_init,condition,todo,step_ms,loop_times)  do{                         \
                                CONCAT_2(id, temp_line_num) = __LINE__ ;    \
                                CONCAT_2(id, is_blocked) = DoesAppThreadForLoop(id,CONCAT_2(id, temp_line_num),var_init,condition,todo,step_ms,loop_times);     \
                                if(CONCAT_2(id, is_blocked) == X_True) {return;}                             \
                            }while(0)

#define APP_TH_INFINITE_WHILE(id,todo,step_ms)    APP_TH_WHILE(id,(one_step)0,(does_in_conditon)0,todo,step_ms)

#define APP_TH_SIMPLE_FOR(id,todo,step_ms,loop_times) APP_TH_FOR(id,(one_step)0,(does_in_conditon)0,todo,step_ms,loop_times)

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

#ifdef __cplusplus
		}
#endif

#endif

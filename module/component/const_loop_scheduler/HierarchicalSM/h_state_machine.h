/****************************************************************************************************
the T_task_marco use line number as state flag to implement state-jump. if you want to be more efficient, use ProtoThread
in the task body:
there must be a T_marco at the beginning of each code line 
only one T_marco in the same code line
********************************************************************************************************/

#ifndef __H_STATE_MACHINE_H
#define __H_STATE_MACHINE_H

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "../x_task_common.h"		

#define BOOL_BLOCK               X_True
#define BOOL_NO_BLOCK            X_False    

#define MAX_HIERARCHICAL_DEEPTH                   10
#define MAX_HSM_IF_NEST_DEEPTH                    5
#define MAX_HSM_LOOP_NEST_DEEPTH                  4


typedef struct
{
	/*~~~~~~~~~~~~~~~~T task value ~~~~~~~~~~~~~~~~~~~~~~*/
	uint32_t ms_backup;  // = 0                            
 	uint32_t cur_line_num; // = 0 ;                              
 	
 	uint8_t if_condition_buf[MAX_HSM_IF_NEST_DEEPTH + 1];      
 	uint32_t if_line_num_buf[MAX_HSM_IF_NEST_DEEPTH + 1];       
 	uint8_t loop_condition_buf[MAX_HSM_LOOP_NEST_DEEPTH + 1];      
 	uint32_t loop_line_num_buf[MAX_HSM_LOOP_NEST_DEEPTH + 1];      
 	uint32_t loop_end_line_num_buf[MAX_HSM_LOOP_NEST_DEEPTH + 1];                           
 	uint8_t if_line_flag_array[MAX_HSM_LOOP_NEST_DEEPTH + 2][MAX_HSM_IF_NEST_DEEPTH + 1]; 
 	uint8_t isErrorOccur ; // ) = 0;                              
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}s_HSM_CommonParam;

typedef struct s_T_taskBaseParam_tag
{
    s_HSM_CommonParam level_cur;
    struct s_T_taskBaseParam_tag * const p_level_next;
}s_T_taskBaseParam;

typedef X_Boolean (*hsm_task_func)(uint32_t ms,s_T_taskBaseParam * p_base);

typedef struct 
{
    hsm_task_func    	   p_hsm_func;
    uint8_t              * p_is_freeze;
    uint8_t              * p_is_init_ok;
    uint8_t             const h_deepth;  // deepth == 0 means : only one level hsm 
	s_T_taskBaseParam 	 * p_hsm_param;
}s_HSM_Manager;

/**********************************************************************************************************************/

/**********************************************************************************************************************/
X_Void hsm_one_level_param_Init(s_T_taskBaseParam * p_param); // called by the module , not for the user 
///////// X_Boolean hsm_param_init(s_T_taskBaseParam * p_top_param,uint8_t deepth);
e_x_task_error_code hsm_init(const s_HSM_Manager *p_manager);                               
e_x_task_error_code hsm_run(const s_HSM_Manager *p_manager,uint32_t ms);

e_x_task_error_code freeze_hsm(const s_HSM_Manager *p_manager);
e_x_task_error_code unfreeze_hsm(const s_HSM_Manager *p_manager);
e_x_task_error_code restart_hsm(const s_HSM_Manager *p_manager);
/**********************************************************************************************************************/
//#include <string.h> //strrchr()函数所需头文件
//windows:
//#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
//linux :
//#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// printf( "filename:%s\n", filename( __FILE__ ) );


/**********************************************************************************************************************/
#define H_do(todo_sentence)     do{ 													\
                                    cur_line_num_temp = __LINE__ ;    \
                                    if (p_base -> level_cur.cur_line_num <  cur_line_num_temp){  \
                                        p_base -> level_cur.cur_line_num = cur_line_num_temp;  \
                                        todo_sentence;                                   \
                                    }                                               \
                                  }while(0)
/**********************************************************************************************************************/
#define H_delay(x_wait_ms)  do{                         \
                                cur_line_num_temp = __LINE__ ;    \
                                if (p_base ->level_cur.cur_line_num <  cur_line_num_temp){  \
                                        p_base ->level_cur.cur_line_num = cur_line_num_temp;  \
                                        p_base ->level_cur.ms_backup = ms;     \
                                        return X_True;                             \
                                }                                                                            \
                                else if(p_base ->level_cur.cur_line_num ==  cur_line_num_temp){          \
                                    if(((x_wait_ms) != 0) && ((ms - p_base ->level_cur.ms_backup) < (x_wait_ms))) {return X_True;}   \
                                }                                                                            \
                              }while(0)

#define H_wait_until(condition_break_sentence)    do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->level_cur.cur_line_num < cur_line_num_temp){       \
                                                                p_base ->level_cur.cur_line_num = cur_line_num_temp;  \
                                                                return X_True;                                                    \
                                                        }                                                                            \
                                                        else if(p_base -> level_cur.cur_line_num ==  cur_line_num_temp){          \
                                                            if(!(condition_break_sentence)) {return X_True;} }               \
                                                    }while(0)

#define H_time_limited_wait(x_limit_ms,condition_break_sentence)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                        \
                                                        if (p_base ->level_cur.cur_line_num <  cur_line_num_temp){          \
                                                                p_base -> level_cur.cur_line_num = cur_line_num_temp;  \
                                                                p_base ->level_cur.ms_backup = ms;    \
                                                                return X_True;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->level_cur.cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((ms - p_base ->level_cur.ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return X_True;}                         \
                                                                }   \
                                                            }               \
                                                    }while(0)

#define H_time_limited_wait_with_result(x_limit_ms,condition_break_sentence,wait_result_bool_variable)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->level_cur.cur_line_num <  cur_line_num_temp){  \
                                                                p_base ->level_cur.cur_line_num = cur_line_num_temp;  \
                                                                p_base ->level_cur.ms_backup = ms;    \
                                                                wait_result_bool_variable = X_False;      \
                                                                return X_True;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->level_cur.cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((ms - p_base ->level_cur.ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return X_True;}                         \
                                                                    wait_result_bool_variable = X_True;     \
                                                                }   \
                                                            }               \
                                                    }while(0)
/**********************************************************************************************************************/ 

/**********************************************************************************************************************/
#define H_return  do{                                                       \
                        if(p_base ->level_cur.isErrorOccur == 0) {                             \
                            hsm_one_level_param_Init(p_base);                                            \
                        }                                                           \
                        else {                                                                     \
                        X_TASK_LOG(1,(" ----  line %d : fatal error occur , hsm stop !!!\r\n" ,__LINE__));                   \
                        p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;        \
                        }                                           \
                        return X_False;                                     \
                    }while(0)
      
/**********************************************************************************************************************/
#define H_if(conditon)   do{     \
                                cur_line_num_temp = __LINE__ ;    \
                                nest_cnt ++;                                                \
                                if((nest_cnt - 1) > MAX_HSM_IF_NEST_DEEPTH)  {  \
                                    X_TASK_LOG(1,(" ---- if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                    ,(nest_cnt - 1),MAX_HSM_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                    p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                                    return X_False; \
                                }    \
                                X_Task_if_line_push(loop_nest_cnt,nest_cnt - 1,&p_base ->level_cur.if_line_flag_array[loop_nest_cnt][0]) ;       \
                                if (p_base ->level_cur.if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  									\
                                    p_base ->level_cur.if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;                              \
                                    p_base ->level_cur.if_condition_buf[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                }                        \
                                else if(p_base ->level_cur.if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                    if((p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                    p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                    }       \
                                    else {                \
                                    p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0;}                    \
                                }                                                                  \
                                else{                                                                   \
                                    p_base ->level_cur.if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                }                          \
                            }while(0);   \
                            if(p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0xF0)

#define H_else_if(conditon)    do{     \
                                    cur_line_num_temp = __LINE__ ;    \
                                    if((nest_cnt - 1) > MAX_HSM_IF_NEST_DEEPTH )  {  \
                                        X_TASK_LOG(1,(" ---- else if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,nest_cnt - 1,MAX_HSM_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                        p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                                        return X_False;                  \
                                    }    \
                                    if (p_base ->level_cur.if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  \
                                        p_base ->level_cur.if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;  \
                                        if((p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0x0F) == 0) {     \
                                        	p_base ->level_cur.if_condition_buf[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                        }                              \
                                        else {                         \
                                        	p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0x0F;       \
                                        	p_base ->level_cur.if_line_num_buf[nest_cnt - 1] ++;                \
                                        }                                                                           \
                                    }                        \
                                    else if(p_base ->level_cur.if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                        if((p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                        	p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                        	p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{                                                                  \
                                        p_base ->level_cur.if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0xF0)

#define H_else               do{     \
                                    cur_line_num_temp = __LINE__ ;    \
                                    if((nest_cnt - 1) > MAX_HSM_IF_NEST_DEEPTH)  {  \
                                        X_TASK_LOG(1,(" ---- else nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,nest_cnt - 1,MAX_HSM_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                        p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                                        return X_False; \
                                    }    \
                                    if (p_base ->level_cur.if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  \
                                        p_base ->level_cur.if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;  \
                                        if((p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0x0F) == 0) {     \
                                            p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0xFF;   \
                                        }                              \
                                        else {                         \
                                            p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0x0F;       \
                                        }                                                                           \
                                    }                        \
                                    else if(p_base ->level_cur.if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                        if((p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                            p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                            p_base ->level_cur.if_condition_buf[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{ \
                                        p_base ->level_cur.if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(p_base ->level_cur.if_condition_buf[nest_cnt - 1] & 0xF0) 

#define H_endif   do{ 													\
                        cur_line_num_temp = __LINE__ ;    \
                        nest_cnt --;                                                \
                        if(nest_cnt > MAX_HSM_IF_NEST_DEEPTH )  {  \
                        X_TASK_LOG(1,(" ---- end if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                        ,nest_cnt,MAX_HSM_IF_NEST_DEEPTH,cur_line_num_temp));  \
                        p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                        return X_False; \
                        }    \
                        if (p_base ->level_cur.if_line_num_buf[nest_cnt] <  cur_line_num_temp){  \
                            p_base ->level_cur.if_line_num_buf[nest_cnt] = cur_line_num_temp;  \
                        }                                               \
                    }while(0);

/**********************************************************************************************************************/
                 
#define H_while(conditon_true)      do{                                                           \
                                        cur_line_num_temp = __LINE__ ;    \
                                        loop_nest_cnt ++;                                                \
                                        if((loop_nest_cnt - 1) > MAX_HSM_LOOP_NEST_DEEPTH)  {  \
                                            X_TASK_LOG(1,(" ---- loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                            ,loop_nest_cnt - 1,MAX_HSM_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                                            p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                                            return X_False; \
                                        }    \
                                        if (p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1] <=  cur_line_num_temp \
                                            && p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt - 1] < cur_line_num_temp){  \
                                            p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1] = cur_line_num_temp;  \
                                            p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt - 1] = cur_line_num_temp;  \
                                            if(conditon_true) {                                                       \
                                                p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] = 0xFF;  \
                                                loop_line_num_backup = cur_line_num_temp;}                              \
                                            else {                                                                    \
                                                p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] = 0;}                        \
                                        }                                                                                  \
                                        else if(p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1] == cur_line_num_temp \
                                                && p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt - 1] == cur_line_num_temp) {   \
                                                if((p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] & 0x0F)!= 0) {   \
                                                    p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] = 0xFF ;                \
                                                    loop_line_num_backup = cur_line_num_temp;}                       \
                                                else {                                                                          \
                                                    p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] = 0 ;  }              \
                                        }                                                                  \
                                        else if(p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1] > cur_line_num_temp \
                                                && p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt - 1] != cur_line_num_temp) { \
                                            p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] &= 0x0F;                    \
                                        }                          \
                                        else {   \
                                        X_TASK_LOG(1,(" ----  loop nest error occur ; line %d ; loop_line %d , loop_end_line %d\r\n",cur_line_num_temp \
                                        ,p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1],p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt - 1]));  \
                                            p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                                            return X_False;}  \
                                    }while(0);   \
                                    if(p_base ->level_cur.loop_condition_buf[loop_nest_cnt - 1] & 0xF0)


#define H_endloop      do{ 				                                     		\
                            cur_line_num_temp = __LINE__ ;    \
                            if(p_base ->level_cur.loop_line_num_buf[loop_nest_cnt - 1] < cur_line_num_temp) {                                            \
                            X_Task_if_line_pop_All(loop_nest_cnt,MAX_HSM_IF_NEST_DEEPTH+1,  \
                                        &p_base ->level_cur.if_line_flag_array[loop_nest_cnt][0],p_base ->level_cur.if_line_num_buf);}  \
                            loop_nest_cnt --;                                                \
                            if(loop_nest_cnt > MAX_HSM_LOOP_NEST_DEEPTH)  {  \
                            X_TASK_LOG(1,(" ---- end loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,loop_nest_cnt,MAX_HSM_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                            p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                            return X_False; \
                            }    \
                            if(p_base ->level_cur.cur_line_num < cur_line_num_temp)  {                \
                                 X_Task_loop_line_pop_all(loop_nest_cnt+1,MAX_HSM_LOOP_NEST_DEEPTH+1,  \
                                                            p_base ->level_cur.loop_end_line_num_buf,p_base ->level_cur.loop_line_num_buf);  \
                                if ( p_base ->level_cur.loop_condition_buf[loop_nest_cnt] == 0){                                     \
                                    p_base ->level_cur.loop_line_num_buf[loop_nest_cnt] = cur_line_num_temp;  \
                                    p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt] = cur_line_num_temp;          \
                                    p_base ->level_cur.cur_line_num = cur_line_num_temp;        \
                                }                                               \
                                else {                                                                   \
                                    p_base ->level_cur.loop_line_num_buf[loop_nest_cnt] = loop_line_num_backup;   \
                                    p_base ->level_cur.loop_end_line_num_buf[loop_nest_cnt] = 0;                                       \
                                    p_base ->level_cur.cur_line_num = loop_line_num_backup;                               \
                                return X_True;    }      \
                            }         \
                        }while(0);

#define H_break        do{ 				                                     		\
                            cur_line_num_temp = __LINE__ ;                              \
                            if((loop_nest_cnt - 1) > MAX_HSM_LOOP_NEST_DEEPTH)  {  \
                            X_TASK_LOG(1,(" ---- break nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,(loop_nest_cnt - 1),MAX_HSM_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                            p_base ->level_cur.cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->level_cur.isErrorOccur = 1; \
                            return X_False; \
                            }    \
                            X_Task_if_line_pop_All(loop_nest_cnt,MAX_HSM_IF_NEST_DEEPTH+1,  \
                                        &p_base ->level_cur.if_line_flag_array[loop_nest_cnt][0],p_base ->level_cur.if_line_num_buf);  \
                            X_Task_loop_line_pop_all(loop_nest_cnt,MAX_HSM_LOOP_NEST_DEEPTH+1,  \
                                                            p_base ->level_cur.loop_end_line_num_buf,p_base ->level_cur.loop_line_num_buf);  \
                            p_base ->level_cur.loop_end_line_num_buf[(loop_nest_cnt - 1)] = cur_line_num_temp; \
                            p_base ->level_cur.loop_line_num_buf[(loop_nest_cnt - 1)] = cur_line_num_temp;  \
                            p_base ->level_cur.loop_condition_buf[(loop_nest_cnt - 1)] = 0;                                     \
                            p_base ->level_cur.cur_line_num = cur_line_num_temp;        \
                            return X_True;                                                                          \
                        }while(0)
/**********************************************************************************************************************/

/**********************************************************************************************************************/

#define __H_BASE_PARAM_EXPAND_0(__id)   {{0,0,{0},{0},{0},{0},{0},{0},0,}, (s_T_taskBaseParam*)0,},
#define __H_BASE_PARAM_EXPAND_1(__id)    __H_BASE_PARAM_EXPAND_0(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[0],},
#define __H_BASE_PARAM_EXPAND_2(__id)    __H_BASE_PARAM_EXPAND_1(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[1],},
#define __H_BASE_PARAM_EXPAND_3(__id)    __H_BASE_PARAM_EXPAND_2(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[2],},
#define __H_BASE_PARAM_EXPAND_4(__id)    __H_BASE_PARAM_EXPAND_3(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[3],},
#define __H_BASE_PARAM_EXPAND_5(__id)    __H_BASE_PARAM_EXPAND_4(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[4],},
#define __H_BASE_PARAM_EXPAND_6(__id)    __H_BASE_PARAM_EXPAND_5(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[5],},
#define __H_BASE_PARAM_EXPAND_7(__id)    __H_BASE_PARAM_EXPAND_6(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[6],},
#define __H_BASE_PARAM_EXPAND_8(__id)    __H_BASE_PARAM_EXPAND_7(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[7],},
#define __H_BASE_PARAM_EXPAND_9(__id)    __H_BASE_PARAM_EXPAND_8(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[8],},
#define __H_BASE_PARAM_EXPAND_10(__id)    __H_BASE_PARAM_EXPAND_9(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &CONCAT_2(__id,_hsm_param)[9],},
   

#define  __H_BASE_PARAM_EXPAND(__id,__n)     CONCAT_2(__H_BASE_PARAM_EXPAND_,__n)(__id)
#define H_BASE_PARAM_EXPAND(__id,__n)        __H_BASE_PARAM_EXPAND(__id,__n) 

#define __HSM_TO_STR(y)     #y
#define HSM_TO_STR(y)       __HSM_TO_STR(y)

#define HSM_TOP_LEVEL_BODY_BEGIN(id,level_deepth)            	   \
            static s_T_taskBaseParam CONCAT_2(id,_hsm_param)[level_deepth + 1] = {              \
                                H_BASE_PARAM_EXPAND(id,level_deepth) };                          \
            static uint8_t CONCAT_2(id,_is_init_ok) = 0;    \
            static uint8_t CONCAT_2(id,_is_freeze) = 0;    \
			static X_Boolean CONCAT_2(id,_hsm_top_func)(uint32_t ms,s_T_taskBaseParam * p_base);   	\
			static const s_HSM_Manager CONCAT_2(id,_HSM_manager) = {   		            \
                        CONCAT_2(id,_hsm_top_func),                             \
                        &CONCAT_2(id,_is_freeze),                               \
						&CONCAT_2(id,_is_init_ok),                           \
						level_deepth,                                       \
						&CONCAT_2(id,_hsm_param)[level_deepth],                       \
						};                                                                    \
            static const s_HSM_Manager * id = &CONCAT_2(id,_HSM_manager);                           \
            static X_Boolean CONCAT_2(id,_hsm_top_func)(uint32_t ms,s_T_taskBaseParam * p_base){               \
                            uint32_t cur_line_num_temp = 0 ;                                                            \
                            uint16_t nest_cnt = 0,loop_nest_cnt = 0;   uint32_t loop_line_num_backup = 0;                       \
                            if( p_base == (s_T_taskBaseParam *)0)    {                      \
                            X_TASK_LOG(1,(" ---- id %s line %d : parameter error occur \r\n",HSM_TO_STR(id),__LINE__)); return X_False; }      \
                            if(p_base ->level_cur.isErrorOccur == 0)                          
                                                                        
                                                    // H_do
                                                    // H_wait
                                                    // H_for 
                                                    // H_while
                                                    // ...

#define HSM_BODY_END                                                                                            \
                            if(p_base ->level_cur.isErrorOccur == 0 && nest_cnt == 0 && loop_nest_cnt == 0) {                      \
                                hsm_one_level_param_Init(p_base);                                            \
                                UNUSED_VARIABLE(ms);                                                     \
                                UNUSED_VARIABLE(loop_line_num_backup);                                              \
                                UNUSED_VARIABLE(cur_line_num_temp);                                                     \
                                UNUSED_VARIABLE(p_base ->level_cur.ms_backup);                          \
                            }                                                           \
                            else if (p_base -> level_cur.isErrorOccur == 1 || nest_cnt != 0 || loop_nest_cnt != 0){                                                   \
                            X_TASK_LOG(1,(" ---- id %s line %d : fatal error occur , hsm stop !!!\r\n",HSM_TO_STR(id),__LINE__));                   \
                            p_base -> level_cur.isErrorOccur = 2 ;  nest_cnt = 0;    loop_nest_cnt = 0;                      \
                            }                                           \
                            return X_False;                  \
                        }                   

#define HSM_LOW_LEVEL_BODY_BEGIN(__func_name)      \
            static X_Boolean __func_name(uint32_t ms,s_T_taskBaseParam * p_base){               \
                            uint32_t cur_line_num_temp = 0 ;                                                            \
                            uint16_t nest_cnt = 0,loop_nest_cnt = 0;   uint32_t loop_line_num_backup = 0;                       \
                            if( p_base == (s_T_taskBaseParam *)0)    {                      \
                            X_TASK_LOG(1,(" ---- id %s line %d : parameter error occur \r\n",HSM_TO_STR(__func_name),__LINE__)); return X_False; }      \
                            if(p_base ->level_cur.isErrorOccur == 0)                           


//HSM_TOP_LEVEL_BODY_BEGIN(hello,10)
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

#ifdef __cplusplus
		}
#endif

#endif

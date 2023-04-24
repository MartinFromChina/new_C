// include it before a new task declaration
#undef X_if 
#undef X_else_if
#undef X_else
#undef X_endif
#undef X_do
#undef X_delay
#undef X_wait_until
#undef X_time_limited_wait
#undef X_time_limited_wait_with_result
#undef X_while
#undef X_endloop
#undef X_break
#undef X_return
#undef X_TASK_REGISTE
#undef TASK_BODY_BEGIN
#undef TASK_BODY_END
/**********************************************************************************************************************/
#ifndef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  0
#endif

/**********************************************************************************************************************/  
#undef MFUNC_IN_U8_DEC_VALUE
///////////////////////#undef MFUNC_OUT_DEC_STR
#define  MFUNC_IN_U8_DEC_VALUE      (TASK_TAG_INDEX + 1)

#include "../../common/mf_u8_dec2str.h"

#undef TASK_TAG_INDEX
#define TASK_TAG_INDEX              MFUNC_OUT_DEC_STR
/**********************************************************************************************************************/

#define CUR_TASK_TAG       CONCAT_2(TASK,MFUNC_OUT_DEC_STR)
/**********************************************************************************************************************/
/**********************************************************************************************************************/
#define X_do(todo_sentence)     do{ 													\
                                    CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                    if (CONCAT_2(CUR_TASK_TAG,cur_line_num) <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                        todo_sentence;                                   \
                                    }                                               \
                                  }while(0)
/**********************************************************************************************************************/
#define X_delay(x_wait_ms)  do{                         \
                                CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                if (CONCAT_2(CUR_TASK_TAG,cur_line_num) <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                        CONCAT_2(CUR_TASK_TAG,ms_backup) = x_task_param_ms;     \
                                        return;                             \
                                }                                                                            \
                                else if((CONCAT_2(CUR_TASK_TAG,cur_line_num) ==  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp))){          \
                                    if(((x_wait_ms) != 0) && ((x_task_param_ms - CONCAT_2(CUR_TASK_TAG,ms_backup)) < (x_wait_ms))) {return;}   \
                                }                                                                            \
                              }while(0)

#define X_wait_until(condition_break_sentence)    do{                         \
                                                        CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;                     \
                                                        if (CONCAT_2(CUR_TASK_TAG,cur_line_num) <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if((CONCAT_2(CUR_TASK_TAG,cur_line_num) ==  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp))){          \
                                                            if(!(condition_break_sentence)) {return;} }               \
                                                    }while(0)

#define X_time_limited_wait(x_limit_ms,condition_break_sentence)  do{                         \
                                                        CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;                     \
                                                        if (CONCAT_2(CUR_TASK_TAG,cur_line_num) <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                                                CONCAT_2(CUR_TASK_TAG,ms_backup) = x_task_param_ms;    \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if((CONCAT_2(CUR_TASK_TAG,cur_line_num) ==  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp))){          \
                                                                if(((x_limit_ms) != 0) && ((x_task_param_ms - CONCAT_2(CUR_TASK_TAG,ms_backup)) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return;}                         \
                                                                }   \
                                                            }               \
                                                    }while(0)

#define X_time_limited_wait_with_result(x_limit_ms,condition_break_sentence,wait_result_bool_variable)  do{                         \
                                                        CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;                     \
                                                        if (CONCAT_2(CUR_TASK_TAG,cur_line_num) <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                                                CONCAT_2(CUR_TASK_TAG,ms_backup) = x_task_param_ms;    \
                                                                wait_result_bool_variable = X_False;      \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if((CONCAT_2(CUR_TASK_TAG,cur_line_num) ==  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp))){          \
                                                                if(((x_limit_ms) != 0) && ((x_task_param_ms - CONCAT_2(CUR_TASK_TAG,ms_backup)) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return;}                         \
                                                                    wait_result_bool_variable = X_True;     \
                                                                }   \
                                                            }               \
                                                    }while(0)
/**********************************************************************************************************************/ 

/**********************************************************************************************************************/
#define X_return  do{                                                       \
                        if(CONCAT_2(CUR_TASK_TAG,isErrorOccur) == 0) {                             \
                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = 0;                                       \
                            X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,if_condition_buf),CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1);           \
                            X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,if_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1);           \
                            X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,loop_condition_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);           \
                            X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);           \
                            X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);      \
                            X_Task_ByteBufClear(&CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[0][0],(CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 2) *(CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1) );      \
                        }                                                           \
                        else {                                                                     \
                        X_TASK_LOG(1,(" ---- task(id %d) fatal error occur , task stop !!!\r\n",task_id));                   \
                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;        \
                        }                                           \
                        return ;                                     \
                    }while(0)
      
/**********************************************************************************************************************/
#define X_if(conditon)   do{     \
                                CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                nest_cnt ++;                                                \
                                if((nest_cnt - 1) > CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit))  {  \
                                    X_TASK_LOG(1,(" ---- task(id %d) if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                    ,task_id,(nest_cnt - 1),CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                                    CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                                    return ; \
                                }    \
                                X_Task_if_line_push(loop_nest_cnt,nest_cnt - 1,&CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[loop_nest_cnt][0]) ;       \
                                if (CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                    CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                    CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                }                        \
                                else if(CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] == CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) {                     \
                                    if((CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0x0F) != 0) {     \
                                    CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0xFF;                     \
                                    }       \
                                    else {                \
                                    CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0;}                    \
                                }                                                                  \
                                else{                                                                   \
                                    CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] &= 0x0F;                    \
                                }                          \
                            }while(0);   \
                            if(CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0xF0)

#define X_else_if(conditon)    do{     \
                                    CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                    if((nest_cnt - 1) > CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) )  {  \
                                        X_TASK_LOG(1,(" ---- task(id %d) else if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,task_id,nest_cnt - 1,CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                                        return ; \
                                    }    \
                                    if (CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                        CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                        if((CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0x0F) == 0) {     \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                        }                              \
                                        else {                         \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0x0F;       \
                                        CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] ++;                \
                                        }                                                                           \
                                    }                        \
                                    else if(CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] == CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) {                     \
                                        if((CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0x0F) != 0) {     \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{                                                                  \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0xF0)

#define X_else               do{     \
                                    CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                    if((nest_cnt - 1) > CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit))  {  \
                                        X_TASK_LOG(1,(" ---- task(id %d) else nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,task_id,nest_cnt - 1,CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                                        return ; \
                                    }    \
                                    if (CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                        CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                        if((CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0x0F) == 0) {     \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0xFF;   \
                                        }                              \
                                        else {                         \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0x0F;       \
                                        }                                                                           \
                                    }                        \
                                    else if(CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt - 1] == CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) {                     \
                                        if((CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0x0F) != 0) {     \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{ \
                                        CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(CONCAT_2(CUR_TASK_TAG,if_condition_buf)[nest_cnt - 1] & 0xF0) 

#define X_endif   do{ 													\
                        CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                        nest_cnt --;                                                \
                        if(nest_cnt > CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) )  {  \
                        X_TASK_LOG(1,(" ---- task(id %d) end if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                        ,task_id,nest_cnt,CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                        CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                        return ; \
                        }    \
                        if (CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt] <  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                            CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[nest_cnt] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                        }                                               \
                    }while(0);

/**********************************************************************************************************************/
/*          
for ( init_clause ; cond_expression ; iteration_expression ) 
{
    user code
}

init_clause -> code_expression(true) -> user code -> interation_expression  -> cond_expression(true) -> user code -> interation_expression ...

if(code_expression == false) {break;}
*/                     
#define X_while(conditon_true)      do{                                                           \
                                        CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                                        loop_nest_cnt ++;                                                \
                                        if((loop_nest_cnt - 1) > CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit))  {  \
                                            X_TASK_LOG(1,(" ---- task(id %d) loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                            ,task_id,loop_nest_cnt - 1,CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                                            return ; \
                                        }    \
                                        if (CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1] <=  CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) \
                                            && CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt - 1] < CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)){  \
                                            CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                            CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt - 1] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                            if(conditon_true) {                                                       \
                                                CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] = 0xFF;  \
                                                loop_line_num_backup = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);}                              \
                                            else {                                                                    \
                                                CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] = 0;}                        \
                                        }                                                                                  \
                                        else if(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1] == CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) \
                                                && CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt - 1] == CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) {   \
                                                if((CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] & 0x0F)!= 0) {   \
                                                    CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] = 0xFF ;                \
                                                    loop_line_num_backup = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);}                       \
                                                else {                                                                          \
                                                    CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] = 0 ;  }              \
                                        }                                                                  \
                                        else if(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1] > CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) \
                                                && CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt - 1] != CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) { \
                                            CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] &= 0x0F;                    \
                                        }                          \
                                        else {   \
                                        X_TASK_LOG(1,(" ---- task(id %d) loop nest error occur ; line %d ; loop_line %d , loop_end_line %d\r\n",task_id,CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) \
                                        ,CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1],CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt - 1]));  \
                                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                                            return;}  \
                                    }while(0);   \
                                    if(CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt - 1] & 0xF0)


#define X_endloop      do{ 				                                     		\
                            CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                            if(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt - 1] < CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)) {                                            \
                            X_Task_if_line_pop_All(loop_nest_cnt,CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit)+1,  \
                                        &CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[loop_nest_cnt][0],CONCAT_2(CUR_TASK_TAG,if_line_num_buf));}  \
                            loop_nest_cnt --;                                                \
                            if(loop_nest_cnt > CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit))  {  \
                            X_TASK_LOG(1,(" ---- task(id %d) end loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,task_id,loop_nest_cnt,CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                            return ; \
                            }    \
                            if(CONCAT_2(CUR_TASK_TAG,cur_line_num) < CONCAT_2(CUR_TASK_TAG,cur_line_num_temp))  {                \
                                 X_Task_loop_line_pop_all(loop_nest_cnt+1,CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit)+1,  \
                                                            CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),CONCAT_2(CUR_TASK_TAG,loop_line_num_buf));  \
                                if ( (CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[loop_nest_cnt]) == 0){                                     \
                                    CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                                    CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);          \
                                    CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);        \
                                }                                               \
                                else {                                                                   \
                                CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[loop_nest_cnt] = loop_line_num_backup;   \
                                CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[loop_nest_cnt] = 0;                                       \
                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = loop_line_num_backup; \
                                return;    }      \
                            }         \
                        }while(0);

#define X_break        do{ 				                                     		\
                            CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = __LINE__ ;    \
                            if((loop_nest_cnt - 1) > CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit))  {  \
                            X_TASK_LOG(1,(" ---- task(id %d) break nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,task_id,(loop_nest_cnt - 1),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit),CONCAT_2(CUR_TASK_TAG,cur_line_num_temp)));  \
                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = X_TASK_GOTO_THE_END_LINE_NUM;   CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; \
                            return ; \
                            }    \
                            X_Task_if_line_pop_All(loop_nest_cnt,CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit)+1,  \
                                        &CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[loop_nest_cnt][0],CONCAT_2(CUR_TASK_TAG,if_line_num_buf));  \
                            X_Task_loop_line_pop_all(loop_nest_cnt,CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit)+1,  \
                                                            CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),CONCAT_2(CUR_TASK_TAG,loop_line_num_buf));  \
                            CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[(loop_nest_cnt - 1)] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp); \
                            CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[(loop_nest_cnt - 1)] = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);  \
                            CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[(loop_nest_cnt - 1)] = 0;                                     \
                            CONCAT_2(CUR_TASK_TAG,cur_line_num) = CONCAT_2(CUR_TASK_TAG,cur_line_num_temp);        \
                            return ;                                                                          \
                        }while(0)

/*
X_Task_loop_line_pop_all(loop_nest_cnt - 1,CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit)+1,  \
                                                     CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),CONCAT_2(CUR_TASK_TAG,loop_line_num_buf));  \
*/
/**********************************************************************************************************************/
#define TASK_BODY_BEGIN(task_func_name,task_init_func,max_if_nest_deepth,max_loop_nest_deepth)             \
                        static X_Boolean CONCAT_2(CUR_TASK_TAG,isInit) = X_False;                          \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,ms_backup) = 0 ;                              \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,cur_line_num) = 0 ;                              \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,cur_line_num_temp) = 0 ;                      \
                        static const uint16_t CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) = max_if_nest_deepth;          \
                        static const uint16_t CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) = max_loop_nest_deepth;          \
                        static uint8_t CONCAT_2(CUR_TASK_TAG,if_condition_buf)[max_if_nest_deepth + 1];      \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,if_line_num_buf)[max_if_nest_deepth + 1];       \
                        static uint8_t CONCAT_2(CUR_TASK_TAG,loop_condition_buf)[max_loop_nest_deepth + 1];      \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,loop_line_num_buf)[max_loop_nest_deepth + 1];      \
                        static uint32_t CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf)[max_loop_nest_deepth + 1];      \
                        static x_one_step CONCAT_2(CUR_TASK_TAG,init_func) = task_init_func;                      \
                        static uint8_t CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[max_loop_nest_deepth + 2][max_if_nest_deepth + 1]; \
                        static uint8_t CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 0;                              \
                        X_Void task_func_name(uint32_t x_task_param_ms,uint16_t task_id) {                                         \
                            uint16_t nest_cnt = 0,loop_nest_cnt = 0;   uint32_t loop_line_num_backup = 0;                       \
                            if(CONCAT_2(CUR_TASK_TAG,isInit) == X_False)                                          \
                            {                                                                                  \
                                CONCAT_2(CUR_TASK_TAG,isInit) = X_True;                                          \
                                if(CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) >= 0xfffe) {                   \
                                X_TASK_LOG(X_TASK_LOG_DEBUG,(" ---- task(id %d) error : max_if_nest_deepth beyond scope\r\n",task_id));  \
                                CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; return ;  }                                    \
                                if(CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) >= 0xfffe) {                   \
                                X_TASK_LOG(X_TASK_LOG_DEBUG,(" ---- task(id %d) error : max_loop_nest_deepth beyond scope\r\n",task_id));  \
                                CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 1; return ;  }                                  \
                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = 0;                                        \
                                if(CONCAT_2(CUR_TASK_TAG,init_func) != (x_one_step)0) {                               \
                                    CONCAT_2(CUR_TASK_TAG,init_func)();                             \
                                    X_TASK_LOG(X_TASK_LOG_DEBUG,(" ---- task(id %d) init done ms %d\r\n",task_id,x_task_param_ms));                   \
                                }                                                                     \
                                X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,if_condition_buf),max_if_nest_deepth + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,if_line_num_buf),max_if_nest_deepth + 1);           \
                                X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,loop_condition_buf),max_loop_nest_deepth + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf),max_loop_nest_deepth + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),max_loop_nest_deepth + 1);           \
                                X_Task_ByteBufClear(&CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[0][0],(max_loop_nest_deepth + 2) *(max_if_nest_deepth + 1) );      \
                            }                                                                                  \
                            else if(CONCAT_2(CUR_TASK_TAG,isErrorOccur) == 0)                                                                        

                                                    // X_do
                                                    // X_wait
                                                    // X_for 
                                                    // ...

#define TASK_BODY_END       if(CONCAT_2(CUR_TASK_TAG,isErrorOccur) == 0 && nest_cnt == 0 && loop_nest_cnt == 0) {                      \
                                CONCAT_2(CUR_TASK_TAG,cur_line_num) = 0;                                            \
                                X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,if_condition_buf),CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,if_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1);           \
                                X_Task_ByteBufClear(CONCAT_2(CUR_TASK_TAG,loop_condition_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);           \
                                X_Task_32bitsBufClear(CONCAT_2(CUR_TASK_TAG,loop_end_line_num_buf),CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 1);           \
                                X_Task_ByteBufClear(&CONCAT_2(CUR_TASK_TAG,if_line_flag_array)[0][0],(CONCAT_2(CUR_TASK_TAG,nest_loop_deepth_limit) + 2) *(CONCAT_2(CUR_TASK_TAG,nest_if_deepth_limit) + 1) );      \
                                UNUSED_VARIABLE(x_task_param_ms);                                              \
                                UNUSED_VARIABLE(loop_line_num_backup);                                              \
                                UNUSED_VARIABLE(CONCAT_2(CUR_TASK_TAG,cur_line_num_temp));  \
                                UNUSED_VARIABLE(CONCAT_2(CUR_TASK_TAG,ms_backup));      \
                            }                                                           \
                            else if (CONCAT_2(CUR_TASK_TAG,isErrorOccur) == 1 || nest_cnt != 0 || loop_nest_cnt != 0){                                                                     \
                            X_TASK_LOG(1,(" ---- task(id %d) fatal error occur , task stop !!!\r\n",task_id));                   \
                            CONCAT_2(CUR_TASK_TAG,isErrorOccur) = 2 ;  nest_cnt = 0;    loop_nest_cnt = 0;                      \
                            }                                           \
                        }

#define X_TASK_REGISTE(task_unique_id,task_func_name)  add_task_to_schedule(task_unique_id,task_func_name,&CONCAT_2(CUR_TASK_TAG,isInit))


#ifndef B_STATEMACHINE_H
#define B_STATEMACHINE_H

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "../../common/x_cross_platform.h"

#define BSM_CONCAT_2_(__A, __B)     __A##__B
#define BSM_CONCAT_2(__A, __B)      BSM_CONCAT_2_(__A, __B)

/*
#define BSM_CONCAT_3_(__A, __B, __C)    __A##__B##__C
#define BSM_CONCAT_3(__A, __B, __C)      BSM_CONCAT_3_(__A, __B, __C)

#define BSM_CONCAT_4_(__A, __B, __C ,__D)    __A##__B##__C##__D
#define BSM_CONCAT_4(__A, __B, __C, __D)      BSM_CONCAT_4_(__A, __B, __C ,__D)
*/

#define __BSM_VA_NUM_ARGS_IMPL(  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                                 _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
                                 _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
                                 _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80,  \
                                 _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
                                 _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, \
                                 _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140,  \
                                 _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, _160,  \
                                 _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172, _173, _174, _175, _176, _177, _178, _179, _180,  \
                                 _181, _182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200,  \
                                 _201, _202, _203, _204, _205, _206, _207, _208, _209, _210, _211, _212, _213, _214, _215, _216, _217, _218, _219, _220,  \
                                 _221, _222, _223, _224, _225, _226, _227, _228, _229, _230, _231, _232, _233, _234, _235, _236, _237, _238, _239, _240,  \
                                 _241, _242, _243, _244, _245, _246, _247, _248, _249, _250, _251, _252, _253, __N,...)      __N

#define __BSM_VA_NUM_ARGS(...)                                                \
            __BSM_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, \
                                    239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, \
                                    219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, \
                                    199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, \
                                    179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, \
                                    159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, \
                                    139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, \
                                    119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, \
                                    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
                                    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
                                    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
                                    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                                    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define INVALID_B_STATE_NUMBER		(0xFF)
#define B_STATE_NUMBER_STILL        (0xFE)
#define MAX_B_STATE_NUMBER    		(0xFD)
#define DEFAULT_B_STATE_NUMBER        0

typedef uint8_t 			StateNumber;
/*
typedef struct
{
	uint32_t ms_backup;  // = 0                            
 	uint32_t cur_line_num; // = 0 ;                                                      
}s_SM_Blocked_Param;
*/

typedef  struct
{
	StateNumber cur_state;
    uint32_t ms_backup;                       
 	uint32_t cur_line_num; 
}s_BSM_Param; //
typedef StateNumber (*BSM_Action)(s_BSM_Param *p_base,uint32_t ms);  // the null pointer check will be done by the BSM module 

typedef struct
{
	const StateNumber 	TotalStateNum;
    const X_Boolean     is_state_self_ctrl;
    const BSM_Action  * p_Action_buf;
    StateNumber       * p_CurrentStateNum;
    StateNumber       * p_SuddenChangeState;
}s_B_StateMachine;

#define BLOCKABLE_SM_DEF_NO_POINTER(__id,isSelfCtrlState,...)           \
    static StateNumber BSM_CONCAT_2(__id, _current_state_number) = DEFAULT_B_STATE_NUMBER;	                \
    static StateNumber BSM_CONCAT_2(__id, _sudden_change_state_number) = INVALID_B_STATE_NUMBER;				\
    static const BSM_Action BSM_CONCAT_2(__id,_action_buf)[__BSM_VA_NUM_ARGS(__VA_ARGS__)]  = {__VA_ARGS__};  \
    static const s_B_StateMachine BSM_CONCAT_2(__id, _instance) = {                                         \
                                        __BSM_VA_NUM_ARGS(__VA_ARGS__),                                   \
                                        isSelfCtrlState,                                              \
                                        BSM_CONCAT_2(__id,_action_buf),                             \
                                        &BSM_CONCAT_2(__id, _current_state_number),                 \
                                        &BSM_CONCAT_2(__id, _sudden_change_state_number),             \
                                    };                                                               

#define BLOCKABLE_SM_DEF(__id,isSelfCtrlState,...)         \
    static StateNumber BSM_CONCAT_2(__id, _current_state_number) = DEFAULT_B_STATE_NUMBER;	                \
    static StateNumber BSM_CONCAT_2(__id, _sudden_change_state_number) = INVALID_B_STATE_NUMBER;				\
    static const BSM_Action BSM_CONCAT_2(__id,_action_buf)[__BSM_VA_NUM_ARGS(__VA_ARGS__)]  = {__VA_ARGS__};  \
    static const s_B_StateMachine BSM_CONCAT_2(__id, _instance) = {                                         \
                                        __BSM_VA_NUM_ARGS(__VA_ARGS__),                                   \
                                        isSelfCtrlState,                                              \
                                        BSM_CONCAT_2(__id,_action_buf),                             \
                                        &BSM_CONCAT_2(__id, _current_state_number),                 \
                                        &BSM_CONCAT_2(__id, _sudden_change_state_number),             \
                                    };                                                               \
    static const s_B_StateMachine* __id = &BSM_CONCAT_2(__id, _instance)

/*
#define BSM_PARAM_EXTERN(__struct_ext)  uint32_t cur_line_num_temp = ms;   \
                                        __struct_ext *p_this = (__struct_ext *)p_base;   \
                                        cur_line_num_temp = p_this ->base.cur_line_num;   \
                                        cur_line_num_temp = cur_line_num_temp;
*/

#define BSM_PARAM_EXTERN(__struct_ext)  uint32_t cur_line_num_temp;   \
                                        __struct_ext *p_this = (__struct_ext *)p_base;   \
                                        ((void)(cur_line_num_temp));     \
                                         ((void)(p_this));            \
                                        ((void)(ms));

X_Boolean BSM_Run( const s_B_StateMachine *p_bsm,s_BSM_Param *p_param_base,uint32_t ms);

X_Boolean BSM_SetState(const s_B_StateMachine *p_bsm,StateNumber state);






/**********************************************************************************************************************/
#define S_do(todo_sentence)     do{ 													\
                                    cur_line_num_temp = __LINE__ ;    \
                                    if (p_base -> cur_line_num <  cur_line_num_temp){  \
                                        p_base -> cur_line_num = cur_line_num_temp;  \
                                        todo_sentence;                                   \
                                    }                                               \
                                  }while(0)
/**********************************************************************************************************************/
#define S_delay(x_wait_ms)  do{                         \
                                cur_line_num_temp = __LINE__ ;    \
                                if (p_base ->cur_line_num <  cur_line_num_temp){  \
                                        p_base ->cur_line_num = cur_line_num_temp;  \
                                        p_base ->ms_backup = ms;     \
                                        return B_STATE_NUMBER_STILL;                             \
                                }                                                                            \
                                else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                    if(((x_wait_ms) != 0) && ((ms - p_base ->ms_backup) < (x_wait_ms))) {return B_STATE_NUMBER_STILL;}   \
                                }                                                                            \
                              }while(0)
/**********************************************************************************************************************/
#define S_wait_until(condition_break_sentence)    do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->cur_line_num < cur_line_num_temp){       \
                                                                p_base ->cur_line_num = cur_line_num_temp;  \
                                                                return B_STATE_NUMBER_STILL;                                                    \
                                                        }                                                                            \
                                                        else if(p_base -> cur_line_num ==  cur_line_num_temp){          \
                                                            if(!(condition_break_sentence)) {return B_STATE_NUMBER_STILL;} }               \
                                                    }while(0)

#define S_time_limited_wait(x_limit_ms,condition_break_sentence)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                        \
                                                        if (p_base ->cur_line_num <  cur_line_num_temp){          \
                                                                p_base ->cur_line_num = cur_line_num_temp;  \
                                                                p_base ->ms_backup = ms;    \
                                                                return B_STATE_NUMBER_STILL;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((ms - p_base ->ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return B_STATE_NUMBER_STILL;}                         \
                                                                }   \
                                                            }               \
                                                    }while(0)

#define S_time_limited_wait_with_result(x_limit_ms,condition_break_sentence,wait_result_bool_variable)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->cur_line_num <  cur_line_num_temp){  \
                                                                p_base ->cur_line_num = cur_line_num_temp;  \
                                                                p_base ->ms_backup = ms;    \
                                                                wait_result_bool_variable = X_False;      \
                                                                return B_STATE_NUMBER_STILL;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((ms - p_base ->ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return B_STATE_NUMBER_STILL;}                         \
                                                                    wait_result_bool_variable = X_True;     \
                                                                }   \
                                                            }               \
                                                    }while(0)
/**********************************************************************************************************************/ 
// #define S_
/**********************************************************************************************************************/
#define S_return(return_value)  return return_value
/**********************************************************************************************************************/


#ifdef __cplusplus
		}
#endif

#endif
        

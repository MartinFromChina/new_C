#ifndef __X_TASK_COMMON_H
#define __X_TASK_COMMON_H

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
#define BOOL_BLOCK               X_True
#define BOOL_NO_BLOCK            X_False  
#define MAX_X_TASK_NUMBER      (0xffff)
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
typedef X_Void (*x_task_func)(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
typedef X_Void (*x_one_step)(uint32_t ms);

#define X_TASK_NULL_INIT_FUNC      ((x_one_step)0)
/**********************************************************************************************************************/
typedef enum
{
	xte_ok = 0,
	xte_pointer_null,
	xte_beyond_scope,
	xte_init_failed,
	xte_unknow_error,
    xte_frozen,
}e_x_task_error_code;

/**********************************************************************************************************************/

#define X_TASK_INVALID_LINE_NUM        0
#define X_TASK_GOTO_THE_END_LINE_NUM        0xFFFFFFFF
#define X_TASK_INVALID_DEEPTH          0xFFFF
X_Void X_Task_ByteBufClear(uint8_t *p_buf,uint16_t length);
X_Void X_Task_32bitsBufClear(uint32_t *p_buf,uint16_t length);

X_Void X_Task_if_line_push(uint16_t loop_deepth,uint16_t if_deepth,uint8_t *p_line_flag_buf);
X_Void X_Task_if_line_pop_All(uint16_t loop_deepth,uint16_t max_if_deepth,uint8_t *p_line_flag_buf,uint32_t *P_line_buf);
X_Void X_Task_loop_line_pop_all(uint16_t loop_deepth,uint16_t max_loop_deepth,uint32_t *P_end_line_buf,uint32_t *P_line_buf);

#ifdef __cplusplus
		}
#endif

#endif


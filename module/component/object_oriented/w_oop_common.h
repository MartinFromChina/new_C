#ifndef __W_OOP_COMMON_H
#define __W_OOP_COMMON_H


#ifdef __cplusplus
	extern "C" {
      #define COMMON_MULL_FUNC    nullptr 
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif

      #define COMMON_MULL_FUNC    X_Null   
#endif

#include "../../common/x_cross_platform.h"

// #define FUNC_PTR(ptr,...) ((ptr) != COMMON_MULL_FUNC ? (ptr)(__VA_ARGS__) : (void)0) 
/*************************************CONCAT marco
 * Auxiliary macro used by @ref CONCAT_2
*************************************************/
#define WOOP_CONCAT_2_(p1, p2)     p1##p2
#define WOOP_CONCAT_2(p1, p2)      WOOP_CONCAT_2_(p1, p2)

#define WOOP_CONCAT_3_(__A, __B, __C)    __A##__B##__C
#define WOOP_CONCAT_3(__A, __B, __C)      WOOP_CONCAT_3_(__A, __B, __C)

#define WOOP_CONCAT_4_(__A, __B, __C ,__D)    __A##__B##__C##__D
#define WOOP_CONCAT_4(__A, __B, __C, __D)      WOOP_CONCAT_4_(__A, __B, __C ,__D)

/**************************************************************************************/

/***************************************get the arguments number 
 * 
***********************************************/
#define MAX_WOOP_ARGS_NUM            16

#define __WOOP_PURE_NUM_ARGS_IMPL(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,  \
                                    _13,_14,_15,_16,__N,...)      __N
#define __WOOP_PURE_NUM_ARGS(...)                                                \
            __WOOP_PURE_NUM_ARGS_IMPL(,##__VA_ARGS__,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)

#define __WOOP_VA_NUM_ARGS_IMPL(   _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,  \
                                    _13,_14,_15,_16,__N,...)      __N
#define __WOOP_VA_NUM_ARGS(...)                                                \
            __WOOP_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)

#define __WOOP_OLF_NUM_ARGS_IMPL(   _0,_00,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,  \
                                    _13,_14,_15,_16,__N,...)      __N
#define __WOOP_OLF_NUM_ARGS(...)                                                \
            __WOOP_OLF_NUM_ARGS_IMPL( 0,512,##__VA_ARGS__,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)
/**************************************************************************************/

#define __using1(__declare)                                                     \
            for (__declare, *WOOP_CONCAT_3(__using_, __LINE__,_ptr) = NULL;          \
                 WOOP_CONCAT_3(__using_, __LINE__,_ptr)++ == NULL;                   \
                )
 
 
 
 
#define __using2(__declare, __on_leave_expr)                                    \
            for (__declare, *WOOP_CONCAT_3(__using_, __LINE__,_ptr) = NULL;          \
                 WOOP_CONCAT_3(__using_, __LINE__,_ptr)++ == NULL;                   \
                 __on_leave_expr                                                \
                )
 
 
 
 
#define __using3(__declare, __on_enter_expr, __on_leave_expr)                   \
            for (__declare, *WOOP_CONCAT_3(__using_, __LINE__,_ptr) = NULL;          \
                 WOOP_CONCAT_3(__using_, __LINE__,_ptr)++ == NULL ?                  \
                    ((__on_enter_expr),1) : 0;                                  \
                 __on_leave_expr                                                \
                )
 
 
#define __using4(__dcl1, __dcl2, __on_enter_expr, __on_leave_expr)              \
            for (__dcl1, __dcl2, *WOOP_CONCAT_3(__using_, __LINE__,_ptr) = NULL;     \
                 WOOP_CONCAT_3(__using_, __LINE__,_ptr)++ == NULL ?                  \
                    ((__on_enter_expr),1) : 0;                                  \
                 __on_leave_expr                                                \
                )
 
 #define USING(...)  \
    WOOP_CONCAT_2(__using, __WOOP_PURE_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define WITH(__type, __addr,__short_name)             USING(__type * WOOP_CONCAT_2(this_,__short_name)=(__addr))
#define CONST_WITH(__type, __addr,__short_name)       USING(__type * const * WOOP_CONCAT_2(this_,__short_name)=(__addr))
#define C_THIS(__short_name)                      (*WOOP_CONCAT_2(this_,__short_name))

/**********************************************************************************************/
#ifndef W_OOP_LOG_DEBUG_METHOD
    #include <stdio.h>
    #define  W_OOP_LOG_DEBUG_METHOD printf
#endif

#ifndef USE_W_OOP_LOG
    #define  USE_W_OOP_LOG 1
#endif

#if (USE_W_OOP_LOG != 0)
    #define W_OOP_LOG(flag,message)   do{ 																\
                                            if((flag) != 0)	\
                                            {	W_OOP_LOG_DEBUG_METHOD	message ;	   	}			\
                                        }while(0)

#else
    #define W_OOP_LOG(flag,message)  do{ }while(0)
#endif




#define FUNC_PTR_CHECK(__ptr,__RETURN)              \
                            if((__ptr) == COMMON_MULL_FUNC)   \
                            {                                   \
                                W_OOP_LOG_DEBUG_METHOD("fatal error !!! ,func prt null at line %d [%s]\r\n",__LINE__,__FUNCTION__);     \
                                __RETURN;                         \
                            }


#ifdef __cplusplus
		}
#endif

#endif

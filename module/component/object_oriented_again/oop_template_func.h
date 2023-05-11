#ifndef __OOP_TEMPLATE_FUNC_H
#define __OOP_TEMPLATE_FUNC_H

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "oop_common.h"

/********************************************************************************************************************************************************************/
// https://blog.csdn.net/wgp2hpp/article/details/110604082


/*************************************************************************************************/
#define MAX_TEMPLATE_FUNC_NUM                  20

#define __WOOP_TFUNC_NUM_ARGS_IMPL(   _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,  \
                                    _13,_14,_15,_16,_17,_18,_19,_20,__N,...)      __N
#define __WOOP_TFUNC_NUM_ARGS(...)                                                \
            __WOOP_TFUNC_NUM_ARGS_IMPL( 0,##__VA_ARGS__,20,19,18,17,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)

#define __T_FUNC_INIT_EXPAND_0(__prefix,...)    UNKNOWLOG(1,(" !!!  template func num can not be 0  \r\n"))
#define __T_FUNC_INIT_EXPAND_1(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func);
#define __T_FUNC_INIT_EXPAND_2(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_1(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_3(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_2(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_4(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_3(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_5(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_4(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_6(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func) ; __T_FUNC_INIT_EXPAND_5(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_7(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_6(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_8(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_7(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_9(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_8(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_10(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_9(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_11(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_10(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_12(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_11(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_13(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_12(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_14(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_13(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_15(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_14(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_16(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_15(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_17(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_16(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_18(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_17(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_19(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_18(__VA_ARGS__)
#define __T_FUNC_INIT_EXPAND_20(__prefix,...)    __prefix WOOP_CONCAT_2(__prefix,_instance_func); __T_FUNC_INIT_EXPAND_19(__VA_ARGS__)


#define  __T_FUNC_INIT_EXPAND(__prefix,...)     WOOP_CONCAT_2(__T_FUNC_INIT_EXPAND_,__WOOP_TFUNC_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define T_FUNC_INIT_EXPAND(__prefix,...)         __T_FUNC_INIT_EXPAND(__prefix,__VA_ARGS__) 


#define __P_T_FUNC_EXPAND_0(__prefix,...)    UNKNOWLOG(1,(" !!!  template func num can not be 0  \r\n"))
#define __P_T_FUNC_EXPAND_1(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0;
#define __P_T_FUNC_EXPAND_2(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_1(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_3(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_2(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_4(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_3(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_5(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_4(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_6(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_5(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_7(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_6(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_8(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_7(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_9(__prefix,...)    __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_8(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_10(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_9(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_11(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_10(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_12(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_11(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_13(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_12(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_14(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_13(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_15(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_14(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_16(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_15(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_17(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_16(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_18(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_17(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_19(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_18(__VA_ARGS__)
#define __P_T_FUNC_EXPAND_20(__prefix,...)   __prefix  WOOP_CONCAT_2(__prefix,_p_func) = (__prefix )0; __P_T_FUNC_EXPAND_19(__VA_ARGS__)


#define  __P_T_FUNC_EXPAND(__prefix,...)     WOOP_CONCAT_2(__P_T_FUNC_EXPAND_,__WOOP_TFUNC_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define P_T_FUNC_EXPAND(__prefix,...)         __P_T_FUNC_EXPAND(__prefix,__VA_ARGS__) 


// 三元运算与if else到底哪个更合适呢  ？
#define __P_T_FUNC_ASSIGN0_EXPAND_0(__prefix,...)    UNKNOWLOG(1,(" !!!  template func num can not be 0  \r\n"))
#define __P_T_FUNC_ASSIGN0_EXPAND_1(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;}
#define __P_T_FUNC_ASSIGN0_EXPAND_2(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_1(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_3(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_2(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_4(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_3(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_5(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_4(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_6(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_5(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_7(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_6(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_8(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_7(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_9(__prefix,...)    if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_8(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_10(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_9(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_11(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_10(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_12(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_11(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_13(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_12(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_14(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_13(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_15(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_14(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_16(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_15(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_17(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_16(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_18(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_17(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_19(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_18(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN0_EXPAND_20(__prefix,...)   if( parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN0_EXPAND_19(__VA_ARGS__)

#define  __P_T_FUNC_ASSIGN0_EXPAND(__prefix,...)     WOOP_CONCAT_2(__P_T_FUNC_ASSIGN0_EXPAND_,__WOOP_TFUNC_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define P_T_FUNC_ASSIGN0_EXPAND(__prefix,...)         __P_T_FUNC_ASSIGN0_EXPAND(__prefix,__VA_ARGS__) 

#define __P_T_FUNC_ASSIGN1_EXPAND_0(__prefix,...)    UNKNOWLOG(1,(" !!!  template func num can not be 0  \r\n"))
#define __P_T_FUNC_ASSIGN1_EXPAND_1(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}
#define __P_T_FUNC_ASSIGN1_EXPAND_2(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_1(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_3(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_2(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_4(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_3(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_5(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_4(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_6(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_5(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_7(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_6(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_8(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_7(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_9(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_8(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_10(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_9(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_11(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_10(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_12(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_11(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_13(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_12(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_14(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_13(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_15(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_14(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_16(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_15(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_17(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_16(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_18(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_17(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_19(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_18(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN1_EXPAND_20(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN1_EXPAND_19(__VA_ARGS__)

#define  __P_T_FUNC_ASSIGN1_EXPAND(__prefix,...)     WOOP_CONCAT_2(__P_T_FUNC_ASSIGN1_EXPAND_,__WOOP_TFUNC_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define P_T_FUNC_ASSIGN1_EXPAND(__prefix,...)         __P_T_FUNC_ASSIGN1_EXPAND(__prefix,__VA_ARGS__) 



#define __P_T_FUNC_ASSIGN2_EXPAND_0(__prefix,...)    UNKNOWLOG(1,(" !!!  template func num can not be 0  \r\n"))
#define __P_T_FUNC_ASSIGN2_EXPAND_1(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} 
#define __P_T_FUNC_ASSIGN2_EXPAND_2(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_1(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_3(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_2(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_4(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_3(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_5(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_4(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_6(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_5(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_7(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_6(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_8(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_7(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_9(__prefix,...)    if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_8(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_10(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_9(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_11(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_10(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_12(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_11(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_13(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_12(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_14(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_13(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_15(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_14(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_16(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_15(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_17(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_16(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_18(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_17(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_19(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_18(__VA_ARGS__)
#define __P_T_FUNC_ASSIGN2_EXPAND_20(__prefix,...)   if( child ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = child ->WOOP_CONCAT_2(__prefix,_instance_func) ;}else if(parent ->WOOP_CONCAT_2(__prefix,_instance_func) != (__prefix)0) { WOOP_CONCAT_2(__prefix,_p_func) = parent ->WOOP_CONCAT_2(__prefix,_instance_func) ;} __P_T_FUNC_ASSIGN2_EXPAND_19(__VA_ARGS__)

#define  __P_T_FUNC_ASSIGN2_EXPAND(__prefix,...)     WOOP_CONCAT_2(__P_T_FUNC_ASSIGN2_EXPAND_,__WOOP_TFUNC_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define P_T_FUNC_ASSIGN2_EXPAND(__prefix,...)         __P_T_FUNC_ASSIGN2_EXPAND(__prefix,__VA_ARGS__) 

/*************************************************************************************************/
#define T_FUNC(__func_type)                             WOOP_CONCAT_2(__func_type,_p_func)
#define T_FUNC_NULL(__func_type)                        ((__func_type)0)
#define TEMPLATE_METHOD(__t_funcs_struct)               WOOP_CONCAT_2(__t_funcs_struct,_algorithm)
#define TEMPLATE_METHOD_TYPE_DEFINE(__t_funcs_struct)   typedef X_Boolean  (*WOOP_CONCAT_2(func_,__t_funcs_struct))( __t_funcs_struct const * parent, __t_funcs_struct const *child)
#define TEMPLATE_METHOD_TYPE(__t_funcs_struct)          WOOP_CONCAT_2(func_,__t_funcs_struct)
#define TEMPLATE_METHOD_DECLARE(__t_funcs_struct)       X_Boolean  TEMPLATE_METHOD(__t_funcs_struct)(__t_funcs_struct const * parent,__t_funcs_struct const *child)

#define TEMPLATE_METHOD_DEFINE(__t_funcs_struct,...)                                     \
                            typedef struct                                                  \
                            {                                                                   \
                               T_FUNC_INIT_EXPAND(__t_funcs_struct,__VA_ARGS__)                  \
                            }__t_funcs_struct;                                                      \
        TEMPLATE_METHOD_DECLARE(__t_funcs_struct){                                              \
            P_T_FUNC_EXPAND(__t_funcs_struct,__VA_ARGS__)                                     \
            if(parent == (__t_funcs_struct *)0 && child == (__t_funcs_struct *)0) {return X_False;}   \
            if(parent != (__t_funcs_struct *)0 && child != (__t_funcs_struct *)0)                     \
            {                                                                                   \
                P_T_FUNC_ASSIGN2_EXPAND(__t_funcs_struct,__VA_ARGS__)                       \
            }                                                                               \
            else                                                                                \
            {                                                                           \
                if(parent != (__t_funcs_struct *)0)  {P_T_FUNC_ASSIGN0_EXPAND(__t_funcs_struct,__VA_ARGS__) } \
                else { P_T_FUNC_ASSIGN1_EXPAND(__t_funcs_struct,__VA_ARGS__) }           \
            }                                                                      \
            if(1)
        
       





#define TEMPLATE_METHOD_END   return X_True; }

/********************************************************************************************************************************************************************/



#ifdef __cplusplus
		}
#endif

#endif

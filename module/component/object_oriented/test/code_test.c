#define __PLOOC_VA_NUM_ARGS_IMPL(   _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,  \
                                    _13,_14,_15,_16,__N,...)      __N
#define __PLOOC_VA_NUM_ARGS(...)                                                \
            __PLOOC_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)

// http://www.javashuo.com/article/p-ziimmrnc-ws.html   关于递归宏的讲解
/*
#define __16_PLOOC_EVAL(__FUNC, __NO_ARGS)  __FUNC##__NO_ARGS
#define __15_PLOOC_EVAL(__FUNC, __NO_ARGS)  __16_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __14_PLOOC_EVAL(__FUNC, __NO_ARGS)  __15_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __13_PLOOC_EVAL(__FUNC, __NO_ARGS)  __14_PLOOC_EVAL(__FUNC, __NO_ARGS)

#define __12_PLOOC_EVAL(__FUNC, __NO_ARGS)  __13_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __11_PLOOC_EVAL(__FUNC, __NO_ARGS)  __12_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __10_PLOOC_EVAL(__FUNC, __NO_ARGS)  __11_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __9_PLOOC_EVAL(__FUNC, __NO_ARGS)   __10_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __8_PLOOC_EVAL(__FUNC, __NO_ARGS)   __9_PLOOC_EVAL(__FUNC, __NO_ARGS)

#define __7_PLOOC_EVAL(__FUNC, __NO_ARGS)   __8_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __6_PLOOC_EVAL(__FUNC, __NO_ARGS)   __7_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __5_PLOOC_EVAL(__FUNC, __NO_ARGS)   __6_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __4_PLOOC_EVAL(__FUNC, __NO_ARGS)   __5_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __3_PLOOC_EVAL(__FUNC, __NO_ARGS)   __4_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __2_PLOOC_EVAL(__FUNC, __NO_ARGS)   __3_PLOOC_EVAL(__FUNC, __NO_ARGS)
#define __1_PLOOC_EVAL(__FUNC, __NO_ARGS)   __2_PLOOC_EVAL(__FUNC, __NO_ARGS)
*/

#define __1_PLOOC_EVAL(__FUNC, __NO_ARGS)   __FUNC##__NO_ARGS
#define __0_PLOOC_EVAL(__FUNC, __NO_ARGS)   __1_PLOOC_EVAL(__FUNC, __NO_ARGS)

#define __PLOOC_EVAL(__FUNC, ...)           __0_PLOOC_EVAL(                     \
                                                __FUNC,                         \
                                                __PLOOC_VA_NUM_ARGS(__VA_ARGS__))

__PLOOC_EVAL(class_A,1,2,3,4,5,6,7,8,9,10,11,12,13,14)


__PLOOC_VA_NUM_ARGS(1,7,6,5)



#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
#define CONCAT_2_(p1, p2)     p1##p2

#define __T_BASE_INIT_EXPAND_1(__t_data_struct)    &##__t_data_struct##[0]##.base
#define __T_BASE_INIT_EXPAND_2(__t_data_struct)    __T_BASE_INIT_EXPAND_1(__t_data_struct),&##__t_data_struct##[1]##.base
#define __T_BASE_INIT_EXPAND_3(__t_data_struct)    __T_BASE_INIT_EXPAND_2(__t_data_struct),&##__t_data_struct##[2]##.base
#define __T_BASE_INIT_EXPAND_4(__t_data_struct)    __T_BASE_INIT_EXPAND_3(__t_data_struct),&##__t_data_struct##[3]##.base
#define __T_BASE_INIT_EXPAND_5(__t_data_struct)    __T_BASE_INIT_EXPAND_4(__t_data_struct),&##__t_data_struct##[4]##.base

#define  __T_BASE_INIT_EXPAND(__t_data_struct,__n)     CONCAT_2(__T_BASE_INIT_EXPAND_,__n)(__t_data_struct)
#define T_BASE_INIT_EXPAND(__t_data_struct,__n)        { __T_BASE_INIT_EXPAND(__t_data_struct,__n) }


typedef struct{
  s_T_taskCommonParam base;
  uint8_t state;
  uint16_t on_time;
  uint16_t off_time;
}s_LedParam;

static s_LedParam led_task_param[3];




static s_T_taskCommonParam const*  p_base_buf[3] = T_BASE_INIT_EXPAND(led_task_param,3);

各位，再请教下，我想利用宏展开初始化结构体指针数组。图片中的数组长度3如果替换为sizeof(led_task_param
)/sizeof(led_task_param[0]),46到53行的宏就会展开成sizeof，有什么办法能在预编译阶段自动获取到数组长度并用宏展开吗？
/****************************************************************************************************
the T_task_marco use line number as state flag to implement state-jump. if you want to be more efficient, use ProtoThread
in the task body:
there must be a T_marco at the beginning of each code line 
only one T_marco in the same code line
********************************************************************************************************/

#ifndef __TEMPLATE_TASK_H
#define __TEMPLATE_TASK_H

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "../x_task_common.h"

#define __T_BASE_INIT_EXPAND_0(__t_data_struct)   X_TASK_LOG(1,(" !!! task template instacne num can not be 0  \r\n"))
#define __T_BASE_INIT_EXPAND_1(__t_data_struct)    &(((__t_data_struct)[0]).base)
#define __T_BASE_INIT_EXPAND_2(__t_data_struct)    __T_BASE_INIT_EXPAND_1(__t_data_struct),&((__t_data_struct)[1]).base
#define __T_BASE_INIT_EXPAND_3(__t_data_struct)    __T_BASE_INIT_EXPAND_2(__t_data_struct),&((__t_data_struct)[2]).base
#define __T_BASE_INIT_EXPAND_4(__t_data_struct)    __T_BASE_INIT_EXPAND_3(__t_data_struct),&((__t_data_struct)[3]).base
#define __T_BASE_INIT_EXPAND_5(__t_data_struct)    __T_BASE_INIT_EXPAND_4(__t_data_struct),&((__t_data_struct)[4]).base
#define __T_BASE_INIT_EXPAND_6(__t_data_struct)    __T_BASE_INIT_EXPAND_5(__t_data_struct),&((__t_data_struct)[5]).base
#define __T_BASE_INIT_EXPAND_7(__t_data_struct)    __T_BASE_INIT_EXPAND_6(__t_data_struct),&((__t_data_struct)[6]).base
#define __T_BASE_INIT_EXPAND_8(__t_data_struct)    __T_BASE_INIT_EXPAND_7(__t_data_struct),&((__t_data_struct)[7]).base
#define __T_BASE_INIT_EXPAND_9(__t_data_struct)    __T_BASE_INIT_EXPAND_8(__t_data_struct),&((__t_data_struct)[8]).base
#define __T_BASE_INIT_EXPAND_10(__t_data_struct)    __T_BASE_INIT_EXPAND_9(__t_data_struct),&((__t_data_struct)[9]).base

#define __T_BASE_INIT_EXPAND_11(__t_data_struct)    __T_BASE_INIT_EXPAND_10(__t_data_struct),&((__t_data_struct)[10]).base
#define __T_BASE_INIT_EXPAND_12(__t_data_struct)    __T_BASE_INIT_EXPAND_11(__t_data_struct),&((__t_data_struct)[11]).base
#define __T_BASE_INIT_EXPAND_13(__t_data_struct)    __T_BASE_INIT_EXPAND_12(__t_data_struct),&((__t_data_struct)[12]).base
#define __T_BASE_INIT_EXPAND_14(__t_data_struct)    __T_BASE_INIT_EXPAND_13(__t_data_struct),&((__t_data_struct)[13]).base
#define __T_BASE_INIT_EXPAND_15(__t_data_struct)    __T_BASE_INIT_EXPAND_14(__t_data_struct),&((__t_data_struct)[14]).base
#define __T_BASE_INIT_EXPAND_16(__t_data_struct)    __T_BASE_INIT_EXPAND_15(__t_data_struct),&((__t_data_struct)[15]).base
#define __T_BASE_INIT_EXPAND_17(__t_data_struct)    __T_BASE_INIT_EXPAND_16(__t_data_struct),&((__t_data_struct)[16]).base
#define __T_BASE_INIT_EXPAND_18(__t_data_struct)    __T_BASE_INIT_EXPAND_17(__t_data_struct),&((__t_data_struct)[17]).base
#define __T_BASE_INIT_EXPAND_19(__t_data_struct)    __T_BASE_INIT_EXPAND_18(__t_data_struct),&((__t_data_struct)[18]).base
#define __T_BASE_INIT_EXPAND_20(__t_data_struct)    __T_BASE_INIT_EXPAND_19(__t_data_struct),&((__t_data_struct)[19]).base

#define __T_BASE_INIT_EXPAND_21(__t_data_struct)    __T_BASE_INIT_EXPAND_20(__t_data_struct),&((__t_data_struct)[20]).base
#define __T_BASE_INIT_EXPAND_22(__t_data_struct)    __T_BASE_INIT_EXPAND_21(__t_data_struct),&((__t_data_struct)[21]).base
#define __T_BASE_INIT_EXPAND_23(__t_data_struct)    __T_BASE_INIT_EXPAND_22(__t_data_struct),&((__t_data_struct)[22]).base
#define __T_BASE_INIT_EXPAND_24(__t_data_struct)    __T_BASE_INIT_EXPAND_23(__t_data_struct),&((__t_data_struct)[23]).base
#define __T_BASE_INIT_EXPAND_25(__t_data_struct)    __T_BASE_INIT_EXPAND_24(__t_data_struct),&((__t_data_struct)[24]).base
#define __T_BASE_INIT_EXPAND_26(__t_data_struct)    __T_BASE_INIT_EXPAND_25(__t_data_struct),&((__t_data_struct)[25]).base
#define __T_BASE_INIT_EXPAND_27(__t_data_struct)    __T_BASE_INIT_EXPAND_26(__t_data_struct),&((__t_data_struct)[26]).base
#define __T_BASE_INIT_EXPAND_28(__t_data_struct)    __T_BASE_INIT_EXPAND_27(__t_data_struct),&((__t_data_struct)[27]).base
#define __T_BASE_INIT_EXPAND_29(__t_data_struct)    __T_BASE_INIT_EXPAND_28(__t_data_struct),&((__t_data_struct)[28]).base
#define __T_BASE_INIT_EXPAND_30(__t_data_struct)    __T_BASE_INIT_EXPAND_29(__t_data_struct),&((__t_data_struct)[29]).base

#define __T_BASE_INIT_EXPAND_31(__t_data_struct)    __T_BASE_INIT_EXPAND_30(__t_data_struct),&((__t_data_struct)[30]).base
#define __T_BASE_INIT_EXPAND_32(__t_data_struct)    __T_BASE_INIT_EXPAND_31(__t_data_struct),&((__t_data_struct)[31]).base
#define __T_BASE_INIT_EXPAND_33(__t_data_struct)    __T_BASE_INIT_EXPAND_32(__t_data_struct),&((__t_data_struct)[32]).base
#define __T_BASE_INIT_EXPAND_34(__t_data_struct)    __T_BASE_INIT_EXPAND_33(__t_data_struct),&((__t_data_struct)[33]).base
#define __T_BASE_INIT_EXPAND_35(__t_data_struct)    __T_BASE_INIT_EXPAND_34(__t_data_struct),&((__t_data_struct)[34]).base
#define __T_BASE_INIT_EXPAND_36(__t_data_struct)    __T_BASE_INIT_EXPAND_35(__t_data_struct),&((__t_data_struct)[35]).base
#define __T_BASE_INIT_EXPAND_37(__t_data_struct)    __T_BASE_INIT_EXPAND_36(__t_data_struct),&((__t_data_struct)[36]).base
#define __T_BASE_INIT_EXPAND_38(__t_data_struct)    __T_BASE_INIT_EXPAND_37(__t_data_struct),&((__t_data_struct)[37]).base
#define __T_BASE_INIT_EXPAND_39(__t_data_struct)    __T_BASE_INIT_EXPAND_38(__t_data_struct),&((__t_data_struct)[38]).base
#define __T_BASE_INIT_EXPAND_40(__t_data_struct)    __T_BASE_INIT_EXPAND_39(__t_data_struct),&((__t_data_struct)[39]).base

#define __T_BASE_INIT_EXPAND_41(__t_data_struct)    __T_BASE_INIT_EXPAND_40(__t_data_struct),&((__t_data_struct)[40]).base
#define __T_BASE_INIT_EXPAND_42(__t_data_struct)    __T_BASE_INIT_EXPAND_41(__t_data_struct),&((__t_data_struct)[41]).base
#define __T_BASE_INIT_EXPAND_43(__t_data_struct)    __T_BASE_INIT_EXPAND_42(__t_data_struct),&((__t_data_struct)[42]).base
#define __T_BASE_INIT_EXPAND_44(__t_data_struct)    __T_BASE_INIT_EXPAND_43(__t_data_struct),&((__t_data_struct)[43]).base
#define __T_BASE_INIT_EXPAND_45(__t_data_struct)    __T_BASE_INIT_EXPAND_44(__t_data_struct),&((__t_data_struct)[44]).base
#define __T_BASE_INIT_EXPAND_46(__t_data_struct)    __T_BASE_INIT_EXPAND_45(__t_data_struct),&((__t_data_struct)[45]).base
#define __T_BASE_INIT_EXPAND_47(__t_data_struct)    __T_BASE_INIT_EXPAND_46(__t_data_struct),&((__t_data_struct)[46]).base
#define __T_BASE_INIT_EXPAND_48(__t_data_struct)    __T_BASE_INIT_EXPAND_47(__t_data_struct),&((__t_data_struct)[47]).base
#define __T_BASE_INIT_EXPAND_49(__t_data_struct)    __T_BASE_INIT_EXPAND_48(__t_data_struct),&((__t_data_struct)[48]).base
#define __T_BASE_INIT_EXPAND_50(__t_data_struct)    __T_BASE_INIT_EXPAND_49(__t_data_struct),&((__t_data_struct)[49]).base

#define __T_BASE_INIT_EXPAND_51(__t_data_struct)    __T_BASE_INIT_EXPAND_50(__t_data_struct),&((__t_data_struct)[50]).base
#define __T_BASE_INIT_EXPAND_52(__t_data_struct)    __T_BASE_INIT_EXPAND_51(__t_data_struct),&((__t_data_struct)[51]).base
#define __T_BASE_INIT_EXPAND_53(__t_data_struct)    __T_BASE_INIT_EXPAND_52(__t_data_struct),&((__t_data_struct)[52]).base
#define __T_BASE_INIT_EXPAND_54(__t_data_struct)    __T_BASE_INIT_EXPAND_53(__t_data_struct),&((__t_data_struct)[53]).base
#define __T_BASE_INIT_EXPAND_55(__t_data_struct)    __T_BASE_INIT_EXPAND_54(__t_data_struct),&((__t_data_struct)[54]).base
#define __T_BASE_INIT_EXPAND_56(__t_data_struct)    __T_BASE_INIT_EXPAND_55(__t_data_struct),&((__t_data_struct)[55]).base
#define __T_BASE_INIT_EXPAND_57(__t_data_struct)    __T_BASE_INIT_EXPAND_56(__t_data_struct),&((__t_data_struct)[56]).base
#define __T_BASE_INIT_EXPAND_58(__t_data_struct)    __T_BASE_INIT_EXPAND_57(__t_data_struct),&((__t_data_struct)[57]).base
#define __T_BASE_INIT_EXPAND_59(__t_data_struct)    __T_BASE_INIT_EXPAND_58(__t_data_struct),&((__t_data_struct)[58]).base
#define __T_BASE_INIT_EXPAND_60(__t_data_struct)    __T_BASE_INIT_EXPAND_59(__t_data_struct),&((__t_data_struct)[59]).base

#define __T_BASE_INIT_EXPAND_61(__t_data_struct)    __T_BASE_INIT_EXPAND_60(__t_data_struct),&((__t_data_struct)[60]).base
#define __T_BASE_INIT_EXPAND_62(__t_data_struct)    __T_BASE_INIT_EXPAND_61(__t_data_struct),&((__t_data_struct)[61]).base
#define __T_BASE_INIT_EXPAND_63(__t_data_struct)    __T_BASE_INIT_EXPAND_62(__t_data_struct),&((__t_data_struct)[62]).base
#define __T_BASE_INIT_EXPAND_64(__t_data_struct)    __T_BASE_INIT_EXPAND_63(__t_data_struct),&((__t_data_struct)[63]).base
#define __T_BASE_INIT_EXPAND_65(__t_data_struct)    __T_BASE_INIT_EXPAND_64(__t_data_struct),&((__t_data_struct)[64]).base
#define __T_BASE_INIT_EXPAND_66(__t_data_struct)    __T_BASE_INIT_EXPAND_65(__t_data_struct),&((__t_data_struct)[65]).base
#define __T_BASE_INIT_EXPAND_67(__t_data_struct)    __T_BASE_INIT_EXPAND_66(__t_data_struct),&((__t_data_struct)[66]).base
#define __T_BASE_INIT_EXPAND_68(__t_data_struct)    __T_BASE_INIT_EXPAND_67(__t_data_struct),&((__t_data_struct)[67]).base
#define __T_BASE_INIT_EXPAND_69(__t_data_struct)    __T_BASE_INIT_EXPAND_68(__t_data_struct),&((__t_data_struct)[68]).base
#define __T_BASE_INIT_EXPAND_70(__t_data_struct)    __T_BASE_INIT_EXPAND_69(__t_data_struct),&((__t_data_struct)[69]).base

#define __T_BASE_INIT_EXPAND_71(__t_data_struct)    __T_BASE_INIT_EXPAND_70(__t_data_struct),&((__t_data_struct)[70]).base
#define __T_BASE_INIT_EXPAND_72(__t_data_struct)    __T_BASE_INIT_EXPAND_71(__t_data_struct),&((__t_data_struct)[71]).base
#define __T_BASE_INIT_EXPAND_73(__t_data_struct)    __T_BASE_INIT_EXPAND_72(__t_data_struct),&((__t_data_struct)[72]).base
#define __T_BASE_INIT_EXPAND_74(__t_data_struct)    __T_BASE_INIT_EXPAND_73(__t_data_struct),&((__t_data_struct)[73]).base
#define __T_BASE_INIT_EXPAND_75(__t_data_struct)    __T_BASE_INIT_EXPAND_74(__t_data_struct),&((__t_data_struct)[74]).base
#define __T_BASE_INIT_EXPAND_76(__t_data_struct)    __T_BASE_INIT_EXPAND_75(__t_data_struct),&((__t_data_struct)[75]).base
#define __T_BASE_INIT_EXPAND_77(__t_data_struct)    __T_BASE_INIT_EXPAND_76(__t_data_struct),&((__t_data_struct)[76]).base
#define __T_BASE_INIT_EXPAND_78(__t_data_struct)    __T_BASE_INIT_EXPAND_77(__t_data_struct),&((__t_data_struct)[77]).base
#define __T_BASE_INIT_EXPAND_79(__t_data_struct)    __T_BASE_INIT_EXPAND_78(__t_data_struct),&((__t_data_struct)[78]).base
#define __T_BASE_INIT_EXPAND_80(__t_data_struct)    __T_BASE_INIT_EXPAND_79(__t_data_struct),&((__t_data_struct)[79]).base

#define __T_BASE_INIT_EXPAND_81(__t_data_struct)    __T_BASE_INIT_EXPAND_80(__t_data_struct),&((__t_data_struct)[80]).base
#define __T_BASE_INIT_EXPAND_82(__t_data_struct)    __T_BASE_INIT_EXPAND_81(__t_data_struct),&((__t_data_struct)[81]).base
#define __T_BASE_INIT_EXPAND_83(__t_data_struct)    __T_BASE_INIT_EXPAND_82(__t_data_struct),&((__t_data_struct)[82]).base
#define __T_BASE_INIT_EXPAND_84(__t_data_struct)    __T_BASE_INIT_EXPAND_83(__t_data_struct),&((__t_data_struct)[83]).base
#define __T_BASE_INIT_EXPAND_85(__t_data_struct)    __T_BASE_INIT_EXPAND_84(__t_data_struct),&((__t_data_struct)[84]).base
#define __T_BASE_INIT_EXPAND_86(__t_data_struct)    __T_BASE_INIT_EXPAND_85(__t_data_struct),&((__t_data_struct)[85]).base
#define __T_BASE_INIT_EXPAND_87(__t_data_struct)    __T_BASE_INIT_EXPAND_86(__t_data_struct),&((__t_data_struct)[86]).base
#define __T_BASE_INIT_EXPAND_88(__t_data_struct)    __T_BASE_INIT_EXPAND_87(__t_data_struct),&((__t_data_struct)[87]).base
#define __T_BASE_INIT_EXPAND_89(__t_data_struct)    __T_BASE_INIT_EXPAND_88(__t_data_struct),&((__t_data_struct)[88]).base
#define __T_BASE_INIT_EXPAND_90(__t_data_struct)    __T_BASE_INIT_EXPAND_89(__t_data_struct),&((__t_data_struct)[89]).base

#define __T_BASE_INIT_EXPAND_91(__t_data_struct)    __T_BASE_INIT_EXPAND_90(__t_data_struct),&((__t_data_struct)[90]).base
#define __T_BASE_INIT_EXPAND_92(__t_data_struct)    __T_BASE_INIT_EXPAND_91(__t_data_struct),&((__t_data_struct)[91]).base
#define __T_BASE_INIT_EXPAND_93(__t_data_struct)    __T_BASE_INIT_EXPAND_92(__t_data_struct),&((__t_data_struct)[92]).base
#define __T_BASE_INIT_EXPAND_94(__t_data_struct)    __T_BASE_INIT_EXPAND_93(__t_data_struct),&((__t_data_struct)[93]).base
#define __T_BASE_INIT_EXPAND_95(__t_data_struct)    __T_BASE_INIT_EXPAND_94(__t_data_struct),&((__t_data_struct)[94]).base
#define __T_BASE_INIT_EXPAND_96(__t_data_struct)    __T_BASE_INIT_EXPAND_95(__t_data_struct),&((__t_data_struct)[95]).base
#define __T_BASE_INIT_EXPAND_97(__t_data_struct)    __T_BASE_INIT_EXPAND_96(__t_data_struct),&((__t_data_struct)[96]).base
#define __T_BASE_INIT_EXPAND_98(__t_data_struct)    __T_BASE_INIT_EXPAND_97(__t_data_struct),&((__t_data_struct)[97]).base
#define __T_BASE_INIT_EXPAND_99(__t_data_struct)    __T_BASE_INIT_EXPAND_98(__t_data_struct),&((__t_data_struct)[98]).base
#define __T_BASE_INIT_EXPAND_100(__t_data_struct)    __T_BASE_INIT_EXPAND_99(__t_data_struct),&((__t_data_struct)[99]).base

#define __T_BASE_BUFF_SIZE(__t_data_struct) 		 (sizeof(__t_data_struct)/sizeof(__t_data_struct[0]))
#define T_BASE_BUFF_SIZE(__t_data_struct)  			__T_BASE_BUFF_SIZE(__t_data_struct)          

#define  __T_BASE_INIT_EXPAND(__t_data_struct,__n)     CONCAT_2(__T_BASE_INIT_EXPAND_,__n)(__t_data_struct)
#define T_BASE_INIT_EXPAND(__t_data_struct,__n)        { __T_BASE_INIT_EXPAND(__t_data_struct,__n) }


#define __T_TASK_NUM_INSTANCE_IMPL(   _0,_00,_000, 	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,   \
 													_11, _12, _13, _14, _15, _16, _17, _18, _19, _20,   \
 													_21, _22, _23, _24, _25, _26, _27, _28, _29, _30,   \
 													_31, _32, _33, _34, _35, _36, _37, _38, _39, _40,   \
 													_41, _42, _43, _44, _45, _46, _47, _48, _49, _50,   \
 													_51, _52, _53, _54, _55, _56, _57, _58, _59, _60,   \
 													_61, _62, _63, _64, _65, _66, _67, _68, _69, _70,   \
 													_71, _72, _73, _74, _75, _76, _77, _78, _79, _80,   \
 													_81, _82, _83, _84, _85, _86, _87, _88, _89, _90,   \
 													_91, _92, _93, _94, _95, _96, _97, _98, _99, _100,  \
													__N,...)      __N



#define __T_TASK_NUM_INSTANCE(...)                                                \
            __T_TASK_NUM_INSTANCE_IMPL( 0,00,000,##__VA_ARGS__, 100,               \
 																99, 98, 97, 96, 95, 94, 93, 92, 91, 90,  \
 																89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 	\
 																79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 	\
 																69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 		\
 																59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 		\
 																49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 			\
 																39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 		\
 																29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 		\
 																19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 		\
 																9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 					\
									)



									  

#define MAX_TEMPLATE_INSTANCE_NUM                    100 
#define MAX_TEMPLATE_NAME_LENGTH                     15
#define MAX_TEMPLATE_INSTANCE_NAME_LENGTH            15
#define MAX_T_TASK_IF_NEST_DEEPTH                    8
#define MAX_T_TASK_LOOP_NEST_DEEPTH                  8


typedef struct
{
	/*~~~~~~~~~~~~~~~~T task value ~~~~~~~~~~~~~~~~~~~~~~*/
	uint32_t ms_backup;  // = 0                            
 	uint32_t cur_line_num; // = 0 ;                              
 	
 	uint8_t if_condition_buf[MAX_T_TASK_IF_NEST_DEEPTH + 1];      
 	uint32_t if_line_num_buf[MAX_T_TASK_IF_NEST_DEEPTH + 1];       
 	uint8_t loop_condition_buf[MAX_T_TASK_LOOP_NEST_DEEPTH + 1];      
 	uint32_t loop_line_num_buf[MAX_T_TASK_LOOP_NEST_DEEPTH + 1];      
 	uint32_t loop_end_line_num_buf[MAX_T_TASK_LOOP_NEST_DEEPTH + 1];                           
 	uint8_t if_line_flag_array[MAX_T_TASK_LOOP_NEST_DEEPTH + 2][MAX_T_TASK_IF_NEST_DEEPTH + 1]; 
 	uint8_t isErrorOccur ; // ) = 0;                              
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}s_T_taskCommonParam;

typedef X_Void (*t_task_func)(uint32_t t_task_ms,uint8_t *p_isInitDone,s_T_taskCommonParam * p_base,
                                uint16_t instance_id,uint8_t const * p_name);
typedef X_Void (*t_one_step)(uint32_t t_task_ms,
                                uint16_t instance_id,uint8_t const * p_name,
                                X_Void *p_this);

typedef struct 
{
    uint8_t      template_name[MAX_TEMPLATE_NAME_LENGTH];
	uint16_t     instance_num;           // total instance number 
	uint16_t     instance_with_name_num;
    uint8_t      const * p_instance_name_buf;
////////////////	uint16_t     nest_if_deepth_limit;
////////////////	uint16_t     nest_loop_deepth_limit;
	uint8_t      * p_is_init_ok;
	uint8_t      * p_init_flag_buf;
	uint8_t      * p_freeze_flag_buf;
	t_task_func    			p_template_task_func;
	s_T_taskCommonParam 	 *const * p_param_buf;
}s_T_Tasks_Manager;

/**********************************************************************************************************************/

/**********************************************************************************************************************/
e_x_task_error_code t_task_init(const s_T_Tasks_Manager *p_manager);                               
e_x_task_error_code t_task_scheduler(const s_T_Tasks_Manager *p_manager,uint32_t ms);

e_x_task_error_code freeze_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id);
e_x_task_error_code unfreeze_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id);
e_x_task_error_code restart_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id);
/**********************************************************************************************************************/
/**********************************************************************************************************************/
#define T_do(todo_sentence)     do{ 													\
                                    cur_line_num_temp = __LINE__ ;    \
                                    if (p_base -> cur_line_num <  cur_line_num_temp){  \
                                        p_base -> cur_line_num = cur_line_num_temp;  \
                                        todo_sentence;                                   \
                                    }                                               \
                                  }while(0)
/**********************************************************************************************************************/
#define T_delay(x_wait_ms)  do{                         \
                                cur_line_num_temp = __LINE__ ;    \
                                if (p_base ->cur_line_num <  cur_line_num_temp){  \
                                        p_base ->cur_line_num = cur_line_num_temp;  \
                                        p_base ->ms_backup = t_task_ms;     \
                                        return;                             \
                                }                                                                            \
                                else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                    if(((x_wait_ms) != 0) && ((t_task_ms - p_base ->ms_backup) < (x_wait_ms))) {return;}   \
                                }                                                                            \
                              }while(0)

#define T_wait_until(condition_break_sentence)    do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->cur_line_num < cur_line_num_temp){       \
                                                                p_base ->cur_line_num = cur_line_num_temp;  \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if(p_base -> cur_line_num ==  cur_line_num_temp){          \
                                                            if(!(condition_break_sentence)) {return;} }               \
                                                    }while(0)

#define T_time_limited_wait(x_limit_ms,condition_break_sentence)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                        \
                                                        if (p_base ->cur_line_num <  cur_line_num_temp){          \
                                                                p_base -> cur_line_num = cur_line_num_temp;  \
                                                                p_base ->ms_backup = t_task_ms;    \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((t_task_ms - p_base ->ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return;}                         \
                                                                }   \
                                                            }               \
                                                    }while(0)

#define T_time_limited_wait_with_result(x_limit_ms,condition_break_sentence,wait_result_bool_variable)  do{                         \
                                                        cur_line_num_temp = __LINE__ ;                     \
                                                        if (p_base ->cur_line_num <  cur_line_num_temp){  \
                                                                p_base ->cur_line_num = cur_line_num_temp;  \
                                                                p_base ->ms_backup = t_task_ms;    \
                                                                wait_result_bool_variable = X_False;      \
                                                                return;                                                    \
                                                        }                                                                            \
                                                        else if(p_base ->cur_line_num ==  cur_line_num_temp){          \
                                                                if(((x_limit_ms) != 0) && ((t_task_ms - p_base ->ms_backup) < (x_limit_ms))) {  \
                                                                    if(!(condition_break_sentence)) {return;}                         \
                                                                    wait_result_bool_variable = X_True;     \
                                                                }   \
                                                            }               \
                                                    }while(0)
/**********************************************************************************************************************/ 

/**********************************************************************************************************************/
#define T_return  do{                                                       \
                        if(p_base ->isErrorOccur == 0) {                             \
                            p_base ->cur_line_num = 0;                                       \
                            X_Task_ByteBufClear(p_base ->if_condition_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->if_line_num_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(p_base ->loop_condition_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_end_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(&p_base ->if_line_flag_array[0][0],(MAX_T_TASK_LOOP_NEST_DEEPTH + 2) *(MAX_T_TASK_IF_NEST_DEEPTH + 1) );      \
                        }                                                           \
                        else {                                                                     \
                        X_TASK_LOG(1,(" ---- template task %s : instance %d : fatal error occur , task stop !!!\r\n",(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id));                   \
                        p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;        \
                        }                                           \
                        return ;                                     \
                    }while(0)
      
/**********************************************************************************************************************/
#define T_if(conditon)   do{     \
                                cur_line_num_temp = __LINE__ ;    \
                                nest_cnt ++;                                                \
                                if((nest_cnt - 1) > MAX_T_TASK_IF_NEST_DEEPTH)  {  \
                                    X_TASK_LOG(1,(" ---- template task %s : instance %d : if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                    ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,(nest_cnt - 1),MAX_T_TASK_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                    p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                                    return ; \
                                }    \
                                X_Task_if_line_push(loop_nest_cnt,nest_cnt - 1,&p_base ->if_line_flag_array[loop_nest_cnt][0]) ;       \
                                if (p_base ->if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  									\
                                    p_base ->if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;                              \
                                    p_base ->if_condition_buf[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                }                        \
                                else if(p_base ->if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                    if((p_base ->if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                    p_base ->if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                    }       \
                                    else {                \
                                    p_base ->if_condition_buf[nest_cnt - 1] = 0;}                    \
                                }                                                                  \
                                else{                                                                   \
                                    p_base ->if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                }                          \
                            }while(0);   \
                            if(p_base ->if_condition_buf[nest_cnt - 1] & 0xF0)

#define T_else_if(conditon)    do{     \
                                    cur_line_num_temp = __LINE__ ;    \
                                    if((nest_cnt - 1) > MAX_T_TASK_IF_NEST_DEEPTH )  {  \
                                        X_TASK_LOG(1,(" ---- template task %s : instance %d : else if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,nest_cnt - 1,MAX_T_TASK_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                        p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                                        return ;                  \
                                    }    \
                                    if (p_base ->if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  \
                                        p_base ->if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;  \
                                        if((p_base ->if_condition_buf[nest_cnt - 1] & 0x0F) == 0) {     \
                                        	p_base ->if_condition_buf[nest_cnt - 1] = (conditon) ? 0xFF : 0;   \
                                        }                              \
                                        else {                         \
                                        	p_base ->if_condition_buf[nest_cnt - 1] = 0x0F;       \
                                        	p_base ->if_line_num_buf[nest_cnt - 1] ++;                \
                                        }                                                                           \
                                    }                        \
                                    else if(p_base ->if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                        if((p_base ->if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                        	p_base ->if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                        	p_base ->if_condition_buf[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{                                                                  \
                                        p_base ->if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(p_base ->if_condition_buf[nest_cnt - 1] & 0xF0)

#define T_else               do{     \
                                    cur_line_num_temp = __LINE__ ;    \
                                    if((nest_cnt - 1) > MAX_T_TASK_IF_NEST_DEEPTH)  {  \
                                        X_TASK_LOG(1,(" ---- template task %s : instance %d : else nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                        ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,nest_cnt - 1,MAX_T_TASK_IF_NEST_DEEPTH,cur_line_num_temp));  \
                                        p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                                        return ; \
                                    }    \
                                    if (p_base ->if_line_num_buf[nest_cnt - 1] <  cur_line_num_temp){  \
                                        p_base ->if_line_num_buf[nest_cnt - 1] = cur_line_num_temp;  \
                                        if((p_base ->if_condition_buf[nest_cnt - 1] & 0x0F) == 0) {     \
                                            p_base ->if_condition_buf[nest_cnt - 1] = 0xFF;   \
                                        }                              \
                                        else {                         \
                                            p_base ->if_condition_buf[nest_cnt - 1] = 0x0F;       \
                                        }                                                                           \
                                    }                        \
                                    else if(p_base ->if_line_num_buf[nest_cnt - 1] == cur_line_num_temp) {                     \
                                        if((p_base ->if_condition_buf[nest_cnt - 1] & 0x0F) != 0) {     \
                                            p_base ->if_condition_buf[nest_cnt - 1] = 0xFF;                     \
                                        }       \
                                        else {                \
                                            p_base ->if_condition_buf[nest_cnt - 1] = 0;}                    \
                                    }                                                                  \
                                    else{ \
                                        p_base ->if_condition_buf[nest_cnt - 1] &= 0x0F;                    \
                                    }                          \
                                }while(0);   \
                                if(p_base ->if_condition_buf[nest_cnt - 1] & 0xF0) 

#define T_endif   do{ 													\
                        cur_line_num_temp = __LINE__ ;    \
                        nest_cnt --;                                                \
                        if(nest_cnt > MAX_T_TASK_IF_NEST_DEEPTH )  {  \
                        X_TASK_LOG(1,(" ---- template task %s : instance %d : end if nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                        ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,nest_cnt,MAX_T_TASK_IF_NEST_DEEPTH,cur_line_num_temp));  \
                        p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                        return ; \
                        }    \
                        if (p_base ->if_line_num_buf[nest_cnt] <  cur_line_num_temp){  \
                            p_base ->if_line_num_buf[nest_cnt] = cur_line_num_temp;  \
                        }                                               \
                    }while(0);

/**********************************************************************************************************************/
                 
#define T_while(conditon_true)      do{                                                           \
                                        cur_line_num_temp = __LINE__ ;    \
                                        loop_nest_cnt ++;                                                \
                                        if((loop_nest_cnt - 1) > MAX_T_TASK_LOOP_NEST_DEEPTH)  {  \
                                            X_TASK_LOG(1,(" ---- template task %s : instance %d : loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                                            ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,loop_nest_cnt - 1,MAX_T_TASK_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                                            p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                                            return ; \
                                        }    \
                                        if (p_base ->loop_line_num_buf[loop_nest_cnt - 1] <=  cur_line_num_temp \
                                            && p_base ->loop_end_line_num_buf[loop_nest_cnt - 1] < cur_line_num_temp){  \
                                            p_base ->loop_line_num_buf[loop_nest_cnt - 1] = cur_line_num_temp;  \
                                            p_base ->loop_end_line_num_buf[loop_nest_cnt - 1] = cur_line_num_temp;  \
                                            if(conditon_true) {                                                       \
                                                p_base ->loop_condition_buf[loop_nest_cnt - 1] = 0xFF;  \
                                                loop_line_num_backup = cur_line_num_temp;}                              \
                                            else {                                                                    \
                                                p_base ->loop_condition_buf[loop_nest_cnt - 1] = 0;}                        \
                                        }                                                                                  \
                                        else if(p_base ->loop_line_num_buf[loop_nest_cnt - 1] == cur_line_num_temp \
                                                && p_base ->loop_end_line_num_buf[loop_nest_cnt - 1] == cur_line_num_temp) {   \
                                                if((p_base ->loop_condition_buf[loop_nest_cnt - 1] & 0x0F)!= 0) {   \
                                                    p_base ->loop_condition_buf[loop_nest_cnt - 1] = 0xFF ;                \
                                                    loop_line_num_backup = cur_line_num_temp;}                       \
                                                else {                                                                          \
                                                    p_base ->loop_condition_buf[loop_nest_cnt - 1] = 0 ;  }              \
                                        }                                                                  \
                                        else if(p_base ->loop_line_num_buf[loop_nest_cnt - 1] > cur_line_num_temp \
                                                && p_base ->loop_end_line_num_buf[loop_nest_cnt - 1] != cur_line_num_temp) { \
                                            p_base ->loop_condition_buf[loop_nest_cnt - 1] &= 0x0F;                    \
                                        }                          \
                                        else {   \
                                        X_TASK_LOG(1,(" ---- template task %s : instance %d : loop nest error occur ; line %d ; loop_line %d , loop_end_line %d\r\n",(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,cur_line_num_temp \
                                        ,p_base ->loop_line_num_buf[loop_nest_cnt - 1],p_base ->loop_end_line_num_buf[loop_nest_cnt - 1]));  \
                                            p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                                            return;}  \
                                    }while(0);   \
                                    if(p_base ->loop_condition_buf[loop_nest_cnt - 1] & 0xF0)


#define T_endloop      do{ 				                                     		\
                            cur_line_num_temp = __LINE__ ;    \
                            if(p_base ->loop_line_num_buf[loop_nest_cnt - 1] < cur_line_num_temp) {                                            \
                            X_Task_if_line_pop_All(loop_nest_cnt,MAX_T_TASK_IF_NEST_DEEPTH+1,  \
                                        &p_base ->if_line_flag_array[loop_nest_cnt][0],p_base ->if_line_num_buf);}  \
                            loop_nest_cnt --;                                                \
                            if(loop_nest_cnt > MAX_T_TASK_LOOP_NEST_DEEPTH)  {  \
                            X_TASK_LOG(1,(" ---- template task %s : instance %d : end loop nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,loop_nest_cnt,MAX_T_TASK_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                            p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                            return ; \
                            }    \
                            if(p_base ->cur_line_num < cur_line_num_temp)  {                \
                                 X_Task_loop_line_pop_all(loop_nest_cnt+1,MAX_T_TASK_LOOP_NEST_DEEPTH+1,  \
                                                            p_base ->loop_end_line_num_buf,p_base ->loop_line_num_buf);  \
                                if ( p_base ->loop_condition_buf[loop_nest_cnt] == 0){                                     \
                                    p_base ->loop_line_num_buf[loop_nest_cnt] = cur_line_num_temp;  \
                                    p_base ->loop_end_line_num_buf[loop_nest_cnt] = cur_line_num_temp;          \
                                    p_base ->cur_line_num = cur_line_num_temp;        \
                                }                                               \
                                else {                                                                   \
                                    p_base ->loop_line_num_buf[loop_nest_cnt] = loop_line_num_backup;   \
                                    p_base ->loop_end_line_num_buf[loop_nest_cnt] = 0;                                       \
                                    p_base ->cur_line_num = loop_line_num_backup;                               \
                                return;    }      \
                            }         \
                        }while(0);

#define T_break        do{ 				                                     		\
                            cur_line_num_temp = __LINE__ ;                              \
                            if((loop_nest_cnt - 1) > MAX_T_TASK_LOOP_NEST_DEEPTH)  {  \
                            X_TASK_LOG(1,(" ---- template task %s : instance %d : break nest deepth(%d) beyond scope(%d) ; line %d \r\n"                   \
                            ,(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id,(loop_nest_cnt - 1),MAX_T_TASK_LOOP_NEST_DEEPTH,cur_line_num_temp));  \
                            p_base ->cur_line_num = X_TASK_GOTO_THE_END_LINE_NUM;   p_base ->isErrorOccur = 1; \
                            return ; \
                            }    \
                            X_Task_if_line_pop_All(loop_nest_cnt,MAX_T_TASK_IF_NEST_DEEPTH+1,  \
                                        &p_base ->if_line_flag_array[loop_nest_cnt][0],p_base ->if_line_num_buf);  \
                            X_Task_loop_line_pop_all(loop_nest_cnt,MAX_T_TASK_LOOP_NEST_DEEPTH+1,  \
                                                            p_base ->loop_end_line_num_buf,p_base ->loop_line_num_buf);  \
                            p_base ->loop_end_line_num_buf[(loop_nest_cnt - 1)] = cur_line_num_temp; \
                            p_base ->loop_line_num_buf[(loop_nest_cnt - 1)] = cur_line_num_temp;  \
                            p_base ->loop_condition_buf[(loop_nest_cnt - 1)] = 0;                                     \
                            p_base ->cur_line_num = cur_line_num_temp;        \
                            return ;                                                                          \
                        }while(0)
/**********************************************************************************************************************/

/**********************************************************************************************************************/
#define __T_TASK_TO_STR(y)     #y
#define T_TASK_TO_STR(y)       __T_TASK_TO_STR(y)

#define TEMPLATE_TASKS_BODY_BEGIN( t_task_id,               \
                                    task_init_func,                     \
                                    ext_data_struct_descrip,				  \
									instance_num,...)            							\
			static uint8_t CONCAT_2(t_task_id,_is_init_OK) = 0;   									\
			static uint8_t CONCAT_2(t_task_id,_init_flag_buf)[instance_num];   			\
			static uint8_t CONCAT_2(t_task_id,_freeze_flag_buf)[instance_num];  					\
            typedef struct {                                                            \
                s_T_taskCommonParam base;                                                 \
                ext_data_struct_descrip ext_data;                                       \
                }CONCAT_2(t_task_id,_s_all_data);                                       \
            static CONCAT_2(t_task_id,_s_all_data)  CONCAT_2(t_task_id,_all_data_instance)[instance_num];    \
			static s_T_taskCommonParam * const  CONCAT_2(t_task_id,_p_base_buf)[instance_num]  \
									= T_BASE_INIT_EXPAND(CONCAT_2(t_task_id,_all_data_instance),instance_num);       \
			static const uint8_t CONCAT_2(t_task_id,_instance_name_array)[__T_TASK_NUM_INSTANCE(__VA_ARGS__) + 1][MAX_TEMPLATE_INSTANCE_NAME_LENGTH] = {   \
			"head",##__VA_ARGS__};				\
			static X_Void CONCAT_2(t_task_id,_abstract_template_func)(uint32_t t_task_ms,    			\
																uint8_t *p_isInitDone,        			\
																s_T_taskCommonParam * p_base,       \
                                                                uint16_t instance_id,uint8_t const * p_name);   	\
			static const s_T_Tasks_Manager CONCAT_2(t_task_id,_T_Tasks_manager) = {   		\
                        T_TASK_TO_STR(t_task_id),										        \
						instance_num,                                            \
						__T_TASK_NUM_INSTANCE(__VA_ARGS__),                     \
                        &CONCAT_2(t_task_id,_instance_name_array)[0][0],                 \
						&CONCAT_2(t_task_id,_is_init_OK),                           \
						CONCAT_2(t_task_id,_init_flag_buf),                         \
						CONCAT_2(t_task_id,_freeze_flag_buf),                       \
						CONCAT_2(t_task_id,_abstract_template_func),                \
						CONCAT_2(t_task_id,_p_base_buf),    \
						};                                                                    \
            static const s_T_Tasks_Manager * t_task_id = &CONCAT_2(t_task_id,_T_Tasks_manager);  \
            static X_Void CONCAT_2(t_task_id,_abstract_template_func)(  uint32_t t_task_ms,             \
                                                                        uint8_t *p_isInitDone,        			\
																        s_T_taskCommonParam * p_base,          \
                                                                        uint16_t instance_id,uint8_t const * p_name){               \
                            uint32_t cur_line_num_temp = 0 ;                                                            \
                            uint16_t nest_cnt = 0,loop_nest_cnt = 0;   uint32_t loop_line_num_backup = 0;                       \
                            if(p_isInitDone == (uint8_t *)0 || p_base == (s_T_taskCommonParam *)0 || instance_id >= instance_num)    {                      \
                            X_TASK_LOG(1,(" ---- template task %s :instance %d : parameter error occur \r\n",T_TASK_TO_STR(t_task_id) ,instance_id)); return; }      \
                            CONCAT_2(t_task_id,_s_all_data) * p_all = (CONCAT_2(t_task_id,_s_all_data)*) p_base ;                    \
                            ext_data_struct_descrip * p_this = (ext_data_struct_descrip *)&p_all ->ext_data;            \
                            if(*p_isInitDone == 0)                                          \
                            {                                                                                  \
                                *p_isInitDone = 1;                                          \
                                p_base -> cur_line_num      = 0;                                        \
                                p_base -> ms_backup         = t_task_ms;                                           \
                                p_base -> isErrorOccur      = 0;                                            \
                                if((task_init_func) != (t_one_step)0) {                               \
                                    (task_init_func)(t_task_ms,instance_id,p_name,p_this);             \
                                }                                                                     \
                                X_Task_ByteBufClear(p_base ->if_condition_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->if_line_num_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(p_base ->loop_condition_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_end_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(&p_base ->if_line_flag_array[0][0],(MAX_T_TASK_LOOP_NEST_DEEPTH + 2) *(MAX_T_TASK_IF_NEST_DEEPTH + 1) );      \
                            }                                                                                  \
                            else if(p_base -> isErrorOccur == 0)                          
                                                                        
                                                    // T_do
                                                    // T_wait
                                                    // T_for 
                                                    // T_while
                                                    // ...

#define TEMPLATE_TASKS_BODY_END          \
                            if(p_base -> isErrorOccur == 0 && nest_cnt == 0 && loop_nest_cnt == 0) {                      \
                                p_base ->cur_line_num = 0;                                            \
                                X_Task_ByteBufClear(p_base ->if_condition_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->if_line_num_buf,MAX_T_TASK_IF_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(p_base ->loop_condition_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_32bitsBufClear(p_base ->loop_end_line_num_buf,MAX_T_TASK_LOOP_NEST_DEEPTH + 1);           \
                                X_Task_ByteBufClear(&p_base ->if_line_flag_array[0][0],(MAX_T_TASK_LOOP_NEST_DEEPTH + 2) *(MAX_T_TASK_IF_NEST_DEEPTH + 1) );      \
                                UNUSED_VARIABLE(t_task_ms);                                                                                      \
                                UNUSED_VARIABLE(loop_line_num_backup);                                              \
                                UNUSED_VARIABLE(cur_line_num_temp);                                                     \
                                UNUSED_VARIABLE(p_base ->ms_backup);                          \
                            }                                                           \
                            else if (p_base -> isErrorOccur == 1 || nest_cnt != 0 || loop_nest_cnt != 0){                                                   \
                            X_TASK_LOG(1,(" ---- template task %s : instance %d : fatal error occur , task stop !!!\r\n",(p_name != (uint8_t*)0)? (char *)p_name : "unknow " ,instance_id));                   \
                            p_base -> isErrorOccur = 2 ;  nest_cnt = 0;    loop_nest_cnt = 0;                      \
                            }                                           \
                        }                   
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

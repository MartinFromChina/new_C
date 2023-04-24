#ifndef __CP_DRIVE_H
#define __CP_DRIVE_H

#ifdef __cplusplus
	extern "C" {

#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/loopqueue/loop_queues.h"
#include "../../component/const_loop_scheduler/HierarchicalSM/h_state_machine.h"

/*
#define MAX_HIERARCHICAL_DEEPTH                   10
#define MAX_HSM_IF_NEST_DEEPTH                    5
#define MAX_HSM_LOOP_NEST_DEEPTH                  4
*/
#define CP_LENGTH_SIZE_IN_BYTE             2
#define CP_TIMESTAMP_SIZE_IN_BYTE          4
#define CP_MAX_EVENT_NUM                   (0xFFFE)

#ifndef  CP_EVENT_HSM_DEEPTH                           // use gloable marco to replace it ,make sure it take effort for both cp_drive.h and cp_drive.c
    #define CP_EVENT_HSM_DEEPTH               0
#endif

#if (CP_INIT_HSM_DEEPTH > MAX_HIERARCHICAL_DEEPTH)
   #error [CP_INIT_HSM_DEEPTH should be <= MAX_HIERARCHICAL_DEEPTH ]
#endif

#ifndef  CP_RECV_HSM_DEEPTH
    #define CP_RECV_HSM_DEEPTH               0
#endif

#if (CP_RECV_HSM_DEEPTH > MAX_HIERARCHICAL_DEEPTH)
   #error [CP_RECV_HSM_DEEPTH should be <= MAX_HIERARCHICAL_DEEPTH ]
#endif

#ifndef  CP_SEND_HSM_DEEPTH
    #define CP_SEND_HSM_DEEPTH               0
#endif

#if (CP_SEND_HSM_DEEPTH > MAX_HIERARCHICAL_DEEPTH)
   #error [CP_SEND_HSM_DEEPTH should be <= MAX_HIERARCHICAL_DEEPTH ]
#endif

typedef enum{
            CSRE_Successed,
            CSRE_Timeout,
            CSRE_Error,
            CSRE_RecvCheck,
        }e_cp_SendRecEvent;

typedef X_Boolean (*cp_send_rec_cb)(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms);
typedef X_Boolean (*get_byte)(uint8_t *p_data);
typedef X_Void (*passive_evt_interface)(uint32_t ms , uint8_t is_new_data,uint8_t cur_data);

/*
typedef X_Boolean (*send_buf)(const uint8_t *p_buf,uint16_t length);
typedef X_Void    (*clear_recv_cache)(X_Void);
typedef X_Boolean (*does_busy)(X_Void);
*/

typedef enum
{
    CP_idle = 0, // init the basic parameter , software init 
    CP_handle,
    CP_event,
    /////////////// CP_nobackup,
}e_cp_state;

///////////////#pragma pack(1)
typedef struct 
{
   e_cp_state cur_state;
   ///////////////// e_cp_state state_backup;
}s_cp_top_state ;

typedef struct 
{
	uint16_t send_length;
    uint32_t  rec_time_limit_in_ms;
	cp_send_rec_cb send_cb;
	cp_send_rec_cb recv_cb;
}s_cp_send;

typedef struct 
{
    s_T_taskBaseParam base[CP_SEND_HSM_DEEPTH + 1];   
    uint8_t          is_new_send_request;
    uint8_t           * const p_send_data;
    uint16_t          send_length;
    uint8_t           result;
    uint8_t           is_new_data_come;
    uint8_t           new_data;
}s_for_send_process;

typedef struct 
{
    s_T_taskBaseParam base[CP_RECV_HSM_DEEPTH + 1];   
    uint16_t          const  max_length;
    uint8_t           *const p_buf;
    uint16_t           recv_length;
    uint8_t           is_new_recv;
    uint8_t           is_new_data_come;
    uint8_t           new_data;
}s_for_recv_process;

typedef struct 
{
    s_T_taskBaseParam base[CP_EVENT_HSM_DEEPTH + 1];   
    uint8_t           is_new_data_come;
    uint8_t           new_data;
}s_for_active_event_process;

typedef struct 
{
   uint16_t      event_id;
   hsm_task_func event;
}s_active_event_param;


typedef struct 
{
    uint8_t                     *p_is_init_ok;
    uint8_t                     *p_is_freeze;                   
    uint8_t                     event_deepth_backup ; 
    uint8_t                     send_deepth_backup ; 
    uint8_t                     recv_deepth_backup ; 
    hsm_task_func               init;
    hsm_task_func               recv;
    hsm_task_func               send;
      
    s_for_active_event_process  * p_event_param;

    uint8_t                     * p_is_send_processing;      
    s_for_send_process          * p_send_param;
   
    s_for_recv_process          * p_recv_param;
    get_byte                    get_byte_func;
    uint16_t                    query_freq_reload;
    uint16_t                    *p_query_freq;
    /*~~~~~~~~~~~~~~~~~~~~~~for send process ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    const sListManager 	        *p_send_list;
    uint16_t                    max_send_length;
    uint16_t                    send_cache_num;
    uint8_t                     *p_send_buf;    
    s_cp_send                   *p_send_addition;
    s_cp_send                   *p_cur_send;
    uint8_t                     *p_is_send_again;
    uint8_t                     *p_is_wait_response;
    uint32_t                    *p_wait_ms_backup;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~for recv process~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    const sListManager 	        *p_recv_list;
    uint16_t                    recv_cache_num;
    uint8_t                     *p_recv_buf;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~for event process ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    s_active_event_param        const *event_table;
    uint16_t                    *p_cur_event_index;
    uint16_t                    event_num;
    passive_evt_interface       pass_event;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    s_cp_top_state              * p_state;
}s_cp_manager;

///////////////#pragma pack()
// 
typedef X_Void (*cp_addition_op)(const s_cp_manager * p_manager);

#define __CP_BASE_PARAM_EXPAND_0(__id)    {{0,0,{0},{0},{0},{0},{0},{0},0,}, (s_T_taskBaseParam*)0,},
#define __CP_BASE_PARAM_EXPAND_1(__id)    __CP_BASE_PARAM_EXPAND_0(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[0],},
#define __CP_BASE_PARAM_EXPAND_2(__id)    __CP_BASE_PARAM_EXPAND_1(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[1],},
#define __CP_BASE_PARAM_EXPAND_3(__id)    __CP_BASE_PARAM_EXPAND_2(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[2],},
#define __CP_BASE_PARAM_EXPAND_4(__id)    __CP_BASE_PARAM_EXPAND_3(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[3],},
#define __CP_BASE_PARAM_EXPAND_5(__id)    __CP_BASE_PARAM_EXPAND_4(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[4],},
#define __CP_BASE_PARAM_EXPAND_6(__id)    __CP_BASE_PARAM_EXPAND_5(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[5],},
#define __CP_BASE_PARAM_EXPAND_7(__id)    __CP_BASE_PARAM_EXPAND_6(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[6],},
#define __CP_BASE_PARAM_EXPAND_8(__id)    __CP_BASE_PARAM_EXPAND_7(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[7],},
#define __CP_BASE_PARAM_EXPAND_9(__id)    __CP_BASE_PARAM_EXPAND_8(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[8],},
#define __CP_BASE_PARAM_EXPAND_10(__id)   __CP_BASE_PARAM_EXPAND_9(__id) {{0,0,{0},{0},{0},{0},{0},{0},0,}, &__id.base[9],},

#define  __CP_BASE_PARAM_EXPAND(__id,__n)     CONCAT_2(__CP_BASE_PARAM_EXPAND_,__n)(__id)
#define CP_BASE_PARAM_EXPAND(__id,__n)        __CP_BASE_PARAM_EXPAND(__id,__n) 

#define __P_THIS_FROM_BASE_EXTERN_0       
#define __P_THIS_FROM_BASE_EXTERN_1       ->p_level_next __P_THIS_FROM_BASE_EXTERN_0
#define __P_THIS_FROM_BASE_EXTERN_2       ->p_level_next __P_THIS_FROM_BASE_EXTERN_1
#define __P_THIS_FROM_BASE_EXTERN_3       ->p_level_next __P_THIS_FROM_BASE_EXTERN_2
#define __P_THIS_FROM_BASE_EXTERN_4       ->p_level_next __P_THIS_FROM_BASE_EXTERN_3
#define __P_THIS_FROM_BASE_EXTERN_5       ->p_level_next __P_THIS_FROM_BASE_EXTERN_4
#define __P_THIS_FROM_BASE_EXTERN_6       ->p_level_next __P_THIS_FROM_BASE_EXTERN_5
#define __P_THIS_FROM_BASE_EXTERN_7       ->p_level_next __P_THIS_FROM_BASE_EXTERN_6
#define __P_THIS_FROM_BASE_EXTERN_8       ->p_level_next __P_THIS_FROM_BASE_EXTERN_7
#define __P_THIS_FROM_BASE_EXTERN_9       ->p_level_next __P_THIS_FROM_BASE_EXTERN_8
#define __P_THIS_FROM_BASE_EXTERN_10      ->p_level_next __P_THIS_FROM_BASE_EXTERN_9

#define __P_THIS_FROM_BASE_EXTERN(__deepth)   CONCAT_2(__P_THIS_FROM_BASE_EXTERN_,__deepth)
#define P_THIS_FROM_BASE_EXTERN(__deepth)   __P_THIS_FROM_BASE_EXTERN(__deepth)
#define CP_PTHIS_FROM_BASE_EXTERN(__ext_struct_name,__deepth)        __ext_struct_name * p_this = (__ext_struct_name *)p_base P_THIS_FROM_BASE_EXTERN(__deepth)

#define CP_P_THIS_FOR_EVENT(__deepth)    CP_PTHIS_FROM_BASE_EXTERN(s_for_active_event_process,__deepth)   // s_for_init_process * p_this = (s_for_init_process *)p_base P_THIS_FROM_BASE_EXTERN(__deepth)   //CP_PTHIS_FROM_BASE_EXTERN(s_for_init_process,__deepth) 
#define CP_P_THIS_FOR_SEND(__deepth)     CP_PTHIS_FROM_BASE_EXTERN(s_for_send_process,__deepth)  // s_for_send_process * p_this = (s_for_send_process *)p_base P_THIS_FROM_BASE_EXTERN(__deepth)   //CP_PTHIS_FROM_BASE_EXTERN(s_for_send_process,__deepth) 
#define CP_P_THIS_FOR_RECV(__deepth)     CP_PTHIS_FROM_BASE_EXTERN(s_for_recv_process,__deepth)  // s_for_recv_process * p_this = (s_for_recv_process *)p_base P_THIS_FROM_BASE_EXTERN(__deepth)   //CP_PTHIS_FROM_BASE_EXTERN(s_for_recv_process,__deepth) 

#define NO_USER_DEF_EVENT         ((s_active_event_param*)0)
#define NO_USER_DEF_PROCESS       ((hsm_task_func)0)
#define NO_USER_DEF_EVT_PORT      ((passive_evt_interface)0)

#define CP_DRIVE_DEF(id,                      \
                    get_byte_method,  query_freq ,\
                    init_process ,                     \
                    recv_process , recv_cache_num , max_recv_length,   \
                    send_process , send_cache_num , max_send_length,  \
                    user_def_event_table,user_def_event_num,              \
                    passive_event_interface)                           \
                    static uint8_t CONCAT_2(id,_is_init_ok) = 0;                        \
                    static uint8_t CONCAT_2(id,_is_freeze) = 0;                        \
                    static uint8_t CONCAT_2(id,_is_wait_response) = 0;                        \
                    static uint32_t CONCAT_2(id,_ms_backup) = 0;                        \
                    static uint16_t  CONCAT_2(id,_query_freq) = query_freq;              \
                    static uint16_t  CONCAT_2(id,_cur_event_index) = 0;                     \
                    static s_cp_top_state CONCAT_2(id,_top_state) = {CP_idle};              \
                    static uint8_t CONCAT_2(id,_temp_send_buf)[max_send_length];                            \
                    static s_cp_send CONCAT_2(id,_cur_send_addition) = {0,0,(cp_send_rec_cb)0,(cp_send_rec_cb)0};   \
                    static uint8_t CONCAT_2(id,_temp_recv_buf)[max_recv_length];                            \
                    static s_for_active_event_process CONCAT_2(id,_event_param) = {                       \
                                    {                           \
                                        CP_BASE_PARAM_EXPAND(CONCAT_2(id,_event_param),CP_EVENT_HSM_DEEPTH)       \
                                    },                                             \
                                    0,0                                                 \
                                };                                                           \
                    static uint8_t CONCAT_2(id,_is_send_processing) = 0,CONCAT_2(id,_is_send_again) = 0;         \
                    static s_for_send_process CONCAT_2(id,_send_param) = {               \
                                    {                       \
                                       CP_BASE_PARAM_EXPAND(CONCAT_2(id,_send_param),CP_SEND_HSM_DEEPTH)       \
                                    }, 0,                                            \
                                    CONCAT_2(id,_temp_send_buf),0,                    \
                                    0,0,0,                                   \
                                };        \
                    static s_for_recv_process CONCAT_2(id,_recv_param) = {                   \
                                    {                           \
                                        CP_BASE_PARAM_EXPAND(CONCAT_2(id,_recv_param),CP_RECV_HSM_DEEPTH)       \
                                    },                               \
                                    max_recv_length,                                 \
                                    CONCAT_2(id,_temp_recv_buf),   \
                                    0,0,0,0,                                   \
                                };        \
                    APP_LOOPQUEUE_DEF_WITHOUT_POINTER(CONCAT_2(id,_s_list),send_cache_num);      \
                    static uint8_t CONCAT_2(id,_send_array)[send_cache_num][max_send_length];          \
                    static s_cp_send CONCAT_2(id,_send_addition_array)[send_cache_num];   \
                    APP_LOOPQUEUE_DEF_WITHOUT_POINTER(CONCAT_2(id,_r_list),recv_cache_num);      \
                    static uint8_t CONCAT_2(id,_recv_array)[recv_cache_num][max_recv_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE];  \
                    static const s_cp_manager CONCAT_2(id,_cp_entry) = {                            \
                        &CONCAT_2(id,_is_init_ok),                                                \
                        &CONCAT_2(id,_is_freeze),                                           \
                        CP_EVENT_HSM_DEEPTH, CP_SEND_HSM_DEEPTH , CP_RECV_HSM_DEEPTH ,   \
                        init_process,recv_process,send_process,                     \
                        &CONCAT_2(id,_event_param) ,                                         \
                        &CONCAT_2(id,_is_send_processing),    &CONCAT_2(id,_send_param) ,  \
                        &CONCAT_2(id,_recv_param),                                        \
                        get_byte_method, query_freq,&CONCAT_2(id,_query_freq),            \
                        &CONCAT_2(id,_s_list_loopqueue_entry),  max_send_length, send_cache_num, \
                                        &CONCAT_2(id,_send_array)[0][0], &CONCAT_2(id,_send_addition_array)[0], &CONCAT_2(id,_cur_send_addition),      \
                                        &CONCAT_2(id,_is_send_again),&CONCAT_2(id,_is_wait_response),&CONCAT_2(id,_ms_backup), \
                        &CONCAT_2(id,_r_list_loopqueue_entry), recv_cache_num, &CONCAT_2(id,_recv_array)[0][0],      \
                        user_def_event_table,                                                               \
                        &CONCAT_2(id,_cur_event_index),                                                  \
                        user_def_event_num,                                                             \
                        passive_event_interface,                                                     \
                        &CONCAT_2(id,_top_state) ,                                                 \
                    };                                                             \
                    static const s_cp_manager * id = &CONCAT_2(id,_cp_entry)


X_Boolean cp_dri_init(const s_cp_manager * p_manager,cp_addition_op addition_init);
X_Boolean cp_dri_reset(const s_cp_manager * p_manager,cp_addition_op addition_init);
X_Void cp_dri_handle(const s_cp_manager * p_manager,uint32_t ms);
X_Void cp_dri_freeze(const s_cp_manager * p_manager);
X_Void cp_dri_unfreeze(const s_cp_manager * p_manager);

X_Void cp_SendAgain(const s_cp_manager *p_manager);
X_Boolean cp_SendRequest(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length);
X_Boolean cp_SendRequestWithCallBack(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb);
X_Boolean cp_SendRequestWithResponse(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb
																							,uint32_t rec_time_in_ms,cp_send_rec_cb p_recv_cb);
X_Boolean cp_DoesNewDataCome(const s_cp_manager *p_manager,uint8_t **pp_data,uint16_t *p_length,uint32_t *p_ms);


X_Boolean cp_active_event(const s_cp_manager *p_manager,uint16_t event_id); 

#ifdef __cplusplus
		}
#endif

#endif

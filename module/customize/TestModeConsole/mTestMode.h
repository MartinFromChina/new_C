#ifndef __MTESTMODE_H
#define __MTESTMODE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/StateMachine/StateMachine.h"
#include "../../common/loopqueue/loop_queues.h"

#define MAX_TEST_MODE_OUTPUT_STRING_LENGTH        100
#define MAX_TM_COMMAND_CACHE_NUM                  10
#define MAX_TEST_MODE_COMMAND_LENGTH              80 // TMENTER OVER     £º the command length is 5 : ENTER           
#define TEST_MODE_COMMAND_STRING_EXTERN_LENGTH    7 // TM OVER
#define TEST_MODE_RECV_EXTERN_LENGTH              3 // 1 for recv state ,2 for data length
#define TEST_MODE_CMD_ROUTINE_END_STRING          "ENDROUTINE"
#define TEST_SPECIAL_HOLD_TIME_INFINITE           0xFFFFFFFF

typedef X_Boolean (*tm_get_byte)(uint8_t *p_data);
typedef X_Boolean (*tm_send_buf)(const uint8_t *p_data,uint16_t length);
typedef X_Void    (*tm_recovery_method)(X_Void);
typedef X_Boolean (*tm_onWait)(X_Void);
typedef X_Boolean (*tm_onError)(X_Void);
typedef X_Void    (*tm_enter_exit_method)(X_Void);
typedef X_Void    (*tm_kill_all_process_method)(X_Void);

X_Void mTM_SendString(tm_send_buf send_method,const char * sFormat, ...);
					
#define TmLogPrintf(flag,message)      do{ 																\
																												if((flag) != 0)								\
																												{																	\
																													mTM_SendString message;	\
																												}																	\
																										}while(0)

#define TEST_MODE_BASIC_LOG_DEBUG                 1

/*****************************************************************************************************************/
StateNumber TM_IdleAction(s_StateMachineParam *p_this);
StateNumber TM_CommandAnalysisAction(s_StateMachineParam *p_this);
StateNumber TM_DoCommandRoutineAction(s_StateMachineParam *p_this);
StateNumber TM_WaitAction(s_StateMachineParam *p_this);
StateNumber TM_FinishAction(s_StateMachineParam *p_this);
StateNumber TM_SpecialCmdFinishAction(s_StateMachineParam *p_this);

typedef enum
{
	TM_Idle = 0,
	TM_CommandAnalysis,
	TM_DoCommandRoutine,
	TM_Wait,
	TM_Finish,
	TM_SpecialCmdFinish,
}eTestModeState;

typedef struct
{
  uint8_t     * const     p_rec_buf;
	const uint16_t          max_cache_num;
	const uint16_t          max_recv_length;
	const uint16_t          max_command_length;
  const sListManager *    p_queue;
	const tm_send_buf       send_buf_method;
	const s_StateMachine     *p_state_machine;
	s_StateMachineParam      *p_state_param;
	X_Boolean               isHeaderFind;
	uint16_t cur_push_num;
	uint16_t cur_pop_num;
	uint8_t header_index;
	uint8_t data_index;
	uint8_t temp_rec_buf[MAX_TEST_MODE_COMMAND_LENGTH + TEST_MODE_COMMAND_STRING_EXTERN_LENGTH + TEST_MODE_RECV_EXTERN_LENGTH]; 
}sTestModeRecvParam;

typedef struct
{
	X_Boolean isSpecialModeOnly;
	char *command_string;
	uint8_t length; 
	uint8_t compare_length;
	StateNumber (*todo)(s_StateMachineParam *p_base);
}sTmCommandAnalysis;

typedef struct
{
	s_StateMachineParam      base;
  const tm_send_buf        send_buf_method;
	const uint16_t           handle_called_freq_in_ms;
	X_Boolean                isDotPrintDisable;
	uint8_t                  temp_command_buf[MAX_TEST_MODE_COMMAND_LENGTH];
	uint16_t                 temp_command_length;
	uint32_t                 wait_counter;
	tm_recovery_method       recovery;
	tm_onWait                onWait;
	tm_onError               onError;
	uint8_t                  failed_cnt;
	uint8_t                  successed_cnt;
}sTestModeParamExternForUser;

typedef struct
{
	s_StateMachineParam      base;
  const tm_send_buf        send_buf_method;
	const uint16_t           handle_called_freq_in_ms;
	X_Boolean                isDotPrintDisable;
	uint8_t                  temp_command_buf[MAX_TEST_MODE_COMMAND_LENGTH];
	uint16_t                 temp_command_length;
	uint32_t                 wait_counter;
	tm_recovery_method       recovery;
	tm_onWait                onWait;
	tm_onError               onError;
	uint8_t                  failed_cnt;
	uint8_t                  successed_cnt;
	
	const sTmCommandAnalysis *p_command_table;
	const uint16_t           command_table_size;
	const tm_get_byte         get_byte_method;
	const tm_enter_exit_method enter_method;
	const tm_enter_exit_method exit_method;
	const tm_kill_all_process_method       kill_all_method;
	const uint32_t           test_mode_hold_time_in_ms;
	X_Boolean                isNewCommandCome;
	X_Boolean                isInSpecialMode;
	uint32_t                 test_mode_hold_cnt;
	uint16_t	                 routine_cnt;
	uint16_t                  dot_cnt;
	
}sTestModeParamExtern;

typedef struct
{
	X_Boolean      *const    p_isInitOK;
	const uint16_t           handle_called_freq_in_ms;
  const tm_get_byte         get_byte_method;
  const tm_send_buf         send_buf_method;
	s_StateMachineParam      *p_state_param;
	const s_StateMachine     *p_state_machine;
  sTestModeRecvParam *const p_recv;
}sTestModeParam;

#define APP_TEST_MODE_MODULE_DEFINE(p_tm,handle_freq_in_ms                  \
                                    ,input_interface,output_interface                        \
                                    ,command_table_addr,table_size                           \
                                    ,max_cmd_length,max_cmd_cache_num                        \
                                    ,test_mode_enter_cb,test_mode_exit_cb,max_test_mode_hold_time_in_ms \
                                    ,kill_all_cb)   \
         static X_Boolean            CONCAT_2(p_tm,_is_init_ok) = X_False;                      \
         static uint8_t             CONCAT_2(p_tm,_rec_buf_array)[max_cmd_cache_num][max_cmd_length + 2];  \
																																												\
         static sTestModeParamExtern CONCAT_2(p_tm,_sTMPE) = {										\
																							{DEFAULT_STATE_NUMBER},															\
																							output_interface,														\
																							handle_freq_in_ms,													\
																							X_False,{0},0,0,																						\
																							(tm_recovery_method)0,															\
																							(tm_onWait)0,																				\
																							(tm_onError)0,																\
																							0,																			\
																							0,																	\
																							command_table_addr,														\
																							table_size,																	\
																							input_interface,															\
																							test_mode_enter_cb,													\
																							test_mode_exit_cb,													\
																							kill_all_cb,																\
																							max_test_mode_hold_time_in_ms,							\
																							X_False,																	\
																							X_False,																\
																							0,0,0,																\
																						};																					\
         static const StateAction CONCAT_2(p_tm,_TestModeStateAction)[] = {						\
																					{TM_IdleAction},																		\
																					{TM_CommandAnalysisAction},																		\
																					{TM_DoCommandRoutineAction},																		\
																					{TM_WaitAction},																\
																					{TM_FinishAction},																\
																					{TM_SpecialCmdFinishAction},											\
																					};                                                 \
         APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(p_tm,_test_mode_state),																				\
							sizeof(CONCAT_2(p_tm,_TestModeStateAction))/sizeof(CONCAT_2(p_tm,_TestModeStateAction)[0]),		\
							&CONCAT_2(p_tm,_TestModeStateAction)[0]);							\
																																								\
					APP_LOOPQUEUE_DEF_WITHOUT_POINTER(CONCAT_2(p_tm,_queue),max_cmd_cache_num);                                      \
					static  sTestModeRecvParam   CONCAT_2(p_tm,_recv_param) = {                              \
							&CONCAT_2(p_tm,_rec_buf_array)[0][0],                                                        \
							max_cmd_cache_num,                                                              \
							(max_cmd_length + TEST_MODE_COMMAND_STRING_EXTERN_LENGTH + TEST_MODE_RECV_EXTERN_LENGTH),                       \
							max_cmd_length,                                                                     \
							&CONCAT_2(p_tm,_queue_loopqueue_entry),                                           \
							output_interface,                                                            \
							&CONCAT_2(p_tm,_test_mode_state_entry),                                    \
							&(CONCAT_2(p_tm,_sTMPE).base),                                         \
							X_False,0,0,0,0,{0},                                                            \
				 };                                                                              \
         static const sTestModeParam CONCAT_2(p_tm,_test_mode_entry) = {                                        \
                                     &CONCAT_2(p_tm,_is_init_ok),            \
                                     handle_freq_in_ms,                    \
                                     input_interface,                  \
                                     output_interface,                 \
                                     &(CONCAT_2(p_tm,_sTMPE).base),            \
                                     &CONCAT_2(p_tm,_test_mode_state_entry),    \
                                     &CONCAT_2(p_tm,_recv_param),          \
                                         };                                      \
         static const sTestModeParam* p_tm = &CONCAT_2(p_tm,_test_mode_entry)

X_Void mTestModeInit(const sTestModeParam *p_tm);
X_Void mTestModeLoopReceive(const sTestModeParam *p_tm);
X_Void mTestModeHandle(const sTestModeParam *p_tm);

StateNumber TM_RUNALL_todo(s_StateMachineParam *p_base);
StateNumber TM_ENTER_todo(s_StateMachineParam *p_base);
StateNumber TM_EXIT_todo(s_StateMachineParam *p_base);

#ifdef __cplusplus
		}
#endif

#endif

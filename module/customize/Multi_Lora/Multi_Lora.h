#ifndef __MULTI_LORA_H
#define __MULTI_LORA_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "Multi_Lora_func.h"
		
StateNumber LORA_IdleAction(s_StateMachineParam *p_this);
StateNumber LORA_HardWareSelfCheckAction(s_StateMachineParam *p_this);
StateNumber LORA_NormalProcessAction(s_StateMachineParam *p_this);
StateNumber LORA_ErrorAction(s_StateMachineParam *p_this);
StateNumber LORA_Send_Two_Dummy_FrameAction(s_StateMachineParam *p_this);
StateNumber LORA_GetRssiAction(s_StateMachineParam *p_this);
        
typedef struct
{
	s_StateMachineParam 		base;               /*状态机的通用的基本参数（保存了当前状态编号）*/
	const sLoraFuncManager *p_lora_func_manager;
	evt_reg                 event_report;           /*报告状态机运行状态和异常事件的回调函数指针*/
	normal_lora_action      p_fatal_report;
    get_rssi_cb             rssi_report;
	X_Boolean               isBusy;
	X_Boolean               isUseDefaultConfiguration;
    X_Boolean               isGetRssiOnce;
	sLoraConfig             config;
}sLoraParamExtern;

typedef struct
{
	X_Boolean * const p_isInitOK;
	X_Boolean * const p_isModuleLocked;
	normal_lora_action      p_func_pwr_on;
	normal_lora_action      p_func_pwr_off;
    sLoraParamExtern *const p_lora_param;
	const s_StateMachine* lora_manager;
	const sLoraFuncManager *p_lora_func_manager;
}sLoraModuleManager;

#define LORA_MODULE_DEFINE(id,p_func_set_mode,p_func_get_byte,p_func_send_buf,p_func_clear_recv_cache,    \
							p_func_get_time,p_func_set_time,p_func_does_busy,module_pwr_on,module_pwr_off,   \
							max_send_queue_number,max_recv_queue_number)          \
			static X_Boolean CONCAT_2(id,isModuleInitOK) = X_False;                \
			static X_Boolean CONCAT_2(id,isModuleLocked) = X_False;                \
			LORA_FUNC_DEFINE(id,p_func_set_mode,p_func_get_byte,p_func_send_buf,p_func_clear_recv_cache,    \
							    p_func_get_time,p_func_set_time,p_func_does_busy,       \
								max_send_queue_number,max_recv_queue_number);                       \
			static sLoraParamExtern CONCAT_2(id,LPE) = {                          \
								{DEFAULT_STATE_NUMBER},                          \
								&CONCAT_2(id,lora_func_entry),                     \
								(evt_reg)0,  (normal_lora_action)0,(get_rssi_cb)0,           \
								X_True,X_True,X_False,                                     \
								LORA_CONFIG_DEFAULT_PARAM,                      \
							};                 \
			static const StateAction CONCAT_2(id,LORA_StateAction)[] = {            \
								{LORA_IdleAction},                            \
								{LORA_HardWareSelfCheckAction},                  \
								{LORA_NormalProcessAction},                     \
								{LORA_ErrorAction},                                \
								{LORA_Send_Two_Dummy_FrameAction},               \
                                {LORA_GetRssiAction},                              \
							};                                                      \
			APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_state),                                         \
							sizeof(CONCAT_2(id,LORA_StateAction))/sizeof(CONCAT_2(id,LORA_StateAction)[0]),        \
							&CONCAT_2(id,LORA_StateAction)[0]);                                              \
			static const sLoraModuleManager CONCAT_2(id,lora_module) = {                            \
								&CONCAT_2(id,isModuleInitOK),                                     \
								&CONCAT_2(id,isModuleLocked),                           \
								module_pwr_on,module_pwr_off,                            \
								&CONCAT_2(id,LPE),                                        \
								&CONCAT_2(id,p_state_entry),                                  \
								&CONCAT_2(id,lora_func_entry),                               \
							};                                                                              \
			static const sLoraModuleManager * id = &CONCAT_2(id,lora_module)
						
						
uint8_t Multi_Lora_GetNormalStateNumber(X_Void);

X_Boolean Multi_Lora_Init(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_Handle(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_Reset(sLoraConfig * p_config,const sLoraModuleManager *p_manager);

X_Void Multi_Lora_Lock(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_Unlock(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_EventRegister(const sLoraModuleManager *p_manager,evt_reg reg);

X_Void Multi_Lora_EventClear(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_SetNormalStateRequest(const sLoraModuleManager *p_manager);

X_Boolean Multi_Lora_SendRequest(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length);

X_Boolean Multi_Lora_SendRequestWithCallBack(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb);
X_Boolean Multi_Lora_SendRequestWithResponse(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb
																							,uint16_t rec_time_in_ms,cp_send_rec_cb p_recv_cb);

X_Boolean Multi_Lora_DoesNewDataCome(const sLoraModuleManager *p_manager,uint8_t **pp_data,uint16_t *p_length);
X_Boolean Multi_Lora_DoesBusy(const sLoraModuleManager *p_manager);

X_Void Multi_Lora_FatalErrorReporterReg(const sLoraModuleManager *p_manager,X_Void (*fatal_err_report)(X_Void));

// 使用该函数前，要在LORA配置阶段开启“环境噪声使能” (LORA_DEFAULT_RSSI_FUNCTION != 0)
X_Void Multi_Lora_GetRssiRequest(const sLoraModuleManager *p_manager,get_rssi_cb rssi_cb);

#ifdef __cplusplus
		}
#endif

#endif

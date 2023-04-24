#ifndef __MULTI_LORA_FUNC_H
#define __MULTI_LORA_FUNC_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "LogDebugSwitch.h"
#include "../../../common/StateMachine/StateMachine.h"
#include "../../../common/loopqueue/loop_queues.h"
#include "../../../customize/CommuPeripheralDrive/cp_drive.h"
        
////////#include <string.h>

#ifndef MAX_LORA_RECEIVE_FRAME_TO_CACHE
	#define MAX_LORA_RECEIVE_FRAME_TO_CACHE        20
#endif

#ifndef MAX_LORA_SEND_REQUEST_TO_CACHE
	#define MAX_LORA_SEND_REQUEST_TO_CACHE         20
#endif

#ifndef USE_RSSI_BYTE
	#define USE_RSSI_BYTE                 0     // 		据某位技术支持说开启这个会增加延迟 ,就默认不开启吧	
#endif

#ifndef LORA_CONFIG_DEBUG_METHOD		
	#define LORA_CONFIG_DEBUG_METHOD          SysLogDebug
#endif

#define MAX_LORA_SEND_DATA_LENGTH     200 // 	想要达到缓存容量的400，需要手动分包加延时，索性不用这个鸡肋的缓存功能了
#define MAX_LORA_RECV_DATA_LENGTH     200

#define LORA_READ_CMD                 0xc1
		
#define LORA_LENGTH_SIZE_IN_BYTE             2
#define MAX_LORA_CONFIG_DATA_LENGTH          11 // header + reg_value : 3 + 8 = 11;

#define RSSI_RESPONSE_LENGTH                 5  // send : 0xc0,0xc1,0xc2,0xc3,0,2 ; respnse : c1 00 02 rssi_env rssi_recv

typedef enum
{
	LM_Trans = 0,
	LM_WOR_Send,
	LM_WOR_Recv,
	LM_Sleep, // also config mode 
}eLoraMode;

typedef enum
{
	LP_InProcess,
	LP_Finished,
	LP_Error,
}eLoraProcess;


typedef X_Void (*get_rssi_cb)(uint16_t env_rssi,uint16_t recv_rssi); // 虽然手册中RSSI是8bits,但是我还是留了16bits以防万一
typedef X_Boolean (*set_work_mode)(uint8_t mode);
typedef X_Boolean (*get_byte)(uint8_t *p_data);
typedef X_Boolean (*send_buf)(const uint8_t *p_buf,uint16_t length);
typedef X_Void    (*clear_recv_cache)(X_Void);
typedef uint32_t  (*get_time)(X_Void);
typedef X_Boolean (*set_time)(uint32_t time_ms);
typedef X_Boolean (*event_handler)(e_cp_SendRecEvent evt,uint8_t *p_data,uint16_t length);
typedef X_Boolean (*does_aux_busy)(X_Void);


typedef X_Void (*normal_lora_action)(X_Void);
typedef X_Boolean (*lora_config_event_handler)(e_cp_SendRecEvent evt,uint8_t *p_data,uint16_t length,uint8_t *p_cmd,uint16_t cmd_length);
/********************************lora config ***********************************************/

typedef enum
{
	Cb_8N1 = 0,
	Cb_801,
	Cb_8E1,
}eCheckBits;

typedef enum
{
	AS_2_4k = 0,
	AS_4_8k,
	AS_9_6k,
	AS_19_2k,
	AS_38_4k,
	AS_62_5k,
}eAirSpeed;

typedef enum
{
	TP_22dbm = 0,
	TP_17dbm,
	TP_13dbm,
	TP_10dbm,
}eTransPower;

typedef enum
{
	TM_transparent = 0,
	TM_p2p,
}eTransMode;

typedef struct
{
	uint16_t       module_addr;
	uint32_t       baud_rate;
	eCheckBits     check_bits;
	eAirSpeed      air_speed;
	uint16_t       subpackage_length;
	X_Boolean      isRssiEnable;
	eTransPower    trans_power;
	uint8_t        channel;
	X_Boolean      isRssiByteEnable;
	eTransMode     trans_mode;
	X_Boolean      isLBT_Enable; // listen before talk;
	uint16_t       WOR_period_ms;
	uint16_t       secret_key; 
}sLoraConfig;

#define LORA_DEFAULT_ADDR                (0x0001)
#define LORA_DEFAULT_BAUD_RATE           9600
#define LORA_DEFAULT_CHECK_BITES         Cb_8N1
#define LORA_DEFAULT_AIR_SPEED           AS_2_4k
#define LORA_DEFAULT_SUBPACKAGE_LENGTH   200
#define LORA_DEFAULT_RSSI_FUNCTION       X_False
#define LORA_DEFAULT_TRANS_POWER         TP_22dbm
#define LORA_DEFAULT_CHANNEL             23
#define LORA_DEFAULT_RSSI_BYTE_REPORT    (USE_RSSI_BYTE != 0)
#define LORA_DEFAULT_TRANS_MODE          TM_p2p
#define LORA_DEFAULT_LBT_FUNCTION        X_False
#define LORA_DEFAULT_WOR_PERIOD          2000
#define LORA_DEFAULT_SECRET_KEY          (0x0000)

#define LORA_CONFIG_DEFAULT_PARAM							{	LORA_DEFAULT_ADDR, \
																LORA_DEFAULT_BAUD_RATE, \
																LORA_DEFAULT_CHECK_BITES, \
																LORA_DEFAULT_AIR_SPEED, \
																LORA_DEFAULT_SUBPACKAGE_LENGTH, \
																LORA_DEFAULT_RSSI_FUNCTION, \
																LORA_DEFAULT_TRANS_POWER, \
																LORA_DEFAULT_CHANNEL, \
																LORA_DEFAULT_RSSI_BYTE_REPORT, \
																LORA_DEFAULT_TRANS_MODE, \
																LORA_DEFAULT_LBT_FUNCTION, \
																LORA_DEFAULT_WOR_PERIOD, \
																LORA_DEFAULT_SECRET_KEY,}
/********************************************************************************/

StateNumber	LC_IdleAction(s_StateMachineParam *p_this);
StateNumber	LC_WaitAction(s_StateMachineParam *p_this);																				
StateNumber	LC_SetModeAction(s_StateMachineParam *p_this);
StateNumber	LC_SendCMD_Action(s_StateMachineParam *p_this);
StateNumber	LC_GetResponseAction(s_StateMachineParam *p_this);
StateNumber	LC_FinishedAction(s_StateMachineParam *p_this);
StateNumber	LC_ErrorAction(s_StateMachineParam *p_this);
StateNumber	LC_GetConfigAction(s_StateMachineParam *p_this);																							
StateNumber	LC_CompareAction(s_StateMachineParam *p_this);
StateNumber LC_AuxFreeIntervalAction(s_StateMachineParam *p_this);
                                                                
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/																								
StateNumber SRS_IdleAction(s_StateMachineParam *p_this);
StateNumber	SRS_NormalAction(s_StateMachineParam *p_this);
StateNumber	SRS_SendDataAction(s_StateMachineParam *p_this);
StateNumber	SRS_GetDataAction(s_StateMachineParam *p_this);
StateNumber SRS_ResetAction(s_StateMachineParam *p_this);																								
StateNumber SRS_SendIntervalAction(s_StateMachineParam *p_this);
StateNumber SRS_ImmediatelySendAction(s_StateMachineParam *p_this);
//////////////StateNumber SRS_ImmediatelyRecvAction(s_StateMachineParam *p_this);
      
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/	
StateNumber LGR_IdleAction(s_StateMachineParam *p_this);
StateNumber LGR_WaitAction(s_StateMachineParam *p_this);
StateNumber LGR_SendCMDAction(s_StateMachineParam *p_this);
StateNumber LGR_GetResponseAction(s_StateMachineParam *p_this);
StateNumber LGR_FinishedAction(s_StateMachineParam *p_this);
StateNumber LGR_ErrorAction(s_StateMachineParam *p_this);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/	

typedef struct _sLoraFuncManager s_LoraFuncManager;

typedef struct
{
	s_StateMachineParam 		base;
	const set_work_mode         set_mode_func;
	const get_byte              get_byte_func;
	const send_buf              send_buf_func;
	const clear_recv_cache      clear_rec_cache_func;
	const get_time              get_time_func;
	const set_time              set_time_func;
	const does_aux_busy         does_busy_func;
	lora_config_event_handler   evt_handle;
	eLoraProcess 				error_code;
	uint8_t 					data[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t 					length;
	uint8_t 					retry_times;
	StateNumber					state_backup;
	uint8_t recv_data[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t recv_length;
}sLoraConfigParamExtern;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`*/
typedef struct
{
	uint8_t  cur_send_buf[MAX_LORA_SEND_DATA_LENGTH];
	uint16_t length;
	cp_send_rec_cb send_cb;
}sLoraSend;

typedef struct
{
	uint8_t  cur_recv_buf[MAX_LORA_RECV_DATA_LENGTH];
	uint16_t recv_length;
	uint16_t  rec_time_limit_in_ms;
	cp_send_rec_cb recv_cb;
}sLoraRecv;

typedef struct _sLoraSendRequest
{
	uint8_t send_buf[MAX_LORA_SEND_DATA_LENGTH];
	uint16_t send_length;
	cp_send_rec_cb send_cb;
	sLoraRecv recv;
}sLoraSendRequest;

typedef struct
{
	s_StateMachineParam 	   base;
	const s_StateMachine* 	   lora_sr_manager;
	const set_work_mode        set_mode_func;
	const get_byte             get_byte_func;
	const send_buf             send_buf_func;
	const clear_recv_cache     clear_rec_cache_func;
	const get_time             get_time_func;
	const set_time             set_time_func;
	const does_aux_busy        does_busy_func;
	const uint16_t             send_cache_size;
	const uint16_t             recv_cache_size;
	const sListManager    *    p_send_queue;
	const sListManager    *    p_recv_queue;
	sLoraSendRequest  * const p_send_param;
	uint8_t  *   const         p_recv_array_buf;
	sLoraSend                 cur_send;
	sLoraRecv                 cur_recv;
	X_Boolean  				  isContinueWaitRecWhenSendFailed;
	X_Boolean  				  isScanInProcess;
	X_Boolean				  isBusy;
	normal_lora_action        reset_action;
	X_Boolean                 isAuxBusyBackup;
	X_Boolean                 isAuxBusy;
	uint32_t                  timer_backup;
}sSendRecvParamExtern;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`*/
#define ERROR_LORA_RSSI_FLAG      0xFFFF   
typedef struct
{
    s_StateMachineParam 	base;
    const set_work_mode         set_mode_func;
	const get_byte              get_byte_func;
	const send_buf              send_buf_func;
	const clear_recv_cache      clear_rec_cache_func;
	const get_time              get_time_func;
	const set_time              set_time_func;
	const does_aux_busy         does_busy_func;
    uint16_t                cur_env_rssi;
    uint16_t                cur_recv_rssi;
    uint8_t                 recv_data[RSSI_RESPONSE_LENGTH];
    uint8_t                 recv_length;
    eLoraProcess 			error_code;
}sLoraRssiParamExtern;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`*/
typedef struct _sLoraFuncManager
{
	X_Boolean *p_isInitOK;
    sLoraConfigParamExtern * const lora_config_param;
	const s_StateMachine* lora_config_manager;
    sSendRecvParamExtern * const lora_sr_param;
	const s_StateMachine* lora_sr_manager;
    sLoraRssiParamExtern * const lora_rssi_param;
    const s_StateMachine* lora_rssi_manager;
}sLoraFuncManager;

#define LORA_FUNC_DEFINE(id,p_func_set_mode,p_func_get_byte,p_func_send_buf,p_func_clear_recv_cache,    \
							p_func_get_time,p_func_set_time,p_func_does_busy,             \
							max_send_cache_number,max_recv_cache_number)      	                   \
					static X_Boolean CONCAT_2(id,isLoraFuncInitOK) = X_False;                         \
					static sLoraSendRequest CONCAT_2(id,lora_send_param)[max_send_cache_number];          \
					static uint8_t  CONCAT_2(id,lora_recv_buf)[max_recv_cache_number*(MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE)];   \
					APP_LOOPQUEUE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_lora_queue_send),max_send_cache_number);       \
					APP_LOOPQUEUE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_lora_queue_recv),max_send_cache_number);       \
					static sLoraConfigParamExtern CONCAT_2(id,LCPE) = {                             \
									{DEFAULT_STATE_NUMBER},							\
									p_func_set_mode,									\
									p_func_get_byte,                                  \
									p_func_send_buf,                                  \
									p_func_clear_recv_cache,                          \
									p_func_get_time,                               \
									p_func_set_time,                               \
									p_func_does_busy,                               \
									(lora_config_event_handler)0,                                  \
									LP_InProcess,                                    \
									{0},0,0,                                            \
									DEFAULT_STATE_NUMBER,                               \
									{0},0                                              \
							};                                                       \
					static const StateAction  CONCAT_2(id,LoraConfigAction)[] = {  	\
											{LC_IdleAction},						\
											{LC_WaitAction},						\
											{LC_SetModeAction},						\
											{LC_SendCMD_Action},					\
											{LC_GetResponseAction},					\
											{LC_FinishedAction},					\
											{LC_ErrorAction},						\
											{LC_GetConfigAction},					      \
											{LC_CompareAction},						           \
                                            {LC_AuxFreeIntervalAction},                         \
										};												\
                    APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_lora_config),                       \
							sizeof(CONCAT_2(id,LoraConfigAction))/sizeof(CONCAT_2(id,LoraConfigAction)[0]),    \
							&CONCAT_2(id,LoraConfigAction)[0]);                                       \
					static const StateAction CONCAT_2(id,SendRecvAction)[] = {                    \
											{SRS_IdleAction},                                          \
											{SRS_NormalAction},                                      \
											{SRS_SendDataAction},                                       \
											{SRS_GetDataAction},                                       \
											{SRS_ResetAction},                                           \
											{SRS_SendIntervalAction},                                      \
                                            {SRS_ImmediatelySendAction},                                    \
										};                                                        \
					APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_lora_sr),                                        \
							sizeof(CONCAT_2(id,SendRecvAction))/sizeof(CONCAT_2(id,SendRecvAction)[0]),      \
							&CONCAT_2(id,SendRecvAction)[0]);                                            \
                            static sSendRecvParamExtern CONCAT_2(id,SRPE) = {                        \
											{DEFAULT_STATE_NUMBER},							\
											&CONCAT_2(id,p_lora_sr_entry),             \
											p_func_set_mode,									\
											p_func_get_byte,                                  \
											p_func_send_buf,                                  \
											p_func_clear_recv_cache,                          \
											p_func_get_time,                               \
											p_func_set_time,                               \
											p_func_does_busy,                               \
											max_send_cache_number,max_recv_cache_number,          \
											&CONCAT_2(id,p_lora_queue_send_loopqueue_entry),              \
											&CONCAT_2(id,p_lora_queue_recv_loopqueue_entry),         \
											CONCAT_2(id,lora_send_param),                     \
											CONCAT_2(id,lora_recv_buf),                          \
											{{0},0,(cp_send_rec_cb)0},              \
											{{0},0,0,(cp_send_rec_cb)0},                    \
											X_True,X_False,									\
											X_True,                                      \
											(normal_lora_action)0,                                              \
											X_True, X_True, 0,                                  \
										};                                             \
                    static const StateAction  CONCAT_2(id,LoraGetRssiAction)[] = {  	\
											{LGR_IdleAction},						\
											{LGR_WaitAction},						\
											{LGR_SendCMDAction},						\
											{LGR_GetResponseAction},					\
											{LGR_FinishedAction},					\
											{LGR_ErrorAction},					\
										};												\
                    APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(id,p_lora_rssi),                                        \
							sizeof(CONCAT_2(id,LoraGetRssiAction))/sizeof(CONCAT_2(id,LoraGetRssiAction)[0]),      \
							&CONCAT_2(id,LoraGetRssiAction)[0]);                                            \
                    static sLoraRssiParamExtern CONCAT_2(id,RSSIPE) = {                  \
                                            {DEFAULT_STATE_NUMBER},             \
                                            p_func_set_mode,									\
											p_func_get_byte,                                  \
											p_func_send_buf,                                  \
											p_func_clear_recv_cache,                          \
											p_func_get_time,                               \
											p_func_set_time,                               \
											p_func_does_busy,                               \
                                            ERROR_LORA_RSSI_FLAG,ERROR_LORA_RSSI_FLAG,    \
                                            LP_InProcess,                     \
                                        };                   \
                    static const sLoraFuncManager CONCAT_2(id,lora_func_entry) = {                         \
											  &CONCAT_2(id,isLoraFuncInitOK),       \
											  &CONCAT_2(id,LCPE),                   \
											  &CONCAT_2(id,p_lora_config_entry),      \
											  &CONCAT_2(id,SRPE),                     \
											  &CONCAT_2(id,p_lora_sr_entry),             \
                                              &CONCAT_2(id,RSSIPE),                     \
											  &CONCAT_2(id,p_lora_rssi_entry),             \
                                    }                                                

/********************************************************************************/
X_Boolean mFunc_MultiLoraInit(const sLoraFuncManager *p_manager);										
/********************************************************************************/
X_Void mFunc_MultiLoraConfigReset(const sLoraFuncManager *p_manager,sLoraConfig * p_config,uint8_t retry_times,lora_config_event_handler handler);
eLoraProcess mFunc_MultiLoraConfigHandle(const sLoraFuncManager *p_manager);

/********************************************************************************/                                           
eLoraProcess mFunc_MultiLoraRssiHandle(const sLoraFuncManager *p_manager);                                        
X_Void mFunc_MultiLoraGetLatestRssi(const sLoraFuncManager *p_manager,uint16_t *p_env_rssi,uint16_t * p_recv_rssi);
/******************************************************************
X_Boolean send_recv_cb_example(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length)
{
	X_Boolean isOK = X_False;
	switch(event)
	{
		case LE_Successed:
			isOK = X_True;
			break;
		case LE_TimeOut:
			break;
		case LE_Failed:
			break;
		case LE_Check:
			break;
		default:
			break;
	}
	return isOK;
}
*********************************************************************/
X_Void mFunc_MultiLora_SendRecvReset(const sLoraFuncManager *p_manager);
X_Void mFunc_MultiLora_SendRecvHandle(const sLoraFuncManager *p_manager);

X_Void mFunc_MultiLora_SendAgain(const sLoraFuncManager *p_manager);
X_Void mFunc_MultiLora_EnableContinueReceive(const sLoraFuncManager *p_manager);
X_Void mFunc_MultiLora_DisableContinueReceive(const sLoraFuncManager *p_manager);
X_Boolean mFunc_MultiLora_SendRequest(const sLoraFuncManager *p_manager,const uint8_t *p_data,uint16_t length,
										cp_send_rec_cb p_send_cb,
										uint16_t rec_time_in_ms,cp_send_rec_cb p_recv_cb);

X_Boolean mFunc_MultiLora_DoesNewDataCome(const sLoraFuncManager *p_manager,uint8_t **pp_data,uint16_t *p_length);
X_Boolean mFunc_Does_MultiLoraModuleBusy(const sLoraFuncManager *p_manager);

#ifdef __cplusplus
		}
#endif

#endif

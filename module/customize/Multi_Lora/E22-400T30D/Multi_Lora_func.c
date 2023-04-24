#include "Lora_func.h"
#include "../../../../../module/common/StateMachine/StateMachine.h"
#include "../../../../../module/common/AppCommon.h"
#include <string.h>
#include "../../../../../module/common/loopqueue/loop_queues.h"

#ifndef MAX_LORA_RECEIVE_FRAME_TO_CACHE
	#define MAX_LORA_RECEIVE_FRAME_TO_CACHE        6
#endif

#ifndef MAX_LORA_SEND_REQUEST_TO_CACHE
	#define MAX_LORA_SEND_REQUEST_TO_CACHE         3
#endif

#define LORA_LENGTH_SIZE_IN_BYTE               2
#define LORA_VALID_DATA_START_ADDR             LORA_LENGTH_SIZE_IN_BYTE
#define LORA_STATE_CHECK_TIME_MS               500
#define LORA_RECV_MAX_INTERVAL_TIME_IN_MS      100
#define LORA_SEND_INTERVAL_TIME_IN_MS          30  // 防止粘包，未经严格验证，或许还能更小，大概率是发送模块粘包了
#define LORA_SEND_WAIT_TIME_IN_MS_WITHOUT_AUX  100
#define LORA_SEND_WAIT_TIME_IN_MS_WITH_AUX     500
#define LORA_AUX_STABLE_TIME_MS                20


#define SET_CMD           0xc0
#define READ_CMD          LORA_READ_CMD
#define SET_TEMP_CMD      0xc2
#define ERROR_RESPONSE    0xff

#pragma pack(1)
typedef struct
{
	uint8_t cmd;
	uint8_t start_addr;
	uint8_t length;
	uint8_t param[];
}sLoraCommand;
#pragma pack()
/******************************************************************************/
/******************************************************************************/
static set_work_mode        set_mode_func = (set_work_mode)0;
static get_byte             get_byte_func = (get_byte)0;
static send_buf             send_buf_func = (send_buf)0;
static clear_recv_cache     clear_rec_cache_func = (clear_recv_cache)0;
static get_time             get_time_func = (get_time)0;
static set_time             set_time_func = (set_time)0;
static does_aux_busy        does_busy_func = (does_aux_busy)0;

//////////////static X_Boolean DoesLoraBusy(X_Void) // can not be use by other module 
//////////////{
//////////////	if(does_busy_func == X_Null) {return X_True;}
//////////////	return does_busy_func();
//////////////}

X_Boolean mFunc_LoraInit(set_work_mode p_func_set,
												get_byte p_func_get_byte,send_buf p_func_send_buf,clear_recv_cache p_func_clear_recv_cache,
												get_time p_func_get_time,set_time p_func_set_time)
{
	set_mode_func = p_func_set;
	get_byte_func = p_func_get_byte;
	send_buf_func = p_func_send_buf;
	get_time_func = p_func_get_time;
	set_time_func = p_func_set_time;
	clear_rec_cache_func = p_func_clear_recv_cache;
	
	if(set_mode_func == (set_work_mode)0) {return X_False;}
	if(get_byte_func == (get_byte)0) {return X_False;}
	if(send_buf_func == (send_buf)0) {return X_False;}
	if(get_time_func == (get_time)0) {return X_False;}
	if(set_time_func == (set_time)0) {return X_False;}
	return X_True;
}
/********************************************************************************************
********************************************************************************************
********************************************************************************************
********************************************************************************************
********************************************************************************************
********************************************************************************************
先切换模式0再切换模式3是为了起到一种类似模式复位的效果，让配置模式3每次都从模式0进入，有利于清除前面的不确定状态
切换模式过程中，使用延时而不是检测AUX脚来的方式来确定模式是否切换完毕。之所以这么做有两方面原因：
1，配置过程不是一个要求速度的过程，延时100~200ms可以接受
2，增加模块兼容性，有的人在画PCB的时候不连接AUX脚。
********************************************************************************************/

#define BEFORE_MODE_SWITCH_TIME_MS           100
#define MODE_SWITCH_TIME_MS                  200
#define WAIT_RESPONSE_TIME_MS                1000
typedef enum
{
	LC_Idle,
	LC_Wait,
	LC_SetMode,
	LC_SendCMD,
	LC_GetResponse,
	LC_Finished,
	LC_Error,
	LC_GetConfig,
	LC_Compare,
}eLoraConfig;
	
typedef struct
{
	s_StateMachineParam 		base;
	event_handler           evt_handle;
	eLoraProcess 						error_code;
	uint8_t 								data[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t 								length;
	uint8_t 								retry_times;
	StateNumber							state_backup;
	
	uint8_t recv_data[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t recv_length;
}sLoraConfigParamExtern;
	
static sLoraConfigParamExtern sLCPE;
static X_Void LC_Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
/********************************************************************************************
0
********************************************************************************************/	
static StateNumber	LC_IdleAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	
	if(p_ext ->data == X_Null || p_ext ->length == 0 || p_ext ->length > MAX_LORA_CONFIG_DATA_LENGTH) {return LC_Error;}
	
	if(set_mode_func != X_Null)
	{
		if(set_mode_func(LM_Trans) == X_True) 
		{
			if(set_time_func != X_Null) 
			{
				set_time_func(BEFORE_MODE_SWITCH_TIME_MS);
				p_ext ->state_backup = LC_SetMode;
				return LC_Wait;
			}
			return LC_Error;
		}
		return LC_Error;
	}
	return LC_Error;
}
/********************************************************************************************
1
********************************************************************************************/	
static StateNumber	LC_WaitAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(get_time_func == X_Null) {return p_ext ->state_backup;}
	if(get_time_func() == 0) {return p_ext ->state_backup;}
	return p_this ->current_state;
}
/********************************************************************************************
2
********************************************************************************************/	
static StateNumber	LC_SetModeAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(set_mode_func != X_Null)
	{
		if(set_mode_func(LM_Config) == X_True) 
		{
			if(set_time_func != X_Null) 
			{
				set_time_func(MODE_SWITCH_TIME_MS);
				p_ext ->state_backup = LC_GetConfig;
				return LC_Wait;
			}
			return LC_Error;
		}
		return LC_Error;
	}
	return LC_Error;
}
/********************************************************************************************
3
********************************************************************************************/	
static StateNumber	LC_SendCMD_Action(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(send_buf_func == X_Null) {return LC_Error;}
	
	if(p_ext ->retry_times == 0) {return LC_Error;}
	if(clear_rec_cache_func != (clear_recv_cache)0) {clear_rec_cache_func();}
	if(send_buf_func(p_ext ->data,p_ext ->length) != X_True) {return LC_Error;}
	if(set_time_func == X_Null) {return LC_Error;}
	set_time_func(WAIT_RESPONSE_TIME_MS);
	p_ext ->retry_times --;
	
	p_ext ->recv_length = 0;
	return LC_GetResponse;
}
/********************************************************************************************
4
********************************************************************************************/
static StateNumber	LC_GetResponseAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(get_time_func == X_Null || get_byte_func == X_Null) {return LC_Error;}
	if(get_time_func() == 0) 
	{
		p_ext ->evt_handle(LE_TimeOut,p_ext ->recv_data,p_ext ->recv_length);
		return LC_SendCMD;
	}
	
	if(get_byte_func(&cur_data) == X_True)
	{
		INSERT(LogDebug)(LORA_REC_DEBUG,(" lora rec %2x \r\n",cur_data));
		if(p_ext ->recv_length >= MAX_LORA_CONFIG_DATA_LENGTH) {return LC_Error;}
		p_ext ->recv_data[p_ext ->recv_length++] = cur_data;
		if(p_ext ->recv_length >= p_ext ->length)
		{
			if(p_ext ->evt_handle == X_Null) {return LC_Finished;}
			if(p_ext ->evt_handle(LE_Check,p_ext ->recv_data,p_ext ->recv_length) == X_True)
			{
				p_ext ->evt_handle(LE_Successed,p_ext ->recv_data,p_ext ->recv_length);
				return LC_Finished;
			}
			else
			{
				p_ext ->state_backup = LC_SendCMD;
				if(set_time_func == X_Null) {return LC_Error;}
				set_time_func(MODE_SWITCH_TIME_MS);
				return LC_Wait;
			}
		}
	}
	return p_this ->current_state;
}
/********************************************************************************************
5
********************************************************************************************/
static StateNumber	LC_FinishedAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	p_ext ->error_code = LP_Finished;
	return p_this ->current_state;
}
/********************************************************************************************
6
********************************************************************************************/
static StateNumber	LC_ErrorAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(p_ext ->evt_handle != X_Null)
	{
		p_ext ->evt_handle(LE_Failed,p_ext ->recv_data,p_ext ->recv_length);
		p_ext ->evt_handle = X_Null;
	}
	p_ext ->error_code = LP_Error;
	return p_this ->current_state;
}
/********************************************************************************************
7
********************************************************************************************/
static StateNumber	LC_GetConfigAction(s_StateMachineParam *p_this)
{
	uint8_t temp_buf[3];
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	sLoraCommand *p_cmd = (sLoraCommand*)p_ext ->data;
	if(send_buf_func == X_Null) {return LC_Error;}
	
	temp_buf[0] = READ_CMD;
	temp_buf[1] = p_cmd ->start_addr;
	temp_buf[2] = p_cmd ->length;
	if(clear_rec_cache_func != (clear_recv_cache)0) {clear_rec_cache_func();}
	if(send_buf_func(temp_buf,3) != X_True) {return LC_SendCMD;}
	
	if(set_time_func == X_Null) {return LC_Error;}
	set_time_func(WAIT_RESPONSE_TIME_MS);
	
	p_ext ->recv_length = 0;
	return LC_Compare;
	
}
/********************************************************************************************
8
********************************************************************************************/
static StateNumber	LC_CompareAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	sLoraCommand *p_cmd = (sLoraCommand*)p_ext ->data;
	if(get_time_func == X_Null || get_byte_func == X_Null) {return LC_Error;}
	if(get_time_func() == 0) 
	{
		return LC_SendCMD;
	}
	
	if(get_byte_func(&cur_data) == X_True)
	{
		INSERT(LogDebug)(LORA_CONFIG_DEBUG,(" lora get config %2x \r\n",cur_data));

		if(p_ext ->recv_length >= MAX_LORA_CONFIG_DATA_LENGTH) {return LC_Error;}
		p_ext ->recv_data[p_ext ->recv_length++] = cur_data;
		if(p_ext ->recv_length >= (p_cmd ->length + 3))
		{
				if(p_ext ->recv_data[0] == READ_CMD 
								&& memcmp(&p_ext ->data[1],&p_ext ->recv_data[1],p_ext ->recv_length - 1) == 0)
				{
					INSERT(LogDebug)(LORA_CONFIG_DEBUG,(" same lora config ; return \r\n"));
					if(p_ext ->evt_handle == X_Null) {return LC_Finished;}
					p_ext ->evt_handle(LE_Successed,p_ext ->recv_data,p_ext ->recv_length);
					return LC_Finished;
				}
			
				p_ext ->state_backup = LC_SendCMD;
				if(set_time_func == X_Null) {return LC_Error;}
				set_time_func(MODE_SWITCH_TIME_MS);
				return LC_Wait;
		}
	}
	return p_this ->current_state;
	
}
/********************************************************************************************

********************************************************************************************/
static const StateAction LoraConfigAction[] = {
	{LC_IdleAction},
	{LC_WaitAction},
	{LC_SetModeAction},
	{LC_SendCMD_Action},
	{LC_GetResponseAction},
	{LC_FinishedAction},
	{LC_ErrorAction},
	{LC_GetConfigAction},
	{LC_CompareAction},
};

APP_STATE_MACHINE_DEF(p_lora_config,sizeof(LoraConfigAction)/sizeof(LoraConfigAction[0]),&LoraConfigAction[0]);

/*****************************************************************************************************/
#define MAX_LORA_CHANNEL                  83
#define LORA_CONFIG_ERROR_HANDLE          mStateMachineStateSet(p_lora_config,LC_Error);return
#define LORA_CONFIG_DEBUG_METHOD          SysLogDebug
#define MAX_LORA_NETID                        255  //0到255为网络ID取值范围

#define REG_0_ADDR_HIGH(addr)  ((uint8_t)(addr >> 8))
#define REG_1_ADDR_LOW(addr)   ((uint8_t)(addr))

#define REG_2_NETID(net_id)(net_id > MAX_LORA_NETID) ? 0  :  net_id;

#define REG_3_BPS(baud_rate,p_result)   if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(baud_rate)   \
																{    \
																	case 1200:  \
																		*p_result = 0; \
																		break;   \
																	case 2400:  \
																		*p_result = 0x20; \
																		break;   \
																	case 4800:  \
																		*p_result = 0x40; \
																		break;   \
																	case 9600:  \
																		*p_result = 0x60; \
																		break;   \
																	case 19200:  \
																		*p_result = 0x80; \
																		break;   \
																	case 38400:  \
																		*p_result = 0xa0; \
																		break;   \
																	case 57600:  \
																		*p_result = 0xc0; \
																		break;   \
																	case 115200:  \
																		*p_result = 0xe0; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("baud_rate %d is wrong !!!\r\n",baud_rate)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																} // no break for defaut ,because there is a return 

#define REG_3_CHECK_BITES(check_bits,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(check_bits)   \
																{    \
																	case Cb_8N1:  \
																		*p_result = 0; \
																		break;   \
																	case Cb_801:  \
																		*p_result = 0x08; \
																		break;   \
																	case Cb_8E1:  \
																		*p_result = 0x10; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("check_bits %d is wrong !!!\r\n",check_bits)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																}
																
#define REG_3_AIR_SPEED(air_speed,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(air_speed)   \
																{    \
																	case AS_0_3k:  \
																		*p_result = 0x00; \
																		break;   \
																	case AS_1_2k:  \
																		*p_result = 0x01; \
																		break;   \
																	case AS_2_4k:  \
																		*p_result = 0x02; \
																		break;   \
																	case AS_4_8k:  \
																		*p_result = 0x03; \
																		break;   \
																	case AS_9_6k:  \
																		*p_result = 0x04; \
																		break;   \
																	case AS_19_2k:  \
																		*p_result = 0x05; \
																		break;   \
																	case AS_38_4k:  \
																		*p_result = 0x06; \
																		break;   \
																	case AS_62_5k:  \
																		*p_result = 0x07; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("air_speed %d is wrong !!!\r\n",air_speed)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																}
																
																
#define REG_4_SUBPACKAGE(subpackage,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(subpackage)   \
																{    \
																	case 240:  \
																		*p_result = 0; \
																		break;   \
																	case 128:  \
																		*p_result = 0x40; \
																		break;   \
																	case 64:  \
																		*p_result = 0x80; \
																		break;   \
																	case 32:  \
																		*p_result = 0xc0; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("subpackage length %d is wrong !!!\r\n",subpackage)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																}
																
#define REG_4_RSSI_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x20;}  \
													else {*p_result = 0;}

#define REG_4_TRANS_POWER(power,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(power)   \
																{    \
																	case TP_30dbm:  \
																		*p_result = 0; \
																		break;   \
																	case TP_27dbm:  \
																		*p_result = 0x01; \
																		break;   \
																	case TP_24dbm:  \
																		*p_result = 0x02; \
																		break;   \
																	case TP_21dbm:  \
																		*p_result = 0x03; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("trans power %d is wrong !!!\r\n",power)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																}
																
#define REG_5_CHANNEL(channel) (channel > MAX_LORA_CHANNEL) ? 0 : channel;
																
#define REG_6_RSSI_BYTE_REPORT_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x80;}  \
													else {*p_result = 0;}

#define REG_6_TRANS_MODE(mode,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(mode == TM_transparent) {*p_result = 0;}  \
													else {*p_result = 0x40;}
													
#define REG_6_LBT_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x10;}  \
													else {*p_result = 0;}

#define REG_6_WOR_PERIOD(ms,p_result)   if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(ms)   \
																{    \
																	case 500:  \
																		*p_result = 0; \
																		break;   \
																	case 1000:  \
																		*p_result = 0x01; \
																		break;   \
																	case 1500:  \
																		*p_result = 0x02; \
																		break;   \
																	case 2000:  \
																		*p_result = 0x03; \
																		break;   \
																	case 2500:  \
																		*p_result = 0x04; \
																		break;   \
																	case 3000:  \
																		*p_result = 0x05; \
																		break;   \
																	case 3500:  \
																		*p_result = 0x06; \
																		break;   \
																	case 4000:  \
																		*p_result = 0x07; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("wor period %d is wrong !!!\r\n",ms)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																} // no break for defaut ,because there is a return 
																
#define REG_7_SECRET_KEY_HIGH(key)  ((uint8_t)(key >> 8))
#define REG_8_SECRET_KEY_LOW(key)   ((uint8_t)(key))
/*****************************************************************************************************/
X_Void mFunc_LoraConfigReset(sLoraConfig * p_config,uint8_t retry_times,event_handler handler)
{
	uint8_t cmd[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t temp1,temp2,temp3,temp4;
	mStateMachineStateSet(p_lora_config,LC_Idle);
	sLCPE.evt_handle = handler;
	sLCPE.error_code = LP_InProcess;
	
	if(p_config == X_Null ) 
	{
		LORA_CONFIG_ERROR_HANDLE;
	}
	
	cmd[0]  = SET_CMD;
	cmd[1]  = 0; // start addr 0;
	cmd[2]  = 9; // reg from 0 ~ 8
	
	cmd[3]  = REG_0_ADDR_HIGH(p_config->module_addr);
	cmd[4]  = REG_1_ADDR_LOW(p_config->module_addr);
	
	cmd[5]  = REG_2_NETID(p_config ->net_id);
	
	
						REG_3_BPS(p_config->baud_rate,&temp1);
						REG_3_CHECK_BITES(p_config->check_bits,&temp2);
						REG_3_AIR_SPEED(p_config ->air_speed,&temp3);
	cmd[6]  = temp1|temp2|temp3;
	
						REG_4_SUBPACKAGE(p_config ->subpackage_length,&temp1);
						REG_4_RSSI_ENABLE(p_config ->isRssiEnable,&temp2);
						REG_4_TRANS_POWER(p_config ->trans_power,&temp3);
	cmd[7]  = temp1|temp2|temp3;
	
	cmd[8]  = REG_5_CHANNEL(p_config ->channel);
	
						REG_6_RSSI_BYTE_REPORT_ENABLE(p_config ->isRssiByteEnable,&temp1);
						REG_6_TRANS_MODE(p_config ->trans_mode,&temp2);
						REG_6_LBT_ENABLE(p_config ->isLBT_Enable,&temp3);
						REG_6_WOR_PERIOD(p_config ->WOR_period_ms,&temp4);
	cmd[9]  = temp1|temp2|temp3|temp4;

	cmd[10]  = REG_7_SECRET_KEY_HIGH(p_config ->secret_key);
	cmd[11] = REG_8_SECRET_KEY_LOW(p_config ->secret_key);

	CopyBuffer(cmd,sLCPE.data,MAX_LORA_CONFIG_DATA_LENGTH);
	sLCPE.length = MAX_LORA_CONFIG_DATA_LENGTH;
	sLCPE.retry_times = retry_times;
	
	LORA_CONFIG_DEBUG_METHOD(1,("the config command is 0xc0 0x00 0x09 0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x\r\n"
												,cmd[3],cmd[4],cmd[5],cmd[6],cmd[7],cmd[8],cmd[9],cmd[10],cmd[11]));
}
eLoraProcess mFunc_LoraConfigHandle(X_Void)
{
	mStateMachineRun(p_lora_config,&sLCPE.base,LC_Recorder);
	return sLCPE.error_code;
}

X_Boolean mFunc_LoraGetConfigCMD(uint8_t *p_data ,uint8_t *p_length)
{
	if(p_data == X_Null || p_length == X_Null || sLCPE.length > MAX_LORA_CONFIG_DATA_LENGTH) {return X_False;}
	
	*p_length = sLCPE.length;
	CopyBuffer(sLCPE.data,p_data,sLCPE.length);
	
	return X_True;
}

X_Void mFunc_LoraAuxMonitorReg(does_aux_busy aux)
{
	does_busy_func = aux;
}

/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/


////////////typedef X_Boolean (*WaitMethod)(X_Void);

typedef struct
{
	uint8_t *p_data;
	uint16_t length;
	p_lora_send_rec_cb send_cb;
}sSend;

typedef struct
{
	uint8_t *p_rec_data;
	uint16_t recv_length;
	uint16_t  rec_time_limit_in_ms;
	p_lora_send_rec_cb recv_cb;
}sRecv;

typedef struct
{
	uint8_t send_buf[MAX_LORA_SEND_DATA_LENGTH];
	uint16_t send_length;
	p_lora_send_rec_cb send_cb;
	sRecv recv;
}sSendRequest;

INSERT(LOG_ONCE_ENTRY_DEF)(p_once_sr,100);
//////////INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);

APP_LOOPQUEUE_DEF(p_queue_send,MAX_LORA_SEND_REQUEST_TO_CACHE);
APP_LOOPQUEUE_DEF(p_queue_recv,MAX_LORA_RECEIVE_FRAME_TO_CACHE);


static uint8_t lora_send_backup_buf[MAX_LORA_SEND_DATA_LENGTH];
static sSendRequest send_param[MAX_LORA_SEND_REQUEST_TO_CACHE];

static uint8_t  recv_buf[MAX_LORA_RECEIVE_FRAME_TO_CACHE][MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE]; // + 2 to store length
static uint8_t  cur_recv_backup_buf[MAX_LORA_RECV_DATA_LENGTH];

static X_Boolean isContinueWaitRecWhenSendFailed = X_False;
/************************************************************************************
************************************************************************************
************************************************************************************
************************************************************************************/

typedef enum
{
	SRS_Idle,
	SRS_Normal,
	SRS_SendData,											
	SRS_GetData,										/* 该状态用于接收lora 数据 */
	SRS_Reset,
	SRS_SendInterval, // 为了解决连续的发送请求下，模块误将定点传输的3字节帧头（地址信道）作为发射数据发出的问题。
}eSendRecvState;

typedef struct
{
	s_StateMachineParam 		base;
	sSend                   cur_send;
	sRecv                   cur_recv;
	X_Boolean								isBusy;
	X_Void									(*reset_action)(X_Void);
	X_Boolean               isAuxBusyBackup;
	X_Boolean               isAuxBusy;
	uint32_t                timer_backup;
}sSendRecvParamExtern;

static sSendRecvParamExtern sSRPE;
static X_Boolean isScanInProcess = X_False;

static X_Boolean DoesSameSendRequestAgain(X_Void);
static X_Void SR_Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
														
static X_Boolean DoesLoraSend(sSendRecvParamExtern *p_ext)
{
	uint16_t buf_num;
	if(p_ext == X_Null) {return X_False;}
	if(DoesLoopQueueEmpty(p_queue_send) == X_True){return X_False;}
	
	buf_num = LoopQueueFirstOut(p_queue_send);
	if(buf_num < MAX_LORA_SEND_REQUEST_TO_CACHE)
	{
		if(send_param[buf_num].send_length > MAX_LORA_SEND_DATA_LENGTH)
		{
			RealseLoopQueueBuf(p_queue_send,buf_num);
			return X_False;
		}
		
		CopyBuffer(&(send_param[buf_num].send_buf[0]),lora_send_backup_buf,send_param[buf_num].send_length);
		
		p_ext ->cur_send.p_data               = lora_send_backup_buf;
		p_ext ->cur_send.length               = send_param[buf_num].send_length;
		p_ext ->cur_send.send_cb              = send_param[buf_num].send_cb;
		p_ext ->cur_recv.rec_time_limit_in_ms = send_param[buf_num].recv.rec_time_limit_in_ms;
		p_ext ->cur_recv.recv_cb              = send_param[buf_num].recv.recv_cb;

		RealseLoopQueueBuf(p_queue_send,buf_num);
		return X_True;
	}
	return X_False;
}

/**********************************************************************************************************************
	0		
**********************************************************************************************************************/
static StateNumber  SRS_IdleAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_Idle,("------------------ SRS_IdleAction\r\n"));
	isScanInProcess = X_False;
	p_ext ->isBusy = X_True;
	if(set_mode_func != X_Null)
	{
		set_mode_func(LM_Trans);
	}
	return SRS_Normal;
}
/**********************************************************************************************************************
	1
static get_byte             get_byte_func  = (get_byte)0;
static send_buf             send_buf_func  = (send_buf)0;
static get_time             get_time_func  = (get_time)0;
static set_time             set_time_func  = (set_time)0;
static does_aux_busy        does_busy_func = (does_aux_busy)0;
**********************************************************************************************************************/
static X_Boolean isCountDownForRecv = X_False;
static StateNumber	SRS_NormalAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_Normal,("------------------ SRS_NormalAction\r\n"));

	if(get_byte_func == (get_byte)0) {return SRS_Reset;}
	if(get_time_func == (get_time)0) {return SRS_Reset;}
	if(set_time_func == (set_time)0) {return SRS_Reset;}
	
	if(get_byte_func(&cur_data) == X_True)
	{
		cur_recv_backup_buf[0] = cur_data;
		p_ext ->cur_recv.recv_length = 1;
		p_ext ->cur_recv.p_rec_data = cur_recv_backup_buf;
		
		isCountDownForRecv = X_False;
		p_ext ->isAuxBusyBackup = X_True;
		if(p_ext ->cur_recv.rec_time_limit_in_ms != 0) {p_ext ->timer_backup = get_time_func();}
		return SRS_GetData;
	}

	if(p_ext ->cur_recv.rec_time_limit_in_ms != 0)
	{
		if(get_time_func() == 0)
		{
			if(p_ext ->cur_recv.recv_cb != X_Null) {p_ext ->cur_recv.recv_cb(LE_TimeOut,p_ext ->cur_recv.p_rec_data,0);}
			// 本次接受失败后用户没有在回调函数recv_cb中send again.代表整个收发流程彻底失败，可以清除相关参数了
			if(DoesSameSendRequestAgain() == X_False) 
			{
				mFunc_Lora_CurRecvProcessTerminate();
			}
		}
		p_ext ->isBusy = X_True;
		return p_this ->current_state;// still wait for response , can not start another send routine
	}

	if(DoesLoraSend(p_ext) == X_True)
	{
		p_ext ->isBusy = X_True;
		if(does_busy_func == (does_aux_busy)0)
		{
			set_time_func(LORA_SEND_WAIT_TIME_IN_MS_WITHOUT_AUX);
		}
		else
		{
			set_time_func(LORA_SEND_WAIT_TIME_IN_MS_WITH_AUX);
		}
		return SRS_SendData;
	}
	p_ext ->isBusy = X_False;
	return p_this ->current_state;
	
}
/**********************************************************************************************************************
	2		
**********************************************************************************************************************/
static StateNumber	SRS_SendDataAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_SendData,("------------------ SRS_SendDataAction\r\n"));
	
	if(send_buf_func == (send_buf)0) {return SRS_Reset;}
	if(get_time_func == (get_time)0) {return SRS_Reset;}
	if(set_time_func == (set_time)0) {return SRS_Reset;}
	
	if(does_busy_func == (does_aux_busy)0)
	{
		if(get_time_func() == 0)
		{
			send_buf_func(p_ext ->cur_send.p_data,p_ext ->cur_send.length);
		
			if(p_ext ->cur_send.send_cb != X_Null)
			{
				p_ext ->cur_send.send_cb(LE_Successed,p_ext ->cur_send.p_data,p_ext ->cur_send.length);
			}
			
			INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ wait enough time ,lora just send %d bytes\r\n",p_ext ->cur_send.length));
			
			if(p_ext ->cur_recv.rec_time_limit_in_ms != 0)
			{
				set_time_func(p_ext ->cur_recv.rec_time_limit_in_ms);
				return SRS_Normal;
			}

			set_time_func(LORA_SEND_INTERVAL_TIME_IN_MS);
			return SRS_SendInterval;
		}
		else
		{
			return p_this ->current_state; // wait for lora module to get ready
		}
	}
	else
	{
		if(does_busy_func() == X_True) 
		{
			if(get_time_func() == 0) 
			{
				if(p_ext ->cur_send.send_cb != X_Null)
				{
					p_ext ->cur_send.send_cb(LE_TimeOut,p_ext ->cur_send.p_data,p_ext ->cur_send.length);
				}
				/****************************************************************************/
				// 发送都没发出去，更没有回应了。但是这里为什么不清除接收相关的参数呢？
				// 因为接受超时后还需要触发recv_cb，并且用户有可能在recv_cb中再次要求send again。
				// 因此发送和接受的参数在一个完整的收发流程彻底失败之前都要保留
////////				p_ext ->cur_recv.rec_time_limit_in_ms = 0; 
////////				p_ext ->cur_recv.recv_cb = X_Null;
				/****************************************************************************/
				INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ send failed once\r\n"));

				if(isContinueWaitRecWhenSendFailed == X_False)
				{
					if(DoesSameSendRequestAgain() == X_False) // not send again , the send process end 
					{
						mFunc_Lora_CurRecvProcessTerminate();
					}
				}
				return SRS_Normal;
			}
			return p_this ->current_state;
		}
		
		send_buf_func(p_ext ->cur_send.p_data,p_ext ->cur_send.length);
		
		if(p_ext ->cur_send.send_cb != X_Null)
		{
			p_ext ->cur_send.send_cb(LE_Successed,p_ext ->cur_send.p_data,p_ext ->cur_send.length);
		}
		
		INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ aux free,lora just send %d bytes\r\n",p_ext ->cur_send.length));
		
		if(p_ext ->cur_recv.rec_time_limit_in_ms != 0)
		{
			set_time_func(p_ext ->cur_recv.rec_time_limit_in_ms);
			return SRS_Normal;
		}

		set_time_func(LORA_SEND_INTERVAL_TIME_IN_MS);
		return SRS_SendInterval;
		
	}
}


/**********************************************************************************************************************
	3	  
**********************************************************************************************************************/
static StateNumber	SRS_GetDataAction(s_StateMachineParam *p_this)
{
	X_Boolean isRecvFinished = X_False;
	uint8_t cur_data;
	uint16_t buf_num;
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;
	
	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_GetData,("------------------ SRS_GetDataAction\r\n"));
	
	if(get_byte_func == (get_byte)0) {return SRS_Reset;}
	if(get_time_func == (get_time)0) {return SRS_Reset;}
	if(set_time_func == (set_time)0) {return SRS_Reset;}
	

	if(get_byte_func(&cur_data) == X_True)
	{
		isCountDownForRecv = X_False;
		cur_recv_backup_buf[p_ext ->cur_recv.recv_length ++] = cur_data;
		if(p_ext ->cur_recv.recv_length >= MAX_LORA_RECV_DATA_LENGTH){return SRS_Normal;}
		return p_this->current_state;
	}
	
	if(does_busy_func == (does_aux_busy)0)
	{
		if(isCountDownForRecv == X_False && get_time_func() == 0)
		{
			isCountDownForRecv = X_True;
			set_time_func(LORA_RECV_MAX_INTERVAL_TIME_IN_MS);
		}
		
		if(isCountDownForRecv == X_True)
		{
			if(get_time_func() == 0)
			{
				isRecvFinished = X_True;
			}
		}
	}
	else
	{
		p_ext ->isAuxBusy = does_busy_func();
		
		if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_True)
		{
			p_ext ->isAuxBusyBackup = X_False;
			set_time_func(LORA_AUX_STABLE_TIME_MS);		
		}
		
		if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_False)
		{
			if(get_time_func() == 0) {isRecvFinished = X_True;}
		}
		
		if(p_ext ->isAuxBusy == X_True) {p_ext ->isAuxBusyBackup = X_True;}

	}
	
	if(isRecvFinished == X_True)
	{
		if(p_ext ->cur_recv.rec_time_limit_in_ms != 0 && p_ext ->timer_backup != 0) {set_time_func(p_ext ->timer_backup);}
	
		if( p_ext ->cur_recv.rec_time_limit_in_ms != 0 && p_ext ->cur_recv.recv_cb != X_Null)
		{
			if(p_ext ->cur_recv.recv_cb(LE_Check,cur_recv_backup_buf,p_ext ->cur_recv.recv_length) == X_True)
			{
				p_ext ->cur_recv.recv_cb(LE_Successed,cur_recv_backup_buf,p_ext ->cur_recv.recv_length);
				mFunc_Lora_CurRecvProcessTerminate();
				return SRS_Normal;
			}
		}
		
		isScanInProcess = X_True;
		buf_num = LoopQueueFirstIn(p_queue_recv,X_False);
		if(buf_num >= MAX_LORA_RECEIVE_FRAME_TO_CACHE)
		{
			isScanInProcess = X_False;
			INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ----  lora GetMessage unexpected error occur 1\r\n"));
			if(p_ext ->cur_recv.recv_cb != X_Null) {p_ext ->cur_recv.recv_cb(LE_Failed,cur_recv_backup_buf,p_ext ->cur_recv.recv_length);}
			return SRS_Normal;
		}
		
		CopyBuffer(&p_ext ->cur_recv.recv_length,&recv_buf[buf_num][0],LORA_LENGTH_SIZE_IN_BYTE);
		CopyBuffer(cur_recv_backup_buf,&recv_buf[buf_num][LORA_VALID_DATA_START_ADDR],p_ext ->cur_recv.recv_length);
		
		isScanInProcess = X_False;
			
		INSERT(LogDebug)(LORA_MODULE_DEBUG,(" lora GetMessage :finished ;length %d ; \r\n",p_ext ->cur_recv.recv_length));
//////////		INSERT(LogDebug)(LORA_MODULE_DEBUG,(" lora GetMessage :finished ;length %d ; data %2x %2x %2x ... \r\n"
//////////		,p_ext ->cur_recv.recv_length
//////////		,p_ext ->cur_recv.p_rec_data[LORA_VALID_DATA_START_ADDR]
//////////		,p_ext ->cur_recv.p_rec_data[LORA_VALID_DATA_START_ADDR + 1]
//////////		,p_ext ->cur_recv.p_rec_data[LORA_VALID_DATA_START_ADDR + 2]));

		return SRS_Normal;
	}
	return  p_this ->current_state;
}

/**********************************************************************************************************************
	4		
**********************************************************************************************************************/
static StateNumber  SRS_ResetAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_Reset,("------------------ SRS_ResetAction\r\n"));

	if(p_ext ->reset_action != X_Null)
	{
		p_ext ->reset_action();
	}

	return SRS_Idle;
	
}
/**********************************************************************************************************************
	5		
**********************************************************************************************************************/
static StateNumber SRS_SendIntervalAction(s_StateMachineParam *p_this)
{
////	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;
	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_SendInterval,("------------------ SRS_SendIntervalAction\r\n"));

	if(get_time_func == (get_time)0) {return SRS_Reset;}
	if(get_time_func() == 0)
	{
		return SRS_Normal;
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************
	
**********************************************************************************************************************/


static const StateAction SendRecvAction[] = {
	{SRS_IdleAction},
	{SRS_NormalAction},
	{SRS_SendDataAction},
	{SRS_GetDataAction},
	{SRS_ResetAction},
	{SRS_SendIntervalAction},
};
APP_STATE_MACHINE_DEF(p_sr,sizeof(SendRecvAction)/sizeof(SendRecvAction[0]),&SendRecvAction[0]);

static X_Void SendParamClear(X_Void)
{
	uint8_t i;
	LoopQueueInitialize(p_queue_send);
	sSRPE.cur_send.send_cb = X_Null;
	for(i=0;i<MAX_LORA_SEND_REQUEST_TO_CACHE;i++)
	{
		send_param[i].send_length 	= 0;
		send_param[i].send_cb     	= X_Null;
		send_param[i].recv.recv_cb	= X_Null;
		send_param[i].recv.rec_time_limit_in_ms = 0;
	}
}
static X_Void RecvParamClear(X_Void)
{
	LoopQueueInitialize(p_queue_recv);
	sSRPE.cur_recv.recv_cb = X_Null;
	sSRPE.cur_recv.rec_time_limit_in_ms = 0;
	isScanInProcess = X_False;
	isContinueWaitRecWhenSendFailed = X_False;
}

X_Void mFunc_Lora_ReinitFuncRegister(X_Void (*reset)(X_Void))
{
	sSRPE.reset_action = reset;
}

X_Void mFunc_Lora_SendRecvReset(X_Void)
{
	SendParamClear();
	RecvParamClear();
	sSRPE.isBusy = X_True;
	sSRPE.timer_backup = 0;
	mStateMachineStateSet(p_sr,SRS_Idle);
	if(set_mode_func != X_Null)
	{
		set_mode_func(LM_Trans);
	}
}
X_Void mFunc_Lora_SendRecvHandle(X_Void)
{
	mStateMachineRun(p_sr,&sSRPE.base,SR_Recorder);
}
static X_Boolean DoesSameSendRequestAgain(X_Void)
{
	return (mStateMachineGetSuddenChangeState(p_sr) == SRS_SendData);
}
X_Void mFunc_Lora_SendAgain(X_Void)
{
	sSRPE.isBusy = X_True;
	if(set_time_func != (set_time)0) {set_time_func(LORA_STATE_CHECK_TIME_MS);}
	mStateMachineStateSet(p_sr,SRS_SendData);
}

X_Void mFunc_Lora_CurRecvProcessTerminate(X_Void)
{
	sSRPE.cur_recv.recv_cb = X_Null;
	sSRPE.cur_recv.rec_time_limit_in_ms = 0;
}

X_Void mFunc_Lora_EnableContinueReceive(X_Void)
{
	isContinueWaitRecWhenSendFailed = X_True;
}
X_Void mFunc_Lora_DisableContinueReceive(X_Void)
{
	isContinueWaitRecWhenSendFailed = X_False;
}
X_Boolean mFunc_Lora_SendRequest(const uint8_t *p_data,uint16_t length,p_lora_send_rec_cb p_send_cb
											,uint16_t rec_time_in_ms,p_lora_send_rec_cb p_recv_cb)
{

	uint16_t buf_num;
	if(p_data == X_Null || length > MAX_LORA_SEND_DATA_LENGTH) {return X_False;}

	buf_num = LoopQueueFirstIn(p_queue_send,X_False);
	if(buf_num < MAX_LORA_SEND_REQUEST_TO_CACHE)
	{
		send_param[buf_num].send_length = length;
		CopyBuffer(p_data,send_param[buf_num].send_buf,length);
		send_param[buf_num].send_cb = p_send_cb;
		send_param[buf_num].recv.rec_time_limit_in_ms = rec_time_in_ms;
		send_param[buf_num].recv.recv_cb              = p_recv_cb;
		
		INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ Lora get a send request : length %d :\r\n",length));

		return X_True;
	}
	INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ Lora get a send request but no buf to store it !!!\r\n"));
	return X_False;
}
X_Boolean mFunc_Lora_DoesNewDataCome(uint8_t **pp_data,uint16_t *p_length)
{
	uint16_t length,buf_num;
	if(pp_data == X_Null || p_length == X_Null) {return X_False;}
	if(isScanInProcess == X_True) {return X_False; }
	if(DoesLoopQueueEmpty(p_queue_recv) == X_True){return X_False;}
	buf_num = LoopQueueFirstOut(p_queue_recv);
	if(buf_num < MAX_LORA_RECEIVE_FRAME_TO_CACHE)
	{
		*pp_data = &recv_buf[buf_num][LORA_VALID_DATA_START_ADDR];
		CopyBuffer(&recv_buf[buf_num][0],&length,LORA_LENGTH_SIZE_IN_BYTE);
//////////////		INSERT(LogDebug)(BC95_CLOUD_SCAN_DEBUG,(" ~~~~~~~~NewCommandCome~~~~ %d ; %2x %2x %2x\r\n",length,*pp_data[0],*pp_data[1],*pp_data[2] ));
		RealseLoopQueueBuf(p_queue_recv,buf_num); 
		if(length == 0)  {return X_False;}
		*p_length = length;
		
		return X_True;
	}
	return X_False;
}

X_Boolean mFunc_Does_LoraModuleBusy(X_Void)
{
	return sSRPE.isBusy;
}

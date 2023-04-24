#include "Multi_Lora_func.h"

#include "../../../common/AppCommon.h"
#include <string.h>

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

X_Boolean mFunc_MultiLoraInit(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return X_False;}
	
	*p_manager ->p_isInitOK = X_False;
	
	if(p_manager ->lora_sr_param ->send_cache_size == 0 || p_manager ->lora_sr_param ->recv_cache_size == 0) {return X_False;}
	if(p_manager ->lora_sr_param ->send_cache_size > MAX_LORA_SEND_REQUEST_TO_CACHE 
		|| p_manager ->lora_sr_param ->recv_cache_size > MAX_LORA_RECEIVE_FRAME_TO_CACHE) {return X_False;}
			
	if(p_manager ->lora_sr_param ->set_mode_func == (set_work_mode)0) {return X_False;}
	if(p_manager ->lora_sr_param ->get_byte_func == (get_byte)0) {return X_False;}
	if(p_manager ->lora_sr_param ->send_buf_func == (send_buf)0) {return X_False;}
	if(p_manager ->lora_sr_param ->clear_rec_cache_func == (clear_recv_cache)0) {return X_False;}
	if(p_manager ->lora_sr_param ->does_busy_func == (does_aux_busy)0) {return X_False;}
	if(p_manager ->lora_sr_param ->get_time_func == (get_time)0) {return X_False;}
	if(p_manager ->lora_sr_param ->set_time_func == (set_time)0) {return X_False;}
	
//////	p_manager ->lora_sr_param ->p_mana = p_manager;
	*p_manager ->p_isInitOK = X_True;
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
2023 01 04 : 还是决定在配置阶段使用AUX脚。模块被频繁操作后可能5秒都无法进入空闲状态，简单延时几百毫秒无法解决这个问题
********************************************************************************************/
////////INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

#define BEFORE_MODE_SWITCH_TIME_MS           200
#define MODE_SWITCH_TIME_MS                  300
#define WAIT_RESPONSE_TIME_MS                1500
#define AUX_FREE_INTERVAL_TIME_MS            50

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
    LC_AuxFreeInterval, // jump to this state only if aux is free in wait state 
}eLoraConfig;


	

	

static X_Void LC_Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
/********************************************************************************************
0
********************************************************************************************/	
StateNumber	LC_IdleAction(s_StateMachineParam *p_this)
{
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_Idle,("------------------ LC_IdleAction\r\n"));
	if(p_ext ->data == X_Null || p_ext ->length == 0 || p_ext ->length > MAX_LORA_CONFIG_DATA_LENGTH) {return LC_Error;}
	
	if(p_ext ->set_mode_func(LM_Trans) == X_True) // need pointer check , to be strictly 
	{
		p_ext ->set_time_func(BEFORE_MODE_SWITCH_TIME_MS);
		p_ext ->state_backup = LC_SetMode;
		return LC_Wait;
	}
	return LC_Error;
}
/********************************************************************************************
1
********************************************************************************************/	
StateNumber	LC_WaitAction(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_Wait,("------------------ LC_WaitAction\r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(p_ext ->get_time_func() == 0) {return p_ext ->state_backup;}
    if(p_ext ->does_busy_func() == X_False) 
    {
        p_ext ->set_time_func(AUX_FREE_INTERVAL_TIME_MS);            
        return LC_AuxFreeInterval;
    }
	return p_this ->current_state;
}
/********************************************************************************************
2
********************************************************************************************/	
StateNumber	LC_SetModeAction(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_SetMode,("------------------ LC_SetModeAction\r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;

	if(p_ext ->set_mode_func(LM_Sleep) == X_True) 
	{
		p_ext ->set_time_func(MODE_SWITCH_TIME_MS);
		p_ext ->state_backup = LC_GetConfig;
		return LC_Wait;

	}
	return LC_Error;
}
/********************************************************************************************
3
********************************************************************************************/	
StateNumber	LC_SendCMD_Action(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_SendCMD,("------------------ LC_SendCMD_Action\r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	
	if(p_ext ->retry_times == 0) {return LC_Error;} //{return LC_Finished;}//
	p_ext ->clear_rec_cache_func();
	if(p_ext ->send_buf_func(p_ext ->data,p_ext ->length) != X_True) {return LC_Error;}
	p_ext ->set_time_func(WAIT_RESPONSE_TIME_MS);
	p_ext ->retry_times --;
	
	p_ext ->recv_length = 0;
	return LC_GetResponse;
}
/********************************************************************************************
4
********************************************************************************************/
StateNumber	LC_GetResponseAction(s_StateMachineParam *p_this)
{
//////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_GetResponse,("------------------ LC_GetResponseAction\r\n"));
	uint8_t cur_data;
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;

	if(p_ext ->get_time_func() == 0) 
	{
		p_ext ->evt_handle(CSRE_Timeout,p_ext ->recv_data,p_ext ->recv_length,p_ext ->data,p_ext ->length);
		return LC_SendCMD;
	}
	
	if(p_ext ->get_byte_func(&cur_data) == X_True)
	{
		INSERT(LogDebug)(LORA_REC_DEBUG,(" lora rec %2x \r\n",cur_data));
		if(p_ext ->recv_length >= MAX_LORA_CONFIG_DATA_LENGTH) {return LC_Error;}
		p_ext ->recv_data[p_ext ->recv_length++] = cur_data;
		if(p_ext ->recv_length >= p_ext ->length)
		{
			if(p_ext ->evt_handle == X_Null) {return LC_Finished;}
			if(p_ext ->evt_handle(CSRE_RecvCheck,p_ext ->recv_data,p_ext ->recv_length,p_ext ->data,p_ext ->length) == X_True)
			{
				p_ext ->evt_handle(CSRE_Successed,p_ext ->recv_data,p_ext ->recv_length,p_ext ->data,p_ext ->length);
				return LC_Finished;
			}
			else
			{
				p_ext ->state_backup = LC_SendCMD;
				p_ext ->set_time_func(MODE_SWITCH_TIME_MS);
				return LC_Wait;
			}
		}
	}
	return p_this ->current_state;
}
/********************************************************************************************
5
********************************************************************************************/
StateNumber	LC_FinishedAction(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_Finished,("------------------ LC_FinishedAction\r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	p_ext ->error_code = LP_Finished;
	return p_this ->current_state;
}
/********************************************************************************************
6
********************************************************************************************/
StateNumber	LC_ErrorAction(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_Error,("------------------ LC_ErrorAction\r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(p_ext ->evt_handle != X_Null)
	{
		p_ext ->evt_handle(CSRE_Error,p_ext ->recv_data,p_ext ->recv_length,p_ext ->data,p_ext ->length);
		p_ext ->evt_handle = X_Null;
	}
	p_ext ->error_code = LP_Error;
	return p_this ->current_state;
}
/********************************************************************************************
7
********************************************************************************************/
StateNumber	LC_GetConfigAction(s_StateMachineParam *p_this)
{
////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_GetConfig,("------------------ LC_GetConfigAction\r\n"));
	uint8_t temp_buf[3];
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	sLoraCommand *p_cmd = (sLoraCommand*)p_ext ->data;
	
	temp_buf[0] = READ_CMD;
	temp_buf[1] = p_cmd ->start_addr;
	temp_buf[2] = p_cmd ->length;
	p_ext ->clear_rec_cache_func();
	if(p_ext ->send_buf_func(temp_buf,3) != X_True) {return LC_SendCMD;}

	p_ext ->set_time_func(WAIT_RESPONSE_TIME_MS);
	
	p_ext ->recv_length = 0;
	return LC_Compare;
	
}
/********************************************************************************************
8
********************************************************************************************/
StateNumber	LC_CompareAction(s_StateMachineParam *p_this)
{
//////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_Compare,("------------------ LC_CompareAction\r\n"));
	uint8_t cur_data;
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	sLoraCommand *p_cmd = (sLoraCommand*)p_ext ->data;
	if(p_ext ->get_time_func() == 0) 
	{
		return LC_SendCMD;
	}
	
	if(p_ext ->get_byte_func(&cur_data) == X_True)
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
					p_ext ->evt_handle(CSRE_Successed,p_ext ->recv_data,p_ext ->recv_length,p_ext ->data,p_ext ->length);
					return LC_Finished;
				}
			
				p_ext ->state_backup = LC_SendCMD;
				p_ext ->set_time_func(MODE_SWITCH_TIME_MS);
				return LC_Wait;
		}
	}
	return p_this ->current_state;
	
}
/********************************************************************************************
9
********************************************************************************************/
StateNumber LC_AuxFreeIntervalAction(s_StateMachineParam *p_this)
{
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LC_AuxFreeInterval,("------------------ LC_AuxFreeIntervalAction \r\n"));
	sLoraConfigParamExtern *p_ext = (sLoraConfigParamExtern*)p_this;
	if(p_ext ->get_time_func() == 0) {return p_ext ->state_backup;}
	return p_this ->current_state;
}
/********************************************************************************************

********************************************************************************************/

/*****************************************************************************************************/
#define MAX_LORA_CHANNEL                  83
#define LORA_CONFIG_ERROR_HANDLE          mStateMachineStateSet(p_manager ->lora_config_manager,LC_Error);return

#define REG_0_ADDR_HIGH(addr)  ((uint8_t)(addr >> 8))
#define REG_1_ADDR_LOW(addr)   ((uint8_t)(addr))

#define REG_2_BPS(baud_rate,p_result)   if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
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

#define REG_2_CHECK_BITES(check_bits,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
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
																
#define REG_2_AIR_SPEED(air_speed,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(air_speed)   \
																{    \
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
																
																
#define REG_3_SUBPACKAGE(subpackage,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(subpackage)   \
																{    \
																	case 200:  \
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
																
#define REG_3_RSSI_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x20;}  \
													else {*p_result = 0;}

#define REG_3_TRANS_POWER(power,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
																switch(power)   \
																{    \
																	case TP_22dbm:  \
																		*p_result = 0; \
																		break;   \
																	case TP_17dbm:  \
																		*p_result = 0x01; \
																		break;   \
																	case TP_13dbm:  \
																		*p_result = 0x02; \
																		break;   \
																	case TP_10dbm:  \
																		*p_result = 0x03; \
																		break;   \
																	default:  \
																		LORA_CONFIG_DEBUG_METHOD(1,("trans power %d is wrong !!!\r\n",power)); \
																		LORA_CONFIG_ERROR_HANDLE; \
																}
																
#define REG_4_CHANNEL(channel) (channel > MAX_LORA_CHANNEL) ? 0 : channel;
																
#define REG_5_RSSI_BYTE_REPORT_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x80;}  \
													else {*p_result = 0;}

#define REG_5_TRANS_MODE(mode,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(mode == TM_transparent) {*p_result = 0;}  \
													else {*p_result = 0x40;}
													
#define REG_5_LBT_ENABLE(isEnable,p_result) if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
													if(isEnable == X_True) {*p_result = 0x10;}  \
													else {*p_result = 0;}

#define REG_5_WOR_PERIOD(ms,p_result)   if(p_result == X_Null) {LORA_CONFIG_ERROR_HANDLE;}  \
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
																
#define REG_6_SECRET_KEY_HIGH(key)  ((uint8_t)(key >> 8))
#define REG_7_SECRET_KEY_LOW(key)   ((uint8_t)(key))
/*****************************************************************************************************/
X_Void mFunc_MultiLoraConfigReset(const sLoraFuncManager *p_manager,sLoraConfig * p_config,uint8_t retry_times,lora_config_event_handler handler)
{
	uint8_t cmd[MAX_LORA_CONFIG_DATA_LENGTH];
	uint8_t temp1,temp2,temp3,temp4;
	
	if(p_manager == (sLoraFuncManager *)0) {return;}
	
	mStateMachineStateSet(p_manager ->lora_config_manager,LC_Idle);
	p_manager ->lora_config_param->evt_handle = handler;
	p_manager ->lora_config_param->error_code = LP_InProcess;
	
	if(p_config == (sLoraConfig *)0) 
	{
		LORA_CONFIG_ERROR_HANDLE;
	}
	
	cmd[0]  = SET_CMD;
	cmd[1]  = 0; // start addr 0;
	cmd[2]  = 8; // reg from 0 ~ 7
	
	cmd[3]  = REG_0_ADDR_HIGH(p_config->module_addr);
	cmd[4]  = REG_1_ADDR_LOW(p_config->module_addr);
	
						REG_2_BPS(p_config->baud_rate,&temp1);
						REG_2_CHECK_BITES(p_config->check_bits,&temp2);
						REG_2_AIR_SPEED(p_config ->air_speed,&temp3);
	cmd[5]  = temp1|temp2|temp3;
	
						REG_3_SUBPACKAGE(p_config ->subpackage_length,&temp1);
						REG_3_RSSI_ENABLE(p_config ->isRssiEnable,&temp2);
						REG_3_TRANS_POWER(p_config ->trans_power,&temp3);
	cmd[6]  = temp1|temp2|temp3;
	
	cmd[7]  = REG_4_CHANNEL(p_config ->channel);
	
						REG_5_RSSI_BYTE_REPORT_ENABLE(p_config ->isRssiByteEnable,&temp1);
						REG_5_TRANS_MODE(p_config ->trans_mode,&temp2);
						REG_5_LBT_ENABLE(p_config ->isLBT_Enable,&temp3);
						REG_5_WOR_PERIOD(p_config ->WOR_period_ms,&temp4);
	cmd[8]  = temp1|temp2|temp3|temp4;

	cmd[9]  = REG_6_SECRET_KEY_HIGH(p_config ->secret_key);
	cmd[10] = REG_7_SECRET_KEY_LOW(p_config ->secret_key);

	CopyBuffer(cmd,p_manager ->lora_config_param->data,MAX_LORA_CONFIG_DATA_LENGTH);
	p_manager ->lora_config_param->length = MAX_LORA_CONFIG_DATA_LENGTH;
	p_manager ->lora_config_param->retry_times = retry_times;
	
	LORA_CONFIG_DEBUG_METHOD(1,("the config command is 0xc0 0x00 0x08 0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x  0x%2x\r\n"
												,cmd[3],cmd[4],cmd[5],cmd[6],cmd[7],cmd[8],cmd[9],cmd[10]));
}
eLoraProcess mFunc_MultiLoraConfigHandle(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return LP_Error;}
	if(*p_manager ->p_isInitOK != X_True) {return LP_Error;}
	mStateMachineRun(p_manager ->lora_config_manager,&p_manager ->lora_config_param ->base,LC_Recorder);
	return p_manager ->lora_config_param->error_code;
}

/*****************************************************************************************************/
/*****************************************************************************************************/
typedef enum
{
	LGR_Idle,
	LGR_Wait,
	LGR_SendCMD,
	LGR_GetResponse,
	LGR_Finished,
	LGR_Error,
}eLoraGetRssi;

/********************************************************************************************
0
********************************************************************************************/
StateNumber LGR_IdleAction(s_StateMachineParam *p_this)
{
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    p_ext ->error_code = LP_InProcess;
    p_ext ->recv_length = 0;
    
    p_ext ->set_time_func(3000);
    return LGR_Wait;
}
/********************************************************************************************
1
********************************************************************************************/
StateNumber LGR_WaitAction(s_StateMachineParam *p_this)
{
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
    	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    if(p_ext ->get_time_func() == 0) {return LGR_Error;}
    
    if(p_ext ->does_busy_func() == X_False) 
    {
        p_ext ->recv_length = 0;
        return LGR_SendCMD;
    }
    
	return p_this ->current_state;
}
/********************************************************************************************
2
********************************************************************************************/
StateNumber LGR_SendCMDAction(s_StateMachineParam *p_this)
{
    uint8_t Rssi_Cmd[6] = {0xc0,0xc1,0xc2,0xc3,0,2};
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    p_ext ->clear_rec_cache_func();
    p_ext ->send_buf_func(Rssi_Cmd,6);
    p_ext ->set_time_func(1000);
	return LGR_GetResponse;
}
/********************************************************************************************
3
********************************************************************************************/
StateNumber LGR_GetResponseAction(s_StateMachineParam *p_this)
{
    uint8_t cur_data;
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    
	if(p_ext ->get_time_func() == 0) 
	{
		return LGR_Error;
	}
	
	if(p_ext ->get_byte_func(&cur_data) == X_True)
	{
//////        SysLogDebug(1,(" RSSI : lora rec %2x \r\n",cur_data));
		INSERT(LogDebug)(LORA_REC_DEBUG,(" RSSI : lora rec %2x \r\n",cur_data));
		if(p_ext ->recv_length >= RSSI_RESPONSE_LENGTH) {return LGR_Error;}
		p_ext ->recv_data[p_ext ->recv_length++] = cur_data;
		if(p_ext ->recv_length >= RSSI_RESPONSE_LENGTH)
		{
			if(p_ext ->recv_data[0] == 0xC1 && p_ext ->recv_data[2] == 0x02)
			{
				p_ext ->cur_env_rssi = 256 - p_ext ->recv_data[3];
                p_ext ->cur_recv_rssi = 256 - p_ext ->recv_data[4];
				return LGR_Finished;
			}
            else
            {
                return LGR_Error;
            }
		}
	}
	return p_this ->current_state;
}
/********************************************************************************************
4
********************************************************************************************/
StateNumber LGR_FinishedAction(s_StateMachineParam *p_this)
{
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    p_ext ->error_code = LP_Finished;
	return p_this ->current_state;
}
/********************************************************************************************
5
********************************************************************************************/
StateNumber LGR_ErrorAction(s_StateMachineParam *p_this)
{
    ////////	INSERT(LogDebugOnce)(LORA_CONFIG_STATE_DEBUG,p_once,LGR_Idle,("------------------ LGR_IdleAction \r\n"));
	sLoraRssiParamExtern *p_ext = (sLoraRssiParamExtern*)p_this;
    p_ext ->error_code = LP_Error;
	return p_this ->current_state;
}
/********************************************************************************************
********************************************************************************************/
static X_Void LR_Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
eLoraProcess mFunc_MultiLoraRssiHandle(const sLoraFuncManager *p_manager)
{
    if(p_manager == (sLoraFuncManager *)0) {return LP_Error;}
	if(*p_manager ->p_isInitOK != X_True) {return LP_Error;}
	mStateMachineRun(p_manager ->lora_rssi_manager,&p_manager ->lora_rssi_param ->base,LR_Recorder);
	return p_manager ->lora_rssi_param->error_code;
}

X_Void mFunc_MultiLoraGetLatestRssi(const sLoraFuncManager *p_manager,uint16_t *p_env_rssi,uint16_t * p_recv_rssi)
{
    if(p_env_rssi == (uint16_t *)0 || p_recv_rssi == (uint16_t *)0) {return;}
    if(p_manager == (sLoraFuncManager *)0) 
    {
        * p_env_rssi = ERROR_LORA_RSSI_FLAG;
        * p_recv_rssi = ERROR_LORA_RSSI_FLAG;
        return;
    }
    
    * p_env_rssi  = p_manager ->lora_rssi_param ->cur_env_rssi;
    * p_recv_rssi = p_manager ->lora_rssi_param ->cur_recv_rssi;
}
/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/


#define LORA_VALID_DATA_START_ADDR             LORA_LENGTH_SIZE_IN_BYTE
#define LORA_STATE_CHECK_TIME_MS               500
#define LORA_RECV_MAX_INTERVAL_TIME_IN_MS      100
#define LORA_SEND_INTERVAL_TIME_IN_MS          30  // 防止粘包，未经严格验证，或许还能更小，大概率是发送模块粘包了 ,或许应该根据发送的数据长度动态调节这个参数
////////////////#define LORA_SEND_WAIT_TIME_IN_MS_WITHOUT_AUX  100
#define LORA_SEND_WAIT_TIME_IN_MS_WITH_AUX     500
#define LORA_AUX_STABLE_TIME_MS                20


INSERT(LOG_ONCE_ENTRY_DEF)(p_once_sr,100);
//////////INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);

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
    SRS_ImmediatelySend, // immediately send when recv callback OK
////////////    SRS_ImmediatelyRecv, // immediately recv when send callback OK
}eSendRecvState;

static X_Void mFunc_MultiLora_CurRecvProcessTerminate(sSendRecvParamExtern *p_ext);
static X_Boolean DoesSameSendRequestAgain(const s_StateMachine *p_manager);
static X_Void SR_Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
														
static X_Boolean DoesLoraSend(sSendRecvParamExtern *p_ext)
{
	uint16_t buf_num;
	if(p_ext == X_Null) {return X_False;}
	if(DoesLoopQueueEmpty(p_ext ->p_send_queue) == X_True){return X_False;}
	
	buf_num = LoopQueueFirstOut(p_ext ->p_send_queue);
	if(buf_num < p_ext ->send_cache_size)
	{
		
		if(p_ext ->p_send_param[buf_num].send_length > MAX_LORA_SEND_DATA_LENGTH)
		{
			RealseLoopQueueBuf(p_ext ->p_send_queue,buf_num);
			return X_False;
		}
		
		CopyBuffer(&(p_ext ->p_send_param[buf_num].send_buf[0]),
				p_ext  ->cur_send.cur_send_buf,
				p_ext  ->p_send_param[buf_num].send_length);
		
////////////		INSERT(LogDebug)(1,(" ~~~!!!~~~ %2x %2x %2x ; buf_num : %d\r\n"
////////////											,p_ext ->cur_send.p_data[0]
////////////											,p_ext ->cur_send.p_data[1]
////////////											,p_ext ->cur_send.p_data[2]
////////////											,buf_num));
		
		p_ext ->cur_send.length               = p_ext ->p_send_param[buf_num].send_length;
		p_ext ->cur_send.send_cb              = p_ext ->p_send_param[buf_num].send_cb;
		p_ext ->cur_recv.rec_time_limit_in_ms = p_ext ->p_send_param[buf_num].recv.rec_time_limit_in_ms;
		p_ext ->cur_recv.recv_cb              = p_ext ->p_send_param[buf_num].recv.recv_cb;

		RealseLoopQueueBuf(p_ext ->p_send_queue,buf_num);
		return X_True;
	}
	return X_False;
}

/**********************************************************************************************************************
	0		
**********************************************************************************************************************/
StateNumber  SRS_IdleAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_Idle,("------------------ SRS_IdleAction\r\n"));
	p_ext ->isScanInProcess = X_False;
	p_ext ->isBusy  = X_True;
	p_ext ->set_mode_func(LM_Trans);
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
StateNumber	SRS_NormalAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_Normal,("------------------ SRS_NormalAction\r\n"));

	if(p_ext ->get_byte_func(&cur_data) == X_True)
	{
		
		p_ext ->cur_recv.cur_recv_buf[0] = cur_data;
		p_ext ->cur_recv.recv_length = 1;
		
		p_ext ->isAuxBusyBackup = X_True;
		
		if(p_ext ->cur_recv.rec_time_limit_in_ms != 0) {p_ext ->timer_backup = p_ext ->get_time_func();}
		
		return SRS_GetData;
	}

	if(p_ext ->cur_recv.rec_time_limit_in_ms != 0)
	{
		if(p_ext ->get_time_func() == 0)
		{
			if(p_ext ->cur_recv.recv_cb != X_Null) {p_ext ->cur_recv.recv_cb(CSRE_Timeout,p_ext ->cur_recv.cur_recv_buf,0,0);}
			// 本次接受失败后用户没有在回调函数recv_cb中send again.代表整个收发流程彻底失败，可以清除相关参数了
			if(DoesSameSendRequestAgain(p_ext ->lora_sr_manager) == X_False) 
			{
				mFunc_MultiLora_CurRecvProcessTerminate(p_ext);
			}
		}
		p_ext ->isBusy = X_True;
		return p_this ->current_state;// still wait for response , can not start another send routine
	}

	if(DoesLoraSend(p_ext) == X_True)
	{
		p_ext ->isBusy = X_True;
        p_ext ->isAuxBusyBackup = X_True;
		p_ext ->set_time_func(LORA_SEND_WAIT_TIME_IN_MS_WITH_AUX);
		return SRS_SendData;
	}
	p_ext ->isBusy = X_False;
	return p_this ->current_state;
	
}
/**********************************************************************************************************************
	2		
**********************************************************************************************************************/
StateNumber	SRS_SendDataAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;

	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_SendData,("------------------ SRS_SendDataAction\r\n"));
	
    p_ext ->isAuxBusy = p_ext ->does_busy_func();
    
	if(p_ext ->isAuxBusy == X_True) 
	{
        p_ext ->isAuxBusyBackup = X_True;
		if(p_ext ->get_time_func() == 0) 
		{
			if(p_ext ->cur_send.send_cb != X_Null)
			{
				p_ext ->cur_send.send_cb(CSRE_Timeout,p_ext ->cur_send.cur_send_buf,p_ext ->cur_send.length,0);
			}
			/****************************************************************************/
			// 发送都没发出去，更没有回应了。但是这里为什么不清除接收相关的参数呢？
			// 因为接受超时后还需要触发recv_cb，并且用户有可能在recv_cb中再次要求send again。
			// 因此发送和接受的参数在一个完整的收发流程彻底失败之前都要保留
////////				p_ext ->cur_recv.rec_time_limit_in_ms = 0; 
////////				p_ext ->cur_recv.recv_cb = X_Null;
			/****************************************************************************/
			INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ send failed once\r\n"));

			if(p_ext ->isContinueWaitRecWhenSendFailed == X_False)
			{
				if(DoesSameSendRequestAgain(p_ext ->lora_sr_manager) == X_False) // not send again , the send process end 
				{
					mFunc_MultiLora_CurRecvProcessTerminate(p_ext);
				}
			}
			return SRS_Normal;
		}
		return p_this ->current_state;
	}
	
	if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_True)
	{
		p_ext ->isAuxBusyBackup = X_False;
		p_ext ->set_time_func(LORA_AUX_STABLE_TIME_MS);	
        return p_this ->current_state;
	}
	
	if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_False)
	{
		if(p_ext ->get_time_func() > 0) {return p_this ->current_state;}
	}
	
	p_ext->send_buf_func(p_ext ->cur_send.cur_send_buf,p_ext ->cur_send.length);
	
	if(p_ext ->cur_send.send_cb != X_Null)
	{
		p_ext ->cur_send.send_cb(CSRE_Successed,p_ext ->cur_send.cur_send_buf,p_ext ->cur_send.length,0);
	}
	
	INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ aux free,lora just send %d bytes : %2x %2x %2x %2x \r\n"
											,p_ext ->cur_send.length
											,p_ext ->cur_send.cur_send_buf[0]
											,p_ext ->cur_send.cur_send_buf[1]
											,p_ext ->cur_send.cur_send_buf[2]
											,p_ext ->cur_send.cur_send_buf[3]));
	
	if(p_ext ->cur_recv.rec_time_limit_in_ms != 0)
	{
		p_ext->set_time_func(p_ext ->cur_recv.rec_time_limit_in_ms);
		return SRS_Normal;
	}

	p_ext->set_time_func(LORA_SEND_INTERVAL_TIME_IN_MS);
	return SRS_SendInterval;
}


/**********************************************************************************************************************
	3	  
**********************************************************************************************************************/
StateNumber	SRS_GetDataAction(s_StateMachineParam *p_this)
{
	X_Boolean isRecvFinished = X_False;
	uint8_t cur_data;
	uint16_t buf_num;
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;
	
	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_GetData,("------------------ SRS_GetDataAction\r\n"));

	if(p_ext ->get_byte_func(&cur_data) == X_True)
	{
		p_ext ->cur_recv.cur_recv_buf[p_ext ->cur_recv.recv_length ++] = cur_data;
		if(p_ext ->cur_recv.recv_length >= MAX_LORA_RECV_DATA_LENGTH){return SRS_Normal;}
		return p_this->current_state;
	}
	
	
	p_ext ->isAuxBusy = p_ext ->does_busy_func();
	
	if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_True)
	{
		p_ext ->isAuxBusyBackup = X_False;
		p_ext ->set_time_func(LORA_AUX_STABLE_TIME_MS);		
	}
	
	if(p_ext ->isAuxBusy == X_False && p_ext ->isAuxBusyBackup == X_False)
	{
		if(p_ext ->get_time_func() == 0) {isRecvFinished = X_True;}
	}
	
	if(p_ext ->isAuxBusy == X_True) {p_ext ->isAuxBusyBackup = X_True;}

	
	if(isRecvFinished == X_True)
	{
		if(p_ext ->cur_recv.rec_time_limit_in_ms != 0 && p_ext ->timer_backup != 0) 
		{
			p_ext ->set_time_func(p_ext ->timer_backup);
		}
		
		if( p_ext ->cur_recv.rec_time_limit_in_ms != 0 && p_ext ->cur_recv.recv_cb != X_Null)
		{
			if(p_ext ->cur_recv.recv_cb(CSRE_RecvCheck,p_ext ->cur_recv.cur_recv_buf,p_ext ->cur_recv.recv_length,0) == X_True)
			{
				p_ext ->cur_recv.recv_cb(CSRE_Successed,p_ext ->cur_recv.cur_recv_buf,p_ext ->cur_recv.recv_length,0);
				mFunc_MultiLora_CurRecvProcessTerminate(p_ext);
				return SRS_ImmediatelySend;
			}
		}
		
		p_ext ->isScanInProcess = X_True;
		buf_num = LoopQueueFirstIn(p_ext ->p_recv_queue,X_False);
		if(buf_num >= p_ext ->recv_cache_size)
		{
			p_ext ->isScanInProcess = X_False;
			INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ----  lora GetMessage unexpected error occur 1\r\n"));
			if(p_ext ->cur_recv.recv_cb != X_Null) {p_ext ->cur_recv.recv_cb(CSRE_Error,p_ext ->cur_recv.cur_recv_buf,p_ext ->cur_recv.recv_length,0);}
			return SRS_Normal;
		}
		
		
		CopyBuffer(&p_ext ->cur_recv.recv_length,
					&p_ext ->p_recv_array_buf[buf_num*(MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE)],
					LORA_LENGTH_SIZE_IN_BYTE);
		CopyBuffer(p_ext ->cur_recv.cur_recv_buf,
					&p_ext ->p_recv_array_buf[buf_num*(MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE) + LORA_VALID_DATA_START_ADDR],
					p_ext ->cur_recv.recv_length);
		
		p_ext ->isScanInProcess = X_False;
		
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
StateNumber  SRS_ResetAction(s_StateMachineParam *p_this)
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
StateNumber SRS_SendIntervalAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;
	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_SendInterval,("------------------ SRS_SendIntervalAction\r\n"));

	if(p_ext -> get_time_func() == 0)
	{
		return SRS_Normal;
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************
    6
**********************************************************************************************************************/
StateNumber SRS_ImmediatelySendAction(s_StateMachineParam *p_this)
{
	sSendRecvParamExtern *p_ext = (sSendRecvParamExtern*)p_this;
	INSERT(LogDebugOnce)(LORA_SR_STATE_DEBUG,p_once_sr,SRS_ImmediatelySend,("------------------ SRS_ImmediatelySendAction\r\n"));

    if(DoesLoraSend(p_ext) == X_True)
	{
		p_ext ->isBusy = X_True;
		p_ext ->set_time_func(LORA_SEND_WAIT_TIME_IN_MS_WITH_AUX);
		return SRS_SendData;
	}
    
	return SRS_Normal;
}
/**********************************************************************************************************************
	
**********************************************************************************************************************/

/**********************************************************************************************************************
	
**********************************************************************************************************************/


static X_Void SendParamClear( sSendRecvParamExtern * const p_sr_param)
{
	uint8_t i;
	
	if(p_sr_param == (sSendRecvParamExtern *)0) {return;}
	LoopQueueInitialize(p_sr_param ->p_send_queue);
	p_sr_param ->cur_send.send_cb = (cp_send_rec_cb)0;
	for(i=0;i<p_sr_param ->send_cache_size;i++)
	{
		p_sr_param  ->p_send_param[i].send_length 	= 0;
		p_sr_param  ->p_send_param[i].send_cb     	= (cp_send_rec_cb)0;
		p_sr_param  ->p_send_param[i].recv.recv_cb	= (cp_send_rec_cb)0;
		p_sr_param  ->p_send_param[i].recv.rec_time_limit_in_ms = 0;
	}
}
static X_Void RecvParamClear( sSendRecvParamExtern * const p_sr_param)
{
	if(p_sr_param == (sSendRecvParamExtern *)0) {return;}
	LoopQueueInitialize(p_sr_param ->p_recv_queue);
	
	p_sr_param ->cur_recv.recv_cb                = (cp_send_rec_cb)0;
	p_sr_param ->cur_recv.rec_time_limit_in_ms   = 0;
	p_sr_param ->cur_recv.recv_length            = 0;
	p_sr_param ->isContinueWaitRecWhenSendFailed = X_False;
	p_sr_param ->isScanInProcess                 = X_False;
}


X_Void mFunc_MultiLora_SendRecvReset(const sLoraFuncManager *p_manager)
{
	SendParamClear(p_manager ->lora_sr_param);
	RecvParamClear(p_manager ->lora_sr_param);
	p_manager ->lora_sr_param ->isBusy = X_True;
	p_manager ->lora_sr_param ->timer_backup = 0;
	mStateMachineStateSet(p_manager ->lora_sr_manager,SRS_Idle);
	
	p_manager ->lora_sr_param ->set_mode_func(LM_Trans);
}
X_Void mFunc_MultiLora_SendRecvHandle(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return ;}
	if(*p_manager ->p_isInitOK != X_True) {return ;}
	mStateMachineRun(p_manager ->lora_sr_manager,&p_manager ->lora_sr_param ->base,SR_Recorder);
}
static X_Boolean DoesSameSendRequestAgain(const s_StateMachine *p_manager)
{
	return (mStateMachineGetSuddenChangeState(p_manager) == SRS_SendData);
}
X_Void mFunc_MultiLora_SendAgain(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return ;}
	p_manager ->lora_sr_param ->isBusy = X_True;
	p_manager ->lora_sr_param ->set_time_func(LORA_STATE_CHECK_TIME_MS);
	mStateMachineStateSet(p_manager ->lora_sr_manager,SRS_SendData);
}

static X_Void mFunc_MultiLora_CurRecvProcessTerminate(sSendRecvParamExtern *p_ext)
{
	if(p_ext == (sSendRecvParamExtern *)0) {return ;}
	p_ext ->cur_recv.recv_cb              = (cp_send_rec_cb)0;
	p_ext ->cur_recv.rec_time_limit_in_ms = 0;
}

X_Void mFunc_MultiLora_EnableContinueReceive(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return ;}
	p_manager ->lora_sr_param ->isContinueWaitRecWhenSendFailed = X_True;
}
X_Void mFunc_MultiLora_DisableContinueReceive(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return ;}
	p_manager ->lora_sr_param ->isContinueWaitRecWhenSendFailed = X_False;
}
X_Boolean mFunc_MultiLora_SendRequest(const sLoraFuncManager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb
											,uint16_t rec_time_in_ms,cp_send_rec_cb p_recv_cb)
{

	uint16_t buf_num;
	if(p_manager == (sLoraFuncManager *)0) {return X_False;}
	if(p_data == X_Null || length > MAX_LORA_SEND_DATA_LENGTH) {return X_False;}

	buf_num = LoopQueueFirstIn(p_manager ->lora_sr_param ->p_send_queue,X_False);

	if(buf_num < p_manager ->lora_sr_param ->send_cache_size)
	{
		p_manager ->lora_sr_param ->p_send_param[buf_num].send_length               = length;
		CopyBuffer(p_data,p_manager ->lora_sr_param ->p_send_param[buf_num].send_buf,length);
		p_manager ->lora_sr_param ->p_send_param[buf_num].send_cb                   = p_send_cb;
		p_manager ->lora_sr_param ->p_send_param[buf_num].recv.rec_time_limit_in_ms = rec_time_in_ms;
		p_manager ->lora_sr_param ->p_send_param[buf_num].recv.recv_cb              = p_recv_cb;
		
		INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ Lora get a send request : length %d :\r\n",length));
//////////////			INSERT(LogDebug)(1,(" !!! %2x %2x %2x ; buf_num : %d\r\n"
//////////////											,send_param[buf_num].send_buf[0]
//////////////											,send_param[buf_num].send_buf[1]
//////////////											,send_param[buf_num].send_buf[2]
//////////////											,buf_num));

		return X_True;
	}
	INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ Lora get a send request but no buf to store it !!!\r\n"));
	return X_False;
}
X_Boolean mFunc_MultiLora_DoesNewDataCome(const sLoraFuncManager *p_manager,uint8_t **pp_data,uint16_t *p_length)
{
	uint16_t length,buf_num;
	if(p_manager == (sLoraFuncManager *)0) {return X_False;}
	if(pp_data == X_Null || p_length == X_Null) {return X_False;}
	if(p_manager ->lora_sr_param ->isScanInProcess == X_True) {return X_False; }
	if(DoesLoopQueueEmpty(p_manager ->lora_sr_param ->p_recv_queue) == X_True){return X_False;}
	buf_num = LoopQueueFirstOut(p_manager ->lora_sr_param ->p_recv_queue);
	if(buf_num < p_manager ->lora_sr_param ->recv_cache_size)
	{
		*pp_data = &p_manager ->lora_sr_param  ->p_recv_array_buf[(buf_num * (MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE))+ LORA_VALID_DATA_START_ADDR];
		CopyBuffer(&p_manager ->lora_sr_param  ->p_recv_array_buf[buf_num * (MAX_LORA_RECV_DATA_LENGTH + LORA_LENGTH_SIZE_IN_BYTE)],&length,LORA_LENGTH_SIZE_IN_BYTE);
//////////////		INSERT(LogDebug)(BC95_CLOUD_SCAN_DEBUG,(" ~~~~~~~~NewCommandCome~~~~ %d ; %2x %2x %2x\r\n",length,*pp_data[0],*pp_data[1],*pp_data[2] ));
		RealseLoopQueueBuf(p_manager ->lora_sr_param ->p_recv_queue,buf_num); 
		if(length == 0)  {return X_False;}
		*p_length = length;
		
		return X_True;
	}
	return X_False;
}

X_Boolean mFunc_Does_MultiLoraModuleBusy(const sLoraFuncManager *p_manager)
{
	if(p_manager == (sLoraFuncManager *)0) {return X_False;}
	return (p_manager ->lora_sr_param ->isBusy);
}

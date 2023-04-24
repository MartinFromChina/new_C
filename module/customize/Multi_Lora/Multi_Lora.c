#include "Multi_Lora.h"
#include "../../common/StateMachine/StateMachine.h"
#include "../../common/AppCommon.h"
//////////#include "mFunc_SoftTimer.h"

/*********************************************************************************
*********************************************************************************/
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

#define RETRY_TIMES_FOR_LORA_CONFIG        5
//////////#if (USE_INSERT_DEBUG != 0)
//////////static StateNumber latest_state = 0;
//////////#endif

typedef enum
{
	LORA_Idle = 0,											/*  */
	LORA_HardWareSelfCheck,							
	/************************************************************************/
	LORA_NormalProcess,									/*  */     
	/************************************************************************/
	LORA_Error,													/*  */
	/************************************************************************/
	LORA_Send_Two_Dummy_Frame, // not used anymore 
	LORA_GetRssi,                    
	//LORA_Tempre_Config,             // todo later
    // LORA_PowerDownReset,      // todo later
}eLORA_state;

static X_Void Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
#include <string.h>
static X_Boolean lora_event_handler(e_cp_SendRecEvent evt,uint8_t *p_data,uint16_t length,uint8_t *p_cmd,uint16_t cmd_length)
{
	X_Boolean isOK = X_False;
	if(p_cmd == X_Null) {return X_False;}
	switch(evt)
	{
		case CSRE_Successed:
			SysLogDebug(1,("lora config LE_Successed \r\n"));
			isOK = X_True;
			break;
		case CSRE_Error:
			SysLogDebug(1,("lora config LE_Failed \r\n"));
			break;
		case CSRE_RecvCheck:
			SysLogDebug(1,("lora config LE_Check \r\n"));

			if(p_data != X_Null && p_data[0] == LORA_READ_CMD 
							&& memcmp(&p_data[1],&p_cmd[1],length - 1) == 0)
			{
				isOK = X_True;
			}
			break;
		case CSRE_Timeout:
			SysLogDebug(1,("lora config LE_TimeOut \r\n"));
			break;
		default:
			break;
	}
	
	return isOK;
}
/**********************************************************************************************************************
	0
**********************************************************************************************************************/

StateNumber LORA_IdleAction(s_StateMachineParam *p_this)
{
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG,p_once,LORA_Idle,("------------------ LORA_IdleAction\r\n"));
	sLoraParamExtern *p_ext = (sLoraParamExtern*)p_this;
	
	if(p_ext ->isUseDefaultConfiguration == X_True)
	{
		sLoraConfig config = LORA_CONFIG_DEFAULT_PARAM;
		mFunc_MultiLoraConfigReset(p_ext ->p_lora_func_manager,&config,RETRY_TIMES_FOR_LORA_CONFIG,lora_event_handler);
	}
	else
	{
		mFunc_MultiLoraConfigReset(p_ext ->p_lora_func_manager,&p_ext ->config,RETRY_TIMES_FOR_LORA_CONFIG,lora_event_handler);
	}
	
	p_ext ->isBusy = X_True;
	return LORA_HardWareSelfCheck;
}
/**********************************************************************************************************************
	1		
**********************************************************************************************************************/
StateNumber LORA_HardWareSelfCheckAction(s_StateMachineParam *p_this)
{
	eLoraProcess error_code;
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG,p_once,LORA_HardWareSelfCheck,("------------------LORA_HardWareSelfCheckAction\r\n"));
	sLoraParamExtern *p_ext = (sLoraParamExtern*)p_this;
	
	error_code = mFunc_MultiLoraConfigHandle(p_ext ->p_lora_func_manager);
	
	if(error_code == LP_Finished) 
	{
		mFunc_MultiLora_SendRecvReset(p_ext ->p_lora_func_manager);
//////////////		return LORA_Send_Two_Dummy_Frame;
        return LORA_NormalProcess;
	}
	else if(error_code == LP_Error) 
	{
		if(p_ext -> p_fatal_report != X_Null) {p_ext -> p_fatal_report();}
		if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_ErrorOccur,(uint8_t)MID_LORA,(uint8_t)LORA_HardWareSelfCheck);}/*报告错误*/
		return LORA_Error;
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************
	2	
**********************************************************************************************************************/
StateNumber LORA_NormalProcessAction(s_StateMachineParam *p_this)
{
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG,p_once,LORA_NormalProcess,("------------------LORA_NormalProcessAction\r\n"));
	sLoraParamExtern *p_ext = (sLoraParamExtern*)p_this;
    
    if(p_ext ->isBusy == X_False)
    {
        if(p_ext ->isGetRssiOnce == X_True)
        {
            p_ext ->isGetRssiOnce = X_False;
            return LORA_GetRssi;
        }
    }
    
	mFunc_MultiLora_SendRecvHandle(p_ext ->p_lora_func_manager);
	p_ext ->isBusy = mFunc_Does_MultiLoraModuleBusy(p_ext ->p_lora_func_manager);
    
    
    
	return p_this ->current_state;
}
/**********************************************************************************************************************
	3		
**********************************************************************************************************************/
StateNumber LORA_ErrorAction(s_StateMachineParam *p_this)
{
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG ,p_once,LORA_Error,("------------------LORA_ErrorAction\r\n"));
////////////	if(p_ext -> p_fatal_report != X_Null) {p_ext -> p_fatal_report();}
	return LORA_Idle; // fix it later
}
/**********************************************************************************************************************
	4		  经试验，LORA从配置模式切换回传输模式之后，头两包定点发送请求接收方收不到，不知原因，也未经过大量实验，不深究了，
				直接发两包假数据规避这个问题
**********************************************************************************************************************/
StateNumber LORA_Send_Two_Dummy_FrameAction(s_StateMachineParam *p_this)
{
//////	uint8_t temp_buf[10] = {1,2,3,4,5,6,7,8,9,10};
//////	sLoraParamExtern *p_ext = (sLoraParamExtern*)p_this;
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG ,p_once,LORA_Send_Two_Dummy_Frame,("------------------ LORA_Send_Two_Dummy_FrameAction\r\n"));

////////////////	mFunc_MultiLora_SendRequest(p_ext ->p_lora_func_manager,temp_buf,10,X_Null,0,X_Null);
////////////////	mFunc_MultiLora_SendRequest(p_ext ->p_lora_func_manager,temp_buf,10,X_Null,0,X_Null);
	return LORA_NormalProcess;
}
/**********************************************************************************************************************
	5
**********************************************************************************************************************/
StateNumber LORA_GetRssiAction(s_StateMachineParam *p_this)
{
    uint16_t rssi1,rssi2;
    eLoraProcess error_code;
	INSERT(LogDebugOnce)(LORA_MODULE_DEBUG,p_once,LORA_GetRssi,("------------------ LORA_GetRssiAction\r\n"));
	sLoraParamExtern *p_ext = (sLoraParamExtern*)p_this;
	
	error_code = mFunc_MultiLoraRssiHandle(p_ext ->p_lora_func_manager); // 不加时间限制，由handle函数保证不会陷入无限循环
	
	if(error_code == LP_Finished) 
	{
        if(p_ext ->rssi_report != X_Null)
        {
            mFunc_MultiLoraGetLatestRssi(p_ext ->p_lora_func_manager,&rssi1,&rssi2);
            p_ext ->rssi_report(rssi1,rssi2);
        }
		return LORA_NormalProcess;
	}
	else if(error_code == LP_Error) 
	{
		if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_ErrorOccur,(uint8_t)MID_LORA,(uint8_t)LORA_GetRssi);}/*报告错误*/
        if(p_ext ->rssi_report != X_Null)
        {
            p_ext ->rssi_report(ERROR_LORA_RSSI_FLAG,ERROR_LORA_RSSI_FLAG);
        }
		return LORA_NormalProcess; // 不是严重错误，不返回LORA_Error
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************
	
**********************************************************************************************************************/



uint8_t Multi_Lora_GetNormalStateNumber(X_Void)
{
	return LORA_NormalProcess;
}
X_Void Multi_Lora_FatalErrorReporterReg(const sLoraModuleManager *p_manager,X_Void (*fatal_err_report)(X_Void))
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	p_manager ->p_lora_param ->p_fatal_report = fatal_err_report;
}
X_Boolean Multi_Lora_Init(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	
	p_manager ->p_lora_param ->isUseDefaultConfiguration = X_True;
	p_manager ->p_lora_param ->isBusy                    = X_True;
    p_manager ->p_lora_param ->isGetRssiOnce             = X_False;
	p_manager ->p_lora_param ->event_report              = (evt_reg)0;
	*p_manager ->p_isModuleLocked                        = X_False;
	*p_manager ->p_isInitOK                              = mFunc_MultiLoraInit(p_manager ->p_lora_func_manager);
	
	return X_True;
}
X_Void Multi_Lora_Handle(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	if(*p_manager ->p_isInitOK != X_True || *p_manager ->p_isModuleLocked == X_True) {return;}
	
	mStateMachineRun(p_manager ->lora_manager,&p_manager ->p_lora_param ->base,Recorder);
}

X_Void Multi_Lora_Reset(sLoraConfig * p_config,const sLoraModuleManager *p_manager)
{
	// !!! event report should not be clear here
	if(p_manager == (sLoraModuleManager *)0) {return;}
	p_manager ->p_lora_param ->isBusy = X_True;
	if(p_config != (sLoraConfig*)0)
	{
		p_manager ->p_lora_param ->isUseDefaultConfiguration = X_False;
		CopyBuffer(p_config,&p_manager ->p_lora_param ->config,sizeof(sLoraConfig)/sizeof(uint8_t));
	}
	mStateMachineStateSet(p_manager ->lora_manager,LORA_Idle); // mModule_Lora_Reset will be called at run time ,so reset it to idle state is necessary 
}
X_Void Multi_Lora_Lock(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	*p_manager ->p_isModuleLocked = X_True;
}
X_Void Multi_Lora_Unlock(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	*p_manager ->p_isModuleLocked = X_False;
}
X_Void Multi_Lora_EventRegister(const sLoraModuleManager *p_manager,evt_reg reg)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	if(reg != X_Null)
	{
		p_manager ->p_lora_param ->event_report = reg;
	}
}
X_Void Multi_Lora_EventClear(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	p_manager ->p_lora_param ->event_report = (evt_reg)0;
}

X_Void Multi_Lora_SetNormalStateRequest(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return;}
	mStateMachineStateSet(p_manager ->lora_manager,LORA_NormalProcess);
}
X_Boolean Multi_Lora_SendRequest(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	return mFunc_MultiLora_SendRequest(p_manager ->p_lora_func_manager,p_data,length,X_Null,0,X_Null);
}
X_Boolean Multi_Lora_SendRequestWithCallBack(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	return mFunc_MultiLora_SendRequest(p_manager ->p_lora_func_manager,p_data,length,p_send_cb,0,X_Null);
}
X_Boolean Multi_Lora_SendRequestWithResponse(const sLoraModuleManager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb
																							,uint16_t rec_time_in_ms,cp_send_rec_cb p_recv_cb)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	return mFunc_MultiLora_SendRequest(p_manager ->p_lora_func_manager,p_data,length,p_send_cb,rec_time_in_ms,p_recv_cb);
}
X_Boolean Multi_Lora_DoesNewDataCome(const sLoraModuleManager *p_manager,uint8_t **pp_data,uint16_t *p_length)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	return mFunc_MultiLora_DoesNewDataCome(p_manager ->p_lora_func_manager,pp_data,p_length);
}
X_Boolean Multi_Lora_DoesBusy(const sLoraModuleManager *p_manager)
{
	if(p_manager == (sLoraModuleManager *)0) {return X_False;}
	return p_manager ->p_lora_param ->isBusy;
}

X_Void Multi_Lora_GetRssiRequest(const sLoraModuleManager *p_manager,get_rssi_cb rssi_cb)
{
    if(p_manager == (sLoraModuleManager *)0) {return;}
    p_manager ->p_lora_param ->isGetRssiOnce = X_True;
	p_manager ->p_lora_param ->rssi_report = rssi_cb;
    mStateMachineStateSet(p_manager ->p_lora_func_manager ->lora_rssi_manager,DEFAULT_STATE_NUMBER); // set LGR_Idle state 
}
/**********************************************************************************************************************/

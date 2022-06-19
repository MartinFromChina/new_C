#include  "mModule_Wifi.h"
#include "mFunc_SoftTimer.h"
#include  "mModule.h"
#include "mFunc_ETH_Interface.h"

#include "../../../../../../module/common/StateMachine/StateMachine.h"
#include "../../../../../../module/common/AppCommon.h"
//INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

#include <string.h>

extern sRouterInfo *mApp_SysInfo_GetRouterInfo(X_Void);
extern sIp_Port *mApp_SysInfo_GetIp_Port_Info(X_Void);

INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

#define RETRY_TIMES_FOR_WIFY_AT_COMMAND_TEST    5
#define RETRY_TIMES_FOR_SEND_LENGTH_SET         2
#define MAX_ETH_WAIT_RESPONSE_TIME_IN_MS        30000
#define CHECK_LINK_STATUS_FREQ_IN_MS            5000
#define MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS    30000

#define MAX_SETTING_CHAR_LENGTH                 150

#define ROUTER_PARAM_SET_COMMAND                  "AT+CWJAP="
#define IP_CONNECT_PARAM_COMMAND                  "AT+CIPSTART=\"TCP\","
#define MAX_WIFI_PARAM_LENGTH_IN_CHAR           100

#define WIFI_PARAM_RESPONSE                     "+CWSAP_DEF:\"BigShelf?"

////////extern uint8_t mApp_SysInfo_TerminalNumberGet(X_Void);
typedef X_Boolean (*WaitMethod)(X_Void);

typedef enum
{
	ETH_Idle = 0,
	ETH_CheckConfig, // if the wify module is already configed , no need reconfig it again
	ETH_Config,
	ETH_NormalProcess,
	ETH_Error,
	ETH_TimeOut,
	ETH_TestAT,
	ETH_Wait,
	
	ETH_TcpStart,
	ETH_Restore,// Restore to factory settings
	ETH_ReadSetting, // report the mode , wifi ssid , passedword ...
	ETH_TcpClose,
	ETH_SendReady,
	ETH_Send,
	ETH_CheckLinkStatus,
	ETH_WaitSSID,
	ETH_WaitPassword,
	ETH_ReadyForRouterInfo,
}eETH_state;

typedef struct
{
	s_StateMachineParam     base;
	evt_reg                 event_report;
	WaitMethod              onWait;
	uint8_t                 retry_times;
	uint8_t                 link_error_times;
	uint8_t                 config_index;
	StateNumber             state_to_go_when_successed;
	StateNumber             state_to_go_when_failed;
	X_Boolean               isCommandHaveBeenSend;
	X_Boolean               isHugeSend;
}sParamExtern;

static X_Boolean isRestoreRequest = X_False,isReportRequest = X_False,isReconfigRequest = X_False;
static X_Boolean isReportStateToCaller = X_False;
static sParamExtern sPE;
static X_Boolean isModuleLocked = X_False;
static StateNumber state_backup = ETH_Idle;
static uint8_t setting_read_buf[MAX_SETTING_CHAR_LENGTH],rec_index = 0;
static uint8_t wifi_param_buf[MAX_WIFI_PARAM_LENGTH_IN_CHAR];
static X_Boolean isFinalLinkCheckDone = X_False,isGetTailOnce = X_False;
static sRouterInfo router_info;
static sIp_Port    ip_port_info;
static uint8_t     user_data_input_buf[MAX_SETTING_CHAR_LENGTH],input_length = 0;

static X_Boolean Uint16_ToDecString(uint16_t src,char *result_buf,uint8_t max_result_length)// test it later
{
	if(src > HUGE_BUF_TOTAL_SIZE || result_buf == X_Null || max_result_length <= 5) {return X_False;}
	byteBufInit((uint8_t *)result_buf,max_result_length,'\0');
	sprintf(result_buf, "%d", src);
	return X_True;
}

static X_Boolean RouterParamLoad(uint8_t *p_buf,sRouterInfo *p_info) // modify it later 
{
	uint16_t index,length,total_length;
	if(p_buf == X_Null || p_info == X_Null) {return X_False;}
	if(p_info ->isValid != X_True) {return X_False;}
	
	byteBufInit(wifi_param_buf,MAX_WIFI_PARAM_LENGTH_IN_CHAR,'\0');
	
	index = 0;
	length = strlen(ROUTER_PARAM_SET_COMMAND);
	CopyBuffer((uint8_t *)ROUTER_PARAM_SET_COMMAND,&p_buf[index],length);
	total_length = length ;
	p_buf[total_length ++] = '"';
	
	
	index = total_length ;
	length = strlen(p_info->ssid);
	CopyBuffer((uint8_t *)p_info->ssid,&p_buf[index],length);
	total_length += length;
	p_buf[total_length ++] = '"';
	p_buf[total_length ++] = ',';
	p_buf[total_length ++] = '"';
	
	
	index = total_length ;
	length = strlen(p_info->password);
	CopyBuffer((uint8_t *)p_info->password,&p_buf[index],length);
	total_length += length;
	p_buf[total_length ++] = '"';
	p_buf[total_length ++] = '\r';
	p_buf[total_length ++] = '\n';// fix it later : there is no boundary check 
	
//	INSERT(LogDebug)(1,("total_length = %d ----- %s",total_length,wifi_param_buf));
	return X_True;
}
static X_Boolean Ip_Port_ParamLoad(uint8_t *p_buf,sIp_Port *p_info) // modify it later 
{
	uint16_t index,length,total_length;
	if(p_buf == X_Null || p_info == X_Null) {return X_False;}
	if(p_info ->isValid != X_True) {return X_False;}
	
	byteBufInit(wifi_param_buf,MAX_WIFI_PARAM_LENGTH_IN_CHAR,'\0');
	
	index = 0;
	length = strlen(IP_CONNECT_PARAM_COMMAND);
	CopyBuffer((uint8_t *)IP_CONNECT_PARAM_COMMAND,&p_buf[index],length);
	total_length = length ;
	p_buf[total_length ++] = '"';
	
	
	index = total_length ;
	length = strlen(p_info->ip);
	CopyBuffer((uint8_t *)p_info->ip,&p_buf[index],length);
	total_length += length;
	p_buf[total_length ++] = '"';
	p_buf[total_length ++] = ',';
	
	index = total_length ;
	length = strlen(p_info->port);
	CopyBuffer((uint8_t *)p_info->port,&p_buf[index],length);
	total_length += length;
	p_buf[total_length ++] = '\r';
	p_buf[total_length ++] = '\n';// fix it later : there is no boundary check 
	
//	INSERT(LogDebug)(1,("total_length = %d ----- %s",total_length,wifi_param_buf));
	return X_True;
}
static X_Void Recorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	state_backup = state_going_to_leave;
	if(state_going_to_enter == ETH_NormalProcess) {mFunc_ETH_SendRequestEnable();}
	else if(state_going_to_enter == ETH_CheckLinkStatus) {mFunc_ETH_SendRequestDisable();} // 
}
static X_Boolean AlwaysFalse(X_Void){return X_False;} 
static X_Boolean DoesGetTail(uint8_t *p_data,uint16_t length) // find the "OK\r\n" as tail
{
	if(p_data == X_Null || length < 4) {return X_False;}
	if(p_data[length -4 ] == 0x4f && p_data[length -3 ] == 0x4b && p_data[length -2 ] == 0x0d && p_data[length - 1] == 0x0a) {return X_True;}
	return X_False;
}
static X_Boolean DoesFindCertainCharTCP(uint8_t *p_buf,uint16_t length) // find the "TCP" in buf 
{
	// no need null pointer check , because the caller is me
	uint16_t i;
	for(i=0;i<length - 2;i++)
	{
		if(p_buf[i] == 'T')
		{
				if(p_buf[i + 1] == 'C' && p_buf[i + 2] == 'P') {return X_True;}
		}
	}
	return X_False;
}
static X_Boolean DoesResponseGetAndReport(X_Void)
{
	X_Boolean isOK ;
	isOK = mFunc_ETH_DoesResponseGet();
	if(isOK == X_True)
	{
		if(sPE.event_report != X_Null) {sPE.event_report(MEC_Finished,MID_ETH,state_backup);}
	}
	return isOK;
}
static X_Boolean ETH_WaitSpecifiedResponse(char *_pAckStr, uint16_t wait_time_in_ms,sParamExtern * p_ext
																						,StateNumber state_to_go_successed,StateNumber state_to_go_failed,X_Boolean isReportNeed)
{
	uint32_t len;

	if(_pAckStr == X_Null || p_ext == X_Null || wait_time_in_ms > MAX_ETH_WAIT_RESPONSE_TIME_IN_MS) {return X_False;}
	len = strlen(_pAckStr);
	if (len >= MAX_ETH_RECEIVE_CHAR_LENGTH){return X_False;}

	p_ext ->state_to_go_when_successed = state_to_go_successed;
	p_ext ->state_to_go_when_failed = state_to_go_failed;
	if(mFunc_SoftTimerSetValue(STID_ETH,wait_time_in_ms) == X_False) {return X_False;}
	if(mFunc_SoftTimerSetValue(STID_ETH_Recv,wait_time_in_ms) == X_False) {return X_False;}

	p_ext ->onWait = (isReportNeed == X_True)? DoesResponseGetAndReport : mFunc_ETH_DoesResponseGet;
	return mFunc_ETH_WaitResponseEventRegister(_pAckStr);
}

typedef struct _sWifi_Init
{
	char *							p_command;
	char *							p_response;
	uint16_t 						wait_ms;
}sWifi_Init;

static const sWifi_Init WifiConfigEvent[] = {
	  {"AT+RESTORE"					,"OK\r\n"		,5000		}, 	// restore factory setting , this operation will trigger a restart
		{"Wait"								,"None"			,8000 },
		{"AT+CWMODE=3"				,"OK\r\n"		,300		}, // SoftAP+Station mode
};
static const sWifi_Init TcpStartEvent[] = {
		{"Wait"								,"None"			,2000 		},
////////////	  {"AT+CIPMUX=1"				,"OK\r\n"		,300		}, 	// 
////////////		{"AT+CIPSERVER=1"			,"OK\r\n"		,300		}, // 
////////////	{"AT+CIPSTO=3600"				,"OK\r\n"		,300		}, // Tcp timeout threshold : 3600s
};
/**********************************************************************************************************************
	0
**********************************************************************************************************************/
static StateNumber ETH_IdleAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Idle,("------------------ETH_IdleAction\r\n"));
	
	p_ext ->retry_times = RETRY_TIMES_FOR_WIFY_AT_COMMAND_TEST;
	p_ext ->link_error_times = 0;
	// fix it later: to do : quit the send mode if it is just come from a send error state 
	SysLogDebug(1,(" ---- Wifi module Config Begin \r\n"));
	return ETH_TestAT;
}
/**********************************************************************************************************************
	1
**********************************************************************************************************************/
static StateNumber ETH_CheckConfigAction(s_StateMachineParam *p_this)
{
	uint8_t response_buf[] = {WIFI_PARAM_RESPONSE},length;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_CheckConfig,("------------------ETH_CheckConfigAction\r\n"));
	
	if(isRestoreRequest != X_False || isReportRequest != X_False || isReconfigRequest != X_False)
	{
		return ETH_NormalProcess;
	}
	p_ext ->config_index = 0;
	length = strlen(WIFI_PARAM_RESPONSE);
	if(length == 0 || length > MAX_WIFI_PARAM_LENGTH_IN_CHAR) {return ETH_Error;}
	response_buf[length - 1] = 0;//mApp_SysInfo_TerminalNumberGet();
	response_buf[length - 1] += 49;
	mFunc_ETH_SendAT("AT+CWSAP_DEF?");
	if(ETH_WaitSpecifiedResponse((char *)response_buf
							,2000,p_ext,ETH_TcpStart,ETH_Config,X_False) == X_True){return ETH_Wait;}
	return ETH_Error;
}
/**********************************************************************************************************************
	2
**********************************************************************************************************************/
static X_Void ReadyForUserInput(X_Void)
{
	input_length = 0;
	byteBufInit(user_data_input_buf,MAX_SETTING_CHAR_LENGTH,'\0');
////////	mFun_ETH_ReceiveBufClear();
	mFun_TM_ReceiveLocked();
	mFun_TM_ReceiveClear();
}
static X_Void ClearStateForUserInput(X_Void)
{
	mFun_TM_ReceiveUnocked();
}
static StateNumber ETH_ConfigAction(s_StateMachineParam *p_this)
{
	sRouterInfo * p_router;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Config,("------------------ETH_ConfigAction %d\r\n",p_ext ->config_index));
	
	if(p_ext ->config_index < (sizeof(WifiConfigEvent)/sizeof(WifiConfigEvent[0])))
	{
		if(memcmp("Wait", WifiConfigEvent[p_ext ->config_index].p_command,4) == 0)
		{
			p_ext ->onWait = AlwaysFalse;
			p_ext ->state_to_go_when_failed = ETH_Config;
			mFunc_SoftTimerSetValue(STID_ETH,WifiConfigEvent[p_ext ->config_index].wait_ms);
			
			p_ext ->config_index ++;
			return ETH_Wait;
		}
		
		mFunc_ETH_SendAT(WifiConfigEvent[p_ext ->config_index].p_command);
		if(ETH_WaitSpecifiedResponse(WifiConfigEvent[p_ext ->config_index].p_response,WifiConfigEvent[p_ext ->config_index].wait_ms
									,p_ext,ETH_Config,ETH_TimeOut,X_False) == X_True)
		{
			p_ext ->config_index ++;
			return ETH_Wait;
		}
		return ETH_Error;
	}
	else if(p_ext ->config_index == (sizeof(WifiConfigEvent)/sizeof(WifiConfigEvent[0])))
	{
		p_router = mApp_SysInfo_GetRouterInfo();
		if(RouterParamLoad(wifi_param_buf,p_router) != X_True) 
		{
			SysLogDebug(1,(" current SSID %s or password %s invalid ,please enter SSID in %d sec , use \" OVER\" as tail \r\n"
					,p_router ->ssid,p_router->password,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS/1000));
			
////////			mFunc_AudioStart(AT_Error);
			ReadyForUserInput();
		  byteBufInit((uint8_t *)&router_info,sizeof(router_info)/sizeof(uint8_t),'\0');
			mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
			isReportStateToCaller = X_False;
			return ETH_WaitSSID;
		}
		
		INSERT(LogDebug)(ETH_MODULE_DEBUG,("~~~~~~~~~~~~~------------------going to config ETH param : %s\r\n",wifi_param_buf));
		mFunc_ETH_SendAT((char *)wifi_param_buf);
		if(ETH_WaitSpecifiedResponse("OK\r\n",20000,p_ext,ETH_Config,ETH_TimeOut,X_False) == X_True)
		{
			p_ext ->config_index ++;
			SysLogDebug(1,(" ---- connect router with SSID: %s , password: %s\r\n",p_router ->ssid,p_router->password));
			return ETH_Wait;
		}
		return ETH_Error;
	}
	
	p_ext ->config_index = 0;
	SysLogDebug(1,(" ---- connect router successed \r\n"));
	return ETH_TcpStart;
}
/**********************************************************************************************************************
	3
**********************************************************************************************************************/
static uint8_t *p_normal_data;
static uint16_t normal_length;
static uint8_t *p_huge_data;
static uint16_t huge_length;
static StateNumber ETH_NormalProcessAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_NormalProcess,("------------------ETH_NormalProcessAction\r\n"));
	
	if(isRestoreRequest == X_True)
	{
		isRestoreRequest = X_False;
		return ETH_Restore;
	}
	else if(isReportRequest == X_True)
	{
		isReportRequest = X_False;
		p_ext ->isCommandHaveBeenSend = X_False;
		return ETH_ReadSetting;
	}
	else if(isReconfigRequest == X_True)
	{
		isReconfigRequest = X_False;
		return ETH_TcpClose;
	}
	
	mFun_ETH_LoopReceive();
	
	if(mFun_ETH_DoesHugeBufSend(&p_huge_data,&huge_length) == X_True) // must be called before mFun_ETH_DoesSend so that the huge buf will not be interrupt
	{
		p_ext ->retry_times = RETRY_TIMES_FOR_SEND_LENGTH_SET;
		p_ext ->isHugeSend  = X_True;
		return ETH_SendReady;
	}
	
	if(mFun_ETH_DoesSend(&p_normal_data,&normal_length) == X_True)
	{
		p_ext ->retry_times = RETRY_TIMES_FOR_SEND_LENGTH_SET;
		p_ext ->isHugeSend  = X_False;
		return ETH_SendReady;
	}
	
	return p_this ->current_state;
}
/**********************************************************************************************************************
	4
**********************************************************************************************************************/
static StateNumber ETH_ErrorAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Error,("------------------ETH_ErrorAction\r\n"));
	//fix it later: do some reset 
	if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_ErrorOccur,(uint8_t)MID_ETH,(uint8_t)state_backup);}
	p_ext ->onWait = AlwaysFalse;
	p_ext ->state_to_go_when_failed = ETH_Idle;
	mFunc_SoftTimerSetValue(STID_ETH,1000);
	return ETH_Wait;
}
/**********************************************************************************************************************
	5
**********************************************************************************************************************/
static StateNumber ETH_TimeOutAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_TimeOut,("------------------ETH_TimeOutAction\r\n"));
	if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Timeout,(uint8_t)MID_ETH,(uint8_t)state_backup);}
	
	p_ext ->onWait = AlwaysFalse;
	p_ext ->state_to_go_when_failed = ETH_Idle;
	mFunc_SoftTimerSetValue(STID_ETH,1000);
	return ETH_Wait;
}
/**********************************************************************************************************************
	6
**********************************************************************************************************************/
static StateNumber ETH_TestAT_Action(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_TestAT,("------------------ETH_TestAT_Action\r\n"));
	
	if(p_ext ->retry_times > 0) {p_ext ->retry_times --;}
	else 
	{
		if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_ErrorOccur,(uint8_t)MID_ETH,(uint8_t)ETH_TestAT);}
		return ETH_Error;
	}
	
	mFunc_ETH_SendAT("AT");
	if(ETH_WaitSpecifiedResponse("OK",1000,p_ext,ETH_CheckConfig,ETH_TestAT,X_False) == X_True){return ETH_Wait;}
	return ETH_Error;
}
/**********************************************************************************************************************
	7
**********************************************************************************************************************/
static StateNumber ETH_WaitAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Wait,("------------------ETH_WaitAction\r\n"));
	
	if(p_ext ->onWait == X_Null) {return p_ext ->state_to_go_when_successed;}
	if(p_ext ->onWait() == X_True)
	{
		p_ext ->onWait = (WaitMethod)0;
		return p_ext ->state_to_go_when_successed;
	}
	if(mFunc_SoftTimerGetValue(STID_ETH) > 0)
	{
		return p_this ->current_state;
	}
	return p_ext ->state_to_go_when_failed;
}
/**********************************************************************************************************************
	8
**********************************************************************************************************************/
static StateNumber ETH_TcpStartAction(s_StateMachineParam *p_this)
{
	sIp_Port *p_ip;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_TcpStart,("------------------ETH_TcpStartAction %d\r\n",p_ext ->config_index));
	
	if(p_ext ->config_index < (sizeof(TcpStartEvent)/sizeof(TcpStartEvent[0])))
	{
		if(memcmp("Wait", TcpStartEvent[p_ext ->config_index].p_command,4) == 0)
		{
			p_ext ->onWait = AlwaysFalse;
			p_ext ->state_to_go_when_failed = ETH_TcpStart;
			mFunc_SoftTimerSetValue(STID_ETH,TcpStartEvent[p_ext ->config_index].wait_ms);
			
			p_ext ->config_index ++;
			return ETH_Wait;
		}
		
		mFunc_ETH_SendAT(TcpStartEvent[p_ext ->config_index].p_command);
		if(ETH_WaitSpecifiedResponse(TcpStartEvent[p_ext ->config_index].p_response,TcpStartEvent[p_ext ->config_index].wait_ms
									,p_ext,ETH_TcpStart,ETH_TcpClose,X_False) == X_True)
		{
			p_ext ->config_index ++;
			return ETH_Wait;
		}
		return ETH_Error;
	}
	else if(p_ext ->config_index == (sizeof(TcpStartEvent)/sizeof(TcpStartEvent[0])))
	{
		p_ip = mApp_SysInfo_GetIp_Port_Info();
		if(Ip_Port_ParamLoad(wifi_param_buf,p_ip) != X_True) 
		{
			SysLogDebug(1,(" current IP %s or port %s invalid ,please enter IP in %d sec , use \" OVER\" as tail \r\n"
					,p_ip ->ip,p_ip->port,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS/1000));
			
//////////			mFunc_AudioStart(AT_Error);
			ReadyForUserInput();
		  byteBufInit((uint8_t *)&ip_port_info,sizeof(ip_port_info)/sizeof(uint8_t),'\0');
			mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
			isReportStateToCaller = X_False;
			return ETH_NormalProcess;
		}
		
		INSERT(LogDebug)(ETH_MODULE_DEBUG,("~~~~~~~~~~~~~------------------going to config ETH param : %s\r\n",wifi_param_buf));
		mFunc_ETH_SendAT((char *)wifi_param_buf);
		if(ETH_WaitSpecifiedResponse("OK\r\n",20000,p_ext,ETH_TcpStart,ETH_TimeOut,X_False) == X_True)
		{
			p_ext ->config_index ++;
			SysLogDebug(1,(" ---- connect service with IP: %s , port: %s\r\n",p_ip ->ip,p_ip->port));
			return ETH_Wait;
		}
		return ETH_Error;
	}
	p_ext ->config_index = 0;
	if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Finished,(uint8_t)MID_ETH,(uint8_t)ETH_TcpStart);}
	SysLogDebug(1,(" ---- connect service successed \r\n"));
	return ETH_NormalProcess;
//////////	if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Finished,(uint8_t)MID_ETH,(uint8_t)ETH_TcpStart);}
//////////	p_ext ->link_error_times = 0;
//////////	return ETH_CheckLinkStatus;
}
/**********************************************************************************************************************
	9
**********************************************************************************************************************/
static StateNumber ETH_RestoreAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Restore,("------------------ETH_RestoreAction\r\n"));
	
	mFunc_ETH_SendAT("AT+RESTORE");
	if(ETH_WaitSpecifiedResponse("OK\r\n"
							,1000,p_ext,ETH_Idle,ETH_Error,X_True) == X_True){return ETH_Wait;}
	return ETH_Error;
}
/**********************************************************************************************************************
	10
**********************************************************************************************************************/
static StateNumber ETH_ReadSettingAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_ReadSetting,("------------------ETH_ReadSettingAction\r\n"));
	
	if(p_ext ->isCommandHaveBeenSend == X_False)
	{
		p_ext ->isCommandHaveBeenSend = X_True;
		mFunc_SoftTimerSetValue(STID_ETH,1000);
		rec_index = 0;
		byteBufInit(setting_read_buf,MAX_SETTING_CHAR_LENGTH,'\0');
		mFunc_ETH_SendAT("AT+CWSAP_DEF?");
	}
	
	if(mFunc_SoftTimerGetValue(STID_ETH) == 0) 
	{
		if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Timeout,MID_ETH,ETH_ReadSetting);}
		return ETH_NormalProcess;
	}
	
	if(mFunc_ETH_GetByte(&cur_data) == X_True)
	{
		if(rec_index >= (MAX_SETTING_CHAR_LENGTH)) 
		{
			if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_ErrorOccur,MID_ETH,ETH_ReadSetting);}
			return ETH_Error;
		}
		setting_read_buf[rec_index ++] = cur_data;
		
		if(DoesGetTail(setting_read_buf,rec_index) == X_True)
		{
			TM_LogDebug(1,(" \r\n ------------------- the wifi module's basic setting is : %s \r\n",setting_read_buf)); // 
////////			INSERT(LogDebug)(1,(" \r\n ------------------- the wifi module's basic setting is : %s \r\n",setting_read_buf)); // 
			if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Finished,MID_ETH,ETH_ReadSetting);}
			return ETH_NormalProcess;
		}
	}
	return p_this->current_state;
	
}
/**********************************************************************************************************************
	11
**********************************************************************************************************************/
static StateNumber ETH_TcpCloseAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_TcpClose,("------------------ETH_TcpCloseAction\r\n"));
	mFunc_ETH_SendAT("AT+CIPSERVER=0");
	
	p_ext ->onWait = AlwaysFalse;
	p_ext ->state_to_go_when_failed = ETH_Idle;
	mFunc_SoftTimerSetValue(STID_ETH,1000);
	return ETH_Wait;
}
/**********************************************************************************************************************
	12
**********************************************************************************************************************/
static StateNumber ETH_SendReadyAction(s_StateMachineParam *p_this)
{
	uint16_t length;
	char length_string[10];
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_SendReady,("------------------ETH_SendReadyAction\r\n"));
	
	if(p_ext ->retry_times > 0) 
	{
		p_ext ->retry_times --;
		
		length = (p_ext ->isHugeSend == X_True)? huge_length : normal_length;
		if(Uint16_ToDecString(length,length_string,10) == X_False)
		{
			INSERT(LogDebug)(ETH_MODULE_DEBUG,("~~~~~~~~~~------------------mFunc_ETH: Uint16_ToDecString failed\r\n"));
			return p_this ->current_state;
		}	
		
		mFun_ETH_SendBuf((uint8_t *)"AT+CIPSEND=", strlen("AT+CIPSEND="));
		mFun_ETH_SendBuf((uint8_t *)length_string,strlen(length_string));
		mFun_ETH_SendBuf((uint8_t *)"\r\n", 2);
		
		if(ETH_WaitSpecifiedResponse("OK\r\n",1000
									,p_ext,ETH_Send,p_this ->current_state,X_False) == X_True)
		{
			return ETH_Wait;
		}
	}
	else
	{
		mFun_ETH_ReleaseCurSendBuf();
		mModule_ETH_ReconfigRequest();
		return ETH_NormalProcess;
	}
	
	return p_this ->current_state;
}
/**********************************************************************************************************************
	13
**********************************************************************************************************************/
static StateNumber ETH_SendAction(s_StateMachineParam *p_this)
{
	uint16_t length;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_Send,("------------------ETH_SendAction\r\n"));
	
	length = (p_ext ->isHugeSend == X_True)? huge_length : normal_length;
	mFun_ETH_SendBuf((p_ext ->isHugeSend == X_True)? p_huge_data: p_normal_data,length);
	mFun_ETH_ReleaseCurSendBuf();
	if(ETH_WaitSpecifiedResponse("OK\r\n",1000
									,p_ext,ETH_NormalProcess,ETH_Idle,X_False) == X_True)
	{
		return ETH_Wait;
	}
	return ETH_Error;
}
/**********************************************************************************************************************
	14
**********************************************************************************************************************/
static StateNumber ETH_CheckLinkStatusAction(s_StateMachineParam *p_this)
{
	uint32_t timer_value;
	uint8_t  cur_data;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_CheckLinkStatus,("------------------ETH_CheckLinkStatusAction\r\n"));
	
	timer_value = mFunc_SoftTimerGetValue(STID_ETH);
	if(timer_value == 0)
	{
		mFunc_SoftTimerSetValue(STID_ETH,CHECK_LINK_STATUS_FREQ_IN_MS);
		isFinalLinkCheckDone = X_False;
		isGetTailOnce = X_False;
		rec_index = 0;
		byteBufInit(setting_read_buf,MAX_SETTING_CHAR_LENGTH,'\0');
		mFunc_ETH_SendAT("AT+CIPSTATUS");
	}
	else
	{
		if(mFunc_ETH_GetByte(&cur_data) == X_True)
		{
			#if (ETH_BYTE_RECEIVE_DEBUG != 0 && USE_INSERT_DEBUG != 0 )
				uint8_t eth_test_buf[2] = {"?."};
				eth_test_buf[0] = cur_data;
				TM_LogDebug(1,("-%s ",eth_test_buf)); // something wrong when I use insert log debug here , can not understand ; fix it later
			#endif
			if(rec_index >= (MAX_SETTING_CHAR_LENGTH)) 
			{
////////////				TM_LogDebug(1,(" \r\n rec_index overload \r\n"));
				rec_index = 0;
			}
			setting_read_buf[rec_index ++] = cur_data;
			
			if(DoesGetTail(setting_read_buf,rec_index) == X_True)
			{
				isGetTailOnce = X_True;
//				TM_LogDebug(1,(" \r\n ------------------- the link status is : %s \r\n",setting_read_buf));
				if(DoesFindCertainCharTCP(setting_read_buf,MAX_SETTING_CHAR_LENGTH) == X_True)
				{
					INSERT(LogDebug)(ETH_MODULE_DEBUG,("~~~~~~~~~~------------------ETH_CheckLinkStatusAction : tcp link established !!\r\n"));
					return ETH_NormalProcess;
				}
				else 
				{
					// fix it later , other link status handle
				}
			}
			
		}
		
		if(timer_value >= 1 && timer_value <= 10 && isFinalLinkCheckDone == X_False)
		{
			
			if(isGetTailOnce == X_False) // never get "OK\r\n" in 5 seconds , may be the wify module is discharged ;
			{
				INSERT(LogDebug)(ETH_MODULE_DEBUG,("~~~~~~~~~~------------------ETH_CheckLinkStatusAction : no response !!!\r\n"));
				p_ext ->link_error_times ++;
				if(p_ext ->link_error_times > 3) {return ETH_Idle;}
			}
			else {p_ext ->link_error_times = 0;}
			isFinalLinkCheckDone = X_True;
		}
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************
15
#define TM_COMMAND_END_FLAG0            0x20  //
#define TM_COMMAND_END_FLAG1            0x4F  //O
#define TM_COMMAND_END_FLAG2            0x56	//V
#define TM_COMMAND_END_FLAG3            0x45	//E
#define TM_COMMAND_END_FLAG4            0x52	//R
**********************************************************************************************************************/
static StateNumber ETH_WaitSSIDAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
////////	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_WaitSSID,("------------------ETH_WaitSSIDAction \r\n"));
	
	if(mFunc_SoftTimerGetValue(STID_ETH) == 0 || input_length >= MAX_WIFI_PARAM_LENGTH_IN_CHAR)
	{
			SysLogDebug(1,(" Get SSID failed  \r\n"));
		return ETH_Error;
	}
	if(mDri_TestModeGetByteInterface(&cur_data) == X_True)
	{
////////			INSERT(LogDebug)(ETH_MODULE_DEBUG,(" ~~~ %2x \r\n",cur_data));
			user_data_input_buf[input_length ++] = cur_data;
			if(input_length >= 5 
				&& cur_data ==  'R'
				&& user_data_input_buf[input_length - 2] == 'E'
				&& user_data_input_buf[input_length - 3] == 'V' 
				&& user_data_input_buf[input_length - 4] == 'O'
				&& user_data_input_buf[input_length - 5] == ' ')
			{
				if((input_length - 5) > MAX_SSID_LENGTH_IN_CHAR || (input_length - 5) == 0)
				{
					SysLogDebug(1,(" SSID length beyond scope ; please try again  \r\n"));
					mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
					ReadyForUserInput();
					return ETH_WaitSSID;
				}
				
				CopyBuffer(user_data_input_buf,router_info.ssid,input_length - 5);
				SysLogDebug(1,(" Get SSID : %s  \r\n",router_info.ssid));
				SysLogDebug(1,(" please enter Password in %d sec , use \" OVER\" as tail \r\n",MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS/1000));
				mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
				ReadyForUserInput();
				return ETH_WaitPassword;
			}
	}
	
	return p_this->current_state;
}
/**********************************************************************************************************************
16
**********************************************************************************************************************/
static StateNumber ETH_WaitPasswordAction(s_StateMachineParam *p_this)
{
	uint8_t cur_data;
	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_WaitPassword,("------------------ETH_WaitPasswordAction \r\n"));
	
	if(mFunc_SoftTimerGetValue(STID_ETH) == 0 || input_length >= MAX_WIFI_PARAM_LENGTH_IN_CHAR)
	{
		SysLogDebug(1,(" Get Password failed  \r\n"));
		ClearStateForUserInput();
		if(isReportStateToCaller == X_True)
		{
			if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Finished,(uint8_t)MID_ETH,(uint8_t)ETH_WaitPassword);}
		}
		return ETH_Error;
	}
	if(mDri_TestModeGetByteInterface(&cur_data) == X_True)
	{
////			INSERT(LogDebug)(ETH_MODULE_DEBUG,(" ~~~ %2x \r\n",cur_data));
			user_data_input_buf[input_length ++] = cur_data;
			if(input_length >= 5 
				&& cur_data ==  'R'
				&& user_data_input_buf[input_length - 2] == 'E'
				&& user_data_input_buf[input_length - 3] == 'V' 
				&& user_data_input_buf[input_length - 4] == 'O'
				&& user_data_input_buf[input_length - 5] == ' ')
			{
				if((input_length - 5) > MAX_PASSWORD_LENGTH_IN_CHAR || (input_length - 5) < MIN_PASSWORD_LENGTH_IN_CHAR)
				{
					SysLogDebug(1,(" Password length beyond scope ; please try again  \r\n"));
					mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
					ReadyForUserInput();
					return ETH_WaitPassword;
				}
				
				CopyBuffer(user_data_input_buf,router_info.password,input_length - 5);
				SysLogDebug(1,(" Get PassWord : %s  \r\n",router_info.password));
				
				ClearStateForUserInput();
				router_info.isValid = X_True;
////////////				mApp_SysInfo_SetRouterInfo(X_True,&router_info);
				
				if(isReportStateToCaller == X_True)
				{
					if(p_ext ->event_report != X_Null) {p_ext ->event_report(MEC_Finished,(uint8_t)MID_ETH,(uint8_t)ETH_WaitPassword);}
				}
				return ETH_Config;
			}
	}
	
	return p_this->current_state;
}
/**********************************************************************************************************************
17
**********************************************************************************************************************/
static StateNumber ETH_ReadyForRouterInfoAction(s_StateMachineParam *p_this)
{
//////	sParamExtern *p_ext = (sParamExtern*)p_this;
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,ETH_ReadyForRouterInfo,("------------------ETH_ReadyForRouterInfoAction \r\n"));
	
	SysLogDebug(1,("please enter SSID in %d sec , use \" OVER\" as tail \r\n",MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS/1000));
			
	ReadyForUserInput();
	byteBufInit((uint8_t *)&router_info,sizeof(router_info)/sizeof(uint8_t),'\0');
	mFunc_SoftTimerSetValue(STID_ETH,MAX_ETH_WAIT_USER_INPUT_DATA_TIME_MS);
	isReportStateToCaller = X_True;
	return ETH_WaitSSID;
}
/**********************************************************************************************************************

**********************************************************************************************************************/

static const StateAction ETH_StateAction[] = {
		{ETH_IdleAction},
		{ETH_CheckConfigAction},
		{ETH_ConfigAction},
		{ETH_NormalProcessAction},
		{ETH_ErrorAction},
		{ETH_TimeOutAction},
		{ETH_TestAT_Action},
		{ETH_WaitAction},
		{ETH_TcpStartAction},
		{ETH_RestoreAction},
		{ETH_ReadSettingAction},//10
		{ETH_TcpCloseAction},
		{ETH_SendReadyAction},
		{ETH_SendAction},
		{ETH_CheckLinkStatusAction},
		{ETH_WaitSSIDAction},
		{ETH_WaitPasswordAction},
		{ETH_ReadyForRouterInfoAction},
};

APP_STATE_MACHINE_DEF(p_state,sizeof(ETH_StateAction)/sizeof(ETH_StateAction[0]),&ETH_StateAction[0]);

X_Void mModule_WifiInit(X_Void)
{
	isModuleLocked = X_False;
	sPE.isCommandHaveBeenSend = X_False;
	sPE.event_report  = (evt_reg)0;
	sPE.config_index  = 0;
	sPE.isHugeSend    = X_False;
	
	isRestoreRequest = X_False;
	isReportRequest = X_False;
	isReconfigRequest = X_False;
	isReportStateToCaller = X_False;
	
	state_backup = ETH_Idle;
	rec_index = 0;
	isFinalLinkCheckDone = X_False;
	isGetTailOnce = X_False;
	mFunc_ETH_SendRequestInit();
}

X_Void mModule_WifiHandle(X_Void)
{
	mFunc_ETH_SpecificReceiveProcess();
	if(isModuleLocked == X_True) {return;}
	mStateMachineRun(p_state,&sPE.base,Recorder);
}
////////extern X_Boolean mModule_DoesWifiModuleEnable(X_Void);
X_Void mModule_ETH_DataDirectOutput(X_Void)
{
//////	if(mModule_DoesWifiModuleEnable() == X_True) {return;}
////////	if(mFunc_ETH_DoesRecvSendRequest() == X_True) {return;}
////////	if(mFun_ETH_DoesHugeBufSend(&p_huge_data,&huge_length) == X_True)
////////	{
////////		mFun_ETH_SendBuf(p_huge_data,huge_length);
////////		return;
////////	}
////////	if(mFun_ETH_DoesSend(&p_normal_data,&normal_length) == X_True)
////////	{
////////		mFun_ETH_SendBuf(p_normal_data,normal_length);
////////		mFun_ETH_ReleaseCurSendBuf();
////////		return;
////////	}
}
X_Void mModule_ETH_Reset(X_Void)
{
	mStateMachineStateSet(p_state,ETH_Idle);
}
X_Void mModule_ETH_Lock(X_Void)
{
	isModuleLocked = X_True;
	mFunc_ETH_SendRequestDisable();
}
X_Void mModule_ETH_Unlock(X_Void)
{
	isModuleLocked = X_False;
}
X_Void mModule_ETH_EventRegister(evt_reg reg)
{
	if(reg != X_Null)
	{
		sPE.event_report = reg;
	}
}
X_Void mModule_ETH_EventClear(X_Void)
{
	sPE.event_report = (evt_reg)0;
}

X_Void mModule_ETH_RestoreRequest(X_Void)
{
	isRestoreRequest = X_True;
}
X_Void mModule_ETH_GetStatusRequest(X_Void)
{
	isReportRequest = X_True;
}
X_Void mModule_ETH_ReconfigRequest(X_Void)
{
	isReconfigRequest = X_True;
}

X_Void mModule_ETH_InputRouterInfoRequest(X_Void)
{
	mStateMachineStateSet(p_state,ETH_ReadyForRouterInfo);
}


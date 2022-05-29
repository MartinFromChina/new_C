#include "mApp_TestMode.h"
#include "mModule.h"
//#include  "mModule_ETH.h"
#include  <string.h>
#include "mFunc_TM_Interface.h"
#include "mDri_Uart.h"
#include "mDri_SoftTimer.h"
#include "../../../../../../module/common/AppCommon.h"
#include "../../../../../../module/common/StateMachine/StateMachine.h"
#include "../../../../../../module/customize/TestModeConsole/mTestMode.h"
#include "mDri_GPIO.h"

typedef X_Void 									(*recovery_func)(X_Void);
typedef	X_Boolean               (*wait_func)(X_Void);

////////static X_Boolean               isSpecialWaitMethod;
X_Boolean                        isWifiPortRawData;

static StateNumber SYSINFO_todo(s_StateMachineParam *p_base)
{
	sTestModeParamExternForUser *p_ext = (sTestModeParamExternForUser*)p_base;
	TM_LogDebug(1,("\r\n \r\n --------- Show System Infomation;\r\n"));
	TM_LogDebug(1,("\r\n \r\n -------------------Firmware Version : %d . %d . %d ; \r\n"
															,CPU_FIRMWARE_MAJOR_VERSION,CPU_FIRMWARE_SECONDARY_VERSION / 10,CPU_FIRMWARE_SECONDARY_VERSION % 10));
	TM_LogDebug(1,("\r\n ---------[ OK ] \r\n"));
	p_ext ->successed_cnt ++;
	return TM_Finish;
}

static X_Void WIFIRAWDATA_Recovery(X_Void)
{
	isWifiPortRawData = X_False;
}
static StateNumber WIFIRAWDATA_todo(s_StateMachineParam *p_base)
{
	sTestModeParamExternForUser *p_ext = (sTestModeParamExternForUser*)p_base;
	TM_LogDebug(1,(" \r\n \r\n --------- Wifi rawdata ENABLE\r\n"));
	isWifiPortRawData = X_True;
	p_ext ->wait_counter  = 1100000;
	p_ext ->isDotPrintDisable = X_True;
	p_ext ->recovery      = WIFIRAWDATA_Recovery;
	return TM_Wait;
}
/**********************************************************************************************************************************
5
*********************************************************************************************************************************/
static const sTmCommandAnalysis new_command_analysis[] = {
/***************************test rountine begin*******************************/
	{X_True,	"SYSINFO",					7,	7,SYSINFO_todo},
/***************************test rountine end*******************************/
	{X_True,	"RUNALL",					6,	6,	TM_RUNALL_todo},
	{X_False,	"ENTER",					5,	5,	TM_ENTER_todo},
	{X_False,	"EXIT",						4,	4,	TM_EXIT_todo},
	
//	{X_True,	"RTCCALI",				7,	7,	RTCCALI_todo},
	{X_True,	"WIFIRAWDATA",		11,	11,	WIFIRAWDATA_todo},
};

/***********************************************************************************************
***********************************************************************************************
***********************************************************************************************/
static X_Void EnterTestModeCb(X_Void)
{
//	mModule_SetTestMode();
}
static X_Void ClearTestModeCb(X_Void)
{
//	mFun_TM_ReceiveUnocked();
//	mModule_ClearTestMode(X_Null);
}

static X_Void KillAllCb(X_Void)
{
//	isSpecialWaitMethod = X_False;
//	mModule_ResetAndLockAll();
}

APP_TEST_MODE_MODULE_DEFINE(p_test,ACTION_FREQ_IN_MS,
														mFun_TM_GetByte,mDri_TestModeSentBufInterface,
														new_command_analysis,(sizeof(new_command_analysis)/sizeof(new_command_analysis[0])),
														70,3,
														EnterTestModeCb,ClearTestModeCb,1200000,
														KillAllCb);

X_Void mApp_TM_Init(X_Void)
{
////////	isSpecialWaitMethod = X_False;
	isWifiPortRawData  = X_False;
	
	if(mTestModeInit(p_test) != X_True)
	{
		TM_LogDebug(1,("@ test mode module init failed  \r\n"));
	}
}
X_Void mApp_TM_Handle(X_Void)
{
	mTestModeHandle(p_test);
}
X_Void mApp_TM_LoopRecv(X_Void)
{
	uint8_t cur_data;
  mTestModeLoopReceive(p_test);
	if(isWifiPortRawData == X_True)
	{
		if(mDri_WifiGetByteInterface(&cur_data) == X_True)
		{
			mDri_TestModeSentBufInterface(&cur_data,1);
		}
	}
}


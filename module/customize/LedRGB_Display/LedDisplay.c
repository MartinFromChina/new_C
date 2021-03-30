#include "LedDisplay.h"




X_Void LedDisplayInit(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;

	//isModuleForbidden = X_False;
	//isUserDefineParamEmpty = X_True;
	//SimpleQueueInitialize(p_led_event);
}




X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;

	/*
	if(isModuleForbidden == X_True) 
	{
		mFunc_ColorDisable();
		return;
	}
	SimpleStateMachineRun(p_simple_state,&sLSP.base,X_Null
	#if (SEGGER_RTT_DEBUG == 1)
	,StateJumpRecorder
	#else
	,X_Null
	#endif
	);
	if(isLedOnForever == X_True)
	{
		mModule_PowerSourceApply(PS_Pentip,PSA_PentipRGB,COMMON_COLOR_ON_TIME_IN_MS);
	}

	*/
	
}
X_Boolean LedDisplayRegisterEvent(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event)
{
	p_manager = p_manager;
	p_event = p_event;
	return X_True;

	/*
	X_Boolean isOK;
	uint16_t  bufnumber;
	
	if(event == LE_UserDefine && p_param == X_Null) {return;}
	
	bufnumber = SimpleQueueFirstIn(p_led_event,&isOK,X_False);
	if(isOK == X_True && bufnumber < MAX_LED_EVENT_HOLD_COUNT)
	{
		led_event_buf[bufnumber] = event;
		if(event == LE_UserDefine)
		{
			isUserDefineParamEmpty = X_False;
			CopyBuffer(p_param,&sBP_UserDefine,sizeof(sLedDisplayParam)/sizeof(uint8_t));
		}
	}
	*/
}
X_Void LedDisplayEnable(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;
}
X_Void LedDisplayDisable(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;
}

X_Void LedDisplayEnableImmediately(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;
}
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;
}
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager)
{
	p_manager = p_manager;
}



/*
X_Void mModule_LedDisplayReset(X_Void)
{
	isModuleForbidden = X_False;
	isUserDefineParamEmpty = X_True;
	isLedOnForever = X_False;
	
	ClearSimpleQueue(p_led_event);
	SimpleStateMachineStateSet(p_simple_state,LS_Idle);
	mFunc_ColorDisable();
}
*/

#include "LedDisplay.h"




X_Void LedDisplayInit(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	
	if(p_manager ->init == X_Null)  {return;}
	p_manager ->init();
	if(p_manager ->draw == X_Null || p_manager ->off == X_Null ) {return;}
	if(p_manager ->p_operation == X_Null)  {return;}
	p_manager ->p_operation ->queue_init(p_manager ->p_operation ->p_manager);
	
	p_manager ->p_flag ->isEnable = X_True;
	p_manager ->p_flag ->isInitOK  = X_True;

}

X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	if(p_manager ->p_flag ->isInitOK == X_False) {return ;}

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
X_Boolean LedDisplayEventRegister(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event)
{
	uint16_t buf_number;
	if(p_manager == X_Null || p_event == X_Null ) {return X_False;}
	if(p_manager ->p_flag ->isInitOK == X_False) {return X_False;}
	p_event = p_event;

	buf_number = p_manager ->p_operation->queue_fi(p_manager ->p_operation ->p_manager,X_False);
	if(buf_number < p_manager ->max_event_to_cache)
	{
		p_manager ->p_event_buf[buf_number].event_mode 				= p_event->event_mode;
		p_manager ->p_event_buf[buf_number].param.color 			= p_event->param.color;
		p_manager ->p_event_buf[buf_number].param.led_off_time 		= p_event->param.led_off_time;
		p_manager ->p_event_buf[buf_number].param.led_on_time  		= p_event->param.led_on_time;
		p_manager ->p_event_buf[buf_number].param.on_off_cycle 		= p_event->param.on_off_cycle;
		return X_True;
	}
	return X_False;
}
X_Void LedDisplayEnable(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager = p_manager;
}
X_Void LedDisplayDisable(const sLedDisPlayManager *p_manager)
{
	uint16_t buf_num;
	if(p_manager == X_Null) {return;}
	buf_num = p_manager ->p_operation->queue_fi(p_manager ->p_operation ->p_manager,X_False);
	if(buf_num >= p_manager ->max_event_to_cache )  {return;}

	p_manager ->p_event_buf[buf_num].event_mode = LedDisable;
}

X_Void LedDisplayEnableImmediately(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager = p_manager;
}
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager = p_manager;
}
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
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

#include "LedDirectDisplay.h"
/*
#include "LogDebugSwitch.h"
#define STATE_DEBUG 0
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);
*/

typedef X_Void (*recorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);

typedef enum
{
	LDS_Idle = 0,
	LDS_BlinkOn,
	LDS_BlinkOff,
	LDS_BlinkFinish,
}eLedState;
/*
********************************************************************************************************************************
			0
********************************************************************************************************************************
*/
StateNumber LDS_IdleAction(s_StateMachineParam *p_this)
{	
		//INSERT(LogDebugOnce)(STATE_DEBUG,p_once,LDS_Idle,("LS_IdleAction \r\n"));
	sLedDirStateParam *p_ext 										= (sLedDirStateParam *)p_this;
	
	if(p_ext ->isNewEventCome == X_True)
	{
		p_ext ->isNewEventCome = X_False;
		p_ext ->blink_cycle_counter		= 0;
		if(p_ext ->blink_param.led_on_time > 0 && p_ext ->blink_param.on_off_cycle > 0) 
		{
			if(p_ext ->blink_param.on_off_cycle != LED_ON_OFF_CYCLE_INFINITE)
			{
				p_ext ->blink_param.on_off_cycle += p_ext ->blink_param.on_off_cycle;
			}
			p_ext ->blink_cycle_counter = p_ext ->blink_param.led_on_time;
			return LDS_BlinkOn;
		}
	}
	return p_this ->current_state;
}
/*
********************************************************************************************************************************
			1
********************************************************************************************************************************
*/
StateNumber LDS_BlinkOnAction(s_StateMachineParam *p_this)
{
		//INSERT(LogDebugOnce)(STATE_DEBUG,p_once,LDS_BlinkOn,("LS_BlinkOnAction \r\n"));
		sLedDirStateParam *p_ext 	= (sLedDirStateParam *)p_this;
		sLedDirBlinkParam *p_param;
		p_param = &(p_ext ->blink_param);
		
		if(p_ext ->blink_cycle_counter == p_param ->led_on_time)
		{
			if(p_param->on_off_cycle > 0) 
			{
				p_ext->display.on();
				if(p_param->on_off_cycle != LED_ON_OFF_CYCLE_INFINITE){p_param->on_off_cycle --;}
				if(p_param->led_on_time == LED_DIRECT_ON_INFINITE) {return LDS_Idle;}
			}
			else {return  LDS_BlinkFinish;}
		}
		if(p_ext ->blink_cycle_counter >= p_ext ->state_interval)
		{
			p_ext ->blink_cycle_counter -= p_ext ->state_interval;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_param ->led_off_time;
		
		return LDS_BlinkOff;
}

/*
********************************************************************************************************************************
			2
********************************************************************************************************************************
*/
StateNumber LDS_BlinkOffAction(s_StateMachineParam *p_this)
{
		//INSERT(LogDebugOnce)(STATE_DEBUG,p_once,LDS_BlinkOff,("LS_BlinkOffAction \r\n"));
		sLedDirStateParam *p_ext 	= (sLedDirStateParam *)p_this;
		sLedDirBlinkParam *p_param;
		p_param = &(p_ext ->blink_param);
		
		if(p_ext ->blink_cycle_counter == p_param ->led_off_time)
		{
			if(p_param->on_off_cycle > 0) 
			{
				p_ext->display.off();
				if(p_param->on_off_cycle != LED_ON_OFF_CYCLE_INFINITE){p_param->on_off_cycle --;}
			}
			else {return  LDS_BlinkFinish;}
		}
		if(p_ext ->blink_cycle_counter >= p_ext ->state_interval)
		{
			p_ext ->blink_cycle_counter -= p_ext ->state_interval;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_param ->led_on_time;
		
		return LDS_BlinkOn;
}
/*
********************************************************************************************************************************
			3
********************************************************************************************************************************
*/
StateNumber LDS_BlinkFinishAction(s_StateMachineParam *p_this)
{
	sLedDirStateParam *p_ext 										= (sLedDirStateParam *)p_this;
	if(p_ext ->display.DoesLedOn() == X_True)
	{
		p_ext ->display.off();
	}
	return LDS_Idle;
}

/*
********************************************************************************************************************************

********************************************************************************************************************************
*/
X_Void LedDirectDisplayInit(const sLedDirDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	sLedDirStateParam *p_ext 			= (sLedDirStateParam *)p_manager ->p_state_param;
	
	if(p_ext ->display.init == X_Null)  {return;}
	p_ext ->display.init();
	if(p_ext ->display.on == X_Null || p_ext ->display.off == X_Null  || p_ext ->display.DoesLedOn == X_Null) {return;}
	
	p_ext     ->isNewEventCome    = X_False;
	p_manager ->p_flag ->isEnable = X_True;
	p_manager ->p_flag ->isInitOK  = X_True;
}
X_Void LedDirectDisplayHandle(const sLedDirDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	if(p_manager ->p_flag ->isInitOK == X_False || p_manager ->p_flag ->isEnable == X_False) {return ;}
	mStateMachineRun(p_manager ->p_state_machine,p_manager ->p_state_param,(recorder)0);
}
X_Boolean LedDirectDisplayEventRegister(const sLedDirDisPlayManager *p_manager,sLedDirBlinkParam *p_event)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->p_flag ->isInitOK == X_False || p_manager ->p_flag ->isEnable == X_False) {return X_False;}
	sLedDirStateParam *p_ext 			= (sLedDirStateParam *)p_manager ->p_state_param;
	
	if(p_event == X_Null){return X_False;}
	if(p_event ->on_off_cycle > LED_ON_OFF_CYCLE_MAX && p_event ->on_off_cycle != LED_ON_OFF_CYCLE_INFINITE) {return X_False;}
	p_ext->blink_param.led_on_time  = p_event ->led_on_time;
	p_ext->blink_param.led_off_time = p_event ->led_off_time;
	p_ext->blink_param.on_off_cycle = p_event ->on_off_cycle;
	mStateMachineStateSet(p_manager ->p_state_machine,LDS_Idle);
	p_ext ->isNewEventCome          = X_True;
	return X_True;
}
X_Void LedDirectDisplayEnable(const sLedDirDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager ->p_flag ->isEnable = X_True;
}
X_Void LedDirectDisplayDisable(const sLedDirDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager ->p_flag ->isEnable = X_False;
}
X_Void LedDirectDisplayReset(const sLedDirDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	sLedDirStateParam *p_ext 			= (sLedDirStateParam *)p_manager ->p_state_param;
	mStateMachineStateSet(p_manager ->p_state_machine,LDS_Idle);
	p_manager ->p_flag ->isEnable = X_True;
	p_ext ->isNewEventCome        = X_False;
	p_ext ->display.off();
}

#include "LedDisplay.h"

typedef X_Void (*recorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);


typedef enum
{
	LS_Idle = 0,
	LS_LoadEvent,
	LS_ReadyForEvent,
	LS_Recover,
	LS_BlinkOn,
	LS_BlinkOff,
	LS_Wait,
}eLedState;
/*
********************************************************************************************************************************
			0
********************************************************************************************************************************
*/


StateNumber LS_IdleAction(s_StateMachineParam *p_this)
{	
	sLedStateParam *p_ext 										= (sLedStateParam *)p_this;
	p_ext = p_ext;
	/*
	p_ext ->current_event   									= LE_NotCharge;
	p_ext ->state_backup  										= LS_Idle;
	p_ext ->wait_counter_in_ms  							= 0;
	p_ext ->color_backup  										= ColorOff;
	p_ext ->p_on_off_param_backup 						= &sBP_NotCharge;
	p_ext ->onWaitMethod 											= mModule_DoesPentipPowerSupply;
	p_ext ->current_blink_param.BlinkTimes 		= 0;
	p_ext ->current_blink_param.DutyCycleOFF 	= 0;
	p_ext ->current_blink_param.DutyCycleON  	= 0;
	p_ext ->blink_cycle_counter								= 0;

	*/
	return LS_LoadEvent;
}


/*
********************************************************************************************************************************
			1
********************************************************************************************************************************
*/
StateNumber LS_LoadEventAction(s_StateMachineParam *p_this)
{
	
	//X_Boolean isOK;
	//uint16_t  bufnumber;
	sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
	p_ext = p_ext;
	/*
	bufnumber = SimpleQueueFirstOut(p_led_event,&isOK);
	if(isOK == X_True)
	{
		p_ext ->current_event = led_event_buf[bufnumber];
		RealseSimpleQueueBuf(p_led_event,bufnumber);
		return LS_ReadyForEvent;
	}
	*/
	return p_this ->current_state;
}


/*
********************************************************************************************************************************
			2
********************************************************************************************************************************
*/
StateNumber LS_ReadyForEventAction(s_StateMachineParam *p_this)
{
	
	//uint32_t current_color,blink_time;
	sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
	p_ext = p_ext;
	/*
	current_color = p_led_display_param_buf[p_ext ->current_event]->base.color;
	if(p_led_display_param_buf[p_ext ->current_event] ->base.mode == LDM_On_Off) // on off mode
	{
		p_ext ->color_backup = current_color;
		
		if(current_color != ColorOff) // to do : apply power , enable pwm , config pwm 
		{
			mModule_PowerSourceApply(PS_Pentip,PSA_PentipRGB,COMMON_COLOR_ON_TIME_IN_MS);
			mFunc_ColorEnable();
			mFunc_ColorDraw(current_color);// !!! call it after enable
		}
		else
		{
			mModule_PowerSourceRelease(PS_Pentip,PSA_PentipRGB);
//////////			mFunc_ColorDraw(current_color); // necessary ? !!! call it before disable
			mFunc_ColorDisable();
		}
		if(p_ext ->current_event == LE_UserDefine)
		{
			isUserDefineParamEmpty = X_True;
		}
		return LS_LoadEvent;
	}
	else // blink mode
	{
		if(current_color == ColorOff || p_led_display_param_buf[p_ext ->current_event]->ext_param.sLB.BlinkTimes == 0) {return LS_LoadEvent;}
		p_ext ->current_blink_param.BlinkTimes   = p_led_display_param_buf[p_ext ->current_event]->ext_param.sLB.BlinkTimes;
		p_ext ->current_blink_param.BlinkTimes 	+= p_ext ->current_blink_param.BlinkTimes; // double it ;
		p_ext ->current_blink_param.DutyCycleON  = p_led_display_param_buf[p_ext ->current_event]->ext_param.sLB.DutyCycleON;
		p_ext ->current_blink_param.DutyCycleOFF = p_led_display_param_buf[p_ext ->current_event]->ext_param.sLB.DutyCycleOFF;
		
		blink_time = p_ext ->current_blink_param.DutyCycleOFF + p_ext ->current_blink_param.DutyCycleON;
		blink_time = blink_time * p_ext ->current_blink_param.BlinkTimes;
		blink_time = blink_time * MAIN_TICK_INTERVAL_MS;
		blink_time += MAX_CONFIG_WAIT_TIME_IN_MS ;
		mModule_PowerSourceApply(PS_Pentip,PSA_PentipRGB,blink_time);
		mFunc_ColorEnable();
		
		if(p_ext ->color_backup != ColorOff) 
		{
			p_ext ->state_backup = LS_BlinkOff;
			p_ext ->blink_cycle_counter = p_ext ->current_blink_param.DutyCycleOFF;
		}
		else 
		{
			p_ext ->state_backup = LS_BlinkOn;
			p_ext ->blink_cycle_counter = p_ext ->current_blink_param.DutyCycleON;
		}
		p_ext ->wait_counter_in_ms = MAX_CONFIG_WAIT_TIME_IN_MS;
		return LS_Wait;
	}
	*/
	return LS_Wait;
}

/*
********************************************************************************************************************************
			3
********************************************************************************************************************************
*/
StateNumber LS_RecoverAction(s_StateMachineParam *p_this)
{
		
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		p_ext = p_ext;
		/*
		if(p_ext ->color_backup != ColorOff)
		{
			mModule_PowerSourceApply(PS_Pentip,PSA_PentipRGB,1000);
			mFunc_ColorEnable();
			mFunc_ColorDraw(p_ext ->color_backup);// !!! call it after enable
		}
		else
		{
			mModule_PowerSourceRelease(PS_Pentip,PSA_PentipRGB);
//////////			mFunc_ColorDraw(p_ext ->color_backup); // necessary ? !!! call it before disable
			mFunc_ColorDisable();
		}
		if(p_ext ->current_event == LE_UserDefine)
		{
			isUserDefineParamEmpty = X_True;
		}
		*/
		return LS_LoadEvent;
}

/*
********************************************************************************************************************************
			4
********************************************************************************************************************************
*/
StateNumber LS_BlinkOnAction(s_StateMachineParam *p_this)
{
		
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		p_ext = p_ext;
		/*
		if(p_ext ->blink_cycle_counter == p_ext ->current_blink_param.DutyCycleON)
		{
			if(p_ext ->current_blink_param.BlinkTimes > 0) 
			{
				mFunc_ColorDraw(p_led_display_param_buf[p_ext ->current_event]->base.color);
				p_ext ->current_blink_param.BlinkTimes --;
			}
			else {return LS_Recover;}
		}
		if(p_ext ->blink_cycle_counter > 0)
		{
			p_ext ->blink_cycle_counter --;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_ext ->current_blink_param.DutyCycleOFF;
		*/
		return LS_BlinkOff;
}

/*
********************************************************************************************************************************
			5
********************************************************************************************************************************
*/
StateNumber LS_BlinkOffAction(s_StateMachineParam *p_this)
{
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		p_ext = p_ext;
		/*
		if(p_ext ->blink_cycle_counter == p_ext ->current_blink_param.DutyCycleOFF)
		{
			if(p_ext ->current_blink_param.BlinkTimes > 0) 
			{
				mFunc_ColorDraw(ColorOff);
				p_ext ->current_blink_param.BlinkTimes --;
			}
			else {return LS_Recover;}
		}
		if(p_ext ->blink_cycle_counter > 0)
		{
			p_ext ->blink_cycle_counter --;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_ext ->current_blink_param.DutyCycleON;
		*/
		return LS_BlinkOn;
}

/*
********************************************************************************************************************************
			6
********************************************************************************************************************************
*/
StateNumber LS_WaitAction(s_StateMachineParam *p_this)
{
	
	sLedStateParam *p_ext = (sLedStateParam *)p_this;
	p_ext = p_ext;
	/*
	if(p_ext ->onWaitMethod == X_Null) {return p_ext ->state_backup;}
	if(p_ext ->onWaitMethod() == X_True) {return p_ext ->state_backup;}
	if(p_ext ->wait_counter_in_ms < MAIN_TICK_INTERVAL_MS) {return LS_LoadEvent;}
	p_ext ->wait_counter_in_ms -= MAIN_TICK_INTERVAL_MS;
	*/
	return p_this ->current_state;
}

/*******************************************************************************/

X_Void LedDisplayInit(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	sLedStateParam *p_ext 			= (sLedStateParam *)p_manager ->p_state_param;
	
	if(p_ext ->display.init == X_Null)  {return;}
	p_ext ->display.init();
	if(p_ext ->display.draw == X_Null || p_ext ->display.off == X_Null ) {return;}
	if(p_ext ->p_operation == X_Null)  {return;}
	p_ext ->p_operation ->queue_init(p_ext ->p_operation ->p_manager);

	if(p_ext ->display.pow_apply != X_Null && p_ext ->display.DoesPowerOn != X_Null) {p_ext ->is_power_ctrl_needed = X_True;}
	else {p_ext ->is_power_ctrl_needed = X_False;}
	
	p_manager ->p_flag ->isEnable = X_True;
	p_manager ->p_flag ->isInitOK  = X_True;

}

X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	if(p_manager ->p_flag ->isInitOK == X_False || p_manager ->p_flag ->isEnable == X_False) {return ;}
	mStateMachineRun(p_manager ->p_state_machine,p_manager ->p_state_param,(recorder)0);
}
X_Boolean LedDisplayEventRegister(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event)
{
	uint16_t buf_number;
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->p_flag ->isInitOK == X_False) {return X_False;}
	sLedStateParam *p_ext 			= (sLedStateParam *)p_manager ->p_state_param;

	buf_number = p_ext ->p_operation->queue_fi(p_ext ->p_operation ->p_manager,X_False);
	if(buf_number < p_manager ->max_event_to_cache)
	{
		p_ext ->p_event_buf[buf_number].event_mode 				= p_event->event_mode;
		p_ext ->p_event_buf[buf_number].param.color 			= p_event->param.color;
		p_ext ->p_event_buf[buf_number].param.led_off_time 		= p_event->param.led_off_time;
		p_ext ->p_event_buf[buf_number].param.led_on_time  		= p_event->param.led_on_time;
		p_ext ->p_event_buf[buf_number].param.on_off_cycle 		= p_event->param.on_off_cycle;
		return X_True;
	}
	return X_False;
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
	sLedStateParam *p_ext 			= (sLedStateParam *)p_manager ->p_state_param;
	p_ext ->p_operation ->QueueClear(p_ext ->p_operation->p_manager);
	mStateMachineStateSet(p_manager ->p_state_machine,LS_Idle);
	p_manager ->p_flag ->isEnable = X_True;
}

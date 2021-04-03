#include "LedDisplay.h"
#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
	INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);
#endif

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
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_Idle,("LS_IdleAction \r\n"));
	#endif
	sLedStateParam *p_ext 										= (sLedStateParam *)p_this;
	p_ext ->state_backup		 	= LS_Idle;
	p_ext ->event_buf_number_backup = 0;
	p_ext ->wait_counter_in_ms 		= 0;
	p_ext ->color_backup			= LD_COLOR_OFF;
	p_ext ->blink_cycle_counter		= 0;
	p_ext ->p_current_event			= (sLedDisplayEvent*)0;
	p_ext ->onWaitMethod			= p_ext ->display.DoesPowerOn;
	return LS_LoadEvent;
}
/*
********************************************************************************************************************************
			1
********************************************************************************************************************************
*/
StateNumber LS_LoadEventAction(s_StateMachineParam *p_this)
{
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_LoadEvent,("LS_LoadEventAction \r\n"));
	#endif
	uint16_t  bufnumber;
	sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
	s_QueueOperation *p_op  = p_ext ->p_operation;

	if(p_op ->queue_empty(p_op ->p_manager) == X_True) {return p_this ->current_state;}
	bufnumber = p_op ->queue_fo(p_op ->p_manager);
	if(bufnumber < p_ext ->max_event_to_cache)
	{
		p_ext ->p_current_event = &(p_ext->p_event_buf[bufnumber]);
		p_ext ->event_buf_number_backup = bufnumber;
		/*
			it is safer to release node after the blink process is finished ; 
			but I believe the blink process is faster than the speed of user register event;
			so there will not be any problem if the user use the module in a normal way; 
		*/
		/////p_op ->queue_release(p_op ->p_manager,bufnumber);// do it after the blink is finished , in case some challenging situation
		return LS_ReadyForEvent;
	}
	return p_this ->current_state;
}
/*
********************************************************************************************************************************
			2
********************************************************************************************************************************
*/
StateNumber LS_ReadyForEventAction(s_StateMachineParam *p_this)
{
	uint32_t current_color,blink_time;
	sLedBlinkParam *p_param;
	sLedStateParam *p_ext 	= (sLedStateParam *)p_this;

	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_ReadyForEvent,("LS_ReadyForEventAction ;event mode : %d\r\n"
				,p_ext ->p_current_event ->event_mode));
	#endif
	
	if(p_ext ->p_current_event == X_Null) 
	{
		p_ext->p_operation ->queue_release(p_ext->p_operation ->p_manager,p_ext ->event_buf_number_backup);
		return LS_LoadEvent;
	}
	
	current_color = p_ext ->p_current_event ->param.color;
	p_param		  = &(p_ext ->p_current_event ->param);

	if(p_ext ->p_current_event ->event_mode == LedBlink)// blink mode
	{
		p_param ->on_off_cycle  += p_param ->on_off_cycle; // double it ;
		
		blink_time = p_param ->led_off_time + p_param ->led_on_time;
		blink_time = blink_time * p_param ->on_off_cycle;		
		
		if(p_ext ->color_backup != LD_COLOR_OFF) 
		{
			p_ext ->state_backup = LS_BlinkOff;
			p_ext ->blink_cycle_counter = p_param ->led_off_time;
		}
		else 
		{
			p_ext ->state_backup = LS_BlinkOn;
			p_ext ->blink_cycle_counter = p_param ->led_on_time;
		}

		if(p_ext ->is_power_ctrl_needed == X_False)
		{
			return p_ext ->state_backup;
		}
		else
		{
			blink_time += p_ext ->wait_power_setup_time;
			p_ext ->display.pow_apply(blink_time);
			p_ext ->wait_counter_in_ms = p_ext ->wait_power_setup_time;
			p_ext ->wait_counter_in_ms += p_ext ->state_interval * 2;
			return LS_Wait;
		}
		
	}
	else// on off mode
	{
	/*
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
		return LS_LoadEvent;
		*/
	}
	
	return LS_Wait;
}

/*
********************************************************************************************************************************
			3
********************************************************************************************************************************
*/
StateNumber LS_RecoverAction(s_StateMachineParam *p_this)
{
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_Recover,("LS_RecoverAction \r\n"));
	#endif
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		
		//p_ext ->p_operation ->queue_release(p_ext ->p_operation ->p_manager,p_ext ->event_buf_number_backup);
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
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_BlinkOn,("LS_BlinkOnAction \r\n"));
	#endif
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		sLedBlinkParam *p_param;
		p_param = &(p_ext ->p_current_event ->param);
		
		if(p_ext ->blink_cycle_counter == p_param ->led_on_time)
		{
			if(p_param->on_off_cycle > 0) 
			{
				p_ext->display.draw(p_param->color);
				p_param->on_off_cycle --;
				#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
				INSERT(LogDebug)(STATE_DEBUG & (mockable_GetDebugFlag()),(" -----------led on at time: %d\r\n",mockable_GetCurrentTime()));
				#endif
			}
			else {return LS_Recover;}
		}
		if(p_ext ->blink_cycle_counter >= p_ext ->state_interval)
		{
			p_ext ->blink_cycle_counter -= p_ext ->state_interval;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_param ->led_off_time;
		
		return LS_BlinkOff;
}

/*
********************************************************************************************************************************
			5
********************************************************************************************************************************
*/
StateNumber LS_BlinkOffAction(s_StateMachineParam *p_this)
{
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_BlinkOff,("LS_BlinkOffAction \r\n"));
	#endif
		sLedStateParam *p_ext 	= (sLedStateParam *)p_this;
		sLedBlinkParam *p_param;
		p_param = &(p_ext ->p_current_event ->param);
		
		if(p_ext ->blink_cycle_counter == p_param ->led_off_time)
		{
			if(p_param->on_off_cycle > 0) 
			{
				p_ext->display.off();
				p_param->on_off_cycle --;
				#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
				INSERT(LogDebug)(STATE_DEBUG & (mockable_GetDebugFlag()),(" -----------led off at time: %d\r\n",mockable_GetCurrentTime()));
				#endif
			}
			else {return LS_Recover;}
		}
		if(p_ext ->blink_cycle_counter >= p_ext ->state_interval)
		{
			p_ext ->blink_cycle_counter -= p_ext ->state_interval;
			return p_this ->current_state;
		}
		p_ext ->blink_cycle_counter = p_param ->led_on_time;
		
		return LS_BlinkOn;
}

/*
********************************************************************************************************************************
			6
********************************************************************************************************************************
*/
StateNumber LS_WaitAction(s_StateMachineParam *p_this)
{
	#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
		INSERT(LogDebugOnce)(STATE_DEBUG & (mockable_GetDebugFlag()),p_once,LS_Wait,("LS_WaitAction \r\n"));
	#endif
	sLedStateParam *p_ext = (sLedStateParam *)p_this;
	if(p_ext ->onWaitMethod == X_Null) {return p_ext ->state_backup;}
	if(p_ext ->onWaitMethod() == X_True) {return p_ext ->state_backup;}
	if(p_ext ->wait_counter_in_ms < p_ext ->state_interval) {return LS_LoadEvent;}
	p_ext ->wait_counter_in_ms -= p_ext ->state_interval;
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

	if(p_ext ->display.pow_apply != X_Null && p_ext ->display.DoesPowerOn != X_Null && p_ext ->display.pow_release != X_Null) {p_ext ->is_power_ctrl_needed = X_True;}
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
	if(p_manager ->p_flag ->isInitOK == X_False || p_manager ->p_flag ->isEnable == X_False) {return X_False;}
	sLedStateParam *p_ext 			= (sLedStateParam *)p_manager ->p_state_param;

	if(p_event ->event_mode == LedBlink)
	{
		if(p_event ->param.led_on_time < p_ext ->state_interval || p_event ->param.led_off_time < p_ext ->state_interval 
				|| p_event ->param.on_off_cycle == 0 
				||p_event ->param.color == LD_COLOR_OFF || p_event ->param.color == COLOR_RGB_Black) {return X_False;}
	}
	else if(p_event ->event_mode == LedHoldOn || p_event ->event_mode == LedHoldOnRecoverable)
	{
		//////////if(p_event ->param.color == LD_COLOR_OFF || p_event ->param.color == COLOR_RGB_Black) {return X_False;}// no need
	}
	else
	{
		return X_False;
	}
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
	p_manager ->p_flag ->isEnable = X_True;
}
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	p_manager ->p_flag ->isEnable = X_False;
}
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	sLedStateParam *p_ext 			= (sLedStateParam *)p_manager ->p_state_param;
	p_ext ->p_operation ->queue_clear(p_ext ->p_operation->p_manager);
	mStateMachineStateSet(p_manager ->p_state_machine,LS_Idle);
	p_manager ->p_flag ->isEnable = X_True;
	p_ext ->display.off();
}

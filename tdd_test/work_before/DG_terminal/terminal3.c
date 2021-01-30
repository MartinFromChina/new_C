#include "terminal3.h"
#include "DG_common.h"

//INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);
APP_DG_TERMINAL_DEFINE(p_entry3,3,2,4);

X_Void Terminal3Init(X_Boolean isStartPoint)
{
	p_entry3 ->p_action -> Init(p_entry3 ->p_action ->p_manager); // don't forget null check'
	p_entry3 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry3 ->p_wait_ack -> isReSend           = X_True;
	p_entry3 ->p_wait_ack -> wait_times         = 0;

	p_entry3 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry3 ->p_info ->temperature_threshold   = 0;
	p_entry3 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_3(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry3,current_time);
}

X_Void UartIrqCallBack_3(uint8_t data)
{
   UartIrqHandle(p_entry3,data);
}

/****************************************************************************
terminal 4
**************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry4,4,3,5);

X_Void Terminal4Init(X_Boolean isStartPoint)
{
	p_entry4 ->p_action -> Init(p_entry4 ->p_action ->p_manager); // don't forget null check'
	p_entry4 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry4 ->p_wait_ack -> isReSend           = X_True;
	p_entry4 ->p_wait_ack -> wait_times         = 0;

	p_entry4 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry4 ->p_info ->temperature_threshold   = 0;
	p_entry4 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_4(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry4,current_time);
}

X_Void UartIrqCallBack_4(uint8_t data)
{
   UartIrqHandle(p_entry4,data);
}

/****************************************************************************
terminal 5
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry5,5,4,6);

X_Void Terminal5Init(X_Boolean isStartPoint)
{
	p_entry5 ->p_action -> Init(p_entry5 ->p_action ->p_manager); // don't forget null check'
	p_entry5 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry5 ->p_wait_ack -> isReSend           = X_True;
	p_entry5 ->p_wait_ack -> wait_times         = 0;

	p_entry5 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry5 ->p_info ->temperature_threshold   = 0;
	p_entry5 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_5(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry5,current_time);
}

X_Void UartIrqCallBack_5(uint8_t data)
{
   UartIrqHandle(p_entry5,data);
}

/****************************************************************************
terminal 6
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry6,6,5,7);

X_Void Terminal6Init(X_Boolean isStartPoint)
{
	p_entry6 ->p_action -> Init(p_entry6 ->p_action ->p_manager); // don't forget null check'
	p_entry6 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry6 ->p_wait_ack -> isReSend           = X_True;
	p_entry6 ->p_wait_ack -> wait_times         = 0;

	p_entry6 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry6 ->p_info ->temperature_threshold   = 0;
	p_entry6 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_6(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry6,current_time);
}

X_Void UartIrqCallBack_6(uint8_t data)
{
   UartIrqHandle(p_entry6,data);
}

/****************************************************************************
terminal 7
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry7,7,6,8);

X_Void Terminal7Init(X_Boolean isStartPoint)
{
	p_entry7 ->p_action -> Init(p_entry7 ->p_action ->p_manager); // don't forget null check'
	p_entry7 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry7 ->p_wait_ack -> isReSend           = X_True;
	p_entry7 ->p_wait_ack -> wait_times         = 0;

	p_entry7 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry7 ->p_info ->temperature_threshold   = 0;
	p_entry7 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_7(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry7,current_time);
}

X_Void UartIrqCallBack_7(uint8_t data)
{
   UartIrqHandle(p_entry7,data);
}

/****************************************************************************
terminal 8
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry8,8,7,9);

X_Void Terminal8Init(X_Boolean isStartPoint)
{
	p_entry8 ->p_action -> Init(p_entry8 ->p_action ->p_manager); // don't forget null check'
	p_entry8 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry8 ->p_wait_ack -> isReSend           = X_True;
	p_entry8 ->p_wait_ack -> wait_times         = 0;

	p_entry8 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry8 ->p_info ->temperature_threshold   = 0;
	p_entry8 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_8(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry8,current_time);
}

X_Void UartIrqCallBack_8(uint8_t data)
{
   UartIrqHandle(p_entry8,data);
}

/****************************************************************************
terminal 9
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry9,9,8,10);

X_Void Terminal9Init(X_Boolean isStartPoint)
{
	p_entry9 ->p_action -> Init(p_entry9 ->p_action ->p_manager); // don't forget null check'
	p_entry9 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry9 ->p_wait_ack -> isReSend           = X_True;
	p_entry9 ->p_wait_ack -> wait_times         = 0;

	p_entry9 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry9 ->p_info ->temperature_threshold   = 0;
	p_entry9 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_9(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry9,current_time);
}

X_Void UartIrqCallBack_9(uint8_t data)
{
   UartIrqHandle(p_entry9,data);
}

/****************************************************************************
terminal 10
***************************************************************************/
APP_DG_TERMINAL_DEFINE(p_entry10,10,9,0xff);

X_Void Terminal10Init(X_Boolean isStartPoint)
{
	p_entry10 ->p_action -> Init(p_entry10 ->p_action ->p_manager); // don't forget null check'
	p_entry10 ->p_wait_ack -> isExpectAckHasCome = X_True;
	p_entry10 ->p_wait_ack -> isReSend           = X_True;
	p_entry10 ->p_wait_ack -> wait_times         = 0;

	p_entry10 ->p_wait_ack -> isStartPoint       =  isStartPoint;

	p_entry10 ->p_info ->temperature_threshold   = 0;
	p_entry10 ->p_info ->DG_wave_speed   		   = 0;
}

X_Void main_loop_10(uint32_t current_time)
{
   //INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry10,current_time);
}

X_Void UartIrqCallBack_10(uint8_t data)
{
   UartIrqHandle(p_entry10,data);
}









#ifndef __LEDDISPLAY_H
#define __LEDDISPLAY_H


#ifdef __cplusplus
	extern "C"{
#endif


#include "RGB_Color.h"
#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/loopqueue/loop_queues.h"
#include "../../common/StateMachine/StateMachine.h"
#ifdef USE_LOG_DEBUG_IN_GOOGLE_TEST
	#include "./test/LogDebugSwitch.h"
#endif

#define LED_ON_INFINITE_TIME   0xffffffff
#define LD_COLOR_OFF								(COLOR_WITH_FULL_TRANSPORT(COLOR_RGB_Black))

typedef X_Boolean(*onwait)(X_Void);

typedef enum
{
	LedBlink,
	LedHoldOn,
	LedHoldOnRecoverable,
}eLedDisplayMode;

typedef struct
{
	uint32_t color;
	uint16_t led_on_time;// in ms
	uint16_t led_off_time;// in ms
	uint16_t on_off_cycle;
}sLedBlinkParam;

typedef struct
{
	eLedDisplayMode event_mode;
	sLedBlinkParam  param;
}sLedDisplayEvent;

typedef struct
{
	X_Boolean isInitOK;
	X_Boolean isEnable;
}sLedDisplayCommonFlag;

typedef struct
{
	X_Void (*init)(X_Void);
	X_Void (*draw)(uint32_t color);
	X_Void (*off)(X_Void);
	X_Boolean (*DoesLedOn)(X_Void);
	X_Void (*pow_apply)(uint32_t time_in_ms);// suggestion: the param is total blink times in ms ; user could add the power setup time inside the function 
	X_Void (*pow_release)(X_Void);
	X_Boolean (*DoesPowerOn)(X_Void);
}sLedDisplayFunc;

typedef struct
{
	sLedDisplayCommonFlag *			p_flag;
	uint16_t 						max_event_to_cache;
	s_StateMachineParam   *			p_state_param;
	const s_StateMachine  *			p_state_machine;
}sLedDisPlayManager;


typedef struct
{
	s_StateMachineParam 			base;
	sLedDisplayFunc		   const 	display;
	sLedDisplayEvent      *const    p_event_buf;
	s_QueueOperation      *const    p_operation;
	uint16_t 			  const		max_event_to_cache;
	uint16_t              const     state_interval;
	uint16_t 			  const     wait_power_setup_time;
	X_Boolean 						is_power_ctrl_needed;
	sLedDisplayEvent	  			*p_current_event;
	StateNumber						state_backup;
	uint16_t 						event_buf_number_backup;
	uint16_t 						wait_counter_in_ms;
	X_Boolean               		(*onWaitMethod)(X_Void);
	uint16_t						blink_cycle_counter;
	uint32_t 						recover_color;
}sLedStateParam;

StateNumber LS_IdleAction(s_StateMachineParam *p_this);
StateNumber LS_LoadEventAction(s_StateMachineParam *p_this);
StateNumber LS_ReadyForEventAction(s_StateMachineParam *p_this);
StateNumber LS_RecoverAction(s_StateMachineParam *p_this);
StateNumber LS_BlinkOnAction(s_StateMachineParam *p_this);
StateNumber LS_BlinkOffAction(s_StateMachineParam *p_this);
StateNumber LS_WaitAction(s_StateMachineParam *p_this);

#define APP_LED_DISPLAY_MODULE_DEF(	p_manager,												\
								 	color_init,												\
								 	color_draw,												\
								 	color_off,												\
								 	does_led_on,											\
								 	power_apply,											\
								 	power_release,											\
								 	does_power_on,											\
								 	max_event_num,											\
								 	handle_frequency_in_ms,									\
								 	power_setup_in_ms)										\
		static sLedDisplayCommonFlag  CONCAT_2(p_manager, led_display_flag_entry) = {X_False,X_False};		\
		static sLedDisplayEvent 	CONCAT_2(p_manager, led_display_event_buf)[max_event_num];		\
		APP_LOOPQUEUE_DEF(CONCAT_2(p_manager,_led_event_queue),max_event_num);									\
		static s_QueueOperation      CONCAT_2(p_manager,_led_display_queue) = {						\
				0,																				\
				0,																				\
				CONCAT_2(p_manager,_led_event_queue),														\
				LoopQueueInitialize,															\
				LoopQueueFirstIn,																\
				LoopQueueFirstOut,															\
				ClearLoopQueue,																\
				RealseLoopQueueBuf,															\
				GetLoopQueueUsedNodeNumber,													\
				DoesLoopQueueEmpty,															\
		};																						\
		static sLedStateParam CONCAT_2(p_manager,_sLSP) = {										\
				{DEFAULT_STATE_NUMBER},															\
				{	color_init,																						\
					color_draw,																						\
					color_off,																					\
					does_led_on,																				\
					power_apply,																				\
					power_release,																					\
					does_power_on,		},																				\
				&CONCAT_2(p_manager, led_display_event_buf)[0],												\
				&CONCAT_2(p_manager,_led_display_queue),												\
				max_event_num,																				\
				handle_frequency_in_ms,																			\
				power_setup_in_ms,																				\
				X_False,(sLedDisplayEvent*)0,DEFAULT_STATE_NUMBER,0,0,(onwait)0,0,LD_COLOR_OFF									\
			};																					\
		static const StateAction CONCAT_2(p_manager,_SimpleStateAction)[] = {						\
				{LS_IdleAction},																		\
				{LS_LoadEventAction},																		\
				{LS_ReadyForEventAction},																		\
				{LS_RecoverAction},																					\
				{LS_BlinkOnAction},																					\
				{LS_BlinkOffAction},																				\
				{LS_WaitAction},																					\
		};																													\
		APP_STATE_MACHINE_DEF(CONCAT_2(p_manager,_p_simple_state),																				\
							sizeof(CONCAT_2(p_manager,_SimpleStateAction))/sizeof(CONCAT_2(p_manager,_SimpleStateAction)[0]),		\
							&CONCAT_2(p_manager,_SimpleStateAction)[0]);							\
		static const sLedDisPlayManager CONCAT_2(p_manager, led_display_entry) = {							\
			&CONCAT_2(p_manager, led_display_flag_entry),													\
			max_event_num,																				\
			&(CONCAT_2(p_manager,_sLSP).base),																\
			CONCAT_2(p_manager,_p_simple_state),													\
		};																							\
		static const sLedDisPlayManager * p_manager = &CONCAT_2(p_manager, led_display_entry)


X_Void LedDisplayInit(const sLedDisPlayManager *p_manager);
X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager);// call it every "handle_frequency_in_ms" ms
X_Boolean LedDisplayEventRegister(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event);
X_Void LedDisplayEnableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager);

#ifdef __cplusplus
		}
#endif


#endif

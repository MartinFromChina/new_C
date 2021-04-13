#ifndef __LEDDISPLAY_H
#define __LEDDISPLAY_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/StateMachine/StateMachine.h"


#define LED_DIRECT_ON_INFINITE   0xffff

typedef struct
{
	uint16_t led_on_time;// in ms
	uint16_t led_off_time;// in ms
	uint16_t on_off_cycle;
}sLedDirBlinkParam;

typedef struct
{
	X_Boolean isInitOK;
	X_Boolean isEnable;
}sLedDirDisplayCommonFlag;

typedef struct
{
	X_Void (*init)(X_Void);
	X_Void (*on)(X_Void);
	X_Void (*off)(X_Void);
	X_Boolean (*DoesLedOn)(X_Void);
}sLedDirDisplayFunc;

typedef struct
{
	sLedDirDisplayCommonFlag *			p_flag;
	s_StateMachineParam   *			p_state_param;
	const s_StateMachine  *			p_state_machine;
}sLedDirDisPlayManager;


typedef struct
{
	s_StateMachineParam 	base;
	sLedDirDisplayFunc		    const 	display;
	uint16_t              const state_interval;
	uint16_t						  blink_cycle_counter;
	X_Boolean 						isNewEventCome;
	sLedDirBlinkParam     blink_param;
}sLedDirStateParam;

StateNumber LDS_IdleAction(s_StateMachineParam *p_this);
StateNumber LDS_BlinkOnAction(s_StateMachineParam *p_this);
StateNumber LDS_BlinkOffAction(s_StateMachineParam *p_this);
StateNumber LDS_BlinkFinishAction(s_StateMachineParam *p_this);

#define APP_LED_DIRECT_DISPLAY_MODULE_DEF(p_manager,												\
								 	led_init,												\
								 	led_on,												\
								 	led_off,												\
								 	does_led_on,											\
								 	handle_frequency_in_ms)									\
		static sLedDirDisplayCommonFlag  CONCAT_2(p_manager, led_dir_display_flag_entry) = {X_False,X_False};		\
		static sLedDirStateParam CONCAT_2(p_manager,_sLDSP) = {										\
				{DEFAULT_STATE_NUMBER},															\
				{	led_init,																						\
					led_on,																						\
					led_off,																					\
					does_led_on},																				\
				handle_frequency_in_ms,																			\
				0,X_False,{0,0,0}																		\
			};																					\
		static const StateAction CONCAT_2(p_manager,_LedDirStateAction)[] = {						\
				{LDS_IdleAction},																		\
				{LDS_BlinkOnAction},																		\
				{LDS_BlinkOffAction},																		\
				{LDS_BlinkFinishAction},																\
		};																													\
		APP_STATE_MACHINE_DEF_WITHOUT_POINTER(CONCAT_2(p_manager,_p_led_dir_state),																				\
							sizeof(CONCAT_2(p_manager,_LedDirStateAction))/sizeof(CONCAT_2(p_manager,_LedDirStateAction)[0]),		\
							&CONCAT_2(p_manager,_LedDirStateAction)[0]);							\
		static const sLedDirDisPlayManager CONCAT_2(p_manager, led_dir_display_entry) = {							\
			&CONCAT_2(p_manager, led_dir_display_flag_entry),																			\
			&(CONCAT_2(p_manager,_sLDSP).base),																\
			&CONCAT_2(p_manager,_p_led_dir_state_entry),													\
		};																							\
		static const sLedDirDisPlayManager * p_manager = &CONCAT_2(p_manager, led_dir_display_entry)


X_Void LedDirectDisplayInit(const sLedDirDisPlayManager *p_manager);
X_Void LedDirectDisplayHandle(const sLedDirDisPlayManager *p_manager);// call it every "handle_frequency_in_ms" ms
X_Boolean LedDirectDisplayEventRegister(const sLedDirDisPlayManager *p_manager,sLedDirBlinkParam *p_event);
X_Void LedDirectDisplayEnable(const sLedDirDisPlayManager *p_manager);
X_Void LedDirectDisplayDisable(const sLedDirDisPlayManager *p_manager);
X_Void LedDirectDisplayReset(const sLedDirDisPlayManager *p_manager);

#ifdef __cplusplus
		}
#endif


#endif

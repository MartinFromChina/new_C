#ifndef __LEDDISPLAY_H
#define __LEDDISPLAY_H


#ifdef __cplusplus
	extern "C"{
#endif


#include "RGB_Color.h"
#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/loopqueue/loop_queues.h"

#define LED_ON_INFINITE_TIME   0xffff

typedef enum
{
	LedBlink,
	LedReset,
	LedEnable,
	LedDisable,
	LedEnableImmediately,
	LedDisableImmediately,
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
	sLedDisplayCommonFlag *p_flag;
	uint16_t max_event_to_cache;
	X_Void (*init)(X_Void);
	X_Void (*draw)(uint32_t color);
	sLedDisplayEvent      *const    p_event_buf;
	s_QueueOperation      *const    p_operation; 
}sLedDisPlayManager;

#define APP_LED_DISPLAY_MODULE_DEF(p_manager,color_init,color_draw,color_off,max_event_num,handle_frequency_in_ms)   		\
		static sLedDisplayCommonFlag  CONCAT_2(p_manager, led_display_flag_entry) = {X_False,X_False};		\
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
		static sLedDisplayEvent 	CONCAT_2(p_manager, led_display_event_buf)[max_event_num];		\
		static const sLedDisPlayManager CONCAT_2(p_manager, led_display_entry) = {							\
			&CONCAT_2(p_manager, led_display_flag_entry),													\
			max_event_num,																				\
			color_init,																						\
			color_draw,																						\
			&CONCAT_2(p_manager, led_display_event_buf)[0],												\
			&CONCAT_2(p_manager,_led_display_queue),												\
		};																							\
		static const sLedDisPlayManager * p_manager = &CONCAT_2(p_manager, led_display_entry)


X_Void LedDisplayInit(const sLedDisPlayManager *p_manager);
X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager);// call it every "handle_frequency_in_ms" ms
X_Boolean LedDisplayEventRegister(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event);
X_Void LedDisplayEnable(const sLedDisPlayManager *p_manager);
X_Void LedDisplayDisable(const sLedDisPlayManager *p_manager);
X_Void LedDisplayEnableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager);

#ifdef __cplusplus
		}
#endif


#endif

#ifndef __LEDDISPLAY_H
#define __LEDDISPLAY_H


#ifdef __cplusplus
	extern "C"{
#endif


#include "RGB_Color.h"
#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/loopqueue/loop_queues.h"

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
	uint8_t i;
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
	X_Void (*init)(X_Void);
	X_Void (*draw)(X_Void);
}sLedDisPlayManager;

#define APP_LED_DISPLAY_MODULE_DEF(p_manager,color_init,color_draw,max_event_num)   						\
		static sLedDisplayCommonFlag  CONCAT_2(p_manager, led_display_flag_entry) = {X_False,X_False};		\
		static const sLedDisPlayManager CONCAT_2(p_manager, led_display_entry) = {							\
			&CONCAT_2(p_manager, led_display_flag_entry),													\
			color_init,																						\
			color_draw,																						\
		};

X_Void LedDisplayInit(const sLedDisPlayManager *p_manager);
X_Void LedDisplayHandle(const sLedDisPlayManager *p_manager);
X_Boolean LedDisplayRegisterEvent(const sLedDisPlayManager *p_manager,sLedDisplayEvent *p_event);
X_Void LedDisplayEnable(const sLedDisPlayManager *p_manager);
X_Void LedDisplayDisable(const sLedDisPlayManager *p_manager);
X_Void LedDisplayEnableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayDisableImmediately(const sLedDisPlayManager *p_manager);
X_Void LedDisplayReset(const sLedDisPlayManager *p_manager);

#ifdef __cplusplus
		}
#endif


#endif

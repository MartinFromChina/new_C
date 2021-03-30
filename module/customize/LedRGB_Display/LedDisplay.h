#ifndef __LEDDISPLAY_H
#define __LEDDISPLAY_H


#ifdef __cplusplus
	extern "C"{
#endif


#include "RGB_Color.h"
#include "../../common/x_cross_platform.h"


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
	eLedDisplayMode event_mode;
}sLedDisplayEvent;


typedef struct
{
	uint8_t i;
}sLedDisPlayManager;

#define APP_LED_DISPLAY_DEF(p_manager)


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

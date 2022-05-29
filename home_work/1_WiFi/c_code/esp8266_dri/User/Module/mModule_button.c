#include  "mModule_button.h"
#include "ButtonStateMonitor.h"
#include "mDri_GPIO.h"
//INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

#define CURRENT_BUTTON_MODE    (0x0000)     // 0000_0000_0000_0000

#define USER_KEYBOARD_VALID_PUSH_IN_MS_DEFAULT                10
#define USER_KEYBOARD_VALID_RELEASE_IN_MS_DEFAULT             16
#define USER_KEYBOARD_VALID_LONG_PUSH_IN_MS_DEFAULT           600
#define USER_KEYBOARD_VALID_LONG_RELEASE_IN_MS_DEFAULT        150

#define USER_VALID_LONG_PUSH_IN_MS_DEFAULT   USER_KEYBOARD_VALID_LONG_PUSH_IN_MS_DEFAULT
/*************************************
*************************************/
#define UserNeedClick
#define UserNeedDoubleClick
#define UserNeedLongPush
#define UserNeedLongPushRelease

/*****************************long push*******************************/
#ifdef UserNeedLongPush
static void Button1_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 LongPush \r\n"));
}
static void Button2_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 LongPush \r\n"));
}
static void Button3_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 LongPush \r\n"));
}
static void Button4_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 LongPush \r\n"));
}
static void Button5_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 LongPush \r\n"));
}
static void Button6_LongPushDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 LongPush \r\n"));
}
static struct _LongPushDoing
{
	void (*Action)(void);
}
const LongPushDoing[]={
	{Button1_LongPushDoing},
	{Button2_LongPushDoing},
	{Button3_LongPushDoing},
	{Button4_LongPushDoing},
	{Button5_LongPushDoing},
	{Button6_LongPushDoing},
};

static void LongPushBehavior(const CombineButtonValue value)
{
	uint8_t i;
	
	for(i=0;i<sizeof(LongPushDoing)/sizeof(LongPushDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(LongPushDoing[i].Action != X_Null)
			{
				LongPushDoing[i].Action();
			}
		}
	}
}

#endif

/*****************************long push rerlease*******************************/
#ifdef UserNeedLongPushRelease
static void Button1_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}
static void Button2_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}
static void Button3_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button4_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button5_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button6_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static struct _LongPushReleaseDoing
{
	void (*Action)(uint16_t tickcyle);
}
const LongPushReleaseDoing[]={
	{Button1_LongPushReleaseDoing},
	{Button2_LongPushReleaseDoing},
	{Button3_LongPushReleaseDoing},
	{Button4_LongPushReleaseDoing},
	{Button5_LongPushReleaseDoing},
	{Button6_LongPushReleaseDoing},
};

static void LongPushFinishedBehavior(const CombineButtonValue value,uint16_t const*longpushtickcycle)
{
	uint8_t i;

	for(i=0;i<sizeof(LongPushReleaseDoing)/sizeof(LongPushReleaseDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(LongPushReleaseDoing[i].Action != X_Null)
			{
				LongPushReleaseDoing[i].Action(longpushtickcycle[i]);
			}
		}
	}

}
#endif

/***************************** click*******************************/
#ifdef UserNeedClick
static void Button1_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 Click \r\n"));
}
static void Button2_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 Click \r\n"));
}
static void Button3_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 Click \r\n"));
}
static void Button4_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 Click \r\n"));
}
static void Button5_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 Click \r\n"));
}
static void Button6_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 Click \r\n"));
}

static struct _ClickDoing
{
	void (*Action)(void);
}
const ClickDoing[]={
	{Button1_ClickDoing},
	{Button2_ClickDoing},
	{Button3_ClickDoing},
	{Button4_ClickDoing},
	{Button5_ClickDoing},
	{Button6_ClickDoing},
};
static void ClickBehavior(const CombineButtonValue value)
{
	uint8_t i;

	for(i=0;i<sizeof(ClickDoing)/sizeof(ClickDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(ClickDoing[i].Action != X_Null)
			{
				ClickDoing[i].Action();
			}
		}
	}
}
#endif

/*****************************double click*******************************/
#ifdef UserNeedDoubleClick
static void Button1_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 Double Click \r\n"));
}
static void Button2_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 Double Click \r\n"));
}
static void Button3_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 Double Click \r\n"));
}
static void Button4_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 Double Click \r\n"));
}
static void Button5_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 Double Click \r\n"));
}
static void Button6_DoubleClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 Double Click \r\n"));
}

static struct _DoubleClickDoing
{
	void (*Action)(void);
}
const DoubleClickDoing[]={
	{Button1_DoubleClickDoing},
	{Button2_DoubleClickDoing},
	{Button3_DoubleClickDoing},
	{Button4_DoubleClickDoing},
	{Button5_DoubleClickDoing},
	{Button6_DoubleClickDoing},
};

static void DoubleClickBehavior(const CombineButtonValue value)
{
	uint8_t i;

	for(i=0;i<sizeof(DoubleClickDoing)/sizeof(DoubleClickDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(DoubleClickDoing[i].Action != X_Null)
			{
				DoubleClickDoing[i].Action();
			}
		}
	}
}
#endif

static const sParamAboutTime sPAT_User = {
	USER_KEYBOARD_VALID_PUSH_IN_MS_DEFAULT,
	USER_KEYBOARD_VALID_RELEASE_IN_MS_DEFAULT,
	USER_KEYBOARD_VALID_LONG_PUSH_IN_MS_DEFAULT,
	USER_KEYBOARD_VALID_LONG_RELEASE_IN_MS_DEFAULT,
	CURRENT_BUTTON_MODE,
};
static X_Void ButtonTimeParamConfig(sParamSingleButton * p_spsb)
{
	p_spsb ->p_spat = &sPAT_User;
}


CUSTOM_BUTTON_MONITOR_6_DEF(p_monitor
						   ,ACTION_FREQ_IN_MS												
						   ,X_Null
						   ,mDri_GetButtonsValue
						   ,ButtonTimeParamConfig
						   ,ClickBehavior
						   ,DoubleClickBehavior
						   ,LongPushBehavior
						   ,LongPushFinishedBehavior
							 ,X_Null);

X_Void mModule_ButtonInit(X_Void)
{
	
}
X_Void mModule_ButtonActionHandle(X_Void)
{
	CombineButtonValue value;
  ButtonStateMonitor(p_monitor,&value);
}

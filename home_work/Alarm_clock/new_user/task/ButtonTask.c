#include "ButtonTask.h"
#include "../../../../module/component/object_oriented_again/oop.h"
#include "../../../../module/component/const_loop_scheduler/const_loop_scheduler.h"
#include  "../../../../module/customize/ButtonModule/ButtonModule.h"

#define CURRENT_BUTTON_MODE    (0x0000)     // 0000_0000_0000_0000

#define USER_KEYBOARD_VALID_PUSH_IN_MS_DEFAULT                10
#define USER_KEYBOARD_VALID_RELEASE_IN_MS_DEFAULT             16
#define USER_KEYBOARD_VALID_LONG_PUSH_IN_MS_DEFAULT           600
#define USER_KEYBOARD_VALID_LONG_RELEASE_IN_MS_DEFAULT        150

#define USER_VALID_LONG_PUSH_IN_MS_DEFAULT   USER_KEYBOARD_VALID_LONG_PUSH_IN_MS_DEFAULT
#define BUTTON_MODULE_ACTION_FREQ_MS      10

static X_Void mModule_ButtonInit(X_Void);
static X_Void mModule_ButtonActionHandle(X_Void);

/*************************************************************************/
#include "refresh_task_tag.h"

static X_Void button_task_init(uint32_t ms)
{
    ms = ms;
    mModule_ButtonInit();
    SysLogDebug(1,(" button_task_init ms %d\r\n",ms));
}
TASK_BODY_BEGIN(button_task,button_task_init,0,0){
    X_delay(BUTTON_MODULE_ACTION_FREQ_MS);
    X_do(mModule_ButtonActionHandle());
}TASK_BODY_END







#include "controller.h"
extern controller  controller_basic;

#define UserNeedClick
#define UserNeedDoubleClick
#define UserNeedLongPush
#define UserNeedLongPushRelease

/*****************************long push*******************************/
#ifdef UserNeedLongPush
static void Button1_LongPushDoing(void)
{
////	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 LongPush \r\n"));
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(1,ke_long_push,0);
}
static void Button2_LongPushDoing(void)
{
////	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 LongPush \r\n"));
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(2,ke_long_push,0);
}
static void Button3_LongPushDoing(void)
{
//////	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 LongPush \r\n"));
////////    isSpeed_Down_longpush = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(3,ke_long_push,0);
}
static void Button4_LongPushDoing(void)
{
////	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 LongPush \r\n"));
//////    isConfigDone = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(4,ke_long_push,0);
}
static void Button5_LongPushDoing(void)
{
	////INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 LongPush \r\n"));
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(5,ke_long_push,0);
}
static void Button6_LongPushDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 LongPush \r\n"));
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(6,ke_long_push,0);
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
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}
static void Button2_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}
static void Button3_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button4_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button5_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ACTION_FREQ_IN_MS))
										,USER_VALID_LONG_PUSH_IN_MS_DEFAULT
										,(tickcyle*ACTION_FREQ_IN_MS)-USER_VALID_LONG_PUSH_IN_MS_DEFAULT));
	#endif
}

static void Button6_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 LongPush Release; last %d ms : %d + %d\r\n"
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
////////    isAddr_Down_click = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(1,ke_click,0);
}
static void Button2_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 Click \r\n"));
////////    isAddr_Up_click = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(2,ke_click,0);
}
static void Button3_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 Click \r\n"));
////////    isSpeed_Down_click = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(3,ke_click,0);
}
static void Button4_ClickDoing(void)
{
	INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 Click \r\n"));
//////    isConfigDone = X_True;
////////    isSpeed_Up_click = X_True;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(4,ke_click,0);
}
static void Button5_ClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 Click \r\n"));
////////    poll_index ++;
////////    if(poll_index >= CPP_max_poll_num)
////////    {
////////        poll_index = 0;
////////    }
////////    isConfigDone = X_False;
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(5,ke_click,0);
    
}
static void Button6_ClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 Click \r\n"));
//////    if(DoesUnderTestCondition() == X_True)
//////    {
//////        TestStop();
//////    }
//////    else
//////    {
//////        TestStart();
//////    }
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(6,ke_click,0);
    
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
extern X_Void ClearSendRecvCnt(X_Void);

#ifdef UserNeedDoubleClick
static void Button1_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 1 Double Click \r\n"));
    
////////    ClearSendRecvCnt();
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(1,ke_double_click,0);
}
static void Button2_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 2 Double Click \r\n"));
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(2,ke_double_click,0);
}
static void Button3_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 3 Double Click \r\n"));
//////    isSpeed_Down_doubleclick = X_True;
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(3,ke_double_click,0);
}
static void Button4_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 4 Double Click \r\n"));
//////    isConfigDone = X_True;
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(4,ke_double_click,0);
}
static void Button5_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 5 Double Click \r\n"));
//////    isConfigDone = X_False;
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(5,ke_double_click,0);
}
static void Button6_DoubleClickDoing(void)
{
	//INSERT(LogDebug)(BUTTON_DOING_DEBUG,("Button 6 Double Click \r\n"));
//////    if(DoesUnderTestCondition() == X_False)
//////    {
//////        Lora_SR_TestWithRssi();
//////        isSingleTestOnce = X_True;
//////    }
    FUNC_PTR_CHECK(THIS(controller_basic).key_evt,return);
    THIS(controller_basic).key_evt(6,ke_double_click,0);
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

#include "bit_operation.h"
#include "key.h"
/*************************************
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

release ??1  ?? push : 0

byte_rotate_bit
byte_bitoverturn
*************************************/
#define SW_GPIOC_KEYS_MASK       0x000001C0
#define SW_GPIOD_KEYS_MASK       0x0000E000
static uint8_t mDri_GetButtonsValue(X_Void)
{
	uint8_t key0,key1,key2,key3,result;
    
    key0 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4); //PE4
    key1 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3);	//PE3 
    key2 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);//PE2
    key3 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);	//PA0
    
    key0 = (key0 == 1) ? 0 : 1;
    key1 = (key1 == 1) ? 0 : 1;
    key2 = (key2 == 1) ? 0 : 1;
    key3 = (key3 == 1) ? 0 : 1;
	
    result = 0x0F;
	result = key0 + (key1 << 1) + (key2 << 2) + (key3 << 3);
////////    SysLogDebug(1,("button value %2x \r\n",result));
  return (result);
}

CUSTOM_BUTTON_MONITOR_4_DEF(p_monitor
						   ,BUTTON_MODULE_ACTION_FREQ_MS												
						   ,X_Null
						   ,mDri_GetButtonsValue
						   ,ButtonTimeParamConfig
						   ,ClickBehavior
						   ,DoubleClickBehavior
						   ,LongPushBehavior
						   ,LongPushFinishedBehavior
							 ,X_Null);

static X_Void mModule_ButtonInit(X_Void)
{
	
}
static X_Void mModule_ButtonActionHandle(X_Void)
{
	CombineButtonValue value;
    ButtonStateMonitor(p_monitor,&value);
}



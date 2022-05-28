#include "mFunc_SoftTimer.h"
#include "mDri_SoftTimer.h"

#ifndef ENABLE_ISR
	#define ENABLE_ISR          do{}while(0)   
#endif
#ifndef DISABLE_ISR	
	#define DISABLE_ISR 				do{}while(0)
#endif


static uint32_t cnt[SOFT_TIMER_NUMBER];
static volatile X_Boolean isInitOK = X_False;
static uint8_t act_cnt = 0;
static uint16_t run_time_cnt = 0;
static uint32_t sys_run_time = 0;
static volatile uint32_t systick_cnt  = 0;
extern X_Boolean isTimeToAct;
extern	X_Void mModule_LedDisplayHandleInIsrWhenBlocked(X_Void);

static X_Void mFunc_SoftTimerHandle(X_Void);
X_Void mFunc_SoftTimerInit(X_Void)
{
	uint8_t i;
	act_cnt = 0;
	run_time_cnt = 0;
	sys_run_time = 0;
	systick_cnt  = 0;
	for(i=0;i<SOFT_TIMER_NUMBER;i++)
	{
		cnt[i] = 0;
	}
	mDri_SoftTimerInit(mFunc_SoftTimerHandle);
	isInitOK = X_True;
}
uint32_t mFunc_SoftTimerGetSysRunTime(X_Void)
{
	return sys_run_time;
}
uint32_t mFunc_SoftTimerGetSysTickCnt(X_Void)
{
	uint32_t cnt;
	cnt = systick_cnt;
	return cnt;
}
X_Boolean mFunc_SoftTimerSetValue(uint8_t id,uint32_t value)
{
	if(id >= SOFT_TIMER_NUMBER || isInitOK == X_False) 
	{
		// to do : add some assert here 
		return X_False;
	}
	DISABLE_ISR;
	cnt[id] = value;
	ENABLE_ISR;
	
	return X_True;
}
uint32_t mFunc_SoftTimerGetValue(uint8_t id)
{
	uint32_t temp = 0;
	if(id >= SOFT_TIMER_NUMBER || isInitOK == X_False) 
	{
		// to do : add some assert here 
		return 0;
	}
	//DISABLE_ISR;
	temp = cnt[id];
	//ENABLE_ISR;
	return temp;
}
static X_Void mFunc_SoftTimerHandle(X_Void)
{
	uint8_t i;
	systick_cnt ++;
	if(isInitOK == X_False) {return;}
	
	if(act_cnt < (ACTION_FREQ_IN_MS - 1))
	{
		act_cnt ++;
	}
	else
	{
		isTimeToAct = X_True;
		act_cnt = 0;
	}
	
	if(run_time_cnt >= 999)
	{
		run_time_cnt = 0;
		sys_run_time ++;
	}
	else
	{
		run_time_cnt ++;
	}
	
	for(i=0;i<SOFT_TIMER_NUMBER;i++)
	{
		#if (SOFT_TIMER_FREQUENCY_IN_MS == 1)
			if(cnt[i] > 0) {cnt[i]--;}
		#else
			#error [SOFT_TIMER_FREQUENCY_IN_MS isnot 1,so cnt = cnt - SOFT_TIMER_FREQUENCY_IN_MS]
		#endif
	}
}

X_Void mFunc_SoftTimerDelayMs(uint32_t ms)
{
	uint32_t tickstart;
	tickstart = mFunc_SoftTimerGetSysTickCnt();
	
	while(1)
	{
		if((ms == 0) || ((mFunc_SoftTimerGetSysTickCnt() - tickstart) > ms)) {break;}
	}
}

#include "mDri_SoftTimer.h"
#include "mDrive.h"
#if (USE_HAL_TIMER4 != 0)
	#include "tim.h"
#endif

typedef X_Void (*systic_cb)(X_Void);
static systic_cb cb = (systic_cb)0;

X_Void mDri_SoftTimerInit( X_Void (*systic_cb)(X_Void))
{
	cb = systic_cb;
	#if (USE_HAL_TIMER4 != 0)
	MX_TIM4_Init();
	#endif
}
X_Void mDri_SoftTimerHandle(X_Void)
{
	if(cb != X_Null) {cb();}
}
X_Void mDri_SoftTimerDelayMs(uint32_t ms)
{
	uint32_t tickstart;
	tickstart = HAL_GetTick();
	
	while(1)
	{
		if((ms == 0) || ((HAL_GetTick() - tickstart) > ms)) {break;}
	}
}

X_Void mDri_SoftTimerDelayUs(uint32_t Us)
{
	__IO uint32_t Delay = Us * 72 / 8;//(SystemCoreClock / 8U / 1000000U)
  do
  {
    __NOP();
  }
  while (Delay --);
}

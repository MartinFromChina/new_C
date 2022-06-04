#include "mDrive.h"
//////////#include "mDri_SoftTimer.h"
#include "mDri_GPIO.h"
#include "mDri_Uart.h"

extern void SystemClock_Config(void);
X_Void mDriInit(X_Void)
{
	HAL_Init(); // systick 1ms irq enable
	SystemClock_Config();
	mDri_IO_ClockEnable();
	mDri_ButtonIO_Init();
	mDri_UartInit();
	LogDebugInit();
	INSERT(LogDebug)(SYS_INIT_DEBUG,("hello world\r\n"));
}

X_Void mDri_MCU_Reset(X_Void)
{
        __disable_fault_irq();      // STM32 »Ì∏¥Œª  
        NVIC_SystemReset();
}

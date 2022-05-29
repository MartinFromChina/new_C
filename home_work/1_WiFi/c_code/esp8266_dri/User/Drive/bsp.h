#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f1xx_hal.h"
/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
		
#ifdef __cplusplus
		}
#endif

#endif

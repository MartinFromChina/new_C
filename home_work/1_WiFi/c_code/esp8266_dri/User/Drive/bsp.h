#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "stm32f1xx_hal.h"
/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
		
#ifdef __cplusplus
		}
#endif

#endif

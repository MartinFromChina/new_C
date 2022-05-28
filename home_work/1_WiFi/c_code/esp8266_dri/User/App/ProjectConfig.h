#ifndef __PROJECT_CONFIG_H
#define __PROJECT_CONFIG_H

#ifdef __cplusplus
	extern "C"{
#endif

/*********************************************************************************/
#define USE_DEBUG_MODE        1 // !!! in real product this marco must be 0 
/*********************************************************************************/
#define CPU_FIRMWARE_MAJOR_VERSION      			((uint8_t)(0))
#define CPU_FIRMWARE_SECONDARY_VERSION      	((uint8_t)(0))

#define CPU_FIRMWARE_VERSION               ((uint32_t)((CPU_FIRMWARE_MAJOR_VERSION << 8) + CPU_FIRMWARE_SECONDARY_VERSION))
/*********************************************************************************/
#define ACTION_FREQ_IN_MS             10
/*******************************SOFT TIMER********************************************/
#define SOFT_TIMER_NUMBER     				0
#define SOFT_TIMER_FREQUENCY_IN_MS   1 // 1 ms systic irq
/***************************************************************************/

#ifdef __cplusplus
		}
#endif
#endif

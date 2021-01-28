#ifndef __DG_TREMINAL_H
#define __DG_TREMINAL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../../module/common/x_cross_platform.h"


uint8_t GenerateBasicInfSet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t terminal,
								uint8_t forward,uint8_t double_forward,uint8_t backward,uint8_t double_backward);
uint8_t GenerateBasicInfGet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t terminal);
uint8_t GenerateRTC_Config(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t   start_terminal,
									uint16_t  year,uint8_t   mounth,uint8_t   day,uint8_t   hour,uint8_t   minute,uint8_t   second);
uint8_t GenerateInfoMulGet(uint8_t **p_buf,uint8_t src,uint8_t dest,uint8_t   start_terminal);

#ifdef __cplusplus
		}
#endif


#endif



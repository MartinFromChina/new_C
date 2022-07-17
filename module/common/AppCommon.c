#include "AppCommon.h"
#include <stdlib.h>
#include "Math/bit_operation.h"

X_Void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value)
{
	uint16_t i;
	if(length >= 0xffff) {return;}
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}

X_Void twobyteBufInit(uint16_t *p_buf,uint16_t length,uint16_t init_value)
{
	uint16_t i;
	if(length >= 0xffff) {return;}
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}

uint16_t TwoBytesReverse(uint16_t src)
{
	uint8_t high,low;
	low = (uint8_t)src;
	high = (uint8_t)(src >> 8);
	return (high + ((uint16_t)(low<<8)));
}
uint32_t FourBytesReverse(uint32_t src)
{
		uint32_t temp = 0;
		temp = ((src & 0x000000FF) << 24) | ((src & 0x0000FF00) << 8) | 
					 ((src & 0x00FF0000) >> 8) | ((src & 0xFF000000) >> 24) ;
		return temp;
}



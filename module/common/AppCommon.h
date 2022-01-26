#ifndef __APPCOMMON_H
#define __APPCOMMON_H

#ifdef __cplusplus
	extern "C"{
#endif
		
#include "x_cross_platform.h"
#define ARM_COMPILER_PRESENT // ?

#ifndef CopyBuffer
static __inline X_Void __CopyBuffer(X_Void const* src, X_Void* dst, X_UInt length)
{
    X_UInt8 const* _src;
    X_UInt8* _dst;

    _src = (X_UInt8 const*)src;
    _dst = (X_UInt8*)dst;

    while (length != 0)
    {
        *_dst = *_src;
        ++_dst;
        ++_src;
        --length;
    }
}
#define CopyBuffer  __CopyBuffer
#endif

#ifndef ByteGetCheckSum
static __inline uint16_t __ByteGetCheckSum(uint8_t const* src, uint16_t length)
{
   uint16_t i,sum;
	
	 if(src == X_Null) {return 0;}
	 sum = 0;
	 for(i=0;i<length;i++)
	 {
		 sum = src[i] + sum;
	 }
	 return sum;
}
#define ByteGetCheckSum  __ByteGetCheckSum

#endif

X_Void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value);
X_Void twobyteBufInit(uint16_t *p_buf,uint16_t length,uint16_t init_value);
uint16_t TwoBytesReverse(uint16_t src);
uint32_t FourBytesReverse(uint32_t src);
#ifdef __cplusplus
		}
#endif

#endif

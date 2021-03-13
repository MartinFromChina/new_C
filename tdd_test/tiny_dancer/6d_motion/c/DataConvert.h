#ifndef __DATACONVERT_H
#define __DATACONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../module/common/x_cross_platform.h"

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


typedef X_Int32 FixedPointNumber;

#pragma pack(1)
typedef struct
{
	FixedPointNumber x;
	FixedPointNumber y;
	FixedPointNumber z;
}s_3D_acce;

typedef struct
{
	FixedPointNumber x;
	FixedPointNumber y;
	FixedPointNumber z;
}s_3D_tatate;


typedef struct
{
	s_3D_acce acce;
	s_3D_tatate rotate;
}s_6D_data;



#pragma pack()

#ifdef __cplusplus
	}
#endif

#endif

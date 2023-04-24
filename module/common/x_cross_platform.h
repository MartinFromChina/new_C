#ifndef __X_CROSS_PLATFORM_H
#define __X_CROSS_PLATFORM_H

#include <stdint.h>
#include <wchar.h>

#define X_Void      void
typedef int8_t      X_Int8;
typedef uint8_t     X_UInt8;
typedef int16_t     X_Int16;
typedef uint16_t    X_UInt16;
typedef int32_t     X_Int32;
typedef uint32_t    X_UInt32;
typedef int64_t     X_Int64;
typedef uint64_t    X_UInt64;
typedef float       X_Float32;
typedef double      X_Float64;
typedef char        X_Char;
typedef wchar_t     X_WChar;

typedef unsigned int    X_UInt;
typedef int             X_Int;
typedef X_Int64         X_IntLongest;
typedef X_UInt64        X_UIntLongest;

#define X_Null              ((X_Void*)0)

typedef int                 X_Boolean;
#define X_True              (0 == 0)
#define X_False             (0 != 0)

#define MapperAssertion     ((sizeof(X_Int8) == 1) && (sizeof(X_UInt8) == 1) && (sizeof(X_Int16) == 2) && (sizeof(X_UInt16) == 2) && (sizeof(X_Int32) == 4) && (sizeof(X_UInt32) == 4) && (sizeof(X_Float32) == 4) && (sizeof(X_Float64) == 8))

#ifndef __FUNCTION__
    #define __FUNCTION__  "not support "
#endif

#ifndef __func__  
    #define __func__    "not support "
#endif

/*
这两个宏都可以用于获取当前函数名称，但它们在使用方式和适用范围上有所不同。 
 
 __FUNCTION__  宏会在编译时被替换为当前函数的名称字符串，可以在任何函数中使用，包括全局作用域。但是它不是标准宏，不被所有编译器都支持。 
 
 __func__  宏是C99标准的一部分，支持更广泛，并且也会在编译时被替换为当前函数的名称字符串。它只能在函数内使用，因此对于全局作用域或类作用域中的函数， __FUNCTION__  可能是更好的选择。 
 
因此，如果你的编译器支持  __FUNCTION__ ，并且你不需要跨编译器移植代码，则  __FUNCTION__  可以是更通用的选择。如果需要与不同类型的编译器和标准兼容，则应使用  __func__ 。
*/

#endif

#include "CommonMarco.h"

#undef __MFUNC_OUT_DEC_DIGIT_TEMP0
#undef __MFUNC_OUT_DEC_DIGIT_TEMP1
#undef __MFUNC_OUT_DEC_DIGIT_TEMP2
#undef __MFUNC_OUT_DEC_STR_TEMP

/* 获取个位 */
#if     (MFUNC_IN_U8_DEC_VALUE % 10) == 0
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       0
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 1
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       1
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 2
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       2
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 3
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       3
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 4
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       4
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 5
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       5
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 6
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       6
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 7
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       7
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 8
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       8
#elif   (MFUNC_IN_U8_DEC_VALUE % 10) == 9
#   define __MFUNC_OUT_DEC_DIGIT_TEMP0       9
#endif

/* 获取十位数字 */
#if     ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 0
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       0
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 1
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       1
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 2
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       2
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 3
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       3
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 4
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       4
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 5
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       5
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 6
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       6
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 7
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       7
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 8
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       8
#elif   ((MFUNC_IN_U8_DEC_VALUE/10) % 10) == 9
#   define __MFUNC_OUT_DEC_DIGIT_TEMP1       9
#endif

/* 获取百位数字 */
#if     ((MFUNC_IN_U8_DEC_VALUE/100) % 10) == 0
#   define __MFUNC_OUT_DEC_DIGIT_TEMP2       0
#elif   ((MFUNC_IN_U8_DEC_VALUE/100) % 10) == 1
#   define __MFUNC_OUT_DEC_DIGIT_TEMP2       1
#elif   ((MFUNC_IN_U8_DEC_VALUE/100) % 10) == 2
#   define __MFUNC_OUT_DEC_DIGIT_TEMP2       2
#endif

#if __MFUNC_OUT_DEC_DIGIT_TEMP2 == 0 
#   if __MFUNC_OUT_DEC_DIGIT_TEMP1 == 0
#       define MFUNC_OUT_DEC_STR        __MFUNC_OUT_DEC_DIGIT_TEMP0
#   else
#       define MFUNC_OUT_DEC_STR        CONCAT_2(   __MFUNC_OUT_DEC_DIGIT_TEMP1,\
                                                    __MFUNC_OUT_DEC_DIGIT_TEMP0)
#   endif
#else
#   define MFUNC_OUT_DEC_STR            CONCAT_3(   __MFUNC_OUT_DEC_DIGIT_TEMP2,\
                                                    __MFUNC_OUT_DEC_DIGIT_TEMP1,\
                                                    __MFUNC_OUT_DEC_DIGIT_TEMP0)
#endif


#undef MFUNC_IN_U8_DEC_VALUE

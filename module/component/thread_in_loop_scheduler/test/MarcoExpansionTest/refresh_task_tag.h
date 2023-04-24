#undef X_TASK_CREATE
/**********************************************************************************************************************/
#ifndef TASK_TAG_INDEX
    #warning [not define index]
    #define TASK_TAG_INDEX  0
#endif

#if (TASK_TAG_INDEX == 1)
    //#warning [ index 1]
    #undef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  1
#endif

#if (TASK_TAG_INDEX == 2)
    //#warning [ index 1]
    #undef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  2
#endif

#if (TASK_TAG_INDEX == 3)
    //#warning [ index 1]
    #undef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  3
#endif

#if (TASK_TAG_INDEX == 4)
    //#warning [ index 1]
    #undef TASK_TAG_INDEX
    #define TASK_TAG_INDEX  4
#endif

/**********************************************************************************************************************/  
#undef MFUNC_IN_U8_DEC_VALUE
#undef MFUNC_OUT_DEC_STR
#define  MFUNC_IN_U8_DEC_VALUE      (TASK_TAG_INDEX + 1)

#include "mf_u8_dec2str.h"

#undef TASK_TAG_INDEX
#define TASK_TAG_INDEX              MFUNC_OUT_DEC_STR
/**********************************************************************************************************************/
#define CUR_TASK_TAG       CONCAT_2(TASK,MFUNC_OUT_DEC_STR)
/**********************************************************************************************************************/
#define X_TASK_CREATE(task_func_name)            \
                            static X_Boolean CONCAT_2(CUR_TASK_TAG,isInit) = X_False;\
                            static uint32_t CONCAT_2(CUR_TASK_TAG,cur_line_num) = 0 ; 



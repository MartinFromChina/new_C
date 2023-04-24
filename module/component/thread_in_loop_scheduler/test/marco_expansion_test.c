#include "../loop_scheduler.h"
static X_Void InitFunc(X_Void)
{

}
/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_1,InitFunc);

TASK_BODY_BEGIN(task_1)


TASK_BODY_END(task_1)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_2,InitFunc);

TASK_BODY_BEGIN(task_2)


TASK_BODY_END(task_2)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_3,InitFunc);

TASK_BODY_BEGIN(task_3)



TASK_BODY_END(task_3)
/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_4,InitFunc);

TASK_BODY_BEGIN(task_4)





TASK_BODY_END(task_4)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_5,InitFunc);

TASK_BODY_BEGIN(task_5)


TASK5cur_line_num = TASK4cur_line_num;


TASK_BODY_END(task_5)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_6,InitFunc);

TASK_BODY_BEGIN(task_6)





TASK_BODY_END(task_6)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_7,InitFunc);

TASK_BODY_BEGIN(task_7)





TASK_BODY_END(task_7)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_8,InitFunc);

TASK_BODY_BEGIN(task_8)





TASK_BODY_END(task_8)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_9,InitFunc);

TASK_BODY_BEGIN(task_9)





TASK_BODY_END(task_9)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_10,InitFunc);

TASK_BODY_BEGIN(task_10)





TASK_BODY_END(task_10)

/**************************************************************************************/
#include "../refresh_task_tag.h"
X_TASK_CREATE(task_11,InitFunc);

TASK_BODY_BEGIN(task_11)





TASK_BODY_END(task_11)

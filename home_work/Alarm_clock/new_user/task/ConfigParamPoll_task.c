#include "ConfigParamPoll_task.h"
#include "const_loop_scheduler.h"
#include "model.h"

extern model model1;

static X_Void TimeUpAndNotify(X_Void)
{
    FUNC_PTR_CHECK(THIS(model1).time_notify,return);
    FUNC_PTR_CHECK(THIS(model1).time_up,return);
    THIS(model1).time_up();
    THIS(model1).time_notify();
}
static X_Void TimeDownAndNotify(X_Void)
{
    FUNC_PTR_CHECK(THIS(model1).time_notify,return);
    FUNC_PTR_CHECK(THIS(model1).time_down,return);
    THIS(model1).time_down();
    THIS(model1).time_notify();
}
/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(time_up_task,X_Null,0,0){
   X_delay(10);
   X_do(TimeUpAndNotify());
   
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(time_down_task,X_Null,0,0){
   X_delay(10);
   X_do(TimeDownAndNotify());
   
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

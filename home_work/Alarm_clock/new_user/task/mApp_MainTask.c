#include "mApp_MainTask.h"
#include "mTask.h"
#include "const_loop_scheduler.h"

#include "audioplay.h"	
/*************************************************************************/
#include "refresh_task_tag.h"

static X_Void main_task_init(uint32_t ms)
{
    ms = ms;
}


TASK_BODY_BEGIN(main_task,main_task_init,0,0){
   X_delay(21600000);//   6 * 60 * 60 * 1000 = 
   X_do(SysLogDebug(1,(" audio play ms %d \r\n",x_task_param_ms)));
   X_do(audio_play());
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#include "mApp_MainTask.h"
#include "mTask.h"
#include "const_loop_scheduler.h"

#include "audioplay.h"	

#define HOUR_MS         3600000
#define MINUTE_MS       60000
#define SECOND_MS       1000

/*************************************************************************/
#include "refresh_task_tag.h"

uint32_t cur_delay_ms = 21600000; //   6 * 60 * 60 * 1000 = 

static X_Void main_task_init(uint32_t ms)
{
    ms = ms;
    uint32_t hour,minute,second;
    
    hour    = cur_delay_ms/HOUR_MS;
    minute  = (cur_delay_ms%HOUR_MS)/MINUTE_MS;
    second  = (cur_delay_ms%MINUTE_MS)/SECOND_MS;
    
    SysLogDebug(1,(" audio will play after ms %d ; means %d hours, %d minutes, %d seconds\r\n"
                        ,cur_delay_ms,hour,minute,second));
}


TASK_BODY_BEGIN(main_task,main_task_init,0,0){
   X_delay(cur_delay_ms);
   X_do(SysLogDebug(1,(" audio play ms %d \r\n",x_task_param_ms)));
   X_do(audio_play());
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


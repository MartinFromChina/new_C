#include "ConfigParamPoll_task.h"
#include "const_loop_scheduler.h"
#include  "mModule_Lora.h"
#include  "mModule_button.h"
#include "mDri_VGUS_LCD.h"
#include "../../../../../../../../module/common/AppCommon.h"

extern X_Boolean isConfigDone;
/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(distance_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_distance && isConfigDone == X_False)
    {
       X_do(LcdDisplayDistance(0));
       X_delay(200);
       X_do(LcdDisplayDistance(57));
       X_delay(500);
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/

/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(line_num_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_line_num && isConfigDone == X_False)
    {
       X_do(LcdDisplaylinenum(55));
       X_delay(200);
       X_do(LcdDisplaylinenum(0));
       X_delay(500);
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/
 
/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(device_num_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_device_num && isConfigDone == X_False)
    {
       X_do(LcdDisplayDeviceNum(55));
       X_delay(200);
       X_do(LcdDisplayDeviceNum(0));
       X_delay(500);
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/

/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(air_speed_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_air_speed && isConfigDone == X_False)
    {
       
       X_do(LcdDisplayAirSpeed(55));
       X_delay(200);
       X_do(LcdDisplayAirSpeed(0));
       X_delay(500);
        
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/

/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(channel_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_channel && isConfigDone == X_False)
    {
       
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/

/*************************************************************************/
#include "refresh_task_tag.h"

TASK_BODY_BEGIN(frame_length_task,X_Null,0,0){
    X_if(GetCurrentParam() == CPP_frame_length && isConfigDone == X_False)
    {
       
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/


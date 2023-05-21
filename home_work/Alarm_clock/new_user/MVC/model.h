#ifndef __MODEL_H
#define __MODEL_H


#ifdef __cplusplus
 extern "C"{
#endif

#include "../../../../../module/component/object_oriented_again/oop.h"
#include "controller.h"
     
typedef struct model model;
     
CLASS_DEF(model
     ,// public
        X_Boolean (* const constructor)(model * p_this);
        const key_updata key_evt_updata;
        X_Void (* const runner)(uint32_t ms);
        X_Boolean (* const view_observer_reg)(view_updata updata_func);
        X_Void (* const init)(X_Void);
        X_Void (* const time_up)(X_Void);
        X_Void (* const time_down)(X_Void);
        X_Void (* const time_notify)(X_Void);
//////////////        uint32_t (* const get_speed)(X_Void);
     ,// private
        view_updata     view_observer[MAX_VIEW_OBSERVER_NUM];
        uint8_t         observer_num;
        X_Boolean       is_evt_come;
        uint8_t cur_view_num;
        e_view_event cur_ve;
        uint32_t        delay_ms;
        X_Boolean       is_config_begin;
        uint32_t        speed;
        X_Boolean       notify_param_sw;
     
     
);
     
#ifdef __cplusplus
    }
#endif

#endif


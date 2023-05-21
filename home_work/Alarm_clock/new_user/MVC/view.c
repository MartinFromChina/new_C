#include "view.h"
#include "../../../../../module/common/AppCommon.h"
#include "lcd.h"
#include "text.h"	

#define  IMPLEMENT_OF_view     1

static X_Boolean Constructor(view * p_this);
static X_Void Runner(uint32_t ms);
static X_Void Updata(e_basic_key_evt evt,uint8_t key_id);
static X_Void ViewUpdata(e_view_event evt,uint8_t viewer_id,uint32_t delay_ms);

view view1 = {
   // public
	{
        Constructor,
        Updata,
        Runner,
        ViewUpdata,
	},
//////	// protect  :init protect member in construct func 
//////    {0},
    // private  :init private member in construct func
    {0},
};



static X_Boolean Constructor(view * p_this)
{
    if(p_this == (view *)0) {return X_False;}
    PRIVATE_INTERNAL(view,p_this,p_pri);
    
    if(P_THIS(p_this).runner == X_Null){return X_False;}
    p_pri ->hour = 0;
    p_pri ->minute = 0;
    p_pri ->sec = 0;
    
    return X_True;
}

static X_Void Runner(uint32_t ms)
{
    ms = ms;
}

static X_Void Updata(e_basic_key_evt evt,uint8_t key_id)
{
//////    SysLogDebug(1,(" view1 key %d evt %d \r\n",key_id,evt));
}

#define HOUR_MS         3600000
#define MINUTE_MS       60000
#define SECOND_MS       1000

#define STRING_BUF_SIZE     200

#define OFFSET_POS         100

static X_Void ViewUpdata(e_view_event evt,uint8_t viewer_id,uint32_t delay_ms)
{
    uint8_t string_buf[STRING_BUF_SIZE];
    uint32_t hour,minute,second;
    PRIVATE_INTERNAL(view,&view1,p_pri);
    
    if(viewer_id != 1) {return;}
    
    hour    = delay_ms/HOUR_MS;
    minute  = (delay_ms%HOUR_MS)/MINUTE_MS;
    second  = (delay_ms%MINUTE_MS)/SECOND_MS;
    
    p_pri ->hour = hour;
    p_pri ->minute = minute;
    p_pri ->sec = second;
    
    if(evt == ve_set_alarm_clock)
    {
        Show_Str(60,250 + OFFSET_POS,200,16,"alarm_clock_set    ",16,0);
    }
    else if(evt == ve_time_updata)
    {
        byteBufInit(string_buf,200,'\0');
////////        left_length = STRING_BUF_SIZE;
////////        used_length_this_time = 0;
////////        used_length_this_time = 
        snprintf((char*)string_buf,STRING_BUF_SIZE,"TIME : %d hour %d minute %d second ",hour,minute,second);
//////        if(used_length_this_time <= 0 || left_length < used_length_this_time) {return X_False;}
//////        left_length -= used_length_this_time;
        
        Show_Str(60,200 + OFFSET_POS,400,16,string_buf,16,0);
    }
    else if(evt == ve_config_clock)
    {
        Show_Str(60,250 + OFFSET_POS,200,16,"alarm_clock_config",16,0);
    }
    else if(evt == ve_config_speed)
    {
        byteBufInit(string_buf,200,'\0');
        snprintf((char*)string_buf,STRING_BUF_SIZE,"cur speed %d          ",delay_ms); // use delay ms as speed for now 
        Show_Str(60,225 + OFFSET_POS,400,16,string_buf,16,0);
    }
}

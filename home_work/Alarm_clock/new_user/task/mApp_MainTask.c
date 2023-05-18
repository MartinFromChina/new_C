#include "mApp_MainTask.h"
#include "mTask.h"
#include "const_loop_scheduler.h"
#include "loraTask.h"
#include "mDri_VGUS_LCD.h"
#include  "mModule_button.h"

#define MIN_TEST_PERIOD_S        5
#define MAX_TEST_PERIOD_S        60
#define DEFAULT_TEST_PERIOD_S    5

extern uint32_t mDri_RTC_GetCounter(X_Void);
extern X_Boolean main_task_freeze(X_Void);
extern X_Boolean isConfigDone;

static uint8_t speed_s = DEFAULT_TEST_PERIOD_S;
static X_Boolean isTestStart = X_False;
static X_Boolean isWaitRelayLoraConfig = X_True;

static X_Void CommonTaskFreeze(X_Void)
{
    common_task_freeze(TID_TestDestTask);
        
    common_task_freeze(TID_TestPeriodTask);
    common_task_freeze(TID_DistanceTask);
    common_task_freeze(TID_LineNumTask);
    common_task_freeze(TID_DeviceNumTask);
    common_task_freeze(TID_AirSpeed);
    common_task_freeze(TID_channel);
    common_task_freeze(TID_frame_length);
}
static X_Void CommonTaskRestart(X_Void)
{
    common_task_restart(TID_TestDestTask);
        
    common_task_restart(TID_TestPeriodTask);
    common_task_restart(TID_DistanceTask);
    common_task_restart(TID_LineNumTask);
    common_task_restart(TID_DeviceNumTask);
    common_task_restart(TID_AirSpeed);
    common_task_restart(TID_channel);
    common_task_restart(TID_frame_length);
}
/*************************************************************************/
#include "refresh_task_tag.h"

static uint8_t period_cnt_s = DEFAULT_TEST_PERIOD_S;

static X_Void main_task_init(uint32_t ms)
{
    ms = ms;
    speed_s = DEFAULT_TEST_PERIOD_S;
    period_cnt_s = DEFAULT_TEST_PERIOD_S;
    isTestStart = X_False;
	isWaitRelayLoraConfig = X_True;
    common_task_restart(TID_ButtonTask);
    common_task_restart(TID_LoraTask);
    common_task_restart(TID_DelayTask);
    
    CommonTaskRestart();
    
    LcdDisplayStatus(TS_Wait); // wait for lora config done 
}


TASK_BODY_BEGIN(main_task,main_task_init,1,0){
    X_if(isWaitRelayLoraConfig == X_True)
    {
        X_do(isWaitRelayLoraConfig = X_False);
        X_wait_until(DoesRelayLoraConfigDone() == X_True);
        X_do(LcdDisplayStatus(TS_Stop));
        SysLogDebug(1,(" lora config done \r\n"));
    }X_endif
    
    X_if(isTestStart == X_True)
    {
        X_do(period_cnt_s = (speed_s == 0)? DEFAULT_TEST_PERIOD_S:speed_s);
        X_do(LcdDisplayPeriodLeft(period_cnt_s);Lora_SR_TestWithRssi());
        X_while(period_cnt_s --)
        {
            X_delay(1000);
            X_do(LcdDisplayPeriodLeft(period_cnt_s));
            X_if(isTestStart == X_False)
            {
                X_do(LcdDisplayPeriodLeft(speed_s));
                X_break;
            }
        }X_endloop
    }X_endif
    
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*************************************************************************/
X_Boolean DoesUnderTestCondition(X_Void)
{
    return isTestStart;
}

/*************************************************************************/
uint16_t send_ok_cnt = 0,recv_ok_cnt = 0;

extern X_Boolean isSpeed_Up_click,      isSpeed_Up_longpush;
extern X_Boolean isSpeed_Down_click ,   isSpeed_Down_longpush;
extern X_Boolean isSpeed_Up_doubleclick,isSpeed_Down_doubleclick;
extern X_Boolean isAddr_Up_click,       isAddr_Down_click;

X_Void ClearSendRecvCnt(X_Void)
{
    send_ok_cnt = 0;
    recv_ok_cnt = 0;
    LcdDisplayDistance(send_ok_cnt);
    LcdDisplaylinenum(recv_ok_cnt);
}

X_Void TestStart(X_Void)
{
    if(DoesRelayLoraConfigDone() == X_False)
    {
        SysLogDebug(1,(" can not start test beccause lora config is not finished yet \r\n"));
        return;
    }
    if(isTestStart == X_False)
    {
        isTestStart = X_True;
        
        CommonTaskFreeze();
        SysLogDebug(1,(" test start \r\n"));
        LcdDisplayStatus(TS_Run);
////////////////        LcdDisplayRssi(0xFF,0xFF,0xFF,0xFF);
        LcdDisplayDestAddr(GetCurDestAddr());
        LcdDisplayTestPeriod(speed_s);
        LcdDisplayPeriodLeft(speed_s);
        
        ClearSendRecvCnt();
    }
}

X_Void TestStop(X_Void)
{
    if(isTestStart == X_True)
    {
        isTestStart = X_False;
       
        CommonTaskRestart();
        
        SysLogDebug(1,(" test stop \r\n"));
        LcdDisplayStatus(TS_Stop);
        LcdDisplayRssi(0xFF,0xFF,0xFF,0xFF);
        LcdDisplayDestAddr(GetCurDestAddr());
        LcdDisplayTestPeriod(speed_s);
        LcdDisplayPeriodLeft(speed_s);
    }
}
/*************************************************************************/

static X_Void SpeedUp(X_Void)
{
    speed_s ++;
    if(speed_s > MAX_TEST_PERIOD_S) {speed_s = MIN_TEST_PERIOD_S;}
    
    LcdDisplayTestPeriod(speed_s);
    LcdDisplayPeriodLeft(speed_s);
}
static X_Void SpeedDown(X_Void)
{
    if(speed_s <= MIN_TEST_PERIOD_S) {speed_s = MAX_TEST_PERIOD_S;}
    else {speed_s --;}
    
    LcdDisplayTestPeriod(speed_s);
    LcdDisplayPeriodLeft(speed_s);
}   

uint8_t GetSpeed(X_Void)
{
    return speed_s;
}
/*************************************************************************/


#include "refresh_task_tag.h"

static X_Void speed_task_init(uint32_t ms)
{
    ms = ms;
}


TASK_BODY_BEGIN(speed_task,speed_task_init,0,0){
    
    X_if(GetCurrentParam() == CPP_test_speed && isConfigDone == X_False)
    {
       X_do(LcdDisplayTestPeriod(0);LcdDisplayPeriodLeft(0));
       X_delay(200);
       X_do(LcdDisplayTestPeriod(speed_s);LcdDisplayPeriodLeft(speed_s));
       X_delay(500);
    }X_endif
    
	X_if(isSpeed_Up_longpush == X_True)
    {
        X_do(SpeedUp());
        X_delay(70);
    }
    X_else_if(isSpeed_Down_longpush == X_True)
    {
        X_do(SpeedDown());
        X_delay(70);
    }X_endif
    
    X_if(isSpeed_Up_click == X_True || isSpeed_Up_doubleclick == X_True)
    {
        X_do(isSpeed_Up_click = X_False;isSpeed_Up_doubleclick = X_False; SpeedUp());
    }X_endif
    
    X_if(isSpeed_Down_click == X_True || isSpeed_Down_doubleclick == X_True)
    {
        X_do(isSpeed_Down_click = X_False;isSpeed_Down_doubleclick = X_False; SpeedDown());
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*************************************************************************/

/*************************************************************************/
#define MAX_ADDR_NUM             11
static uint8_t cur_dest_index = 0;
static const uint32_t dest_addr_table[MAX_ADDR_NUM] = {
//////    0x00438000,
//////    0x00438010,
//////    0x00438020,
//////    0x00438030,
//////    0x00438040,
//////    0x00438050,
//////    0x00438060,
//////    0x00438070,
//////    0x00438080,
//////    0x00438090,
    0x00439000,
    0x00439010,
    0x00439020,
    0x00439030,
    0x00439040,
    0x00439050,
    0x00439060,
    0x00439070,
    0x00439080,
    0x00439090,
    
    LOCAL_TERMINAL_NUM,
};

uint32_t GetCurDestAddr(X_Void)
{
    if(cur_dest_index >= MAX_ADDR_NUM) {cur_dest_index = 0;}
    
    return dest_addr_table[cur_dest_index];
}
/*************************************************************************/
static X_Void AddrUp(X_Void)
{
    cur_dest_index ++;
    if(cur_dest_index >= MAX_ADDR_NUM) {cur_dest_index = 0;}
    
    LcdDisplayDestAddr(dest_addr_table[cur_dest_index]);
}

static X_Void AddrDown(X_Void)
{
    if(cur_dest_index > 0) {cur_dest_index --;}
    else {cur_dest_index = MAX_ADDR_NUM - 1;}
    
    LcdDisplayDestAddr(dest_addr_table[cur_dest_index]);
}

#include "refresh_task_tag.h"

static X_Void addr_task_init(uint32_t ms)
{
    ms = ms;
}


TASK_BODY_BEGIN(addr_task,addr_task_init,0,0){
    X_if(isAddr_Up_click == X_True)
    {
        X_do(isAddr_Up_click = X_False;AddrUp());
    }X_endif
    
    X_if(isAddr_Down_click == X_True)
    {
        X_do(isAddr_Down_click = X_False;AddrDown());
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*************************************************************************/

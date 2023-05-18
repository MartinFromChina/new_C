#include "DelayExtTask.h"
#include "const_loop_scheduler.h"
#include  "mModule_Lora.h"
#include "../../../../../../../../module/common/AppCommon.h"

/*************************************************************************/
#include "refresh_task_tag.h"
static delay_todo cur_todo_func = (delay_todo)0;
static uint32_t cur_delay_ms = 0;
static X_Void delay_todo_task_init(uint32_t ms)
{
    ms = ms;
    cur_todo_func = (delay_todo)0;
    cur_delay_ms = 0;
}
TASK_BODY_BEGIN(delay_todo_task,delay_todo_task_init,0,0){
    X_if(cur_delay_ms > 0)
    {
        X_delay(cur_delay_ms);
        X_do(if(cur_todo_func != (delay_todo)0) {cur_todo_func();});
        X_do(cur_delay_ms = 0);
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// �������¼���Ҫע�����������������������ᱻ������������Զ�޷�ִ��ĳ���������
// ����ʹ�ö��У������������ʹ�ö��delay�������¼�һһ��Ӧ
X_Void DelayTodoReg(delay_todo todo_func,uint32_t delay_ms)
{
    cur_todo_func = todo_func;
    cur_delay_ms  = delay_ms;
}
/*************************************************************************/


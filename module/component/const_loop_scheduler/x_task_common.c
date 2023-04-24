#include "x_task_common.h"

X_Void X_Task_ByteBufClear(uint8_t *p_buf,uint16_t length)
{
	uint16_t i;
    if(length == 0 || length == 0xffff ) {return;}

	for(i = 0;i<length;i++)
	{
		p_buf[i] = 0;
	}
}
X_Void X_Task_32bitsBufClear(uint32_t *p_buf,uint16_t length)
{
	uint16_t i;
    if(length == 0 || length == 0xffff ) {return;}

	for(i = 0;i<length;i++)
	{
		p_buf[i] = 0;
	}
}
/********************************************************************************************/
X_Void X_Task_if_line_push(uint16_t loop_deepth,uint16_t if_deepth,uint8_t *p_line_flag_buf)
{
    // no need length or pointer check ,because the only caller is X_Task Marco 
    if(loop_deepth == 0) {return;}
    p_line_flag_buf[if_deepth] = 1;
}

X_Void X_Task_if_line_pop_All(uint16_t loop_deepth,uint16_t max_if_deepth,uint8_t *p_line_flag_buf,uint32_t *P_line_buf)
{
    uint16_t i;
    // no need length or pointer check ,because the only caller is X_Task Marco 
    if(loop_deepth == 0) {return;}

    for(i=0;i<max_if_deepth;i++)
    {
        if(p_line_flag_buf[i] != 0) 
        {
            p_line_flag_buf[i] = 0;
            P_line_buf[i] = 0;
        }
    }
}

X_Void X_Task_loop_line_pop_all(uint16_t loop_deepth,uint16_t max_loop_deepth,uint32_t *P_end_line_buf,uint32_t *P_line_buf)
{
    uint16_t i;
    // no need length or pointer check ,because the only caller is X_Task Marco 
    ////////////printf("loop pop from %d to %d\r\n",loop_deepth,max_loop_deepth-1);
    for(i=loop_deepth;i<max_loop_deepth;i++)
    {
        P_end_line_buf[i] = 0;
        P_line_buf[i] = 0;
    }
}
/********************************************************************************************/

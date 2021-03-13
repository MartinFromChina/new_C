#include <stdio.h>
#include "finally.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"
#include "DataMonitor.h"
#include "DataConvert.h"
#include "MotionFusion.h"



#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

#if (USE_INSERT_DEBUG != 0)
//INSERT(LOG_ONCE_ENTRY_DEF)(p_once,1);
INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);
#endif


static uint8_t i = 0;
uint8_t TestFunction(X_Void)
{
    i = i + 1;
	INSERT(LogDebugCounter)(1,p_counter,100,("hello\r\n")); 
	return i;
}

static s_6D_data raw_6d;
static s_3_angle angle = {0,0,0};
X_Void ReadRawData(uint32_t *p_buf)
{
	uint8_t buf_copy[24],i;
	
	if(p_buf == X_Null) {return;}
	INSERT(LogDebug)(0,("x-------%2x %2x %2x %2x ; y-------  %2x %2x %2x %2x ; z -------  %2x %2x %2x %2x\r\n"
					,p_buf[0],p_buf[1],p_buf[2],p_buf[3]
					,p_buf[4],p_buf[5],p_buf[6],p_buf[7]
					,p_buf[8],p_buf[9],p_buf[10],p_buf[11])); 
	

	for(i=0;i<24;i++)
	{
		buf_copy[i] = (uint8_t)p_buf[i];
	}
	CopyBuffer(buf_copy,&raw_6d.acce,12);
	CopyBuffer(&buf_copy[12],&raw_6d.rotate,12);
	
	INSERT(LogDebug)(0,("acce------  %s %s %s ; rotate ------ %s %s %s \r\n"
					,IntConvertFloatChar(raw_6d.acce.x),IntConvertFloatChar(raw_6d.acce.y),IntConvertFloatChar(raw_6d.acce.z)
					,IntConvertFloatChar(raw_6d.rotate.x),IntConvertFloatChar(raw_6d.rotate.y),IntConvertFloatChar(raw_6d.rotate.z)));
					
	MotionFusion(&raw_6d,&angle);
}

uint8_t GetX_Xita(X_Void)
{
	return angle.alpha;
}
uint8_t GetY_Xita(X_Void)
{
	return angle.beta;
}
uint8_t GetZ_Xita(X_Void)
{
	return angle.gama;
}




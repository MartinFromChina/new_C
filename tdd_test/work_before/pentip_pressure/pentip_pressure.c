#include "pentip_pressure.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"

static X_Boolean isInit = X_False;

X_Void WaitAdc(uint32_t wait_cycle)
{
	wait_cycle = wait_cycle;
}

X_Boolean PentipInit(GetRawPressure p_get_raw)
{
	uint32_t sum;
	uint16_t pressure,i;

	if(p_get_raw == X_Null) {return X_False;}
	sum = 0;
	for(i=0;i<SAMPLE_CNT;i++)
	{
		pressure = p_get_raw();
		WaitAdc(100);
		sum += pressure;
		
		insert_LogDebug(1,("pressure is %d ;sum  is %d\r\n",pressure,sum ));
		
	}
	insert_LogDebug(1,("pressure mean is %d \r\n",sum/SAMPLE_CNT));
	
	isInit = X_True;
}

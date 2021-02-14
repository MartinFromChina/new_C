#include "pentip_pressure.h"

#define SAMPLE_CNT   (0xff)
static X_Boolean isInit = X_False;

X_Boolean PentipInit(uint16 (*GetRawPressure)(X_Void))
{
	uint32_t sum;
	uint16_t pressure,i;

	if(GetRawPressure == X_Null) {return X_False;}
	sum = 0;
	for(i=0;i<SAMPLE_CNT;i++)
	{
		pressure = GetRawPressure();
		sum += pressure;
	}
	isInit = True;
}

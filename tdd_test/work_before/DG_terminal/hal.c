#include "DG_common.h"
#include "hal.h"
#include "terminal1.h"
#include "terminal2.h"
#include "../node_interconnect/node_interconnect.h"

typedef struct
{
	X_Void (*main_loop)(X_Void);
}s_main_loop;

static s_main_loop const Mains[] ={
	main_loop_1,
	main_loop_2,
}; 

static X_Void Hal_Main_Loop(X_Void)
{
	uint8_t i;
	for(i=0;i<sizeof(Mains)/sizeof(Mains[0]);i++)
	{
		Mains[i].main_loop();
	}
}

static X_Void Hal_Irq_And_Wireless(X_Void)
{

}

X_Void HAL_BasicSet(X_Void)
{

}
X_Void HAL_Run(X_Void)
{
	Hal_Main_Loop();
	Hal_Irq_And_Wireless();
}

uint32_t GetSysTime(X_Void)
{
	return GetTime();
}
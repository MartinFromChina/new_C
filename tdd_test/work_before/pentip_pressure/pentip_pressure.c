#include "pentip_pressure.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"

static X_Boolean isInit = X_False;
static uint16_t threshold_down,threshold_up;

#ifndef X_Max
#define X_Max(a, b)     (((a) >= (b)) ? (a) : (b))
#endif

#ifndef X_Min
#define X_Min(a, b)     (((a) <= (b)) ? (a) : (b))
#endif


#define PRESSURE_CALIBRATION_COUNT                  SAMPLE_CNT
#define DEVIATION_MINUM                             3.2f            // good one base on actual measurement
#define INIT_OFFSET_IN_DEVIATION                    6.0f            // the difference between stable and just start base on actual measurement
#define S_FACTOR_PRESSURE_SCHMITTTRIGGER_THRESHOLD  3.090232f       // 1/1000
#define S_FACTOR_PRESSURE_OFFSET                    5.041059f       // 1/4320000
#define FOLLOWING_FACTOR_0x800                      0x1B            // period :PRESSURE_CALIBRATION_COUNT   the differrence decreases to 3% of the initial value after 264 periods      temperature sudden change follow time : 660s  base on actual measurement
#define CONST_OFFSET                                30

static X_Int16 pressureSchmittTriggerOffset0 ;
static X_Int16 pressureSchmittTriggerOffset1 ;
static X_Int32 pressureSigma;

static X_UInt16 pressureLast;                                   // 

static X_Int16 pressureSchmittTriggerThreshold0;                // round down
static X_Int16 pressureSchmittTriggerThreshold1;                // round down


X_Void WaitAdc(uint32_t wait_cycle)
{
	wait_cycle = wait_cycle;
}

X_Boolean PentipInit(GetRawPressure p_get_raw)
{
	uint32_t sum,sum_square;
	uint16_t pressure,i;

	if(p_get_raw == X_Null) {return X_False;}
	sum = 0;
	sum_square = 0;
	
	for(i=0;i<SAMPLE_CNT;i++)
	{
		pressure = p_get_raw();
		WaitAdc(100);
		sum += pressure;
		sum_square += pressure * pressure;
		//temp[i] = pressure ;
		////////////insert_LogDebug(1,("pressure is %d ;sum  is %d\r\n",pressure,sum ));
	}
	insert_LogDebug(1,("pressure mean is %d \r\n",sum/SAMPLE_CNT));
	/*
	sum_square = 0;
	mean = sum/SAMPLE_CNT;
	for(i=0;i<SAMPLE_CNT;i++)
	{
		sum_square += (temp[i] - mean) * (temp[i] - mean);
	}
	insert_LogDebug(1,("pressure sigema square  is %d \r\n",sum_square/SAMPLE_CNT));
	*/

    X_Int32 s1, s2;
    X_Int16 v;
    X_Float32 float_thre, vf;

	s1 = sum;
	s2 = sum_square;
	s2 = (s2 * PRESSURE_CALIBRATION_COUNT - s1 * s1);
    float_thre = s2 * ((PRESSURE_CALIBRATION_COUNT * S_FACTOR_PRESSURE_SCHMITTTRIGGER_THRESHOLD * S_FACTOR_PRESSURE_SCHMITTTRIGGER_THRESHOLD) / (PRESSURE_CALIBRATION_COUNT - 1));
    for (v = (X_Int16)(PRESSURE_CALIBRATION_COUNT * DEVIATION_MINUM * S_FACTOR_PRESSURE_SCHMITTTRIGGER_THRESHOLD); v < 0x7FFF; v++)
    {
        vf = (X_Int32)v + 1;
        if (vf * vf > float_thre)
        {
            break;
        }
    }
	insert_LogDebug(1,(" THRE 1  \r\n"));
    pressureSchmittTriggerOffset0 = vf + CONST_OFFSET * PRESSURE_CALIBRATION_COUNT;

	float_thre = s2 * ((PRESSURE_CALIBRATION_COUNT * S_FACTOR_PRESSURE_OFFSET * S_FACTOR_PRESSURE_OFFSET) / (PRESSURE_CALIBRATION_COUNT - 1));
    for (v = X_Max(v, (X_Int16)(PRESSURE_CALIBRATION_COUNT *  DEVIATION_MINUM *  S_FACTOR_PRESSURE_OFFSET)); v < 0x7FFF; v++)
    {
        vf = (X_Int32)v + 1;
        if (vf * vf > float_thre)
        {
            break;
        }
    }
	
	insert_LogDebug(1,(" THRE 2  \r\n"));
    pressureSchmittTriggerOffset1 = vf + CONST_OFFSET * PRESSURE_CALIBRATION_COUNT;
    float_thre = s2 * ((PRESSURE_CALIBRATION_COUNT * INIT_OFFSET_IN_DEVIATION * INIT_OFFSET_IN_DEVIATION) / (PRESSURE_CALIBRATION_COUNT - 1));
    for (v = X_Max(v, (X_Int16)(PRESSURE_CALIBRATION_COUNT * DEVIATION_MINUM * INIT_OFFSET_IN_DEVIATION)); v < 0x7FFF; v++)
    {
        vf = (X_Int32)v + 1;
        if (vf * vf > float_thre)
        {
            break;
        }
    }
    s1 += vf;
	insert_LogDebug(1,(" THRE 3  \r\n"));
    pressureSigma = s1;
		
    pressureSchmittTriggerThreshold0 = (pressureSchmittTriggerOffset0 + s1) / PRESSURE_CALIBRATION_COUNT;
    pressureSchmittTriggerThreshold1 = (pressureSchmittTriggerOffset1 + s1) / PRESSURE_CALIBRATION_COUNT; 
	insert_LogDebug(1,("STrigger0 = %d ; STrigger1 = %d ;\r\n"
	,pressureSchmittTriggerThreshold0,pressureSchmittTriggerThreshold1));
	
	
	isInit = X_True;

	return X_True;
}
/********************************************************************************/

#include <math.h>
static X_Float32 e = 2.71828,root_2pai = 2.506628;

X_Float32 SGN(uint16_t x,uint16_t mean)
{
	if(x>=mean) {return 1.0;}
	return (-1.0);
}

X_Float32 SinglePDF(uint16_t x,uint16_t mean,uint32_t sigma_square)
{
	X_Float32 temp = (x - mean)*(x - mean);
	temp = temp / (2 * sigma_square);
	temp = temp * (-1.0);
	temp = exp(temp);
	temp = temp / root_2pai;
	temp = temp / sqrt(sigma_square);
	return (temp);
}

X_Boolean AnotherPentipInit(GetRawPressure p_get_raw)
{
	uint32_t sum,sum_square,sigma_square,sigma;
	uint16_t pressure,i,temp[SAMPLE_CNT],mean;
	threshold_down = 0;
	threshold_up = 0;

	if(p_get_raw == X_Null) {return X_False;}
	sum = 0;
	sum_square = 0;
	
	for(i=0;i<SAMPLE_CNT;i++)
	{
		pressure = p_get_raw();
		WaitAdc(100);
		sum += pressure;
		sum_square += pressure * pressure;
		temp[i] = pressure ;
		////////////insert_LogDebug(1,("pressure is %d ;sum  is %d\r\n",pressure,sum ));
	}
	insert_LogDebug(1,("pressure mean is %d \r\n",sum/SAMPLE_CNT));
	mean = sum/SAMPLE_CNT;
	sigma_square = (sum_square * SAMPLE_CNT) - (sum * sum);
	sigma_square = sigma_square/(SAMPLE_CNT* SAMPLE_CNT);
	sigma = sqrt(sigma_square);
	insert_LogDebug(1,("pressure sigma_square is %d root %d\r\n",sigma_square,sigma));

	X_Float32 ft,integral = 0;
	uint16_t tri_down = 0xffff,tri_up = 0xffff;
	for(i = 1600;i<1770;i++)
	{
		ft = SinglePDF(i,mean,sigma_square);
		////insert_LogDebug(1,("ft[%d] is %f \r\n",i,ft ));
		integral += ft;
		if(tri_down == 0xffff && integral >= 0.01) {tri_down = i;}
		if(tri_up == 0xffff && integral >= 0.99) {tri_up = i;}
	}
	insert_LogDebug(1,("---------------------tri_down %d tri_up %d ,integral %f\r\n",tri_down,tri_up,integral));
	isInit = X_True;

	return X_True;
	
}


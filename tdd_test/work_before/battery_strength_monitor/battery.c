#include "battery.h"

static uint8_t CurrentBatteryStrength = 0,CurrentBatteryStrength_backup = 0;
static uint16_t adc_value_backup = 0;
static onBatteryAdc adc_value_get = (onBatteryAdc)0;

#if (USE_TDD_MOCK != 0)
#include "./test/test_main.h"

extern Adc_Mock adcm;
X_Void mModule_BatteryDeInit(X_Void)
{
	adc_value_get = (onBatteryAdc)0;
}
#endif

#define ADCVALUE_TABLE_MAX (2001)
#define ADCVALUE_TABLE_MIN (1750)
#define ADCVALUE_TABLE_RELATIVE_MAX (1980)
#define ADCVALUE_TABLE_RELATIVE_MIN (1588)

#define MAX_BATTERY_SPAN_AFTER_RESET   5
#define MAX_BATTERY_SPAN_NORMAL   2
#define BATTERY_ABS_MUL(current,backup)    ((uint8_t)((current >= backup)? current - backup :backup - current ))

static struct
    {
        uint16_t ad_value;
        uint8_t battery_strength;//percentage
		uint16_t adc_Span;
		uint8_t battery_Span;
    }
    const battery_table[] =
	{
        { 1980, 99	,0   ,0 },//4.2V // 1985
        { 1946, 95	,34	,5 },//4.1V
        { 1893, 90	,53	,5 },//4.0V
        { 1846, 78	,47	,12 },//3.9V
        { 1802, 65	,44	,13 },//3.8
        { 1751, 50	,51	,15 },//3.7
        { 1707, 32	,44	,18 },//3.6
        { 1659, 18	,48	,14 },//3.5
        { 1617, 5	,42	,13 },//3.4
		{ 1589, 4	,28	,1 },//3.3
    };
static uint8_t ConvAdcToPercentage(uint16_t adc)//the func need a specific test later
{
	uint8_t value = 1,i;
	uint16_t mul,adc_in_scale;
	adc_in_scale = ((adc - ADCVALUE_TABLE_MIN)*(ADCVALUE_TABLE_RELATIVE_MAX - ADCVALUE_TABLE_RELATIVE_MIN))/(ADCVALUE_TABLE_MAX - ADCVALUE_TABLE_MIN);
	adc_in_scale = adc_in_scale + ADCVALUE_TABLE_RELATIVE_MIN;
		
	for(i=0;i<sizeof(battery_table)/sizeof(battery_table[0]);i++)
	{
		if((adc_in_scale) >= battery_table[i].ad_value)
		{	
			mul = adc_in_scale - battery_table[i].ad_value;
			if(mul == 0 || battery_table[i].adc_Span == 0) {value = battery_table[i].battery_strength;}
			else
			{
				value = battery_table[i].battery_strength;
				value += (battery_table[i].battery_Span * mul) /battery_table[i].adc_Span;
			}
			break;
		}
	}
	return value;
}
static uint8_t BatterySmooth(uint8_t current,uint8_t back_up,uint8_t threshold)
{
	if(current >= back_up )
	{
		if((current - back_up) > threshold ) 
		{
			current = back_up + threshold;
		}
	}
	else
	{
		if((back_up - current ) > threshold ) 
		{
			current = back_up - threshold;
		}
	}
	return current;
}
X_Void mModule_BatteryInit(onBatteryAdc getadc)
{
	CurrentBatteryStrength = 50;
	CurrentBatteryStrength_backup = CurrentBatteryStrength;
 	adc_value_backup = 0;
	adc_value_get = getadc;
}
X_Boolean mModule_BatteryStrengthMonitor(X_Boolean isInCharge)
{
	//uint8_t thres_hold;
	uint16_t adc_value;
	X_Boolean isBatteryStateUpdate = X_False;
	uint32_t time =  MOCKABLE(GetCurrentTime)();
	if(adc_value_get == X_Null) {return isBatteryStateUpdate;}
	if(time <= CONV_MS_TO_TICKS(10000))
	{
		if((time % CONV_MS_TO_TICKS(100)) == 0) 
		{
			adc_value = adc_value_get();
			CurrentBatteryStrength = ConvAdcToPercentage(adc_value);
			//thres_hold = MAX_BATTERY_SPAN_AFTER_RESET;
			CurrentBatteryStrength = BatterySmooth(CurrentBatteryStrength,CurrentBatteryStrength_backup,MAX_BATTERY_SPAN_AFTER_RESET);
		}
	}
	else
	{
		if((time % CONV_MS_TO_TICKS(30000)) == 0)	
		{
			adc_value = adc_value_get();
			CurrentBatteryStrength = ConvAdcToPercentage(adc_value);
			//thres_hold = MAX_BATTERY_SPAN_NORMAL;
			CurrentBatteryStrength = BatterySmooth(CurrentBatteryStrength,CurrentBatteryStrength_backup,MAX_BATTERY_SPAN_NORMAL);	
		}
	}
	if(CurrentBatteryStrength_backup != CurrentBatteryStrength)
	{
		isBatteryStateUpdate = X_True;
		CurrentBatteryStrength_backup = CurrentBatteryStrength;
	}
	
	return isBatteryStateUpdate;
}
uint8_t mModule_GetBatteryStrength(X_Void)
{
	return CurrentBatteryStrength;
}
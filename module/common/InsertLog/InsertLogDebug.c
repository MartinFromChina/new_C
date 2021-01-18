#include "InsertLogDebug.h"

static log_func p_log_func = (log_func)0;

X_Boolean OnceEntryCheck(uint16_t *backup_entry,uint16_t new_value) {
	if(backup_entry == X_Null) {return X_False;}
	if(*backup_entry != new_value)
	{
		*backup_entry = new_value;
		return X_True;
	}
	return X_False;
}

X_Boolean CounterEntryCheck(uint32_t *backup_entry,uint32_t counter_limit){
	if(backup_entry == X_Null) {return X_False;}
	if(counter_limit < 1){return X_False;}

	if(*backup_entry == 0)
	{
		*backup_entry = 1;
		return X_True;
	}

	if(*backup_entry < (counter_limit - 1)) {(*backup_entry) ++ ;}
	else {*backup_entry = 0 ;}
	return X_False;
}
/***************************************/

X_Void insert_OnceEntryValueSet(uint16_t * p_entry,uint16_t value)
{
	if(p_entry != X_Null) {*p_entry = value;}
}
X_Void insert_CounterEntryValueSet(uint32_t * p_entry,uint32_t value)

{
	if(p_entry != X_Null) {*p_entry = value;}
}

X_Void insert_LogDebugInit(X_Boolean  (*Log_method_init)(log_func *pp_func_reg))
{
	if(Log_method_init == X_Null) {return;}
    Log_method_init(&p_log_func);
}

int log_printf(const char * sFormat, ...)
{
	int r = 10;
	va_list ParamList;
	if(p_log_func != X_Null)
	{
		va_start(ParamList, sFormat);
		r = p_log_func(sFormat, ParamList);
		va_end(ParamList);
	}
	return r;
}
/***************************************/
X_Void remove_OnceEntryValueSet(uint16_t * p_entry,uint16_t value){UNUSED_PARAMETER(p_entry);UNUSED_PARAMETER(value);}
X_Void remove_CounterEntryValueSet(uint32_t * p_entry,uint32_t value){UNUSED_PARAMETER(p_entry);UNUSED_PARAMETER(value);}									
X_Void remove_LogDebugInit(X_Boolean  (*Log_method_init)(log_func *pp_func_reg)){UNUSED_PARAMETER(Log_method_init);}



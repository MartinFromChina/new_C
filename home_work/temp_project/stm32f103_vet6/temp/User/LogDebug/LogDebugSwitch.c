#include  "LogDebugSwitch.h"
#include  "SEGGER_RTT.h"

X_Void LogDebugInit(X_Void)
{
	#if (USE_INSERT_DEBUG != 0)
	SEGGER_RTT_Init();
	#endif
}


X_Void SeggerRTT_LogDebug(const char * sFormat, ...)
{
	va_list ParamList;
  va_start(ParamList, sFormat);
  SEGGER_RTT_vprintf(0,sFormat, &ParamList);
  va_end(ParamList);
}

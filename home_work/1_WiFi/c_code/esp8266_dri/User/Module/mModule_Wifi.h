#ifndef __MMODULE_WIFI_H
#define __MMODULE_WIFI_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"

#define MAX_SSID_LENGTH_IN_CHAR         32
#define MAX_PASSWORD_LENGTH_IN_CHAR     32 // check it later
#define MIN_PASSWORD_LENGTH_IN_CHAR     8
		
#define MAX_IP_LENGTH_IN_CHAR           50
#define MAX_PORT_LENGTH_IN_CHAR         20
#pragma pack(1)
/* 系统信息 */
typedef struct
{
	char ssid[MAX_SSID_LENGTH_IN_CHAR];
	char password[MAX_PASSWORD_LENGTH_IN_CHAR];
	X_Boolean isValid;
}sRouterInfo;

typedef struct
{
	char ip[MAX_IP_LENGTH_IN_CHAR];
	char port[MAX_PORT_LENGTH_IN_CHAR];
	X_Boolean isValid;
}sIp_Port;

#pragma pack()

X_Void mModule_WifiInit(X_Void);
X_Void mModule_WifiHandle(X_Void);

X_Void mModule_ETH_DataDirectOutput(X_Void);
X_Void mModule_ETH_Reset(X_Void);
X_Void mModule_ETH_Lock(X_Void);
X_Void mModule_ETH_Unlock(X_Void);
X_Void mModule_ETH_EventRegister(evt_reg reg);
X_Void mModule_ETH_EventClear(X_Void);
X_Void mModule_ETH_RestoreRequest(X_Void);
X_Void mModule_ETH_GetStatusRequest(X_Void);
X_Void mModule_ETH_ReconfigRequest(X_Void);
X_Void mModule_ETH_InputRouterInfoRequest(X_Void);

#ifdef __cplusplus
		}
#endif

#endif

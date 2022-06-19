#include  "mModule.h"
#include  "mModule_button.h"
#include  "mModule_Wifi.h"

//INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

static X_Boolean isInTestMode = X_False;

X_Void mModuleInit(X_Void)
{
	mModule_WifiInit();
	mModule_ButtonInit();
}

X_Boolean mModule_DoesInTestMode(X_Void)
{
	return isInTestMode;
}

static  sRouterInfo router;
static  sIp_Port    ip_port;

sRouterInfo *mApp_SysInfo_GetRouterInfo(X_Void)
{
	return &router;
}
sIp_Port *mApp_SysInfo_GetIp_Port_Info(X_Void)
{
	return &ip_port;
}

X_Void mModule_SetTestMode(X_Void)
{
	isInTestMode = X_True;
	mModule_ETH_Lock();
}
X_Void mModule_ClearTestMode(X_Void(*special_clear)(X_Void))
{
	isInTestMode = X_False;
	mModule_ETH_Unlock();
	//////	mModule_ResetAndUnlockAll();// no need : because the main state machine will reset and lock all the module 
	if(special_clear != X_Null) {special_clear();}
	mFun_TM_ReceiveUnocked();
	
	
}

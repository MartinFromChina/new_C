#ifndef __MDRI_UART_H
#define __MDRI_UART_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"

X_Void mDri_UartInit(X_Void);
		
X_Boolean mDri_WifiGetByteInterface(uint8_t *p_data);
X_Boolean mDri_WifiSentBufInterface(const uint8_t *p_buf,uint16_t length);
X_Boolean mDri_DoesWifiSentBufEmpty(X_Void);
X_Void mDri_WifiReceiveBufClear(X_Void);

		
#ifdef __cplusplus
		}
#endif

#endif

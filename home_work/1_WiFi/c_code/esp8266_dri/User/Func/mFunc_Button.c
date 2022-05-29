#include "mFunc_Button.h"

X_Void mFunc_ButtonInit(X_Void)
{
	
}

//////////uint8_t mFunc_GetButtonsValue(X_Void)
//////////{
//////////	uint8_t temp = 0;
//////////	uint32_t backup;
//////////	
////////////	backup =((~SW_GPIO->IN) & SW_GPIO_KEYS_MASK);

////////////	temp = (backup >> 2);
////////////	temp = (temp | ((backup >> 25)<<2));
////////////	temp = (temp & 0x3F);
//////////	return temp;
//////////}

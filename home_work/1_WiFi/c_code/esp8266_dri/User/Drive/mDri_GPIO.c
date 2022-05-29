#include "mDri_GPIO.h"
#include "gpio.h"
#include "MCU_Board_Speci.h"

X_Void mDri_IO_ClockEnable(X_Void)
{
	 /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

typedef struct _GroupPin
{
	GPIO_TypeDef *port;
	uint16_t     pin;
}sGroupPin;

static const sGroupPin ButtonPin[] = {
	{GPIO_PORT_KEY1,GPIO_PIN_KEY1},
	{GPIO_PORT_KEY2,GPIO_PIN_KEY2},
	{GPIO_PORT_KEY3,GPIO_PIN_KEY3},
	{GPIO_PORT_KEY4,GPIO_PIN_KEY4},
	{GPIO_PORT_KEY5,GPIO_PIN_KEY5},
	{GPIO_PORT_KEY6,GPIO_PIN_KEY6},
};

X_Void mDri_ButtonIO_Init(X_Void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	for(i=0;i<(sizeof(ButtonPin)/sizeof(ButtonPin[0]));i++)
	{
		GPIO_InitStruct.Pin = ButtonPin[i].pin;
		HAL_GPIO_WritePin(ButtonPin[i].port,ButtonPin[i].pin,GPIO_PIN_RESET);
		HAL_GPIO_Init(ButtonPin[i].port, &GPIO_InitStruct);
	}
}

/*************************************
button1 £º1  7
button2 £º2  8
button3 £º3  9
button4 £º4  10
button5 £º5  11
button5 £º6  12
*************************************/
#define SW_GPIO_KEYS_MASK       0x00001F80
uint8_t mDri_GetButtonsValue(X_Void)
{
	uint8_t temp = 0;
	uint32_t backup;
	
	backup =((~GPIOE->IDR) & SW_GPIO_KEYS_MASK);

	temp = (backup >> 7);
////////////	temp = (temp | ((backup >> 25)<<2));
////////////	temp = (temp & 0x3F);
//////////  /* Check the parameters */
//////////  assert_param(IS_GPIO_PIN(GPIO_Pin));
  return temp;
}

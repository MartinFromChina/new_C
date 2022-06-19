#ifndef __MCU_BOARD_SPECI_H
#define __MCU_BOARD_SPECI_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "ProjectConfig.h"
/***************************************************************************/
#define FRAME_MARCO_LED
#define FRAME_MARCO_UART
#define FRAME_MARCO_BUTTON
/*******************************************************************************************
LED PIN
**********************************************************************************************/
	#ifdef FRAME_MARCO_LED
		#define ALL_LED_PIN 					(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC)

		#define GPIO_PORT_COMMON   	 	GPIOD
		#define GPIO_PIN_COMMON	   		GPIO_Pin_6
			
		#define GPIO_PORT_GPS   			GPIOD
		#define GPIO_PIN_GPS	   			GPIO_Pin_1

		#define GPIO_PORT_FPGA   			GPIOD
		#define GPIO_PIN_FPGA	   			GPIO_Pin_7
		
		#define GPIO_PORT_BC95   			GPIOD
		#define GPIO_PIN_BC95	   			GPIO_Pin_0
		
		#define GPIO_PORT_RESERVE2   	GPIOC
		#define GPIO_PIN_RESERVE2	   	GPIO_Pin_7
		
		#define GPIO_PORT_RESERVE3   	GPIOC
		#define GPIO_PIN_RESERVE3	   	GPIO_Pin_6
	#endif
	

/*******************************************************************************************
	UART PIN
	**********************************************************************************************/	
	#ifdef FRAME_MARCO_UART
		#define	UART1_FIFO_EN	1
		#define UART1_BAUD			115200
		
		#define	UART2_FIFO_EN	0
		#define UART2_BAUD			9600
		
		#define	UART3_FIFO_EN	0
		#define UART3_BAUD			9600
		
		#define	UART4_FIFO_EN	0
		#define UART4_BAUD			115200
		
		#define	UART5_FIFO_EN	0
		#define UART5_BAUD			9600
		
		#define ETH_UART_PORT    COM1
		
		#define UART_WIFI_MAX_TX_LENGTH             1024// for eth  
		
		#define UART_COM1_MAX_LENGTH             UART_WIFI_MAX_TX_LENGTH // 

		/* 串口1的GPIO  PA9, PA10 */
		#define USART1_CLK_ENABLE()              RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

		#define USART1_TX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE)
		#define USART1_TX_GPIO_PORT              GPIOA
		#define USART1_TX_PIN                    GPIO_Pin_9

		#define USART1_RX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE)
		#define USART1_RX_GPIO_PORT              GPIOA
		#define USART1_RX_PIN                    GPIO_Pin_10

		/* 串口2的GPIO --- PD5 PD6/PA2 PA3 */
		#define USART2_CLK_ENABLE()              RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)

		#define USART2_TX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE)
		#define USART2_TX_GPIO_PORT              GPIOA
		#define USART2_TX_PIN                    GPIO_Pin_2

		#define USART2_RX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE)
		#define USART2_RX_GPIO_PORT              GPIOA
		#define USART2_RX_PIN                    GPIO_Pin_3

		/* 串口3的GPIO --- PB10 PB11 */
		#define USART3_CLK_ENABLE()              RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE)

		#define USART3_TX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE)
		#define USART3_TX_GPIO_PORT              GPIOB
		#define USART3_TX_PIN                    GPIO_Pin_10

		#define USART3_RX_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE)
		#define USART3_RX_GPIO_PORT              GPIOB
		#define USART3_RX_PIN                    GPIO_Pin_11

		/* 串口4的GPIO --- PC10 PC11 */
		#define UART4_CLK_ENABLE()               RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE)

		#define UART4_TX_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE)
		#define UART4_TX_GPIO_PORT               GPIOC
		#define UART4_TX_PIN                     GPIO_Pin_10

		#define UART4_RX_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE)
		#define UART4_RX_GPIO_PORT               GPIOC
		#define UART4_RX_PIN                     GPIO_Pin_11

		/* 串口5的GPIO --- PC12/UART5_TX PD2/UART5_RX 暂未使用 */
		#define UART5_CLK_ENABLE()               RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE)

		#define UART5_TX_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE)
		#define UART5_TX_GPIO_PORT               GPIOC
		#define UART5_TX_PIN                     GPIO_Pin_12

		#define UART5_RX_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE)
		#define UART5_RX_GPIO_PORT               GPIOD
		#define UART5_RX_PIN                     GPIO_Pin_2
	#endif


/*******************************************************************************************
BUTTON PIN
**********************************************************************************************/
#ifdef FRAME_MARCO_BUTTON
	#define GPIO_PORT_KEY1   			GPIOE
	#define GPIO_PIN_KEY1	   			GPIO_PIN_7
	
	#define GPIO_PORT_KEY2   			GPIOE
	#define GPIO_PIN_KEY2	   			GPIO_PIN_8
	
	#define GPIO_PORT_KEY3   			GPIOE
	#define GPIO_PIN_KEY3	   			GPIO_PIN_9
	
	#define GPIO_PORT_KEY4   			GPIOE
	#define GPIO_PIN_KEY4	   			GPIO_PIN_10
	
	#define GPIO_PORT_KEY5   			GPIOE
	#define GPIO_PIN_KEY5	   			GPIO_PIN_11
	
	#define GPIO_PORT_KEY6   			GPIOE
	#define GPIO_PIN_KEY6	   			GPIO_PIN_12
#endif

#ifdef __cplusplus
		}
#endif
#endif

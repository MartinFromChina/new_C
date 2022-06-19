/*
*********************************************************************************************************
*
*	模块名称 : 消息处理模块
*	文件名称 : bsp_msg.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/

#ifndef __BSP_USER_LIB_H
#define __BSP_USER_LIB_H

#include  "LogDebugSwitch.h"

uint16_t CheckSumCalculate(uint8_t *_buf, uint16_t _len);
int32_t StrToInt(char *_pStr);
int32_t StrToIntFix(char *_pStr, uint8_t _ucLen);
void HexToAscii(uint8_t *_ucpHex, uint8_t *_ucpAscII, uint8_t _ucLenasc);
int32_t StrToHex(char *_pStr, uint8_t _ucLen);
uint16_t TwoBytesReverse(uint16_t src);
uint32_t FourBytesReverse(uint32_t src);
#endif	/* __BSP_USER_LIB_H */

/***************************** XMDNDL (END OF FILE) *********************************/

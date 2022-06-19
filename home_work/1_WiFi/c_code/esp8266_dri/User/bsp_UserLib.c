/*
*********************************************************************************************************
*
*
*********************************************************************************************************
*/

#include "bsp_UserLib.h"

/*
*****************************************************************************
 * 函  数：CheckSum
 * 功  能：计算校验和
 * 参  数：_buf:需要计算的数据buf
 *		   _len：计算的数据长度
 * 返回值：两字节校验和
*****************************************************************************
*/
uint16_t CheckSumCalculate(uint8_t *_buf, uint16_t _len)
{
	uint16_t sum = 0;
	uint16_t i;
	 
	for(i = 0 ; i<_len ; i++)
	{
		sum += _buf[i];
	}
	
	return sum;
}

/*
*********************************************************************************************************
*	函 数 名: StrToInt
*	功能说明: 将ASCII码字符串转换成十进制
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
int32_t StrToInt(char *_pStr)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/*
*********************************************************************************************************
*	函 数 名: StrToIntFix
*	功能说明: 将ASCII码字符串转换成十进制, 给定长度
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*			 _ucLen : 固定长度
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
int32_t StrToIntFix(char *_pStr, uint8_t _ucLen)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
		_ucLen--;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < _ucLen; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/*
*********************************************************************************************************
*	函 数 名: HexToAscii
*	功能说明: 将hex码0x1f转换成'1'和'f'. 结尾填0.
*	形    参: ucpHex 输入缓冲区指针
*		     _ucpAscII 输出缓冲区指针
*		    _ucLenasc ASCII的字符长度.
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
void HexToAscii(uint8_t *_ucpHex, uint8_t *_ucpAscII, uint8_t _ucLenasc)
{
	uint8_t i;
	uint8_t ucTemp;

	for (i = 0; i < _ucLenasc; i++)
	{
		ucTemp = *_ucpHex;
		if ((i&0x01) == 0x00)
			ucTemp = ucTemp >> 4;
		else
		{
			ucTemp = ucTemp & 0x0f;
			_ucpHex++;
		}
		if (ucTemp < 0x0a)
			ucTemp += 0x30;
		else
			ucTemp += 0x37;
		_ucpAscII[i] = ucTemp;
	}
	//--------debug--------//
	_ucpAscII[i] = '\0';
	//--------end----------//
}

/*
*********************************************************************************************************
*	函 数 名: StrToIntFix
*	功能说明: 将ASCII码字符串转换成十6进制, 给定长度
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*			 _ucLen : 固定长度
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
int32_t StrToHex(char *_pStr, uint8_t _ucLen)
{

	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;

	ulInt = 0;
	for (i = 0; i < _ucLen; i++)
	{
		ucTemp = *p;

		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ( ulInt << 4) + (ucTemp - '0');
			p++;
		}
		else if ((ucTemp >= 'A') && (ucTemp <= 'F'))
		{
			ulInt = ( ulInt <<4) + (ucTemp - 55);
			p++;
		}
		else if ((ucTemp >= 'a') && (ucTemp <= 'f'))
		{
			ulInt = (ulInt <<4) + (ucTemp - 87);
			p++;
		}
		else
		{
			break;
		}
	}

	return ulInt;
}


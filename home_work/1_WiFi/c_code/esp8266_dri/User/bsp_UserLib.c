/*
*********************************************************************************************************
*
*
*********************************************************************************************************
*/

#include "bsp_UserLib.h"

/*
*****************************************************************************
 * ��  ����CheckSum
 * ��  �ܣ�����У���
 * ��  ����_buf:��Ҫ���������buf
 *		   _len����������ݳ���
 * ����ֵ�����ֽ�У���
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
*	�� �� ��: StrToInt
*	����˵��: ��ASCII���ַ���ת����ʮ����
*	��    ��: _pStr :��ת����ASCII�봮. �����Զ��Ż�0����
*	�� �� ֵ: ����������ֵ
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
		flag = 1;	/* ���� */
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
		if (ucTemp == '.')	/* ����С���㣬�Զ�����1���ֽ� */
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
*	�� �� ��: StrToIntFix
*	����˵��: ��ASCII���ַ���ת����ʮ����, ��������
*	��    ��: _pStr :��ת����ASCII�봮. �����Զ��Ż�0����
*			 _ucLen : �̶�����
*	�� �� ֵ: ����������ֵ
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
		flag = 1;	/* ���� */
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
		if (ucTemp == '.')	/* ����С���㣬�Զ�����1���ֽ� */
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
*	�� �� ��: HexToAscii
*	����˵��: ��hex��0x1fת����'1'��'f'. ��β��0.
*	��    ��: ucpHex ���뻺����ָ��
*		     _ucpAscII ���������ָ��
*		    _ucLenasc ASCII���ַ�����.
*	�� �� ֵ: ����������ֵ
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
*	�� �� ��: StrToIntFix
*	����˵��: ��ASCII���ַ���ת����ʮ6����, ��������
*	��    ��: _pStr :��ת����ASCII�봮. �����Զ��Ż�0����
*			 _ucLen : �̶�����
*	�� �� ֵ: ����������ֵ
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


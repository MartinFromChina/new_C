#include "char_to_num.h"

#define MaxCharToHexTableSize   55

static const uint8_t CharToHexTable[MaxCharToHexTableSize]={
	   0,	   1,	   2,	   3,      4,      5,      6,      7,      8,      9,// 48~57
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	 0xA,	 0xB,	 0xC,//58~67
	 0xD,	 0xE, 	 0xF,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//68~77
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//78~87
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff, 	 0xa,//88~97
	 0xb,	 0xc,	 0xd, 	 0xe, 	 0xf//98~102
};

uint16_t GetStringLength(char *p_strings)
{
	uint16_t i;
	for(i=0;i<65534;i++)
	{
		if(p_strings[i]==' ' || p_strings[i]== '\n' || p_strings[i]== '\0' || p_strings[i] == 0x0a || p_strings[i] == 0x0d )
		{
			break;
		}
	}
	return i;
}
uint8_t HexCharTo_8bit(char hexchar_H,char hexchar_L,X_Boolean *isOK)
{
	uint8_t result;
	if(hexchar_H < 48 || hexchar_H >102 ||hexchar_L < 48 || hexchar_L >102)
	{
		if(isOK != X_Null){*isOK = X_False;}
		return 0;
	}
	hexchar_H = hexchar_H - 48;
	hexchar_L = hexchar_L - 48;
	if(CharToHexTable[(uint8_t)hexchar_H] == 0xff || CharToHexTable[(uint8_t)hexchar_L] == 0xff)
	{
		if(isOK != X_Null){*isOK = X_False;}
		return 0;
	}
	result = CharToHexTable[(uint8_t)hexchar_L] + (CharToHexTable[(uint8_t)hexchar_H]<<4);
	if(isOK != X_Null){*isOK = X_True;}
	return result;
}
//////////////#include <stdio.h>
uint32_t DecCharTo_32bit(char *p_string,uint8_t length,X_Boolean *isOK)
{
	uint8_t i;
	uint32_t sum = 0,power = 1;
	uint8_t temp;
	
	if(p_string == X_Null ) {return 0;}
	
	for(i=0;i<length;i++)
	{
		temp = p_string[length - i - 1];
		if(temp < 48 || temp >102 )
		{
			if(isOK != X_Null){*isOK = X_False;}
			return 0;
		}
		temp = temp - 48;
		temp = CharToHexTable[temp];
		if(temp > 9 ) 
		{
			if(isOK != X_Null){*isOK = X_False;}
			return 0;
		}
		sum += temp*power;
		power = power * 10;
	}
	return sum;
}


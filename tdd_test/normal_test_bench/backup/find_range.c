static char response_string[] = "HTTP/1.1 206 Partial Content\r\nServer: nginx\r\nDate: Tue, 12 Jul 2022 20:12:43 GMT\r\nContent-Type: application/octet-stream\r\nContent-Length: 1025\r\nLast-Modified: Wed, 29 Jun 2022 05:42:19 GMT\r\nConnection: keep-alive\r\nETag: \"62bbe63b-ab00\"\r\nContent-Range: bytes 0-1024/43776\r\n\r\n";
static char response_string2[] = "HTTP/1.1 206 Partial Content\r\nServer: nginx\r\nDate: Tue, 12 Jul 2022 20:12:43 GMT\r\nContent-Type: application/octet-stream\r\nContent-Length: 625\r\nLast-Modified: Wed, 29 Jun 2022 05:42:19 GMT\r\nConnection: keep-alive\r\nETag: \"62bbe63b-ab00\"\r\nContent-Range: bytes */6553\r\n\r\n";

typedef struct
{
	uint16_t content_length;
	uint32_t total_length;
}sHeaderInfo;

extern X_Boolean FindRange(uint8_t *p_data,uint16_t cur_length,sHeaderInfo *p_header);

TEST(http,find_range)
{
  X_Boolean isOK;
  sHeaderInfo h_info;
  printf("the string is :\r\n %s \r\n",response_string);

  isOK = FindRange((uint8_t *)response_string,(sizeof(response_string)/sizeof(uint8_t)) - 1,&h_info);
  EXPECT_EQ(isOK,X_True);

  if(isOK == X_True)
  {
    EXPECT_EQ(h_info.content_length,1025);
    EXPECT_EQ(h_info.total_length,43776);
  }

  isOK = FindRange((uint8_t *)response_string2,(sizeof(response_string2)/sizeof(uint8_t)) - 1,&h_info);
  EXPECT_EQ(isOK,X_True);

  if(isOK == X_True)
  {
    EXPECT_EQ(h_info.content_length,625);
    EXPECT_EQ(h_info.total_length,6553);
  }

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
static int find_str(char* str1_src,char* str2_to_find)
{
    int i,j,flag=-1;
    for(i=0,j=0;str1_src[i]!=NULL;i++)
    {
        while(str1_src[i]==str2_to_find[j] && str1_src[i] && str2_to_find[j])
        {
            i++;
            j++;
            if(str2_to_find[j]==NULL)
            {
                flag=i-j;
                return flag;
            }
        }
        j=0;
    }
    return flag;
}

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

typedef struct
{
	uint16_t content_length;
	uint32_t total_length;
}sHeaderInfo;

X_Boolean FindRange(uint8_t *p_data,uint16_t cur_length,sHeaderInfo *p_header)
{
	uint8_t i;
//////////////	char string[13] = "Range: bytes ";
	if(p_data == X_Null || p_header == X_Null || cur_length < 27 ) {return X_False;}

	//INSERT(LogDebug)(1,(" last : %2x ; second last : %2x ; %2x\r\n",p_data[cur_length - 1],p_data[cur_length - 2],p_data[cur_length - 3]));

	if(p_data[cur_length - 1] == '\n'&& p_data[cur_length - 2] == '\r')
	{
		for(i=3;i<27;i++)
		{
			if(p_data[cur_length - i] == '/') {break;}
		}

		if(i >= 27) {return X_False;}
		
		p_header ->total_length = StrToIntFix((char *)&p_data[cur_length - i+1],i-3);

		int position;
		position = find_str((char*)p_data,"Content-Length: ");
		if(position == -1) {return X_False;}
	
		////////////		strchr(p_data[position], '\r');
		position += 16;
		for(i=0;i<32;i++)
		{
			if(p_data[position+i] == '\r') {break;}
		}
		if(i >= 32) {return X_False;}

		
		p_header ->content_length = StrToIntFix((char *)&p_data[position],i+1);
		
		INSERT(LogDebug)(1,("content_length : %d ; total_length : %d \r\n",p_header ->content_length,p_header ->total_length));
		
		return X_True;
	}
	return X_False;
//////	if(memcmp(p_data[cur_length - 13],string,13) == 0)
}
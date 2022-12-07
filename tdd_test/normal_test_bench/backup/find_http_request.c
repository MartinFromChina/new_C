#include <string.h>
#define NORMAL_MAX_ETH_FRAME_SEND_LENGTH  180
#define DOWNLOAD_BLOCK_SIZE_IN_BYTES      1024
#define UNKNOW_SIZE                             (0xffffffff)

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
X_Boolean generate_range(char *p_src,uint8_t length,uint32_t total_size,uint32_t downloaded_size)
{
	uint32_t start,end;
	if(p_src == X_Null || length < 25 || length == 0) {return X_False;}
	
	byteBufInit((uint8_t *)p_src,length,'\0');
	
	start = downloaded_size;
	end = start + DOWNLOAD_BLOCK_SIZE_IN_BYTES-1;
	if(total_size != UNKNOW_SIZE && total_size > 0)
	{
		if(start >= total_size) {return X_False;}
		if(end >= total_size)
		{
			end = total_size - 1;
		}
	}
	
	sprintf(p_src,"%d",start); // 
	sprintf(&p_src[strlen(p_src)],"%s","-"); 
	sprintf(&p_src[strlen(p_src)],"%d",end); 
	sprintf(&p_src[strlen(p_src)],"%s","\r\n\r\n"); 
	
	return X_True;
}

X_Boolean ReloadGetRequest(char *string,uint32_t downloaded_size,uint32_t total_size)
{
	int position;
	char temp_string[27];
	
	if(string == X_Null) {return X_False;}
	position = find_str(string,"bytes=");
    
	if(position == -1) {return X_False;}

	if((position + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
	{		
		if(generate_range(temp_string,27,total_size,downloaded_size) == X_True)
		{
            byteBufInit((uint8_t *)&string[position+6],35,'\0');
			sprintf(&string[position+6],"%s",temp_string);
            return X_True;
		}
	}
  return X_False;

}














#include <string.h>

#define NORMAL_MAX_ETH_FRAME_SEND_LENGTH  180
#define DOWNLOAD_BLOCK_SIZE_IN_BYTES      1024
#define UNKNOW_SIZE                             (0xffffffff)

extern X_Boolean generate_range(char *p_src,uint8_t length,uint32_t total_size,uint32_t downloaded_size);
extern X_Boolean ReloadGetRequest(char *string,uint32_t downloaded_size,uint32_t total_size);

typedef struct
{	
	uint16_t                cur_dl_index;
	uint32_t                downloaded_size;
	uint32_t                total_dl_size;
	X_Boolean               isRealData;
}sParamExtern;


static sParamExtern sPE;

static char get_resuest[NORMAL_MAX_ETH_FRAME_SEND_LENGTH];

TEST(generate,get_request)
{
  X_Boolean isOK;
  byteBufInit((uint8_t*)get_resuest,NORMAL_MAX_ETH_FRAME_SEND_LENGTH,'\0');
  sprintf(get_resuest,"%s","GET ");

	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","data/fengming.bin");}
//////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s"," HTTP/1.1\r\n");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","Host: ");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","192.168.0.103");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","\r\n");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","User-Agent: dl\r\n");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{sprintf(&get_resuest[strlen(get_resuest)],"%s","Accept: */*\r\n");}
////////	mDri_WdgFeed();
	if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{
				sprintf(&get_resuest[strlen(get_resuest)],"%s","Connection: keep-alive\r\n");
//////////				isGetRequestOK = X_True;
			}
			
		char temp_string[27];
		if((strlen(get_resuest) + 40) <= NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
			{
				sprintf(&get_resuest[strlen(get_resuest)],"%s","Range: bytes=");
				
				sPE.downloaded_size   = 0;
				sPE.cur_dl_index      = 0; 
				sPE.total_dl_size     = UNKNOW_SIZE;
				sPE.isRealData        = X_False;
				
				if(generate_range(temp_string,27,sPE.total_dl_size,sPE.downloaded_size) == X_True)
				{
					sprintf(&get_resuest[strlen(get_resuest)],"%s",temp_string);
				}
			}

		INSERT(LogDebug)(1,(" get request: %s\r\n",get_resuest));

    isOK = ReloadGetRequest(get_resuest,2345,5000);
    EXPECT_EQ(isOK,X_True);

    INSERT(LogDebug)(1,(" get request: %s\r\n",get_resuest));

    isOK = ReloadGetRequest(get_resuest,2345,3000);
    EXPECT_EQ(isOK,X_True);

    INSERT(LogDebug)(1,(" get request: %s\r\n",get_resuest));
}
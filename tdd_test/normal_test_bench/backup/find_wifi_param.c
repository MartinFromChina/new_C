#define MAX_SSID_LENGTH_IN_CHAR         32
#define MAX_PASSWORD_LENGTH_IN_CHAR     32 // check it later
#define MIN_PASSWORD_LENGTH_IN_CHAR     8
		
#define MAX_IP_LENGTH_IN_CHAR           50
#define MAX_PORT_LENGTH_IN_CHAR         20

#pragma pack(1)
/* 系统信息 */
typedef struct
{
	char ssid[MAX_SSID_LENGTH_IN_CHAR];
	char password[MAX_PASSWORD_LENGTH_IN_CHAR];
	X_Boolean isValid;
}sRouterInfo;

typedef struct
{
	char ip[MAX_IP_LENGTH_IN_CHAR];
	char port[MAX_PORT_LENGTH_IN_CHAR];
	X_Boolean isValid;
}sIp_Port;

#pragma pack()

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

static uint16_t GetLengthByLine(char* p_string) 
{
    uint16_t i;
    for(i=0;i<50;i++)
    {
        if(p_string[i] == '\r' || p_string[i] == '\n' || p_string[i] == '\0')
        {
            break;
        }
    }
    return i;

}

X_Void UpdataTheWifiParam(uint8_t *p_param)
{
    uint16_t i,length;
    sRouterInfo router_info;
    sIp_Port    ip_port_info;

    byteBufInit((uint8_t*)router_info.password,MAX_PASSWORD_LENGTH_IN_CHAR,'\0');
    byteBufInit((uint8_t*)router_info.ssid,MAX_SSID_LENGTH_IN_CHAR,'\0');
    byteBufInit((uint8_t*)ip_port_info.ip,MAX_IP_LENGTH_IN_CHAR,'\0');
    byteBufInit((uint8_t*)ip_port_info.port,MAX_PORT_LENGTH_IN_CHAR,'\0');

    INSERT(LogDebug)(1,(" read context : \r\n%s \r\n",(char*)p_param));

    i=find_str((char *)p_param,"PASSWORD");
   // INSERT(LogDebug)(1,(" PASSWORD %d length %d\r\n",i,GetLengthByLine((char*)&p_param[i+9])));
    CopyBuffer(&p_param[i+9],router_info.password,GetLengthByLine((char*)&p_param[i+9]));

    i=find_str((char *)p_param,"SSID");
   // INSERT(LogDebug)(1,(" SSID %d length %d\r\n",i,GetLengthByLine((char*)&p_param[i+5])));
   CopyBuffer(&p_param[i+5],router_info.ssid,GetLengthByLine((char*)&p_param[i+5]));

    i=find_str((char *)p_param,"IP");
   // INSERT(LogDebug)(1,(" IP %d length %d\r\n",i,GetLengthByLine((char*)&p_param[i+3])));
   CopyBuffer(&p_param[i+3],ip_port_info.ip,GetLengthByLine((char*)&p_param[i+3]));

    i=find_str((char *)p_param,"PORT");
   // INSERT(LogDebug)(1,(" PORT %d length %d\r\n",i,GetLengthByLine((char*)&p_param[i+5])));
   CopyBuffer(&p_param[i+5],ip_port_info.port,GetLengthByLine((char*)&p_param[i+5]));


    INSERT(LogDebug)(1,(" final PASSWORD is %s\r\n",router_info.password));
    INSERT(LogDebug)(1,(" final SSID is %s\r\n",router_info.ssid));
    INSERT(LogDebug)(1,(" final IP is %s\r\n",ip_port_info.ip));
    INSERT(LogDebug)(1,(" final PORT is %s\r\n",ip_port_info.port));


}


extern X_Void UpdataTheWifiParam(uint8_t *p_param);
TEST(IP_PORT_SSID_PASSWORD,updata)
{
	uint8_t temp_buf[200];
	byteBufInit(temp_buf,200,'\0');
	//int err_code;
	FILE* pFile;
	pFile = fopen("../test/src/config.txt","r");

	if(pFile == NULL) 
	{
		INSERT(LogDebug)(1,("open file failed \r\n"));
		return;
	}

    fgets((char*)temp_buf,200,pFile);	
	fgets((char*)&temp_buf[strlen((char*)temp_buf)],200,pFile);	
	fgets((char*)&temp_buf[strlen((char*)temp_buf)],200,pFile);	
	fgets((char*)&temp_buf[strlen((char*)temp_buf)],200,pFile);	

	UpdataTheWifiParam(temp_buf);


}
extern X_Boolean find_bin_file_name(char *p_string,uint16_t length,uint8_t *p_dest);

TEST(find,bin_file)
{
  X_Boolean isOK;
  uint8_t name[30];
  int result;

  byteBufInit(name,30,'\0');

  isOK = find_bin_file_name("http://d.120.mai022.com/data/fengming.bin",strlen("http://d.120.mai022.com/data/fengming.bin"),name);
  EXPECT_EQ(isOK,X_True);

  result = memcmp("fengming",name,8);
  EXPECT_EQ(result,0);
}


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


#define MAX_BIN_FILE_NAME_LENGTH             30
X_Boolean find_bin_file_name(char *p_string,uint16_t length,uint8_t *p_dest)
{
    int position;
    uint16_t i,name_length;

    if(p_dest == X_Null || p_string == X_Null) {return X_False;}

    position = find_str(p_string,".bin");

    if(position <= 0 || position >= length) {return X_False;}

    INSERT(LogDebug)(1,(" position is %d \r\n",position));

    for(i = position;i>0;i--)
    {
        if(p_string[i] == '/') {break;}
    }

    if(i == 0 || i >= position) {return X_False;}

    for(name_length = 0 ; name_length < MAX_BIN_FILE_NAME_LENGTH; name_length ++)
    {
        if(name_length >= (position - i - 1)) {break;}
        p_dest[name_length] = p_string[i+name_length+1];
    }

    INSERT(LogDebug)(1,(" the name is %s \r\n",(char*)p_dest));
    return X_True;
}

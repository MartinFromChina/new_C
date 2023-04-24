#include <stdio.h>
#include "../../../common/AppCommon.h"
extern X_Boolean MT_GeneratorTaskInstance(char src_filename[],char dest_filename[]);
static uint8_t cur_src_file_name[256],cur_dest_file_name[256];

int main(void)
{
	byteBufInit(cur_dest_file_name,256,'\0');
	byteBufInit(cur_src_file_name,256,'\0');

    printf("\r\n hello world \r\n");

    //printf("Please enter the file you want to compress (for example :output_file.rbf , DG_Recv_App.bin): ");
	scanf("%[^\n]s", cur_src_file_name);
	
	//scanf依据一个%d转换说明读取一个整数时，每次读取一个字符，跳过所有空白字符，直到读取到第一个空白字符才开始读取，因为要读取整数，所以scanf希望发现一个数字字符或者一个符号（+或-），如果找到一个数字或符号，它便保存该字符，并读取下一个字符，直到遇到非数字字符便认为读到了整数的结尾，然后scanf将非数字字符放回输入流，这意味着在下次读取输入时，首先读到的是上次读取丢弃的非数字字符。
	char  ch;
	while ((ch= getchar ())!= '\n' )
	{
		continue ;
	} 

    printf("\r\n the input src files is %s \r\n",cur_src_file_name);

    MT_GeneratorTaskInstance((char *)cur_src_file_name,"./dest_files/result.c");
}

// "./src_files/test_template.c"
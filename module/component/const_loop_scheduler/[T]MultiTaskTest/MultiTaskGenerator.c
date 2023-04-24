#include <stdio.h>
#include <stdlib.h>

#include "../../common/AppCommon.h"

static uint32_t MT_read_files(char filename[], uint8_t dest[]){
	FILE * fp = NULL;
	uint32_t index = 0;
	fp = fopen(filename, "r");
	if(fp == NULL)
	{
		//INSERT(LogDebug)(HUFFMAN_DEBUG,("!!! hc_read_files Error: The file is not existed.\n"));
		return 0;
	}


	/*
	因为文本文件中存储的是 ASCII 码，而 ASCII 码中 FF 代表空值（blank），
	所以如果读文件返回了 0xFF，也就说明已经到了文本文件的结尾处。
	也就是说，在语句“while((c=fgetc(fp))!=EOF)”中，当读取的字符为 0xFF 时，
	子语句“c=fgetc(fp)”中的“fgetc(fp)”的值由 0x000000FF 转换为 char 类型（即 c 等于 0xFF）；
	而在执行子语句“c！=EOF”时，字符与整数比较，c 被转换为 0xFFFFFFFF，条件成立，遇到空格字符时就退出。
	由此可见，如果是二进制文件，其中可能会包含许多 0xFF，
	因此不能把读到 EOF 作为文件结束的条件，而此时只能使用 feof() 函数。
	char ch = fgetc(fp);
	while (ch != EOF) {
			str[index++] = ch;
			ch = fgetc(fp);
	}   
	*/
	char ch = fgetc(fp);
	while (feof(fp) == 0) {
			dest[index++] = ch;
			ch = fgetc(fp);
	}

	//INSERT(LogDebug)(HUFFMAN_DEBUG,("----- end file index is %d ,file is : %s\n",index,filename));

	dest[index] = '\0';
	fclose(fp);
	return index;
}

#include <string.h>
#define FILE_STR_SIZE 800000
X_Boolean MT_GeneratorTaskInstance(char src_filename[],char dest_filename[])
{
	uint32_t length;
	FILE * fp = NULL;
	char src_file_data[FILE_STR_SIZE];
	
	byteBufInit((uint8_t *)src_file_data,FILE_STR_SIZE,'\0');
		
	length = MT_read_files(src_filename,(uint8_t *)src_file_data);
    if(length == 0 || length > FILE_STR_SIZE)
    {
        //isErrorOccur = X_True;
        return X_False;
    }
	
	
	fp = fopen(dest_filename, "w");
	if(fp == NULL)
	{
		//INSERT(LogDebug)(HUFFMAN_DEBUG,("Error: Failed to write the data into the file %s\n",dest_filename));
		return X_False;
	}
	fwrite(src_file_data,sizeof(unsigned char),length,fp);
	fclose(fp);

	return X_True;
}

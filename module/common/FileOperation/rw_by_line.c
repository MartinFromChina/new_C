#include "rw_by_line.h"
#include "stdio.h"

X_Boolean WriteFileByLine(const char* p_filename,const char *p_string,...)
{
	return X_True;
}
X_Boolean ReadFileByLine(const char* p_filename,uint16_t line_num,char *p_context)
{
	uint16_t i;
	char temp_buf[MAX_LENGTH_OF_LINE+1];
	FILE* pFile;
	if(line_num == 0xFFFF || p_context == X_Null ) {return X_False;}
	pFile = fopen(p_filename,"r");
	if(pFile == X_Null) {return X_False;}

	for(i=0;i<line_num;i++)
	{
		if(feof(pFile)==0) {fgets(temp_buf,MAX_LENGTH_OF_LINE,pFile);}
	}
	if(feof(pFile)==0) {fgets(p_context,MAX_LENGTH_OF_LINE,pFile);return X_True;}
	return X_False;
}


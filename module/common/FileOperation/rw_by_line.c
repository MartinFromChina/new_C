#include "rw_by_line.h"
#include <stdio.h>
#include <stdarg.h>

#include <string.h>

#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))

static char buf[MAX_LENGTH_OF_FILE_NAME],buf_temp[MAX_LENGTH_OF_FILE_NAME];

static X_Boolean LoadBufTemp(const char* p_filename)
{
	uint16_t i,j;
	for(i=0;i<MAX_LENGTH_OF_FILE_NAME;i++)
	{
		buf_temp[i] = p_filename[i];
	}
	printf(" buf_temp %s\r\n",buf_temp);

	j = 0;
	for(i=0;i<MAX_LENGTH_OF_FILE_NAME;i++)
	{
		if(buf_temp[i] == '/') {j = i;}
	}

	if(j == 0)
	{
		buf_temp[0] = 't';
		buf_temp[1] = '.';
		buf_temp[2] = 't';
		buf_temp[3] = 'x';
		buf_temp[4] = 't';
		buf_temp[5] = '\0';
		return X_True;
	}
	else if((j + 6) <= (MAX_LENGTH_OF_FILE_NAME - 1)) 
	{
		buf_temp[j+1] = 't';
		buf_temp[j+2] = '.';
		buf_temp[j+3] = 't';
		buf_temp[j+4] = 'x';
		buf_temp[j+5] = 't';
		buf_temp[j+6] = '\0';
		return X_True;
	}
	return X_False;
}
static X_Boolean InserOneLineToTempFile(FILE* pSrc,FILE* pDes,uint16_t line_num,const char *p_string)
{
	return X_True;
}
static uint16_t GetFileLineNum(FILE* pFile)
{
	uint16_t i, total_num;
	char temp_buf[MAX_LENGTH_OF_ONE_LINE+1];
	for(i=0,total_num = 0;i<TOTAL_LINE_NUM_MAX;i++)
	{
		if(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) == NULL) 
		{
			 break;
		}
		total_num++;
	}
	return total_num;
}



char* ConvFileStrToChar(const char *src)
{
	uint16_t i;
	for(i=0;i<MAX_LENGTH_OF_FILE_NAME;i++)
	{
		buf[i] = ' ';
	}
	strcpy(buf, src);
	return buf;
}
X_Boolean FileClear(const char* p_filename)
{
	FILE* pFile;
	pFile = fopen(p_filename,"w+");
	if(pFile == X_Null) {return X_False;}
	fclose(pFile);
	return X_True;
}

X_Boolean WriteFileByLine(const char* p_filename,uint16_t line_num,const char *p_string,...)
{
	uint16_t i,total_line;
	X_Boolean isOK = X_False;
	char logstr[MAX_LENGTH_OF_ONE_LINE+1];
	char temp_buf[MAX_LENGTH_OF_ONE_LINE+100];
	
	FILE* pFile,*p_tempF;
	va_list argp;
	if(line_num == 0xFFFF || p_string == X_Null || 0==p_string[0]) {return X_False;}
	if(LoadBufTemp(p_filename) == X_False)  {return X_False;}
	pFile = fopen(p_filename,"r+");
	p_tempF = fopen(buf_temp,"w");
	if(pFile == X_Null || p_tempF == X_Null) {return X_False;}

	//total_line = GetFileLineNum(pFile);

	uint16_t total_num;
	//char temp_buf[MAX_LENGTH_OF_ONE_LINE+1];
	for(i=0,total_num = 0;i<TOTAL_LINE_NUM_MAX;i++)
	{
		if(feof(pFile)!=0) {break;}
		if(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) == NULL) 
		{
			 break;
		}
		else
		{
			printf("...get sth in line %d %s\r\n",i,temp_buf);
		}
		total_num++;
	}
	total_line = total_num;
	printf("...total_line %d\r\n",total_line);
/*
	if(total_line >= line_num)
	{
		//InserOneLineToTempFile();
	}
	else
	{

	}
	for(i=0;i<line_num;i++)
	{
		if(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) == NULL) 
		{
			fputs("\n",pFile);
			fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile);
			printf(" --------------------empty line %d ,put sth in the line and copy to temp t.txt \r\n",i);
			 
		}
		else
		{
			printf("-------------------- get sth in the line %d %s \r\n",i,temp_buf);
		}
		fputs(temp_buf,p_tempF);
	}
	// lock irq
	va_start(argp,p_string);
	if (-1== vsnprintf(logstr,ARRSIZE(logstr),p_string,argp)) logstr[ARRSIZE(logstr)-1]='\0';

	fprintf(p_tempF,"%s\n",logstr);
	printf(" -------------------------------------------------put %s in the certain line\r\n",logstr); 
	va_end(argp);
	// unlock irq
	fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile);
	
	while(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) != NULL)
	{
		fputs(temp_buf,p_tempF);fputs("\n",pFile);
	}
	*/
    fclose(pFile);
	fclose(p_tempF);


	printf(" ----!!!!!!end\r\n");
	printf(" ----!!!!!!end\r\n");
	printf(" ----!!!!!!end\r\n");
	printf(" ----!!!!!!end\r\n");
	
	return X_True;
}
X_Boolean ReadFileByLine(const char* p_filename,uint16_t line_num,char *p_context)
{
	uint16_t i;
	X_Boolean isOK = X_False;
	char temp_buf[MAX_LENGTH_OF_ONE_LINE+1];
	FILE* pFile;
	if(line_num == 0xFFFF || p_context == X_Null ) {return X_False;}
	pFile = fopen(p_filename,"r");
	if(pFile == X_Null) {return X_False;}

	for(i=0;i<line_num;i++)
	{
		if(feof(pFile)==0) {fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile);}
	}
	if(feof(pFile)==0) {fgets(p_context,MAX_LENGTH_OF_ONE_LINE,pFile);isOK = X_True;}
	
	fclose(pFile);
	return isOK;
}



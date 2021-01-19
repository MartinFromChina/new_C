#include "rw_by_line.h"
#include <stdio.h>
#include <stdarg.h>

#include <string.h>

#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))

static char buf_temp[MAX_LENGTH_OF_FILE_NAME];

static X_Boolean LoadBufTemp(const char* p_filename)
{
	uint16_t i,j;
	for(i=0;i<MAX_LENGTH_OF_FILE_NAME;i++)
	{
		buf_temp[i] = p_filename[i];
	}
	//printf(" buf_temp %s\r\n",buf_temp);

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
		if(feof(pFile) != 0) {break;}
		if(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) == NULL) 
		{
			 break;
		}
		total_num++;
	}
	fseek(pFile,0,SEEK_SET);
	return total_num;
}



char* ConvFileStrToChar(const char *src,char *p_buf)
{
	uint16_t i;
	if(p_buf == X_Null) {return (char *)0;}
	for(i=0;i<MAX_LENGTH_OF_FILE_NAME;i++)
	{
		p_buf[i] = ' ';
	}
	strcpy(p_buf, src);
	return p_buf;
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
	
	FILE* pFile,*p_tempF;
	va_list argp;
	if(line_num == 0xFFFF || p_string == X_Null || 0==p_string[0]) {return X_False;}
	if(LoadBufTemp(p_filename) == X_False)  {return X_False;}
	pFile = fopen(p_filename,"r+");
	p_tempF = fopen(buf_temp,"w");
	if(pFile == X_Null || p_tempF == X_Null) {return X_False;}

	total_line = GetFileLineNum(pFile);
	//printf(" --- %d\r\n",total_line);

	if(total_line >= (line_num + 1))
	{
		//InserOneLineToTempFile();
	}
	else
	{
		for(i = 0;i<(total_line);i++)// copy file to temp file
		{
			if(fgets(logstr,MAX_LENGTH_OF_ONE_LINE,pFile)!=NULL){fputs(logstr,p_tempF);}
			else{fputs("\n",p_tempF);}
			//printf(" copy %s to line %d\r\n",temp_buf,i);
		}

		for(i = 0;i<(line_num - total_line);i++)
		{
			fputs("\n",p_tempF);
			//printf(" insert next line to line %d \r\n",i);
		}

		// lock irq
		va_start(argp,p_string);
		if (-1== vsnprintf(logstr,ARRSIZE(logstr),p_string,argp)) logstr[ARRSIZE(logstr)-1]='\0';

		fprintf(p_tempF,"%s\n",logstr);
		va_end(argp);
		// unlock irq
	}
	fclose(pFile);
	remove(p_filename);
	rename(buf_temp,p_filename);
	fclose(p_tempF);
	
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
X_Boolean CompareTwoFileByLine(const char* p_filename1,const char* p_filename2,uint16_t start_line,uint16_t end_line)
{	
	FILE *pFile1,*pFile2;	
	uint16_t i,j;
	X_Boolean isSame = X_True;
	char buf1[MAX_LENGTH_OF_ONE_LINE],buf2[MAX_LENGTH_OF_ONE_LINE];

	if(start_line > end_line || end_line == 0xffff) {return X_False;}
	if(p_filename1 == p_filename2) {return X_True;}

	pFile1 = fopen(p_filename1,"r");
	pFile2 = fopen(p_filename2,"r");
	
	if(pFile1 == X_Null || pFile2 == X_Null) {return X_False;}
/*
	if(feof(pFile1)==0){fgets(buf1,MAX_LENGTH_OF_ONE_LINE,pFile1);}
	if(feof(pFile2)==0){fgets(buf2,MAX_LENGTH_OF_ONE_LINE,pFile2);}
	
	fseek(pFile1,0,SEEK_SET);fseek(pFile2,0,SEEK_SET);
	*/
	
	for(i=0;i<start_line;i++)
	{
		fgets(buf1,MAX_LENGTH_OF_ONE_LINE,pFile1);
		fgets(buf2,MAX_LENGTH_OF_ONE_LINE,pFile2);
	}
	
	for(i = start_line; i <= end_line;i++)
	{
		fgets(buf1,MAX_LENGTH_OF_ONE_LINE,pFile1);
		fgets(buf2,MAX_LENGTH_OF_ONE_LINE,pFile2);

		for(j = 0;j<MAX_LENGTH_OF_ONE_LINE;j++)
		{
			
			if(buf1[j] == '\0' && buf2[j] == '\0'){break;}
			
			if(buf1[j] != buf2[j])
			{
				isSame = X_False;
			//	printf(" line %d : buf1[%d]  = % d not equal buf2[%d]  = % d\r\n",i,j,buf1[j],j,buf2[j]);
				i = 0xfffe;
				break;
			}
			//printf(" line %d : buf[%d] same :%2x\r\n",i,j,buf1[j]);
			
		}
		
	}
	 fclose(pFile1);
	fclose(pFile2);
	return isSame;
}


#include "rw_by_line.h"
#include <stdio.h>
#include <stdarg.h>

#include <string.h>

#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))

static char buf[MAX_LENGTH_OF_FILE_NAME];
	
char* ConvFileStrToChar(const char *src)
{
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
	uint16_t i,offset;
	X_Boolean isOK = X_False;
	char logstr[MAX_LENGTH_OF_ONE_LINE+1];
	char temp_buf[MAX_LENGTH_OF_ONE_LINE+100];
	
	FILE* pFile;
	va_list argp;
	if(line_num == 0xFFFF || p_string == X_Null || 0==p_string[0]) {return X_False;}
	pFile = fopen(p_filename,"r+");
	if(pFile == X_Null) {return X_False;}

	for(i=0;i<MAX_LENGTH_OF_ONE_LINE;i++)
	{
		logstr[i] = ' ';
	}

	offset = 0;
	for(i=0;i<line_num;i++)
	{
		if(fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile) == NULL) 
		{
			fputs("\n",pFile);
			offset ++;
			fgets(temp_buf,MAX_LENGTH_OF_ONE_LINE,pFile);
			printf(" --------------------empty line %d ,put sth in the line  offset : %d\r\n",i,offset);
			 
		}
		else
		{
			offset +=strlen(temp_buf) ;
			//offset++;
			printf("-------------------- get sth in the line %d %s offset :%d\r\n",i,temp_buf,offset);
		}
	}
	// lock irq
	 va_start(argp,p_string);
	if (-1== vsnprintf(logstr,ARRSIZE(logstr),p_string,argp)) logstr[ARRSIZE(logstr)-1]='\0';
/*
	for(i=0;i<MAX_LENGTH_OF_ONE_LINE;i++)
	{
		if(logstr[i] == '\0') 
		{
			printf("ARRSIZE(logstr) %d %d\r\n",i,logstr[i+1]);
			logstr[i] = ' ';
			break;
		}
	}
	*/
	  i = fseek(pFile,offset,SEEK_SET);/*定位到要修改的位置，注意，这个位置是上一次读的最后，故写的时候要先写换行，第一行除外*/
	  /*
	  if(line_num!=0) 
	  { 
	   fprintf(pFile,"\n"); 
	  } 
	  */
	  fprintf(pFile,"%s\n",logstr);
	  printf(" -------------------------------------------------put %s in the offset %d\r\n",logstr,offset);
	  //fprintf(pFile,"%s","\r\n"); 
	//fputs();
	//fwrite(logstr,1,8,pFile);
	va_end(argp);
	// unlock irq
    fclose(pFile);


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



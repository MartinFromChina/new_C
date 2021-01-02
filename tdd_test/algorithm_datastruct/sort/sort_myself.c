#include <stdio.h>
#include "sort_myself.h"

#define MAX_UINT16  0xffff
static uint16_t current_smallist = MAX_UINT16;
static uint16_t get_smallist_and_reload(uint16_t *sour1,uint16_t *sour2,uint16_t current_length)
{
	// no need length check ,because the check is done by the caller: sort_from_small_to_big
	uint16_t i,j,smallist_position = 0 ;
	
	for(i = 0;i < current_length;i++)
	{	
      //printf("sour1[%d] =  %d\r\n",i,sour1[i]);
	  if(sour1[i] <= current_smallist)
	  {
		//printf("sour1[%d] =  %d <= current_smallist  %d\r\n",i,sour1[i],current_smallist);
	  	current_smallist = sour1[i];
		smallist_position = i;
		
	  }	  
	}
	//printf("current_length %d current_smallist  %d smallist_position %d \r\n"
	//	,current_length,current_smallist,smallist_position);

	for(i = 0,j = 0;i < current_length - 1 ;i++)
	{
		if(i == smallist_position) { j++;}
		sour2[i] = sour1[i + j];
	}


	return current_smallist;
}

#define TEMPORARY_LENGTH 100

bool sort_from_small_to_big(uint16_t *p_in,uint16_t *p_out,uint16_t length)
{
	uint16_t i,smallist = 0;
	uint16_t temp1[TEMPORARY_LENGTH],temp2[TEMPORARY_LENGTH];
	//printf("the length is %d\r\n",length);
	if(length == MAX_UINT16) {return false;}
	if(p_in == NULL or p_out == NULL) {return false;}
	
	for(i = 0 ; i < length  ; i++)
	{
		//printf("the fisrt loop i = %d\r\n",i);
		temp1[i] = p_in[i];
	}
    for(i = 0 ; i < length  ; i++)
	{
		current_smallist = MAX_UINT16;
		if( (i % 2) == 0)
		{
			smallist = get_smallist_and_reload(temp1,temp2,length - i);
		}
		else
		{
			smallist = get_smallist_and_reload(temp2,temp1,length - i);
		}
		p_out[i] = smallist;
	}

	return true;
}

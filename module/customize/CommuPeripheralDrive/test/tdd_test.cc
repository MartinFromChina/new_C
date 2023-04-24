#include <iostream>
#include "tdd_common.h"

//#include "../const_loop_scheduler.h"

#include <stdio.h>
#define LOG_DEBUG_METHOD   printf
#include "../../../common/InsertLog/InsertLogDebug.h"
#include "../../../common/AppCommon.h"
using namespace std;

// #include "../../../component/const_loop_scheduler/x_task_common.h"

static X_Boolean StringCompare(char * c1,char * c2, uint16_t length )
{
  printf("length is %d %s  %s\r\n",length,c1,c2);
  if(length == 0) {return X_False;}
  for(uint16_t i=0;i<length;i++)
  {
      if(c1[i] != c2[i]) {return X_False;}
  }
  return X_True;
}
static X_Boolean DataCompare(uint8_t * c1,uint8_t * c2, uint16_t length )
{
  if(length == 0) {return X_False;}
  for(uint16_t i=0;i<length;i++)
  {
      if(c1[i] != c2[i]) {return X_False;}
  }
  return X_True;
}
/*************************************************************************************/
static char test_string[] = {
"\"\",124rpdfgfsd,\"abc\"......+PBK Ready  \r\n O O OK O ab Ok kO KO   \"\"\"\"\"\"\"\"\"\"\"\"\" \"004390abcdefwqn\" \"0043907\" +SMS Ready +SYSTEM:RESET 13 \r\n +sYSTEM:RESET \r\n 4 +SYSTEM :RESET \r\n OK Ok \"1234567890\"..."
};
/*************************************************************************************/
#include "../cp_drive.h"


static uint8_t *p_recv_data;
static uint16_t recv_length;
static uint8_t GetByteTestbuf[20000];
static uint16_t get_byte_start = 0,get_byte_length = 0,call_cnt = 0;
static X_Boolean GetByteTestFunc(uint8_t *p_data)
{
    call_cnt ++;
    if((call_cnt + 1) >= 20000) {return X_False;}
    if(call_cnt < get_byte_start) {return X_False;}
    if(call_cnt >= (get_byte_length + get_byte_start) ) {return X_False;}

    *p_data = GetByteTestbuf[call_cnt - get_byte_start];

    // printf("  %2x \r\n",*p_data);

    return X_True;
}
/*************************************************************************************/
static uint8_t temp_cnt = 0,log_buf[2];
static uint16_t send1_cnt = 0,send2_cnt = 0;

static uint8_t RecvDataHandleState = 0;
static uint16_t RecvDataHandleIndex = 0;
static X_Boolean RecvDataHandle(uint8_t cur_data,uint16_t * p_recv_length,uint8_t *p_recv_buf,uint16_t max_length)
{
  X_Boolean isOK = X_False;
	uint16_t length;
	uint8_t *p_data;

	switch(cur_data)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
      if(RecvDataHandleIndex < max_length && RecvDataHandleState == 1)
      {
        p_recv_buf[RecvDataHandleIndex ++] = cur_data;
      }
			break;
		case '"':             // recv finished    or  recv start 
        if(RecvDataHandleState == 0) 
        {
            RecvDataHandleState = 1;
        }
				if(RecvDataHandleState != 0 && RecvDataHandleIndex > 1 && RecvDataHandleIndex <= max_length)
				{
					*p_recv_length = RecvDataHandleIndex ;
					//  printf(" !!! load %d ;to recv_data %s \r\n",*p_recv_length,(char*)p_recv_buf);                       
					isOK = X_True;
				} 
        RecvDataHandleIndex = 0;
			break;
		default:  // recv failed 
        RecvDataHandleState = 0;
				RecvDataHandleIndex = 0;
			break;
	}
  
  return isOK;
}

static X_Void RecvInit(const s_cp_manager * p_manager)
{
  RecvDataHandleState = 0;
  RecvDataHandleIndex = 0;
  byteBufInit(p_manager ->p_recv_param ->p_buf,p_manager ->p_recv_param ->max_length,'\0');
}

HSM_LOW_LEVEL_BODY_BEGIN(normal_recv){
  
  CP_P_THIS_FOR_RECV(CP_RECV_HSM_DEEPTH);
  //H_delay(1000); // do not use delay function ; use delay will miss the recv data 
  //printf("  ~~~~~~~~~~~~~~~~~~ms %d %2x : %d  max recv length is %d \r\n",ms,p_this ->new_data,p_this ->is_new_data_come,p_this ->max_length);
 H_while(p_this ->is_new_recv == 0)
  {
    H_wait_until(p_this ->is_new_data_come != 0);
    //////// H_do(log_buf[0] = p_this ->new_data;log_buf[1] = '\0';printf(" ms %d recving %s\r\n",ms,(char*)log_buf));
    H_do(p_this ->is_new_recv = (RecvDataHandle(p_this ->new_data,&p_this ->recv_length,p_this ->p_buf,p_this ->max_length) == X_True) ? 1 : 0);  
  }H_endloop
 
   /*
  if(p_this ->is_new_data_come != 0)
  {
    log_buf[0] = p_this ->new_data;log_buf[1] = '\0';printf(" ms %d recving %s\r\n",ms,(char*)log_buf);
    p_this ->is_new_recv = (RecvDataHandle(p_this ->new_data,&p_this ->recv_length,p_this ->p_buf,p_this ->max_length) == X_True) ? 1 : 0;
    return (p_this ->is_new_recv == 0);
  }
  else
  {
    return X_True;
  }
 */

}HSM_BODY_END

HSM_LOW_LEVEL_BODY_BEGIN(normal_send){
  CP_P_THIS_FOR_SEND(CP_SEND_HSM_DEEPTH);
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  /*
  printf(" ms %d send length %d : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\r\n",ms,p_this ->send_length,
                          p_this ->p_send_data[0],
                          p_this ->p_send_data[1],
                          p_this ->p_send_data[2],
                          p_this ->p_send_data[3],
                          p_this ->p_send_data[4],
                          p_this ->p_send_data[5],
                          p_this ->p_send_data[6],
                          p_this ->p_send_data[7],
                          p_this ->p_send_data[8],
                          p_this ->p_send_data[9]);
  */

  p_this ->result = 1;
  switch(ms)
  {
    case 3:
    case 1002:
    case 2002:
        send1_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_this ->p_send_data,p_this ->send_length));
      break;
    case 5:
    case 102:
    case 202:
    case 302:
    case 402:
    case 502:
    case 602:
    case 702:
    case 802:
    case 902:
    case 1004:
    case 1102:
    case 1202:
    case 1302:
    case 1402:
    case 1502:
    case 1602:
    case 1702:
    case 1802:
    case 1902:
    case 2004:
    case 2102:
    case 2202:
    case 2302:
    case 2402:
    case 2502:
    case 2602:
    case 2702:
    case 2802:
    case 2902:
        send2_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf,p_this ->p_send_data,p_this ->send_length));
      break;
    default:
      break;
  }
}HSM_BODY_END

CP_DRIVE_DEF(normal,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            normal_send,2,20,
            0,0,0);

static uint8_t relay_retry_cnt = 0;
static X_Void relaySendAgain(X_Void)
{
  cp_SendAgain(normal);
}
static X_Boolean relay_lora_send_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
	X_Boolean isOK = X_False;

  switch(ms)
  {
    case 2:
    case 1002:
    case 2002:
        EXPECT_EQ(event,CSRE_Successed);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
      break;
    default:
      break;
  }

	switch(event)
	{
		case CSRE_Successed:
			// printf(" --------------- LE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- LE_TimeOut retry left %d ms %d\r\n",relay_retry_cnt,ms);
		
			if(relay_retry_cnt > 0)
			{
				relay_retry_cnt --;
				relaySendAgain();
			}
			break;
		case CSRE_Error:
			printf(" --------------- LE_Failed ms %d\r\n",ms);
		
			if(relay_retry_cnt > 0)
			{
				relay_retry_cnt --;
				relaySendAgain();
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- CSRE_RecvCheck ms %d\r\n",ms);
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(recv,normal)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  uint32_t time_ms = 0;
  temp_cnt = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(normal,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(normal,i+1);
      if(cp_DoesNewDataCome(normal,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
          case 2:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

      if(((i+1) % 100) == 0 || i== 0)
      {
        /*
        if(i == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(normal,temp_send_buf1,10,relay_lora_recv_cb);
            cp_SendRequest(normal,temp_send_buf,10);
        }
        */
        
        if(((i+1) % 1000) == 0 || i == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(normal,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(normal,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(normal,temp_send_buf,10);
        }
        
          
      }
  }
  EXPECT_EQ(temp_cnt,3);
  EXPECT_EQ(send1_cnt,3);
  EXPECT_EQ(send2_cnt,30);
}
/*************************************************************************************/
static uint8_t cnt2 = 0;
HSM_LOW_LEVEL_BODY_BEGIN(send_test_func){
  CP_P_THIS_FOR_SEND(CP_SEND_HSM_DEEPTH);
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  /*
  printf(" ms %d send length %d : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\r\n",ms,p_this ->send_length,
                          p_this ->p_send_data[0],
                          p_this ->p_send_data[1],
                          p_this ->p_send_data[2],
                          p_this ->p_send_data[3],
                          p_this ->p_send_data[4],
                          p_this ->p_send_data[5],
                          p_this ->p_send_data[6],
                          p_this ->p_send_data[7],
                          p_this ->p_send_data[8],
                          p_this ->p_send_data[9]);
    */
    
    
    switch(ms)
    {
      case 102:
      case 654:
      case 1206:
        send1_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_this ->p_send_data,p_this ->send_length));
        break;
      case 446:
      case 1759:
        send2_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf,p_this ->p_send_data,p_this ->send_length));
       break;
      default:
        break;
    }
}HSM_BODY_END
CP_DRIVE_DEF(send_test,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            send_test_func,3,20,
            0,0,0);


static X_Void SendAgain(X_Void)
{
  cp_SendAgain(send_test);
}


X_Boolean test2_recv_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
	X_Boolean isOK = X_False;

  switch(ms)
  {
    case 443:
      EXPECT_EQ(event,CSRE_RecvCheck);
      EXPECT_EQ(length,10);
      EXPECT_EQ(X_True,StringCompare((char*)"1234567890",(char*)p_data,length));
      break;
    case 269:
      EXPECT_EQ(event,CSRE_RecvCheck);
      EXPECT_EQ(length,7);
      EXPECT_EQ(X_True,StringCompare((char*)"0043907",(char*)p_data,length));
      break;
    default:
      break;
  }

	switch(event)
	{
		case CSRE_Successed:
			printf(" --------------- CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain();
			}
			break;
		case CSRE_Error:
			printf(" --------------- LE_Failed ms %d\r\n",ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain();
			}
			break;
		case CSRE_RecvCheck:
				// printf(" --------------- CSRE_RecvCheck ms %d\r\n",ms);
        /*
        if(length == 10 && StringCompare((char*)"1234567890",(char*)p_data,length) == X_True)
        {
            isOK = X_True;
        }
        */
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(send,with_recv)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  uint32_t time_ms = 0;
  temp_cnt = 0;
  cnt2 = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(send_test,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(send_test,i+1);
      if(cp_DoesNewDataCome(send_test,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        // printf("get length %d \r\n",recv_length);
        
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
          case 2:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

      if(((i+1) % 100) == 0 || i == 0)
      {
        
        if((i+1)== 100 )
        {
            cnt2 = 2;
            cp_SendRequestWithResponse(send_test,temp_send_buf1,10,(cp_send_rec_cb)0,550,test2_recv_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        

        /*
        if((i % 1000) == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(send_test,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(send_test,temp_send_buf,10);
        }
        */
          
      }
  }
  EXPECT_EQ(temp_cnt,3);
  EXPECT_EQ(send1_cnt,3);
  EXPECT_EQ(send2_cnt,1);

}
/*************************************************************************************/
X_Boolean test2_1_recv_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
	X_Boolean isOK = X_False;

  switch(ms)
  {
    /*
    case 443:
      EXPECT_EQ(event,CSRE_RecvCheck);
      EXPECT_EQ(length,10);
      EXPECT_EQ(X_True,StringCompare((char*)"1234567890",(char*)p_data,length));
      break;
    */
    case 269:
      EXPECT_EQ(event,CSRE_RecvCheck);
      EXPECT_EQ(length,7);
      EXPECT_EQ(X_True,StringCompare((char*)"0043907",(char*)p_data,length));
      break;
    default:
      break;
  }

	switch(event)
	{
		case CSRE_Successed:
			printf(" --------------- CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain();
			}
			break;
		case CSRE_Error:
			printf(" --------------- LE_Failed ms %d\r\n",ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain();
			}
			break;
		case CSRE_RecvCheck:
				// printf(" --------------- CSRE_RecvCheck ms %d\r\n",ms);
        
        if(length == 10 && StringCompare((char*)"1234567890",(char*)p_data,length) == X_True)
        {
            isOK = X_True;
        }
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(send,with_recv_ok)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  uint32_t time_ms = 0;
  temp_cnt = 0;
  cnt2 = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(send_test,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(send_test,i + 1);
      if(cp_DoesNewDataCome(send_test,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        // printf("get length %d \r\n",recv_length);
        
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
          case 2:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

  
        
        if((i + 1) == 100)
        {
            cnt2 = 2;
            cp_SendRequestWithResponse(send_test,temp_send_buf1,10,(cp_send_rec_cb)0,550,test2_1_recv_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        

        /*
        if((i % 1000) == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(send_test,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(send_test,temp_send_buf,10);
        }
        */
          
      
  }
  EXPECT_EQ(temp_cnt,2);
  EXPECT_EQ(send1_cnt,1);
  EXPECT_EQ(send2_cnt,1);

}



/*************************************************************************************/
static uint32_t send_ms_backup = 0;
static uint8_t pre_data = 0;
static X_Boolean isNewDataCome = X_False;
HSM_LOW_LEVEL_BODY_BEGIN(send_test_func3){
  CP_P_THIS_FOR_SEND(CP_SEND_HSM_DEEPTH);
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
   switch(ms)
    {
      case 102:
      case 133:
        send1_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_this ->p_send_data,p_this ->send_length));
        break;
      case 155:
        send2_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf,p_this ->p_send_data,p_this ->send_length));
       break;
      default:
        break;
    }
  
  H_if(p_this ->is_new_send_request != 0)
  {
    H_do(printf(" ms %d send length %d : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\r\n",ms,p_this ->send_length,
                          p_this ->p_send_data[0],
                          p_this ->p_send_data[1],
                          p_this ->p_send_data[2],
                          p_this ->p_send_data[3],
                          p_this ->p_send_data[4],
                          p_this ->p_send_data[5],
                          p_this ->p_send_data[6],
                          p_this ->p_send_data[7],
                          p_this ->p_send_data[8],
                          p_this ->p_send_data[9]);
                          p_this ->is_new_send_request = 0;
                          send_ms_backup = ms);
  }H_endif
  

  H_while((ms - send_ms_backup) < 30 && p_this ->result  == 0)
  {
      // H_time_limited_wait_with_result(500,,isNewDataCome);
      //H_do();
      H_if(p_this ->is_new_data_come != 0)
      {
        H_if(pre_data == 'O' && p_this ->new_data == 'K')
        {
            H_do(p_this ->result = 1;printf("~~~~~~~  ms %d find OK",ms));
        }H_endif
        H_do(pre_data = p_this ->new_data);
      }H_endif
  }H_endloop
  
  
    

}HSM_BODY_END

CP_DRIVE_DEF(send_test3,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            send_test_func3,3,20,
            0,0,0);

static X_Void SendAgain3(X_Void)
{
  cp_SendAgain(send_test3);
}
X_Boolean test3_send_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
	X_Boolean isOK = X_False;
  
  switch(ms)
  {
    case 153:
        EXPECT_EQ(event,CSRE_Successed);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
      break;
    case 132:
        EXPECT_EQ(event,CSRE_Error);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
    default:
      break;
  }
  
	switch(event)
	{
		case CSRE_Successed:
			 printf(" --------------- CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain3();
			}
			break;
		case CSRE_Error:
			printf(" --------------- LE_Failed retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain3();
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- CSRE_RecvCheck ms %d\r\n",ms);
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(send,failed_1_then_ok)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  pre_data = 0;
  uint32_t time_ms = 0;
  temp_cnt = 0;
  cnt2 = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(send_test3,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(send_test3,i+1);
      if(cp_DoesNewDataCome(send_test3,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        // printf("get length %d \r\n",recv_length);
        
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
          case 2:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

      if(((i+1) % 100) == 0 || i == 0)
      {
        
        if((i+1) == 100)
        {
            cnt2 = 2;
            cp_SendRequestWithResponse(send_test3,temp_send_buf1,10,test3_send_cb,550,(cp_send_rec_cb)0);
            cp_SendRequest(send_test3,temp_send_buf,10);
        }
        

        /*
        if((i % 1000) == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(send_test,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(send_test,temp_send_buf,10);
        }
        */
          
      }
  }
  EXPECT_EQ(temp_cnt,3);
  EXPECT_EQ(send1_cnt,2);
  EXPECT_EQ(send2_cnt,1);

}

/*************************************************************************************/

HSM_LOW_LEVEL_BODY_BEGIN(send_test_func4){
  CP_P_THIS_FOR_SEND(CP_SEND_HSM_DEEPTH);
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  
   switch(ms)
    {
      case 102:
      case 133:
      case 164:
        send1_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_this ->p_send_data,p_this ->send_length));
        break;
      case 196:
        send2_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf,p_this ->p_send_data,p_this ->send_length));
       break;
      default:
        break;
    }
    
  
  H_if(p_this ->is_new_send_request != 0)
  {
    
    H_do(/*printf(" ms %d send length %d : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\r\n",ms,p_this ->send_length,
                          p_this ->p_send_data[0],
                          p_this ->p_send_data[1],
                          p_this ->p_send_data[2],
                          p_this ->p_send_data[3],
                          p_this ->p_send_data[4],
                          p_this ->p_send_data[5],
                          p_this ->p_send_data[6],
                          p_this ->p_send_data[7],
                          p_this ->p_send_data[8],
                          p_this ->p_send_data[9]);*/
                          p_this ->is_new_send_request = 0;
                          send_ms_backup = ms);
        
  }H_endif
  

  H_while((ms - send_ms_backup) < 30 && p_this ->result  == 0)
  {
      H_if(p_this ->is_new_data_come != 0)
      {
        H_if(pre_data == 'O' && p_this ->new_data == 's')
        {
            H_do(p_this ->result = 1;printf("~~~~~~~  ms %d find Os",ms));
        }H_endif
        H_do(pre_data = p_this ->new_data);
      }H_endif
  }H_endloop


}HSM_BODY_END

CP_DRIVE_DEF(send_test4,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            send_test_func4,3,20,
            0,0,0);



static X_Void SendAgain4(X_Void)
{
  cp_SendAgain(send_test4);
}

X_Boolean test4_send_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
	X_Boolean isOK = X_False;
  
  switch(ms)
  {
    case 153:
        EXPECT_EQ(event,CSRE_Successed);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
      break;
    case 132:
        EXPECT_EQ(event,CSRE_Error);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
    default:
      break;
  }
  
	switch(event)
	{
		case CSRE_Successed:
			 printf(" --------------- send CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- send CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain4();
			}
			break;
		case CSRE_Error:
			printf(" --------------- send LE_Failed retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain4();
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- send CSRE_RecvCheck ms %d\r\n",ms);
				
			break;
		default:
			break;
	}
	return isOK;
}

X_Boolean test4_recv_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  X_Boolean isOK = X_False;
  switch(event)
	{
		case CSRE_Successed:
			 printf(" --------------- recv CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- recv CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain4();
			}
			break;
		case CSRE_Error:
			printf(" --------------- recv LE_Failed retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain4();
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- recv CSRE_RecvCheck ms %d\r\n",ms);
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(send_recv,failed_2_then_over)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  pre_data = 0;
  uint32_t time_ms = 0;
  temp_cnt = 0;
  cnt2 = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(send_test4,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(send_test4,i+1);
      if(cp_DoesNewDataCome(send_test4,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        // printf("get length %d \r\n",recv_length);
        
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
          case 2:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

      if(((i+1) % 100) == 0 || i == 0)
      {
        
        if((i+1) == 100)
        {
            cnt2 = 2;
            cp_SendRequestWithResponse(send_test4,temp_send_buf1,10,test4_send_cb,550,test4_recv_cb); // 
            cp_SendRequest(send_test4,temp_send_buf,10);
        }
        

        /*
        if((i % 1000) == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(send_test,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(send_test,temp_send_buf,10);
        }
        */
          
      }
  }
  EXPECT_EQ(temp_cnt,3);
  EXPECT_EQ(send1_cnt,3);
  EXPECT_EQ(send2_cnt,1);

}

/*************************************************************************************/
static X_Boolean isNeedCheckOk = X_True;
static uint8_t   recv_cnt = 0;
HSM_LOW_LEVEL_BODY_BEGIN(send_test_func5){
  CP_P_THIS_FOR_SEND(CP_SEND_HSM_DEEPTH);
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  
   switch(ms)
    {
      case 102:
      case 133:
        send1_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_this ->p_send_data,p_this ->send_length));
        break;
      case 272:
        send2_cnt ++;
        EXPECT_EQ(p_this ->send_length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf,p_this ->p_send_data,p_this ->send_length));
       break;
      default:
        break;
    }
    
  
  H_if(p_this ->is_new_send_request != 0)
  {
    
    H_do(/*printf(" ms %d send length %d : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\r\n",ms,p_this ->send_length,
                          p_this ->p_send_data[0],
                          p_this ->p_send_data[1],
                          p_this ->p_send_data[2],
                          p_this ->p_send_data[3],
                          p_this ->p_send_data[4],
                          p_this ->p_send_data[5],
                          p_this ->p_send_data[6],
                          p_this ->p_send_data[7],
                          p_this ->p_send_data[8],
                          p_this ->p_send_data[9]);  */
                          p_this ->is_new_send_request = 0;
                          send_ms_backup = ms);
        
  }H_endif
  

  H_while((ms - send_ms_backup) < 30 && p_this ->result  == 0)
  {
      H_if(isNeedCheckOk == X_False)
      {
        H_do(p_this ->result = 1);
        H_break;
      }H_endif
      H_if(p_this ->is_new_data_come != 0)
      {
        H_if(pre_data == 'O' && p_this ->new_data == 'K')
        {
            H_do(p_this ->result = 1;printf("~~~~~~~  ms %d find OK",ms));
        }H_endif
        H_do(pre_data = p_this ->new_data);
      }H_endif
  }H_endloop


}HSM_BODY_END

CP_DRIVE_DEF(send_test5,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            send_test_func5,3,20,
            0,0,0);



static X_Void SendAgain5(X_Void)
{
  cp_SendAgain(send_test5);
}

X_Boolean test5_send_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
	X_Boolean isOK = X_False;
  
  switch(ms)
  {
    case 133:
        EXPECT_EQ(event,CSRE_Successed);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
      break;
    case 102:
        EXPECT_EQ(event,CSRE_Error);
        EXPECT_EQ(length,10);
        EXPECT_EQ(X_True,DataCompare(temp_send_buf1,p_data,length));
    default:
      break;
  }
  
	switch(event)
	{
		case CSRE_Successed:
			 printf(" --------------- send CSRE_Successed ms %d\r\n",ms);
       //////// recv_cnt = 2;
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- send CSRE_Timeout retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain5();
			}
			break;
		case CSRE_Error:
			printf(" --------------- send LE_Failed retry left %d ms %d\r\n",cnt2,ms);
		
			if(cnt2 > 0)
			{
				cnt2 --;
				SendAgain5();
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- send CSRE_RecvCheck ms %d\r\n",ms);
				
			break;
		default:
			break;
	}
	return isOK;
}

X_Boolean test5_recv_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
  X_Boolean isOK = X_False;
  switch(event)
	{
		case CSRE_Successed:
			 printf(" --------------- recv CSRE_Successed ms %d\r\n",ms);
			isOK = X_True;
			break;
		case CSRE_Timeout:
			printf(" --------------- recv CSRE_Timeout retry left %d ms %d\r\n",recv_cnt,ms);
      isNeedCheckOk = X_False;
			if(recv_cnt > 0)
			{
				recv_cnt --;
				SendAgain5();
        cnt2 = 2;
			}
			break;
		case CSRE_Error:
			printf(" --------------- recv LE_Failed retry left %d ms %d\r\n",recv_cnt,ms);
		
			if(recv_cnt > 0)
			{
				recv_cnt --;
				SendAgain5();
        cnt2 = 2;
			}
			break;
		case CSRE_RecvCheck:
				printf(" --------------- recv CSRE_RecvCheck ms %d\r\n",ms);
        if(length == 7 && StringCompare((char*)"0043907",(char*)p_data,length) == X_True)
        {
            isOK = X_True;
        }
				
			break;
		default:
			break;
	}
	return isOK;
}

TEST(send_recv,failed_1_then_ok_then_recv)
{
  uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  isNeedCheckOk = X_True;
  pre_data = 0;
  recv_cnt = 0;
  uint32_t time_ms = 0;
  temp_cnt = 0;
  cnt2 = 0;
  send1_cnt = 0;send2_cnt = 0;
  call_cnt       = 0;
  get_byte_start = 30;
  get_byte_length = sizeof(test_string)/sizeof(uint8_t);
  CopyBuffer(test_string,GetByteTestbuf,(sizeof(test_string)/sizeof(char)));

  if(cp_dri_init(send_test5,RecvInit) == X_False)
  {
    printf("init failed !!!\r\n");
    return;
  }

  for(uint16_t i =0 ;i<3000;i++)
  {   
      cp_dri_handle(send_test5,i+1);
      if(cp_DoesNewDataCome(send_test5,&p_recv_data,&recv_length,&time_ms) == X_True)
      {
        // printf("get length %d \r\n",recv_length);
        
        switch(temp_cnt)
        {
          case 0:
            EXPECT_EQ(time_ms,99);
            EXPECT_EQ(recv_length,3);
            EXPECT_EQ(X_True,StringCompare((char *)"abc",(char *)p_recv_data,recv_length));
            break;
            /*
          case 1:
            EXPECT_EQ(time_ms,269);
            EXPECT_EQ(recv_length,7);
            EXPECT_EQ(X_True,StringCompare((char *)"0043907",(char *)p_recv_data,recv_length));
            break;
            */
          case 1:
            EXPECT_EQ(time_ms,443);
            EXPECT_EQ(recv_length,10);
            EXPECT_EQ(X_True,StringCompare((char *)"1234567890",(char *)p_recv_data,recv_length));
            break;
          default:
            break;
        }
        temp_cnt ++;
      }

      if(((i+1) % 100) == 0 || i == 0)
      {
        
        if((i+1) == 100)
        {
            cnt2 = 2;
            recv_cnt = 2;
            cp_SendRequestWithResponse(send_test5,temp_send_buf1,10,test5_send_cb,100,test5_recv_cb); // 
            cp_SendRequest(send_test5,temp_send_buf,10);
        }
        

        /*
        if((i % 1000) == 0)
        {
            relay_retry_cnt = 2;
            cp_SendRequestWithCallBack(send_test,temp_send_buf1,10,relay_lora_send_cb);
            cp_SendRequest(send_test,temp_send_buf,10);
        }
        else 
        {
          cp_SendRequest(send_test,temp_send_buf,10);
        }
        */
          
      }
  }
  EXPECT_EQ(temp_cnt,2);
  EXPECT_EQ(send1_cnt,2);
  EXPECT_EQ(send2_cnt,1);

}
/*************************************************************************************/

/*************************************************************************************/

/*************************************************************************************/
GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from tdd_test.cc\n";
  testing::InitGoogleTest(&argc, argv);
  #if (WINDOWS_64_SYSTEM == 0)
  	return
  #else
	int value = 
  #endif
  RUN_ALL_TESTS();

#if (WINDOWS_64_SYSTEM!=0)
  value = value;
  while(1)
  {

  }
#endif
}

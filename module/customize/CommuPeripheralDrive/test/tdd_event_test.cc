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

static X_Boolean normal_init(uint32_t ms, s_T_taskBaseParam *p_base);
static X_Boolean normal_get_rssi(uint32_t ms, s_T_taskBaseParam *p_base);
static X_Void passive_event_monitor(uint32_t ms , uint8_t is_new_data,uint8_t cur_data);
static const s_active_event_param  a_event_table[] = {
  {100, normal_init}, // {100, (hsm_task_func)0}, // normal_init
  {1  , normal_get_rssi},
};

CP_DRIVE_DEF(normal,GetByteTestFunc,1,
            NO_USER_DEF_PROCESS,
            normal_recv,5,100,
            normal_send,2,20,
            a_event_table,sizeof(a_event_table)/sizeof(a_event_table[0]),
            passive_event_monitor);

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

TEST(recv11,normal)
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
/*************************************************************************************/
static uint8_t init_cnt = 0;
HSM_LOW_LEVEL_BODY_BEGIN(normal_init){
  CP_P_THIS_FOR_EVENT(CP_EVENT_HSM_DEEPTH);

  H_do(init_cnt ++);
  H_do(printf(" 1 send Idle command\r\n"));
  H_delay(30);
  H_do(printf(" 2 send cfg command\r\n"));
  H_time_limited_wait(100,(p_this ->is_new_data_come == X_True && p_this ->new_data == 'O'));
}HSM_BODY_END;

static uint8_t rssi_cnt = 0;
HSM_LOW_LEVEL_BODY_BEGIN(normal_get_rssi){
  CP_P_THIS_FOR_EVENT(CP_EVENT_HSM_DEEPTH);

  H_do(rssi_cnt ++);
  H_do(printf(" 1 get rssi command ms %d\r\n",ms));
  H_delay(30);
  H_do(printf(" 2 get rssi 56\r\n"));
  H_time_limited_wait(100,(p_this ->is_new_data_come == X_True && p_this ->new_data == 'O'));
}HSM_BODY_END;

static uint8_t pre_data = 0;

static char test1_string[] = {"+SYSTEM:RESET"};

static X_Boolean DoesGetTail(uint8_t cur_data)
{
    if(pre_data == '\r' && cur_data == '\n')
    {
        printf(" get tail  \r\n");
        return X_True;
    }
    return X_False;
}

typedef X_Boolean (*GetTailMethod)(uint8_t cur_data);         

typedef struct 
{
    char      *p_string;
    uint16_t  length;
    GetTailMethod does_get_tail;          
    uint8_t   *p_ext_string;
    uint16_t  ext_length;
}s_CertainStringEvent;


static uint8_t ext_string_buf[100];

static const s_CertainStringEvent CS_1 = {
    test1_string,
    sizeof(test1_string)/sizeof(test1_string[0]),
    DoesGetTail,
    ext_string_buf,
    100,
};

static uint16_t cur_index = 0;
static uint16_t  cur_state = 0;
X_Void FindCertainString(uint8_t cur_data)
{
    uint8_t char_temp[2];
    char_temp[0] = cur_data;
    char_temp[1] = '\0';
    // printf(" %d cur_state %d cur_index %d curdata %s \r\n",CS_1.length,cur_state,cur_index,char_temp);
    switch(cur_state)
    {
        case 0:
            if(cur_data == CS_1.p_string[cur_index])
            {
                cur_state = 1;
                cur_index ++;
            }
            break;
        case 0xffff: // find ext string 
            CS_1.p_ext_string[cur_index ++] = cur_data;
            if(cur_index >= 2 && (cur_index >= CS_1.ext_length || CS_1.does_get_tail(cur_data) == X_True))
            {
                // report the event 
                printf("find ext string length %d \r\n",cur_index - 2);
                for(uint8_t i = 0;i<(cur_index - 2);i++)
                {
                    char_temp[0] = CS_1.p_ext_string[i];
                    char_temp[1] = '\0';
                    printf("  %s \r\n",char_temp);
                }
                cur_state = 0;
                cur_index = 0;
            }
            break;
        default:
            if(cur_data != CS_1.p_string[cur_index])
            {
                if(cur_data == CS_1.p_string[0])
                {
                    //cur_state = 1;
                    cur_index = 1;
                    break;
                }
                cur_state = 0;
                cur_index = 0;
            }
            else
            {
                if((cur_index + 2)>= CS_1.length )
                {
                    printf("find string %s length %d \r\n",CS_1.p_string,CS_1.length);

                    if(CS_1.does_get_tail != (GetTailMethod)0  && CS_1.p_ext_string != (uint8_t *)0 && CS_1.ext_length >= 3)
                    {
                        cur_state = 0xffff;
                        cur_index = 0;
                        break;
                    }
                    cur_state = 0;
                    cur_index = 0;
                }
                cur_index ++;
            }
            break;
    }
    pre_data = cur_data;
}

static X_Void passive_event_monitor(uint32_t ms , uint8_t is_new_data,uint8_t cur_data)
{
  if(is_new_data != 0)
  {
    FindCertainString(cur_data);
  }
}

TEST(event,active)
{
  static X_Boolean isOK = X_False;
   uint8_t temp_send_buf[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t temp_send_buf1[10] = {10,9,8,7,6,5,4,3,2,1};
  uint32_t time_ms = 0;
  init_cnt = 0;
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

  isOK = cp_active_event(normal,5);
  EXPECT_EQ(isOK,X_False);
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
        
          
      }else if(i == 123)
      {
         isOK = cp_active_event(normal,100);
         EXPECT_EQ(isOK,X_True);
      }
      else if(i == 133)
      {
         isOK = cp_active_event(normal,1);
         EXPECT_EQ(isOK,X_True);
      }
      
  }

  EXPECT_EQ(temp_cnt,3);
  EXPECT_EQ(send1_cnt,3);
  EXPECT_EQ(send2_cnt,30);
  EXPECT_EQ(init_cnt,1);
  EXPECT_EQ(rssi_cnt,1);
}

/*************************************************************************************/
/*
TEST(event,passive)
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
*/
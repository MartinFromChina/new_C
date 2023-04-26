#include "test_bench.h"
#include <string.h>
#include <stdio.h>
#include "../../module/common/AppCommon.h"
#include "../../module/common/InsertLog/InsertLogDebug.h"

void c_tb_selfintroduce(void)
{
    printf("hello , I am c test bench \r\n");
}

/**********************************************add the test code below************************************************/
#define UID_LENGTH_IN_BYTE   12
#define MAX_CLOULD_DATA_LENGTH   512

#pragma pack(1)

typedef struct
{
	uint8_t   message_type;
	uint16_t  total_length;
	uint8_t 	uid[UID_LENGTH_IN_BYTE];
}sCloudDataCommonHeader;

typedef union {
	uint32_t address;
	struct {
        uint32_t device_port:  4; //     (bit0 ~ bit3)	  
        uint32_t line_num:     4; //     (bit4 ~ bit7)    00 subnet  0x01 relay 0x02 backup_relay 0x03-06 terminal 0x0e gateway entrance 0xf broadcast
        uint32_t section_num:  4; //     (bit8 ~ bit11)   section number in this kilo
        uint32_t kilometer:   10; //     (bit12 ~ bit21)
        uint32_t rail_way_id: 10; //     (bit22 ~ bit31)
	 }bit;
}u_AddrResolution;

typedef struct{                 // length 21
  uint16_t head; // 0x55aa
  uint32_t src_addr;
  uint32_t dest_addr;
  uint32_t next_addr;
  uint16_t pTrace;
  uint16_t nTrace;
  uint8_t  type;
  uint16_t length;       // payload length

}sLoraCommonHeader;
#pragma pack()

#define MAX_LORA_RECV_DATA_LENGTH    200
#define LORA_DATA_HEADER_SIZE                   ((uint16_t)(sizeof(sLoraCommonHeader)/sizeof(uint8_t)))  
#define CLOULD_DATA_HEADER_SIZE                 ((uint16_t)(sizeof(sCloudDataCommonHeader)/sizeof(uint8_t))) 

#pragma pack(1)

typedef struct
{
    uint8_t ack_cnt; // 0 means no need to wait ack
    uint8_t ack_time_500ms; // 
    uint32_t dest; // src is local 
    uint8_t  p_trace; // uitn16_t ptrace low 8 bits
    uint8_t type;
    uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
    uint8_t param[];
}s_OneCombinateOpCmd;

typedef struct
{
    sLoraCommonHeader head;
    uint8_t op_number;
    uint8_t op_params[MAX_LORA_RECV_DATA_LENGTH - LORA_DATA_HEADER_SIZE - 1 - 3]; // - 1 for opnumber ; - 3 for loramodule uint16_t addr ,uint8_t channel 
}s_CombinateOp_CMD;

typedef struct
{
	sCloudDataCommonHeader clould_head;
	uint8_t config_type;
	uint16_t config_length;
	uint8_t payload[MAX_CLOULD_DATA_LENGTH];
}sClouldCommandCommon;

#pragma pack()

#define MESSAGE_TYPE_COMMAND                                (0x03)
#define CT_LowPowerCmds                                     (0x20)

static X_Void Generator_TM_CMD(char * string,uint8_t *p_data,uint16_t length)
{
    printf("the command is :\r\n");
    for(uint16_t i=0;i<length;i++)
    {
        printf("%2x,",p_data[i]);
    }
    printf("\r\n");


    sClouldCommandCommon clould;
    uint16_t check_sum;
    clould.clould_head.message_type = MESSAGE_TYPE_COMMAND;
    clould.clould_head.total_length = length + 2 + CLOULD_DATA_HEADER_SIZE + 3;
    clould.clould_head.uid[0] = 0X20 ;
    clould.clould_head.uid[1] = 0X39 ;
    clould.clould_head.uid[2] = 0X34 ;
    clould.clould_head.uid[3] = 0X35 ;
    clould.clould_head.uid[4] = 0X56 ;
    clould.clould_head.uid[5] = 0X43 ;
    clould.clould_head.uid[6] = 0X50 ;
    clould.clould_head.uid[7] = 0X12 ;
    clould.clould_head.uid[8] = 0X00 ;
    clould.clould_head.uid[9] = 0X4b ;
    clould.clould_head.uid[10] = 0X00 ;
    clould.clould_head.uid[11] = 0X2d ;
    clould.config_type         = CT_LowPowerCmds;
    clould.config_length       = length;

    CopyBuffer(p_data,clould.payload,length);

    check_sum = ByteGetCheckSum((uint8_t *)&clould,clould.clould_head.total_length - 2);
    clould.payload[clould.clould_head.total_length - CLOULD_DATA_HEADER_SIZE - 3 - 2]  = check_sum;
    clould.payload[clould.clould_head.total_length - CLOULD_DATA_HEADER_SIZE - 3 - 1]  = check_sum >> 8;



               

    uint8_t *p_cloud = (uint8_t *)&clould;
    printf(" %s : the TM command is :\r\nTMCLOUD",string);
    for(uint16_t i=0;i<(clould.clould_head.total_length);i++)
    {
        if(p_cloud[i] < 16)
        {
            printf("0%x",p_cloud[i]);
        }
        else
        {
            printf("%2x",p_cloud[i]);
        }
    }
    printf(" OVER\r\n");

}

#define FRAME_TYPE_COMBINATE_OP                 0xCC
#define FRAME_TYPE_DELAY_TIMING                              0X15 
#define FRAME_TYPE_TRIGGERED_DELAY_TIMING                    0X14

#define FRAME_TYPE_START_SENDWAVE_CMD           0X24
#define FRAME_TYPE_START_RECV_WAVE_CMD                       0x23
#define FRAME_TYPE_TEST_END_CMD                 0X22


#define FRAME_TYPE_RECEIVER_SUBMITS_DATA1                    0X32      //???????1:
#define FRAME_TYPE_RECEIVER_SUBMITS_DATA2                    0X33      //???????2:
#define FRAME_TYPE_TRANSMITTER_SUBMITS_DATA1                 0X34      //???????1:
#define FRAME_TYPE_TERMINAL_SUBMISSION_DATA3                 0X35      //??????3:
#define FRAME_TYPE_RELAY_SUBMISSION_DATA                     0X36      //??????:


X_Void GeneratorCMD(X_Void)
{
    uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    cer_cmd.head.length    = 1;
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 1;
    total_length += 1;

    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" ",(uint8_t *)&cer_cmd,total_length);

}


X_Void GeneratorCMDTiming(X_Void)
{
    uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    cer_cmd.head.length    = 1 + 9 + 10;
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 1;
    total_length += 1;

    /*
    typedef struct
    {
        uint8_t ack_cnt; // 0 means no need to wait ack
        uint8_t ack_time_500ms; // 
        uint32_t dest; // src is local 
        uint8_t  p_trace; // uitn16_t ptrace low 8 bits
        uint8_t type;
        uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
        uint8_t param[];
    }s_OneCombinateOpCmd;

    typedef struct{
        uint32_t      total_time_ms;
        uint32_t      time_accept_addr;
        uint16_t 		delay_ms;    // ?? ping ?? /2
        uint16_t 		check_sum;
    }sTriggeredTimingPayload;
    */
    uint32_t      time_out_ms = 20500;
    uint32_t      time_accept_addr = 0x00038040;
    uint16_t 	  delay_ms = 300; 
    s_OneCombinateOpCmd *p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[0];
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 40;
    p_op ->dest = 0x00038010; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);

    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" ",(uint8_t *)&cer_cmd,total_length);

}


X_Void GeneratorCMDTrigTiming(X_Void) // trigger R1 timing to R2
{
     uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    cer_cmd.head.length    = 1 + 9 + 10;
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 1;
    total_length += 1;

    /*
    typedef struct
    {
        uint8_t ack_cnt; // 0 means no need to wait ack
        uint8_t ack_time_500ms; // 
        uint32_t dest; // src is local 
        uint8_t  p_trace; // uitn16_t ptrace low 8 bits
        uint8_t type;
        uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
        uint8_t param[];
    }s_OneCombinateOpCmd;

    typedef struct{
        uint32_t      total_time_ms;
        uint32_t      time_accept_addr;
        uint16_t 		delay_ms;    // ?? ping ?? /2
        uint16_t 		check_sum;
    }sTriggeredTimingPayload;
    */
    uint32_t      time_out_ms = 20500;
    uint32_t      time_accept_addr = 0x00038610;
    uint16_t 	  delay_ms = 800; 
    s_OneCombinateOpCmd *p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[0];
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 41;
    p_op ->dest = 0x00038010; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);

    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" trigger R1 timing to R2 ",(uint8_t *)&cer_cmd,total_length);
}


X_Void GeneratorCMD_NeighborTrigTiming_LocalTrigTiming(X_Void)
{
    uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    cer_cmd.head.length    = 1 + (9 + 10) +(9+10);
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 2;
    total_length += 1;

    /*
    typedef struct
    {
        uint8_t ack_cnt; // 0 means no need to wait ack
        uint8_t ack_time_500ms; // 
        uint32_t dest; // src is local 
        uint8_t  p_trace; // uitn16_t ptrace low 8 bits
        uint8_t type;
        uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
        uint8_t param[];
    }s_OneCombinateOpCmd;

    typedef struct{
        uint32_t      total_time_ms;
        uint32_t      time_accept_addr;
        uint16_t 		delay_ms;    // ?? ping ?? /2
        uint16_t 		check_sum;
    }sTriggeredTimingPayload;
    */
    uint32_t      time_out_ms = 20500;
    uint32_t      time_accept_addr = 0x00038640;
    uint16_t 	  delay_ms = 300; 
    s_OneCombinateOpCmd *p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[0]; // op 1
    p_op ->ack_cnt = 1;
    p_op ->ack_time_500ms = 41;
    p_op ->dest = 0x00038610; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);



    time_out_ms = 20500;
    time_accept_addr = 0x00038040;
    delay_ms = 300; 
    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10];   // op 2
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 40;
    p_op ->dest = 0x00038010; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);

    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" ",(uint8_t *)&cer_cmd,total_length);

}


X_Void GeneratorCMD_All(X_Void)
{
    uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 5;
    total_length += 1;

    /*
    typedef struct
    {
        uint8_t ack_cnt; // 0 means no need to wait ack
        uint8_t ack_time_500ms; // 
        uint32_t dest; // src is local 
        uint8_t  p_trace; // uitn16_t ptrace low 8 bits
        uint8_t type;
        uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
        uint8_t param[];
    }s_OneCombinateOpCmd;
    */
   /**********************************************1 trigger neighbor timing *********************************************/
    uint32_t      time_out_ms = 20500;
    uint32_t      time_accept_addr = 0x00038640;
    uint16_t 	  delay_ms = 600; 
    s_OneCombinateOpCmd *p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[0]; // op 1
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 41;
    p_op ->dest = 0x00038610; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);
    /*******************************************************************************************/

    /*********************************************2 trigger local timing**********************************************/
    time_out_ms = 20500;
    time_accept_addr = 0x00038040;
    delay_ms = 600; 
    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10];   // op 2
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 40;
    p_op ->dest = 0x00038010; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);
    /*******************************************************************************************/


    /*
    typedef struct{
        uint8_t RecvMode;			// 0  mcu??  1 FPGA??
        uint8_t FREQ;
        uint8_t Signal1_Width;
        uint8_t InterVal_Width; 
        uint8_t Signal2_Width;
        uint32_t threshold;  
        uint16_t check_sum;
    }sStartRecvWaveCmdPayload;     

    r_wave.head.head            = LORA_CMD_FRAME_HEAD;
     r_wave.head.src_addr        = local.address;
     r_wave.head.dest_addr       = cur_dest_addr;
     // r_wave.head.next_addr       = // tansfer will handle it 
     r_wave.head.pTrace          = 0;
     r_wave.head.nTrace          = 0;
     r_wave.head.type            = FRAME_TYPE_START_RECV_WAVE_CMD;
     r_wave.head.length          = RECV_WAVE_REQ_PAYLOAD_SIZE - 2;
     
     r_wave.payload.rwave_request.RecvMode         = cur_mode;
     r_wave.payload.rwave_request.FREQ             = 28;
     
     

    */
     /*****************************************3 make T1 listen**************************************************/
    uint8_t RecvMode = 0;			// 0  mcu??  1 FPGA??
    uint8_t FREQ = 28;
    uint8_t Signal1_Width;
    uint8_t InterVal_Width; 
    uint8_t Signal2_Width;
    uint32_t threshold;  

    if(RecvMode == 0)
    {
        Signal1_Width    = 150;
        InterVal_Width   = 100;
        Signal2_Width    = 150;
        threshold        = 0x400;
    }
    else
    {
        Signal1_Width    = 200;
        InterVal_Width   = 0;
        Signal2_Width    = 0;
        threshold        = 50000;
    }

    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10 + 9+ 10];   // op 3
    p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 4;
    p_op ->dest = 0x00038040; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_START_RECV_WAVE_CMD;
    p_op ->length  = 9;
    p_op ->param[0] = RecvMode;
    p_op ->param[1] = FREQ;
    p_op ->param[2] = Signal1_Width;
    p_op ->param[3] = InterVal_Width;
    p_op ->param[4] = Signal2_Width;
    CopyBuffer(&threshold,&p_op ->param[5],4);
    total_length += (9+9);
    /*******************************************************************************************/

    
     /*****************************************4**************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10 + 9+ 10 + 9 + 9];   // op 3
    /*******************************************************************************************/

    
     /*****************************************5**************************************************/

    /*******************************************************************************************/

    
     /*****************************************6**************************************************/

    /*******************************************************************************************/

    
     /*****************************************7**************************************************/

    /*******************************************************************************************/

    
     /*****************************************8**************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10 + 9+ 10 + 9 + 9];   // op 8
     p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 4;
    p_op ->dest = 0x00038040; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TEST_END_CMD;
    p_op ->length  = 0;
    total_length += (9);
    /*******************************************************************************************/

    
     /*****************************************9**************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[9+10 + 9+ 10 + 9 + 9 + 9];   // op 8
     p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 8;
    p_op ->dest = 0x00038640; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TEST_END_CMD;
    p_op ->length  = 0;
    total_length += (9);
    /*******************************************************************************************/

    cer_cmd.head.length    = total_length - 2 - LORA_DATA_HEADER_SIZE;
    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" all ",(uint8_t *)&cer_cmd,total_length);


}




X_Void GeneratorCMD_All_2(X_Void)
{
    uint16_t check_sum,total_length = 2;
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.next_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    
    total_length += LORA_DATA_HEADER_SIZE;


    cer_cmd.op_number      = 8;
    total_length += 1;

    /*
    typedef struct
    {
        uint8_t ack_cnt; // 0 means no need to wait ack
        uint8_t ack_time_500ms; // 
        uint32_t dest; // src is local 
        uint8_t  p_trace; // uitn16_t ptrace low 8 bits
        uint8_t type;
        uint8_t length; // lora frame is smaller then 200 ; 8bits is enough 
        uint8_t param[];
    }s_OneCombinateOpCmd;
    */
   /*********************************************2 trigger local timing**********************************************/
    uint32_t time_out_ms = 20500;
    uint32_t time_accept_addr = 0x00038040;
    uint16_t delay_ms = 600; 
    s_OneCombinateOpCmd *  p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 2
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 40;
    p_op ->dest = 0x00038010; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);
    /*******************************************************************************************/

   /**********************************************1 trigger neighbor timing *********************************************/
          time_out_ms = 20500;
          time_accept_addr = 0x00038640;
     	  delay_ms = 600; 
    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE]; // op 1
    p_op ->ack_cnt = 0xFF;
    p_op ->ack_time_500ms = 41;
    p_op ->dest = 0x00038610; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TRIGGERED_DELAY_TIMING;
    p_op ->length  = 10;
    CopyBuffer(&time_out_ms,p_op ->param,4);
    CopyBuffer(&time_accept_addr,&p_op ->param[4],4);
    CopyBuffer(&delay_ms,&p_op ->param[8],2);
    total_length += (9+10);
    /*******************************************************************************************/

    


    /*
    typedef struct{
        uint8_t RecvMode;			// 0  mcu??  1 FPGA??
        uint8_t FREQ;
        uint8_t Signal1_Width;
        uint8_t InterVal_Width; 
        uint8_t Signal2_Width;
        uint32_t threshold;  
        uint16_t check_sum;
    }sStartRecvWaveCmdPayload;     

    r_wave.head.head            = LORA_CMD_FRAME_HEAD;
     r_wave.head.src_addr        = local.address;
     r_wave.head.dest_addr       = cur_dest_addr;
     // r_wave.head.next_addr       = // tansfer will handle it 
     r_wave.head.pTrace          = 0;
     r_wave.head.nTrace          = 0;
     r_wave.head.type            = FRAME_TYPE_START_RECV_WAVE_CMD;
     r_wave.head.length          = RECV_WAVE_REQ_PAYLOAD_SIZE - 2;
     
     r_wave.payload.rwave_request.RecvMode         = cur_mode;
     r_wave.payload.rwave_request.FREQ             = 28;
     
     

    */
     /*****************************************3 make T1 listen**************************************************/
    uint8_t RecvMode = 0;			// 0  mcu??  1 FPGA??
    uint8_t FREQ = 28;
    uint8_t Signal1_Width;
    uint8_t InterVal_Width; 
    uint8_t Signal2_Width;
    uint32_t threshold;  

    if(RecvMode == 0)
    {
        Signal1_Width    = 150;
        InterVal_Width   = 100;
        Signal2_Width    = 150;
        threshold        = 0x400;
    }
    else
    {
        Signal1_Width    = 200;
        InterVal_Width   = 0;
        Signal2_Width    = 0;
        threshold        = 50000;
    }

    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 3
    p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 4;
    p_op ->dest = 0x00038040; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_START_RECV_WAVE_CMD;
    p_op ->length  = 9;
    p_op ->param[0] = RecvMode;
    p_op ->param[1] = FREQ;
    p_op ->param[2] = Signal1_Width;
    p_op ->param[3] = InterVal_Width;
    p_op ->param[4] = Signal2_Width;
    CopyBuffer(&threshold,&p_op ->param[5],4);
    total_length += (9+9);
    /*******************************************************************************************/

    /*****************************************4 make T2 send wave**************************************************/
     

    if(RecvMode == 0)
    {
        Signal1_Width    = 150;
        InterVal_Width   = 100;
        Signal2_Width    = 150;
    }
    else
    {
        Signal1_Width    = 200;
        InterVal_Width   = 0;
        Signal2_Width    = 0;
    }

    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 4 
    p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 4;
    p_op ->dest = 0x00038640; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_START_SENDWAVE_CMD;
    p_op ->length  = 4;
    p_op ->param[0] = FREQ;
    p_op ->param[1] = Signal1_Width;
    p_op ->param[2] = InterVal_Width;
    p_op ->param[3] = Signal2_Width;
    total_length += (9+4);

    /*******************************************************************************************/

    /*
     submit_data.head.head            = LORA_CMD_FRAME_HEAD;
     submit_data.head.src_addr        = local.address;
     submit_data.head.dest_addr       = cur_dest_addr;
     // submit_data.head.next_addr       = // tansfer will handle it 
     submit_data.head.pTrace          = 0;
     submit_data.head.nTrace          = 0;
     submit_data.head.type            = FRAME_TYPE_SUBMIT_DATA_CMD;
     submit_data.head.length          = SUBMIT_DATA_PAYLOAD_SIZE - 2;
    
    submit_data.payload.submit_data.DataType = FRAME_TYPE_RECEIVER_SUBMITS_DATA1;

    typedef struct{           // FRAME_TYPE_RECEIVER_SUBMITS_DATA1
        //////  sLoraCommonHeader head;
        uint16_t Voltage[2];		//TWO BATTERY VOLT 
        uint16_t LORA_rssi[2];	//LORA RSSI
        uint16_t tempratrue;  	//TEMPRATURE
        uint16_t TestResult;        //CONNECTED,BREAK,TRAIN PASSING
        uint16_t AMP1[16];		//SIGNAL AMP
        uint16_t AMP0[16];		//NO SIGNAL AMP
        uint16_t check_sum;
    }sReceiverSubmitsData1Payload;


    #define FRAME_TYPE_RECEIVER_SUBMITS_DATA1                    0X32      //???????1:
    #define FRAME_TYPE_RECEIVER_SUBMITS_DATA2                    0X33      //???????2:
    #define FRAME_TYPE_TRANSMITTER_SUBMITS_DATA1                 0X34      //???????1:
    #define FRAME_TYPE_TERMINAL_SUBMISSION_DATA3                 0X35      //??????3:
    #define FRAME_TYPE_RELAY_SUBMISSION_DATA                     0X36      //??????:
    */
     /*****************************************5 make T1 report data **************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 5 
     
     p_op ->ack_cnt = 3;
     p_op ->ack_time_500ms = 4;
     p_op ->dest = 0x00038040; //  
     p_op ->p_trace = 0;
     p_op ->type    = FRAME_TYPE_RECEIVER_SUBMITS_DATA1;
     p_op ->length  = 0;
    total_length += (9);
    
    /*******************************************************************************************/

    
     /*****************************************6  make T2 report data**************************************************/
    p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 6 
     
     p_op ->ack_cnt = 3;
     p_op ->ack_time_500ms = 4;
     p_op ->dest = 0x00038640; //  
     p_op ->p_trace = 0;
     p_op ->type    = FRAME_TYPE_TRANSMITTER_SUBMITS_DATA1;
     p_op ->length  = 0;
    total_length += (9);
    /*******************************************************************************************/

    
     /*****************************************7**************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 8
     p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 4;
    p_op ->dest = 0x00038040; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TEST_END_CMD;
    p_op ->length  = 0;
    total_length += (9);
    /*******************************************************************************************/

    
     /*****************************************8**************************************************/
     p_op = (s_OneCombinateOpCmd *) &cer_cmd.op_params[total_length - 3 - LORA_DATA_HEADER_SIZE];   // op 9
     p_op ->ack_cnt = 3;
    p_op ->ack_time_500ms = 8;
    p_op ->dest = 0x00038640; //  
    p_op ->p_trace = 0;
    p_op ->type    = FRAME_TYPE_TEST_END_CMD;
    p_op ->length  = 0;
    total_length += (9);
    /*******************************************************************************************/

    cer_cmd.head.length    = total_length - 2 - LORA_DATA_HEADER_SIZE;
    check_sum = ByteGetCheckSum((uint8_t *)&cer_cmd,total_length - 2);
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 1 - 1]  = check_sum >> 8;
    cer_cmd.op_params[total_length - LORA_DATA_HEADER_SIZE - 2 - 1]  = check_sum;
    
    Generator_TM_CMD(" all 2",(uint8_t *)&cer_cmd,total_length);


}
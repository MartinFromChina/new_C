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

static X_Void Generator_TM_CMD(uint8_t *p_data,uint16_t length)
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
    printf("the TM command is :\r\n TMCLOUD");
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
    
    Generator_TM_CMD((uint8_t *)&cer_cmd,total_length);

}
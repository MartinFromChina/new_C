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
#pragma pack(1)

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

#pragma pack()


#define FRAME_TYPE_COMBINATE_OP                 0xCC

X_Void GeneratorCMD(X_Void)
{
    s_CombinateOp_CMD cer_cmd;

    cer_cmd.head.head = 0x55aa;
    cer_cmd.head.src_addr = 0x000380e0;
    cer_cmd.head.dest_addr = 0x00038010;
    cer_cmd.head.nexr_addr = 0x00038010;
    cer_cmd.head.pTrace    = 0;
    cer_cmd.head.nTrace    = 0;
    cer_cmd.head.type      = FRAME_TYPE_COMBINATE_OP;
    cer_cmd.head.length    = ;


    
}
#ifndef __LORACOMMUPROTOCOL_H
#define __LORACOMMUPROTOCOL_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../common/x_cross_platform.h"
#include "../../../common/CommonMarco.h"
        
#define NORMAL_WAIT_ACK_TIME_IN_MS     1800
#define LONG_FRAME_WAIT_ACK_TIME_IN_MS 3000
		
#define MAX_ROUTE_TABLE_SIZE           17  // 0 : front link tail ; 16: next link head ; 1 ~ 15 :cur link       
#define INVALID_ROUTE_ADDR             0xFFFFFFFF

#define NUMBER_OF_CURRENT_MEASURE_CHANNEL      2
#define NUMBER_OF_CURRENT_SAMPLES              30    

#define HANDHELD_TERMINAL_DEVICE_NUMBER        (0xFFFFEEEE)        
        
typedef struct //
{
	uint32_t LocalAddr;
	uint8_t  sub_device_flag; // 
}sRoutingGroup;

typedef struct //
{
	sRoutingGroup front_link_tail;
	sRoutingGroup cur_link_table[MAX_ROUTE_TABLE_SIZE];
	sRoutingGroup next_link_head;
}sRoutingTable;
		
typedef enum
{
	FFR_forMe,
	FFR_forMeAndTrans,
	FFR_trans,
    FFR_boardcast, // same frame trans once , maybe for me maybe not 
	FFR_not_care,
}eFrameFilterResult;

typedef enum
{
    LI_Near,
    LI_Relay,
}eLoraInstance;
/********************************************************************************
lora command and response data struct
*******************************************************************************/
#define MAX_BROADCAST_PERIOD_IN_SEC             20   // Լ���ƶ�20����ֻ�ᷢ��1֡�㲥����֡
#define BROADCAST_DEVICE_NUMBER_MASK            0X0F       //�㲥��ַ 0xF
#define FRAME_SRC_ADDR_CLOULD                   0x00000000  // 

#define LORA_CMD_FRAME_HEAD                     0x55AA       //֡ͷ     
#define LORA_ACK_RESP_FRAME_HEAD                0x66CC       //֡ͷ    
#define LORA_RESP_FRAME_HEAD                    0x7788 

#define FRAME_TYPE_RECV_ACK_REPIY               0x11         //���ջظ�ȷ��֡������Ϊ�յ���֡���ͣ�
#define FRAME_TYPE_TEXT_START                   0x21         //���Կ�ʼ    //  TEST
#define FRAME_TYPE_TEXT_RESULT_REPORT           0x25         //���Խ������
#define FRAME_TYPE_TEST_RESULT_QUERY            0x50         //�������м�ѯ�ʲ��Խ��

#define FRAME_TYPE_SENDWAVE                     0x31         //���Բ��η�������
#define FRAME_TYPE_SENDWAVE_AFFIRM              0x32         //���Բ��η���ȷ�ϣ��յ���֡����300ms�����ʱ��

#define FRAME_TYPE_SYNCRONIZE                   0x51         //�ն˽ӵ���ָ���������
#define FRAME_TYPE_GET_CURRENT                  0x52         //
#define FRAME_TYPE_CONFIG                       0x55         //



#define FRAME_TYPE_BROADCAST_DEVICE_CONFIG      0x42
#define FRAME_TYPE_SINGLE_DEVICE_CONFIG         0x43
#define FRAME_TYPE_QUERY_DEVICE_CONFIG          0x45   

#define FRAME_TYPE_LORA_SEND_RECV_TEST          0xE0

#define FRAME_TYPE_FILES_VERSION_REQUEST        0xF0
#define FRAME_TYPE_PROGRAM_CERTAIN_FILE         0xF1
#define FRAME_TYPE_FILE_TRANS_TO_NEIGHBOR       0xF2
#define FRAME_TYPE_LOG_REPORT                   0xF3
/*******************************************************************************/
#pragma pack(1)

typedef union {
	uint32_t address;
	struct {
	  uint32_t device_port: 4; //     (bit0 ~ bit3)
      uint32_t device_num:  4; //     (bit4 ~ bit7)
      uint32_t line_num:    4; //     (bit8 ~ bit11)
	  uint32_t kilometer:  10; //     (bit12 ~ bit21)
      uint32_t rail_way_id:10; //     (bit22 ~ bit31)
	 }bit;
}u_AddrResolution;

typedef struct
{
	uint32_t addr_table[MAX_ROUTE_TABLE_SIZE];
}s_RouteTable;

typedef struct                          // header 
{
	uint8_t header[2];
	uint32_t source_addr;
	uint32_t dest_addr;
	uint32_t next_addr;
	uint16_t length;
	uint8_t  type;
}sLoraDataHeader;

typedef struct                          // common
{
	sLoraDataHeader head;
	uint8_t payload[];
}sLoraDataCommon;

typedef struct                         // lora ack 
{
	sLoraDataHeader head;
	uint16_t checksum;
}sLoraNormalAckFrame;

typedef struct
{
	uint32_t wave_send_addr;
	uint32_t wave_recv_addr;
	uint8_t  on_off_flag;
	uint16_t send_amp;
	uint16_t local_recv_amp;
	uint16_t remote_recv_amp;
}sTestResult;
typedef struct                         // test start cmd
{
	uint32_t time_stamp;
	uint32_t head_addr;
	uint32_t tail_addr;
	uint8_t  direction;  // 0 reverse  !=0 ,forward 
////	sTestResult result[8];
    uint32_t wave_recv_threshold;
    uint32_t neighbor_addr; // �ٶ˸������������豸���ն˺�
	uint16_t checksum;
}sTestStartCommandPayload;

typedef struct                        // wave send request 
{
	uint16_t reserve;
	uint16_t checksum;
}sWaveSendRequestPayload;


typedef struct
{
    uint16_t freq_hz;
    uint16_t width_ms;
    uint32_t threshold;
}sWaveParam;
typedef struct
{
    uint32_t wave_small_addr;
    uint32_t wave_large_addr;
}sWaveNeighborTable;
typedef struct                        // single device config cmd  // for relay device 
{
	uint8_t                 uid[12]; // 12
    uint32_t                terminal_num;
	uint16_t                work_period_s; 
	s_RouteTable            route_table;
    uint32_t                test_head_addr; 
    uint32_t                test_tail_addr; 
    uint16_t                near_lora_channel;  // Mhz
	uint8_t                 near_lora_air_speed; 
	uint16_t                relay_lora_channel;  // Mhz
	uint8_t                 relay_lora_air_speed; 
    uint8_t                 test_line_num_array;  //  bit 1 : in test line ; bit 0 : not in test line  ; (87654321 <-> bit7,bit6,bit5,bit4,bit3,bit2,bit1,bit0)
    uint16_t                vibrate_threshold;
    sWaveParam              wave_param;
    sWaveNeighborTable      wave_neighbor_table[4];
    uint8_t                 pre_order_time_s;           
	uint16_t checksum;
}sSingleDeviceConfigPayload;

typedef struct
{
	uint16_t  TestResult;   //on 01 off 02    recv device communication error 03  send device communication error 04  recv mode work error 05 
	uint32_t  FFT1[15];
	uint32_t  FFT2[15];	
	uint16_t  adc[8];
	uint8_t	master_rssi[2]; // [0] env rssi ; [1] recv rssi
    
    uint8_t relay_rssi[2]; // [0] env rssi ; [1] recv rssi
    uint16_t relay_adc[3]; // [0]temperatue: [1] voltage [2] current
	uint16_t  check_sum;
}sAllDeviceData;
// MAX_FILE_NAME_LENGTH    16
typedef struct                        // program certain file cmd 
{
    uint8_t   type;
    uint16_t  version;
    uint16_t checksum;
}sProgramFileCmdPayLoad;
typedef struct                        // trans certain file to neighbor cmd 
{
    uint32_t file_dest_addr;
    uint8_t  file_type; // 
    uint16_t file_version;
    uint16_t checksum;
}sTransFileCmdPayload;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// MAX_VERSION_FOR_ONE_TYPE_FILE     3
// MAX_MASTER_DEVICE_NUM_WITH_ONE_RELAY    4

//////typedef enum{               // from file_sys.h
//////    CFT_relay = 0,
//////    CFT_master,
//////    CFT_wave,
//////    CFT_fpga,
//////}eCompressedFileType;

typedef struct                        
{
    uint16_t relay[4];  // index 0: file version in use ; index 1,2,3 file version in flash 
    uint16_t master[4];
    uint16_t wave[4];
    uint16_t fpga[4];
}sDeviceFileVersion;
typedef struct                        // file version response  
{
    sDeviceFileVersion  m_device[5];  // 1 relay device : 4 master device max 
    uint16_t checksum;
}sFileVersionPayLoad;

typedef struct                        // current  response  
{
    uint16_t current[NUMBER_OF_CURRENT_MEASURE_CHANNEL][NUMBER_OF_CURRENT_SAMPLES];
    uint16_t checksum;
}sCurrentPayload;

typedef struct                        
{
    uint8_t  env_rssi;
    uint8_t  recv_rssi;
    uint16_t checksum;
}sLora_SR_Test_Payload;

typedef struct
{
	sLoraDataHeader head;
	union{
				sTestStartCommandPayload    TestStart;
				sWaveSendRequestPayload     WaveRequest;
                sSingleDeviceConfigPayload  SingleConfig;
                sFileVersionPayLoad         FileVersion;
                sProgramFileCmdPayLoad      ProgramFile;
                sTransFileCmdPayload        FileTrans;
				sAllDeviceData              AllDeviceData;
                sCurrentPayload             current;
                sLora_SR_Test_Payload       sr_test;
			}payload;
}sLoraDataMultiParse;

#pragma pack()

typedef enum
{
    TTR_on = 1,
    TTR_off = 2,
    TTR_receiver_lost = 3, // // relay set TTR_receiver_lost
    TTR_senter_lost   = 4,
    TTR_recviver_inner_error = 5, // WAVE MCU not ready 
    TTR_no_error = 6,
}e_TermimalTestResult;


#define LORA_DATA_HEADER_SIZE                   ((uint16_t)(sizeof(sLoraDataHeader)/sizeof(uint8_t)))
#define MAX_LORA_FRAME_SIZE                     ((uint16_t)(sizeof(sLoraDataMultiParse)/sizeof(uint8_t)))

#define LORA_AllDATA_PAYLOAD_SIZE  				((uint16_t)(sizeof(sAllDeviceData)/sizeof(uint8_t)))
#define LORA_TEST_START_PAYLOAD_SIZE            ((uint16_t)(sizeof(sTestStartCommandPayload)/sizeof(uint8_t)))
#define LORA_WAVE_SEND_REQUEST_PAYLOAD_SIZE     ((uint16_t)(sizeof(sWaveSendRequestPayload)/sizeof(uint8_t)))
#define LORA_SINGLE_CONFIG_PAYLOAD_SIZE         ((uint16_t)(sizeof(sSingleDeviceConfigPayload)/sizeof(uint8_t)))
#define FILE_VERSION_PAYLOAD_SIZE               ((uint16_t)(sizeof(sFileVersionPayLoad)/sizeof(uint8_t)))
#define PROGRAM_FILE_PAYLOAD_SIZE               ((uint16_t)(sizeof(sProgramFileCmdPayLoad)/sizeof(uint8_t)))   
#define FILE_TRANS_PAYLOAD_SIZE                 ((uint16_t)(sizeof(sTransFileCmdPayload)/sizeof(uint8_t)))   
#define MASTER_CURRENT_PAYLOAD_SIZE  		    ((uint16_t)(sizeof(sCurrentPayload)/sizeof(uint8_t)))
#define LORA_SR_RSSI_PAYLOAD_SIZE  		        ((uint16_t)(sizeof(sLora_SR_Test_Payload)/sizeof(uint8_t)))
/*******************************************************************************/

//////X_Void LoraCommonHeaderLoad(uint8_t type,sCloudDataCommonHeader *p_header,uint16_t total_length);



#ifdef __cplusplus
		}
#endif

#endif

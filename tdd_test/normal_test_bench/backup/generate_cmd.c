#define LENGTH_BEFORE_PAYLOAD    12
#define FRAME_TYPE_DEVICE_CONFIG               0x42
#define FRAME_TEPY_TEXT_START           0x21

#pragma pack(1)

typedef struct
{
	uint8_t header[2];         
	uint16_t source_addr;     
	uint16_t dest_addr;     
	uint16_t next_addr;
	uint16_t length;                      
}sLoraDataHeader;

typedef struct
{
	sLoraDataHeader head;
	uint8_t payload[];
}sLoraData;

typedef struct
{
	sLoraDataHeader head;
    uint8_t  type;
	uint8_t uid[12];
	uint16_t t_number;
	uint32_t sys_time;
	uint8_t lora_rssi[4];
	uint8_t work_freq;
	uint8_t rec_width;
	uint8_t send_width;
	uint32_t wave_rec_threshold;
	uint16_t vibrate_threshold;
	uint16_t work_period_s;
	uint16_t wave_small_t_num;
	uint16_t wave_large_t_num;
    uint8_t  reserve[8];
	uint16_t checksum;
}sLoraConfig;

typedef struct
{
	sLoraDataHeader head;
	uint8_t  type;   
	uint16_t rail_way_status;
	uint16_t checksum;
}sTestStartCommand;

#pragma pack()

X_Void DisplayData(uint8_t *p_data,uint16_t length)
{
    uint16_t i;
    printf("00 01 4C ");
    for(i=0;i<length;i++)
    {
        if(p_data[i] == 0)
        {
            printf("00 ");
        }
        else if((p_data[i] & 0xF0) == 0)
        {
            printf("0%x ",p_data[i]);
        }
        else
        {
            printf("%2x ",p_data[i]);
        }
       
    }
    printf("\r\n");

}

X_Void GenerateProtocolData(X_Void)
{
    uint16_t i;
    sLoraConfig config_data;

    config_data.head.header[0] =  0x55;
    config_data.head.header[1] =  0xAA;
    config_data.head.source_addr = TwoBytesReverse(0);
    config_data.head.dest_addr   = TwoBytesReverse(1);
    config_data.head.next_addr   = TwoBytesReverse(2);

    config_data.head.length      = sizeof(sLoraConfig)/sizeof(uint8_t);
    config_data.head.length      -= LENGTH_BEFORE_PAYLOAD;
    config_data.head.length      = TwoBytesReverse(config_data.head.length);
    config_data.type             = FRAME_TYPE_DEVICE_CONFIG;

    

    for(i=0;i<12;i++)
    {
        config_data.uid[i]     = 0x78;
    }

    config_data.t_number       = TwoBytesReverse(0x01);

    config_data.sys_time       = TwoBytesReverse(0);

    for(i=0;i<12;i++)
    {
        config_data.lora_rssi[i] = 99;
    }
	
	config_data.work_freq = 28;
	config_data.rec_width = 200;
	config_data.send_width = 200;

	config_data.wave_rec_threshold = FourBytesReverse(15000);
	config_data.vibrate_threshold  = TwoBytesReverse(1000);
	config_data.work_period_s      = TwoBytesReverse(60);
	config_data.wave_small_t_num   = TwoBytesReverse(0);
	config_data.wave_large_t_num   = TwoBytesReverse(2);

    for(i=0;i<8;i++)
    {
        config_data.reserve[i] = 0;
    }
    
	config_data.checksum = ByteGetCheckSum((uint8_t *)&config_data,sizeof(sLoraConfig)/sizeof(uint8_t) - 2);
    config_data.checksum = TwoBytesReverse(config_data.checksum);

    DisplayData((uint8_t *)&config_data,sizeof(sLoraConfig)/sizeof(uint8_t));

    
    sTestStartCommand test_start_data;

    test_start_data.head.header[0] =  0x55;
    test_start_data.head.header[1] =  0xAA;

    test_start_data.head.source_addr = TwoBytesReverse(0);
    test_start_data.head.dest_addr   = TwoBytesReverse(1);
    test_start_data.head.next_addr   = TwoBytesReverse(0xffff);

    test_start_data.head.length      = sizeof(sTestStartCommand)/sizeof(uint8_t);
    test_start_data.head.length      -= LENGTH_BEFORE_PAYLOAD;
    test_start_data.head.length      = TwoBytesReverse(test_start_data.head.length);
    test_start_data.type             = FRAME_TEPY_TEXT_START;
    test_start_data.rail_way_status  = 0;

    test_start_data.checksum = ByteGetCheckSum((uint8_t *)&test_start_data,sizeof(sTestStartCommand)/sizeof(uint8_t) - 2);
    test_start_data.checksum = TwoBytesReverse(test_start_data.checksum);

     DisplayData((uint8_t *)&test_start_data,sizeof(sTestStartCommand)/sizeof(uint8_t));
}

extern X_Void GenerateProtocolData(X_Void);
TEST(protol,generate)
{
  GenerateProtocolData();
}
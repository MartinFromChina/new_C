#include "node_test.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

static s_node_manager node_1,node_2,node_3,node_4,node_5;
static s_node         node1,node2,node3,node4,node5;

#define NODE_RECV_DEBUG 0
#define NODE_RECV1_DEBUG 0


#define DISTANCE_BETWEEN_NODE_1_2   2
#define DISTANCE_BETWEEN_NODE_2_3   3
#define DISTANCE_BETWEEN_NODE_3_4   5
#define DISTANCE_BETWEEN_NODE_4_5   8  

typedef struct
{
	uint8_t node_num;
	uint32_t time;
	uint8_t data[3];
}s_node_rec_table;

static s_node_rec_table node_recv_table[] =
	{
		{2,2,{0x55,0xaa,0}},
		{3,5,{0x55,0xaa,0}},
		{2,8,{0x77,0x88,0x99}},
		{4,10,{0x55,0xaa,0}},
		{4,10,{0x77,0x88,0x99}},
		{1,10,{0x77,0x88,0x99}},
		{5,18,{0x77,0x88,0x99}},
		
	};

static uint16_t recv_times = 0;

static s_node * NodeBasicInit(s_node *p_node,uint8_t mode_num,uint16_t forward_distance)
{
	//s_node *p_node = (s_node *)0;
	if(p_node == X_Null) {return (s_node *)0;}
	 p_node->node_number  				= mode_num;
	 p_node->forware_distance			= forward_distance;
	 p_node->current_hold_wave          = 0;

	 for(uint8_t i = 0;i<MAX_WAVE_ONE_NODE_HOLD;i++)
	 {
		p_node ->wave_trans[i].isDisapper = X_True;
	 }
	 return p_node;
}

static s_node_manager * BasicSet(p_node_handle handle)
{
	X_Boolean isOK;
	s_node_manager *p_manager;
	p_manager = WaveTransInit(handle);

	recv_times = 0;
	
	node_1.p_node = NodeBasicInit(&node1,1,INVALID_NODE_DISTANCE);
	isOK = NodeAdd(p_manager,&node_1);
	EXPECT_EQ(isOK, X_True);

	node_2.p_node = NodeBasicInit(&node2,2,DISTANCE_BETWEEN_NODE_1_2);
	isOK = NodeAdd(p_manager,&node_2);
	EXPECT_EQ(isOK, X_True);
	
	node_3.p_node = NodeBasicInit(&node3,3,DISTANCE_BETWEEN_NODE_2_3);
	isOK = NodeAdd(p_manager,&node_3);
	EXPECT_EQ(isOK, X_True);

	node_4.p_node = NodeBasicInit(&node4,4,DISTANCE_BETWEEN_NODE_3_4);
	isOK = NodeAdd(p_manager,&node_4);
	EXPECT_EQ(isOK, X_True);

	node_5.p_node = NodeBasicInit(&node5,5,DISTANCE_BETWEEN_NODE_4_5);
	isOK = NodeAdd(p_manager,&node_5);
	EXPECT_EQ(isOK, X_True);
	return p_manager;
}

static s_wave * NodeWaveAdd(s_node *p_node,uint8_t *p_buf,uint16_t length,e_direction direc,uint16_t max_distance)
{
	uint8_t i,j;
	s_wave *p_wave = (s_wave *)0;

	for(i=0;i<MAX_WAVE_ONE_NODE_HOLD;i++)
	{
		if(p_node->wave_trans[i].isDisapper == X_True)
		{
			p_wave = &(p_node->wave_trans[i]);
			p_wave ->content_length = length;
			p_wave ->direct         = direc;
			p_wave ->isDisapper     = X_False;
			p_wave ->passed_node_cnt    = 0;
			p_wave ->max_trans_distance = max_distance;
			for(j=0;j<length;j++)
			{
				p_wave ->context[j] = p_buf[j];
			}
			p_node->current_hold_wave ++;
			break;
		}
	
	}
	return p_wave;
}


static X_Boolean NodeRecvHandle(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	uint16_t i;
	s_wave * p_wave;
	UNUSED_PARAMETER(p_manager);UNUSED_PARAMETER(length);

	uint8_t buf2[3] = {0x77,0x88,0x99};
	INSERT(LogDebug)(NODE_RECV_DEBUG,("0: node %d receive at time %d \r\n",current_node_num,GetTime()));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(NODE_RECV_DEBUG,(" %2x ",p_data[i]));
	}
	INSERT(LogDebug)(NODE_RECV_DEBUG,(" \r\n "));
	
	EXPECT_EQ(current_node_num,node_recv_table[recv_times].node_num);
	EXPECT_EQ(GetTime(),node_recv_table[recv_times].time);
	EXPECT_EQ(p_data[0],node_recv_table[recv_times].data[0]);
	recv_times ++;

	if(current_node_num == 3)
	{
		p_wave = NodeWaveAdd(&node3,buf2,3,ED_bidirection,100);
		if(p_wave != X_Null)
		{
			SendWave(p_manager,GetTime(),node3.node_number,p_wave);
		}
	}
	
	return X_True;
}


TEST(trans_response,same_time)
{
	s_wave * p_wave;
	s_node_manager *p_manager;
	X_Boolean isRun = X_True;
	p_manager = BasicSet(NodeRecvHandle);

	EXPECT_EQ(GetNodeNum(),5);

	uint8_t buf1[2] = {0x55,0xaa};
	
	p_wave = NodeWaveAdd(&node1,buf1,2,ED_bidirection,10);
	if(p_wave != X_Null){SendWave(p_manager,GetTime(),node1.node_number,p_wave);}
	
	
	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	EXPECT_EQ(recv_times,7);
	WaveTransDeInit();
}


/*
static s_node_rec_table node_recv_table1[] =
	{
		{3,5,{0x55,0xaa,0}},
		{5,8,{0x55,0xaa,0}},
		{2,8,{0x55,0xaa,0}},
		{2,8,{0x77,0x88,0x99}},
		{1,10,{0x55,0xaa,0}},
		{3,11,{0x77,0x88,0x99}},
		{4,16,{0x77,0x88,0x99}},
		
	};
	*/
static X_Boolean NodeRecvHandle1(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length)
{
	uint16_t i;
	s_wave * p_wave;
	X_Boolean isOK;
	uint8_t buf[3] = {0xaa,0xaa,0xaa};
	UNUSED_PARAMETER(p_manager);UNUSED_PARAMETER(current_node_num);UNUSED_PARAMETER(p_data);UNUSED_PARAMETER(length);
	INSERT(LogDebug)(NODE_RECV1_DEBUG,("0: node %d receive at time %d \r\n",current_node_num,GetTime()));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(NODE_RECV1_DEBUG,(" %d ",p_data[i]));
	}
	INSERT(LogDebug)(NODE_RECV1_DEBUG,(" \r\n "));
	
	//EXPECT_EQ(current_node_num,node_recv_table1[recv_times].node_num);
	//EXPECT_EQ(GetTime(),node_recv_table1[recv_times].time);
	//EXPECT_EQ(p_data[0],node_recv_table1[recv_times].data[0]);
	recv_times ++;
	if(current_node_num == 4)
	{
		p_wave = NodeWaveAdd(&node4,buf,3,ED_bidirection,100);
		if(p_wave != X_Null)
		{
			isOK = SendWave(p_manager,GetTime(),node4.node_number,p_wave);
			EXPECT_EQ(isOK,X_True);
		}
		else
		{
			EXPECT_EQ(0,1)<<"-------------------------------node4 send new wave failed \r\n";
		}
	}
	return X_True;
}
TEST(trans_response,max_response)
{
	s_wave * p_wave;
	s_node_manager *p_manager;
	X_Boolean isRun = X_True;
	p_manager = BasicSet(NodeRecvHandle1);

	EXPECT_EQ(GetNodeNum(),5);

	uint8_t buf1[1] = {1};
	uint8_t buf2[2] = {2,2};
	uint8_t buf3[3] = {3,0x55,3};
	uint8_t buf4[4] = {4,0x77,0x88,4};
	uint8_t buf5[5] = {5,0x55,0xaa,3,5};
	uint8_t buf6[6] = {6,0x77,0x88,0x99,5,6};
	uint8_t buf7[7] = {7,0x55,0xaa,1,2,3,7};
	uint8_t buf8[8] = {8,0x77,0x88,0x99,1,2,3,8};
	uint8_t buf9[9] = {9,0x55,0xaa,1,2,3,4,5,9};
	uint8_t buf10[10] = {10,0x77,0x88,0x99,6,5,4,3,2,10};
	uint8_t buf11[11] = {11,0x77,0x88,0x99,6,5,4,3,2,1,11};
	
	p_wave = NodeWaveAdd(&node4,buf1,1,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime(),node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf2,2,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+101,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf3,3,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+102,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf4,4,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+103,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf5,5,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+104,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf6,6,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+105,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf7,7,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+106,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf8,8,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+107,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf9,9,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+108,node4.node_number,p_wave);}
	p_wave = NodeWaveAdd(&node4,buf10,10,ED_bidirection,100);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+109,node4.node_number,p_wave);}
	
	p_wave = NodeWaveAdd(&node1,buf11,3,ED_backward,10);
	if(p_wave != X_Null){SendWave(p_manager,GetTime()+50,node1.node_number,p_wave);}
	
	
	while(isRun == X_True)
	{
		isRun = RunNodeCommunicationProcess();
	}
	EXPECT_EQ(recv_times,47);
	WaveTransDeInit();
}


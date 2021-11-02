#define 接收探头数       4
#define 发射探头数       2
#define 相邻终端数       4   // 特指本端能监测到的临端总数
#define 数据缓存数       相邻终端数

typedef struct
{
     uint32_t FftData_L[数据缓存数][128];
     uint32_t FftData_R[数据缓存数][128];
}sFftData;
static sFftData AllFftData[接收探头数]; // 频谱数据的数据结构

typedef struct
{
     uint32_t noise_amp;
     uint16_t msec_cnt[相邻终端数];
     uint32_t Signal[相邻终端数];
}sSignalNoise;

static sSignalNoise AllSignalNoiseData[接收探头数]; // 信号噪声数据的数据结构

static uint32_t LocalSignal[发射探头数]; // 本端发射幅度的数据结构

int main(void)
{
  while (1)
  {
	mModule_BC95_Handle();
  }
  
}
#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usmart.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	 
#include "wm8978.h"	 
#include "audioplay.h"	
//ALIENTEK ̽����STM32F407������ ʵ��43
//���ֲ����� ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 

#define TIME_TO_PLAY_MUSIC_AFTER_PWR_UP_IN_MINUTES       360 // 6hours = 6 * 60 = 360 minutes
#define TIME_TO_PLAY_MUSIC_AFTER_PWR_UP_IN_MS            ((uint32_t)(TIME_TO_PLAY_MUSIC_AFTER_PWR_UP_IN_MINUTES * 60000))

static uint32_t cnt = 0;
extern void SetSysCnt(uint32_t ms);
extern uint32_t GetSysCnt(void);
int main(void)
{        
	SetSysCnt(TIME_TO_PLAY_MUSIC_AFTER_PWR_UP_IN_MS);
	cnt = TIME_TO_PLAY_MUSIC_AFTER_PWR_UP_IN_MS;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	usmart_dev.init(84);		//��ʼ��USMART
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ��  
	W25QXX_Init();				//��ʼ��W25Q128
	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(40,40);	//������������
	WM8978_SPKvol_Set(50);		//������������
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  	f_mount(fs[0],"0:",1); 		//����SD��  
	POINT_COLOR=RED;      
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"Explorer STM32F4������",16,0);				    	 
	Show_Str(60,70,200,16,"���ֲ�����ʵ��",16,0);				    	 
	Show_Str(60,90,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(60,110,200,16,"2014��5��24��",16,0);
	Show_Str(60,130,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,150,200,16,"KEY_UP:PAUSE/PLAY",16,0);
	while(1)
	{ 
//		if(GetSysCnt() > 0) 
//		{
//			continue;
//		}
		if(cnt > 0)
		{
			cnt --;
			delay_us(1050);
			continue;
		}
		audio_play();
	} 
}








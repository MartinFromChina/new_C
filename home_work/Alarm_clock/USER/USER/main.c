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
//#include "piclib.h"	
#include "usbh_usr.h" 
//ALIENTEK 探索者STM32F407开发板 实验43
//音乐播放器 实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,320,200,16,"设备连接成功!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//设置字体为蓝色	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16); //显示U盘总容量 MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	POINT_COLOR=RED;//设置字体为红色	   
	Show_Str(30,140,200,16,"设备连接中...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 

int main(void)
{        
////    u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	usmart_dev.init(84);		//初始化USMART
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化  
	W25QXX_Init();				//初始化W25Q128
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(40,40);	//耳机音量设置
	WM8978_SPKvol_Set(50);		//喇叭音量设置
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存  
  	f_mount(fs[0],"0:",1); 		//挂载SD卡  
		f_mount(fs[1],"1:",1); 	//挂载SD卡  
  	f_mount(fs[2],"2:",1); 	//挂载U盘
	POINT_COLOR=RED;      
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"Explorer STM32F4开发板",16,0);				    	 
	Show_Str(60,70,200,16,"音乐播放器实验",16,0);				    	 
	Show_Str(60,90,200,16,"正点原子@ALIENTEK",16,0);				    	 
	Show_Str(60,110,200,16,"2014年5月24日",16,0);
	Show_Str(60,130,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,150,200,16,"KEY_UP:PAUSE/PLAY",16,0);
	
	Show_Str(30,200,200,16,"探索者STM32F407开发板",16,0);				    	 
	Show_Str(30,220,200,16,"USB U盘实验",16,0);					    	 
	Show_Str(30,240,200,16,"2014年7月22日",16,0);	    	 
	Show_Str(30,260,200,16,"正点原子@ALIENTEK",16,0); 
	Show_Str(30,290,200,16,"设备连接中...",16,0);			 		
	
	//初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);
	while(1)
	{ 
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
//		audio_play();
	} 
}





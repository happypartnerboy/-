#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"

 
 
/************************************************
 ALIENTEKս��STM32������ʵ��40
 ������ʾ ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
//��̬ȫ�ֱ���
static u8 u8temperature 	= 25;
static u8 u8humidity 		= 60;
static u8 AConcentration = 5; 
static u8 CO2Concentration = 500; 

//LCD��ʼ����ʾ
void LCD_display_init();
 
 
 int main(void)
 {	
	u32 fontcnt;		  
	u8 i,j;
	u8 fontx[2];//gbk��
	u8 key,t;   	    

	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
	W25QXX_Init();				//��ʼ��W25Q128
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	font_init();
	while(font_init()) 			//����ֿ�
	{
UPD:    
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(30,50,200,16,16,"WarShip STM32");
		while(SD_Init())			//���SD��
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//�����ֿ�
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	       
	}  
	LCD_display_init();
#if 0
	Show_Str(30,70,200,16,"GBK�ֿ���Գ���",16,0);				    	 
	Show_Str(30,90,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(30,110,200,16,"2015��1��20��",16,0);
	Show_Str(30,130,200,16,"��KEY0,�����ֿ�",16,0);
 	POINT_COLOR=BLUE;  
	Show_Str(30,150,200,16,"������ֽ�:",16,0);				    	 
	Show_Str(30,170,200,16,"������ֽ�:",16,0);				    	 
	Show_Str(30,190,200,16,"���ּ�����:",16,0);

	Show_Str(30,220,200,24,"��Ӧ����Ϊ:",24,0); 
	Show_Str(30,244,200,16,"��Ӧ����(16*16)Ϊ:",16,0);			 
	Show_Str(30,260,200,12,"��Ӧ����(12*12)Ϊ:",12,0);
#endif
>>>>>>> 57a8b333a13ee81b585321ef862c2bd92e3fe450
//��д�Լ������ҵ��	
	while(1)
	{
#if 0
		fontcnt=0;
		for(i=0x81;i<0xff;i++)
		{		
			fontx[0]=i;
			LCD_ShowNum(118,150,i,3,16);		//��ʾ������ֽ�    
			for(j=0x40;j<0xfe;j++)
			{
				if(j==0x7f)continue;
				fontcnt++;
				LCD_ShowNum(118,170,j,3,16);	//��ʾ������ֽ�	 
				LCD_ShowNum(118,190,fontcnt,5,16);//���ּ�����ʾ	 
			 	fontx[1]=j;
				Show_Font(30+132,220,fontx,24,0);	  
				Show_Font(30+144,244,fontx,16,0);	  		 		 
				Show_Font(30+108,260,fontx,12,0);	  		 		 
				t=200;
				while(t--)//��ʱ,ͬʱɨ�谴��
				{
					delay_ms(1);
					key=KEY_Scan(0);
//					if(key==KEY0_PRES)goto UPD;
				}
				LED0=!LED0;
			}   
		}	
#endif
	} 
}

void LCD_display_init()
{
		POINT_COLOR=BLUE; 
//�¶�
		Show_Str(0,50,50,16,"�¶�:",16,0);	
		Show_Str(80,50,20,16,"C",16,0);					//50 + 
	
//ʪ��
		Show_Str(0,70,50,16,"ʪ��:",16,0);	
		Show_Str(80,70,20,16,"%",16,0);
	
//����Ũ��
		Show_Str(0,90,80,16,"����Ũ��:",16,0);	
		Show_Str(110,90,25,16,"ppm",16,0);

//������̼Ũ��
		Show_Str(0,110,110,16,"������̼Ũ��:",16,0);	
		Show_Str(145,110,25,16,"ppm",16,0);
	
		POINT_COLOR=RED; 
		LCD_ShowNum(52,50,u8temperature,3,16);	
		LCD_ShowNum(52,70,u8humidity,3,16);
		LCD_ShowNum(82,90,AConcentration,3,16);
		LCD_ShowNum(120,110,CO2Concentration,3,16);	
}
















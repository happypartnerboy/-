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
#include "sgp30.h"
#include "dht11.h" 	
 
/************************************************
 ALIENTEK战舰STM32开发板实验40
 汉字显示 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
//静态全局变量
static u8 u8temperature 	= 25;
static u8 u8humidity 		= 60;
static u8 AConcentration = 5; 
static u8 CO2Concentration = 500; 

//LCD初始化显示
void LCD_display();

//延迟时间
void Delay (uint32_t nCount);
 
 int main(void)
 {	
	u32 fontcnt; 
	u8 i,j;
	u8 fontx[2];//gbk码
	u8 key,t;   	    
	int16_t err;
  uint16_t tvoc_ppb, co2_eq_ppm;
  uint32_t iaq_baseline;
  uint16_t ethanol_signal, h2_signal;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	W25QXX_Init();				//初始化W25Q128
	DHT11_Init();
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.
	font_init();
	printf("delay_us  start\n");
	delay_us(1);
	printf("delay_us  end\n");
#if 0
	RCC_ClocksTypeDef RCC_Clocks;
	/* Configure SysTick IRQ and SysTick Timer to generate interrupts every 500*/
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
#endif
	while(font_init()) 			//检查字库
	{
UPD:    
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(30,50,200,16,16,"WarShip STM32");
		while(SD_Init())			//检测SD卡
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	       
	}  
	LCD_display();
//编写自己的相关业务	
		while(1)
	{	    	    
 		if(t%10==0)			//每100ms读取一次
		{				
			DHT11_Read_Data(&u8temperature,&u8humidity);	//读取温湿度值			
			printf("温度 temperature %d\r\n", u8temperature);
			printf("湿度 humidity %d\r\n", u8humidity); 	   
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
		}
		
		//LED进行数据刷新
		LCD_display();
	}
	   /* Busy loop for initialization. The main loop does not work without a sensor. */
#if 0
  while (sgp_probe() != STATUS_OK) {
         printf("SGP sensor probing failed...\r\n"); 
			   Delay(0x400000); 
    }
    printf("SGP sensor probing successful!\r\n"); 

    /* Read gas signals */
    err = sgp_measure_signals_blocking_read(&ethanol_signal,&h2_signal);
    if (err == STATUS_OK) {
         //Print ethanol signal and h2 signal 
         printf("Ethanol signal: %u\t", ethanol_signal); 
         printf("H2 signal: %u\r\n", h2_signal); 
    } else {
         printf("error reading signals\r\n"); 
    }


    /* Consider the two cases (A) and (B):
     * (A) If no baseline is available or the most recent baseline is more than
     *     one week old, it must discarded. A new baseline is found with
     *     sgp_iaq_init() */
    err = sgp_iaq_init();
		
		
    /* (B) If a recent baseline is available, set it after sgp_iaq_init() for
     * faster start-up */
    /* IMPLEMENT: retrieve iaq_baseline from presistent storage;
     * err = sgp_set_iaq_baseline(iaq_baseline);
     */

    /* Run periodic IAQ measurements at defined intervals */
    while (1) {
        err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
        if (err == STATUS_OK) {
             printf("tVOC  : %dppb\t", tvoc_ppb);
             printf("CO2eq : %dppm\r\n", co2_eq_ppm);
             
        } else {
             printf("error reading IAQ values\r\n"); 
        }

        /*
        * IMPLEMENT: get absolute humidity to enable humidity compensation
        * u32 ah = get_absolute_humidity(); // absolute humidity in mg/m^3
        * sgp_set_absolute_humidity(ah);
        */

        /* Persist the current baseline every hour */
        if (++i % 3600 == 3599) {
            err = sgp_get_iaq_baseline(&iaq_baseline);
            if (err == STATUS_FAIL) {
                printf("Failed to get baseline readings\r\n");
            }
						printf("****Baseline values: eCO2: 0x%d\r\n",iaq_baseline>>16); 
					  printf(" & TVOC: 0x%d \r\n",iaq_baseline & 0x0000ffff);
        }

        /* The IAQ measurement must be triggered exactly once per second (SGP30)
         * to get accurate values.
         */
         Delay(0x200000);  // SGP30 */
    }
#endif
#if 0
	while(1)
	{

		fontcnt=0;
		for(i=0x81;i<0xff;i++)
		{		
			fontx[0]=i;
			LCD_ShowNum(118,150,i,3,16);		//显示内码高字节    
			for(j=0x40;j<0xfe;j++)
			{
				if(j==0x7f)continue;
				fontcnt++;
				LCD_ShowNum(118,170,j,3,16);	//显示内码低字节	 
				LCD_ShowNum(118,190,fontcnt,5,16);//汉字计数显示	 
			 	fontx[1]=j;
				Show_Font(30+132,220,fontx,24,0);	  
				Show_Font(30+144,244,fontx,16,0);	  		 		 
				Show_Font(30+108,260,fontx,12,0);	  		 		 
				t=200;
				while(t--)//延时,同时扫描按键
				{
					delay_ms(1);
					key=KEY_Scan(0);
//					if(key==KEY0_PRES)goto UPD;
				}
				LED0=!LED0;
			}   
		}	
	} 
#endif
}

void Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void LCD_display()
{
		POINT_COLOR=BLUE; 
//温度
		Show_Str(0,50,50,16,"温度:",16,0);	
		Show_Str(80,50,20,16,"C",16,0);					//50 + 
	
//湿度
		Show_Str(0,70,50,16,"湿度:",16,0);	
		Show_Str(80,70,20,16,"%",16,0);
	
//氨气浓度
		Show_Str(0,90,80,16,"氨气浓度:",16,0);	
		Show_Str(110,90,25,16,"ppm",16,0);

//二氧化碳浓度
		Show_Str(0,110,110,16,"二氧化碳浓度:",16,0);	
		Show_Str(145,110,25,16,"ppm",16,0);
	
		POINT_COLOR=RED; 
		LCD_ShowNum(52,50,u8temperature,3,16);	
		LCD_ShowNum(52,70,u8humidity,3,16);
		LCD_ShowNum(82,90,AConcentration,3,16);
		LCD_ShowNum(120,110,CO2Concentration,3,16);	
}
















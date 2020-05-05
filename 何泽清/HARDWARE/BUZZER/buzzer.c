#include "buzzer.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void buzzer_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 	//ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 				//����PD-7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 					//�����趨������ʼ��GPIOD.7
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}
 

void buzzer_Enable(int Enable)
{
		if(0 == Enable)
		{
				GPIO_ResetBits(GPIOD,GPIO_Pin_7);					//��λ
		}
		else
		{
				GPIO_SetBits(GPIOD,GPIO_Pin_7);					//��λ
		}
}
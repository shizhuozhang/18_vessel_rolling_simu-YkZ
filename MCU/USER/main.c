#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
#include "timer.h"
#include "Queue.h"
#include "pwm.h"
#include "IO.h"
#include  "Encoder.h"
#include "run.h"


int main(void)
{ 
//	u16 i=0;
//	u8 j=0;
	u8 req_flag=1;
	float exp_time=0,exp_posi=0;

	Stm32_Clock_Init(9); //ϵͳʱ������	
	delay_init(72);	     //��ʱ��ʼ��
	LED_Init();
	BEEP_Init();
	KEY_Init();
	ELSE_IO_Init();
	//EXTIx_Init();
	usart1(72,9600);  //���ڳ�ʼ��	
	usart2(36,9600);  //���ڳ�ʼ��	
	DMA1_Channel6_Init(12,(u32)&USART2->DR,(u32)(RXData.ca_SendData+2));//DMA��ʼ��
	DMA1_Channel7_Init(DATA_NUM,(u32)&USART2->DR,(u32)(SendData.ca_SendData+2));//DMA��ʼ��
	Encoder4_Init();
	TIM1_PWM_Init(45000-1,16-1);
	
	RELAY_ON=1;
	delay_ms(1500);
	delay_ms(1500);
	
	SON=1;	
	while(Ready);
	Set_Speed(5000);
	Find_Center();//5Khz���ٶ���ԭ��
	
	TIM2_Init(720-1,100-1);//1KHZ��1ms,�жϷ���������λ�õ��жϣ���ֹ��ͻӦ�ŵ�����ʼ��
	Time=0;//��ʼ��ʱ	
	while(1)
	{
		if(Zero_flag==1)
		{
			Zero_flag=0;
			Center_Again();
			Send_Position(0,0,ZERO);
		}else
		{
			switch(statue)
			{
				case STOP:
					Pause_Flag=0;
					if(lstatue==RUNNING)
					{
						if(dequeue(&exp_time,&exp_posi))
						{	
							Set_Exp(exp_time,exp_posi);//�ɻ�
							Send_Position(Time/1000.0,Roll_num*10+TIM4->CNT/1000.0,RUNNING);
						}else
						{
							delay_ms(30);
							Send_Position(0,0,END);
							lstatue=STOP;
						}
					}else if(lstatue==PAUSE)
					{
						Qclear();
						lstatue=STOP;
					}
				break;

				case PAUSE:
						Pause_Flag=1;
				break;
				
				case RUNNING:
					Pause_Flag=0;
					if(lstatue==STOP)
					{
						Time=0;
						lstatue=RUNNING;
					}
					if(dequeue(&exp_time,&exp_posi))			
					{
						Set_Exp(exp_time,exp_posi);//�ɻ�
						if(size()<=10&&req_flag==1)	
						{
							req_flag=0;		
							if(statue==PAUSE)
							{
								Send_Position(Time/1000.0,Roll_num*10+TIM4->CNT/1000.0,RUNNING);
							}
							else
							{
								Send_Position(Time/1000.0,Roll_num*10+TIM4->CNT/1000.0,REQUEST);
							}
						}else
						{
								req_flag=1;
							Send_Position(Time/1000.0,Roll_num*10+TIM4->CNT/1000.0,RUNNING);
						}
					}else
					{
						Send_Position(Time/1000.0,Roll_num*10+TIM4->CNT/1000.0,REQUEST);
					}
					
				break;
					
				default:
				break;
			}
		}
	}
}
		






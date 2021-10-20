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

	Stm32_Clock_Init(9); //系统时钟设置	
	delay_init(72);	     //延时初始化
	LED_Init();
	BEEP_Init();
	KEY_Init();
	ELSE_IO_Init();
	//EXTIx_Init();
	usart1(72,9600);  //串口初始化	
	usart2(36,9600);  //串口初始化	
	DMA1_Channel6_Init(12,(u32)&USART2->DR,(u32)(RXData.ca_SendData+2));//DMA初始化
	DMA1_Channel7_Init(DATA_NUM,(u32)&USART2->DR,(u32)(SendData.ca_SendData+2));//DMA初始化
	Encoder4_Init();
	TIM1_PWM_Init(45000-1,16-1);
	
	RELAY_ON=1;
	delay_ms(1500);
	delay_ms(1500);
	
	SON=1;	
	while(Ready);
	Set_Speed(5000);
	Find_Center();//5Khz的速度找原点
	
	TIM2_Init(720-1,100-1);//1KHZ，1ms,中断服务函数中有位置的判断，防止冲突应放到最后初始化
	Time=0;//开始计时	
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
							Set_Exp(exp_time,exp_posi);//干活
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
						Set_Exp(exp_time,exp_posi);//干活
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
		






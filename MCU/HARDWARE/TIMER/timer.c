#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "IO.h"

void TIM2_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;//��ʱ��2��ʼ��
	TIM2->ARR=arr;
	TIM2->PSC=psc;
	TIM2->DIER|=1<<0;//ʹ���ж�
	TIM2->CR1|=1<<7;//arpeʹ��
	TIM2->CR1|=1<<0;//ʹ�ܼ�����
	MY_NVIC_Init(0,0,TIM2_IRQn,2);
}

volatile  u32 Time=0;
u8 Num_Tim=0;
u8 Count1=0;
u8 Count2=0;
u8 Pause_Flag=0;//��ͣʱ��־λ��1
void TIM2_IRQHandler(void) //�������ж�
{

	if(TIM2->SR&0x0001)
	{	
		if(Pause_Flag==0)			
			Time++;		
		
		Num_Tim++;		
		if(Num_Tim>200)
		{
			Num_Tim=0;
			LED0=~LED0;
		}
		
		if(R_LIM==0&&MOTOR_DIR==RIGHT)
		{
			Count1++;
			if(Count1>1)
			{
				TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
				TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
				Count1=0;
			}	
		}
		if(L_LIM==0&&MOTOR_DIR==LEFT)
		{
			Count2++;
			if(Count2>1)
			{
				TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
				TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
				Count2=0;
			}
		}
	
	}
	TIM2->SR&=~(1<<0);//����жϱ�־λ 
}


void Send_Position(float _time,float _position,u8 status)
{
	u16 sum=0;
	SendData.s_SendData.c_FH1=0xef;//֡ͷ
	SendData.s_SendData.c_FH2=0xfe;//
	SendData.s_SendData.u_PointData.s_PointData.f_Time=_time;
	SendData.s_SendData.u_PointData.s_PointData.f_Position=_position;
	SendData.s_SendData.c_Order=status;
	sum=(u16)SendData.ca_SendData[2]+SendData.ca_SendData[3]+SendData.ca_SendData[4]+SendData.ca_SendData[5]+SendData.ca_SendData[6]+SendData.ca_SendData[7]+SendData.ca_SendData[8]+SendData.ca_SendData[9]+SendData.ca_SendData[10]+SendData.ca_SendData[11]+SendData.ca_SendData[12];//У�����ݼ���
	SendData.s_SendData.c_Verify=sum&0xff;
	while(UATR2_COMP);//�ȴ��������
	UATR2_COMP=1;
	DMA1_Channel7->CCR&=~(1<<0);			//ͨ������
	DMA1_Channel7->CNDTR=DATA_NUM;    	//DMA1,����������
	DMA1_Channel7->CCR|=1<<0;			//ͨ������
}




#include "pwm.h"
#include "stdio.h"
#include "io.h"


void TIM1_PWM_Init(u16 arr,u16 psc)//arr���Զ���װֵ; psc��ʱ��Ԥ��Ƶ��
{		 					 
	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��  
	RCC->APB2ENR|=1<<2;		//IO�˿�A��ʱ��ʹ��
	GPIOA->CRH&=0XFFFFFFF0;	//PA8,PA9,PA10,PA11
	GPIOA->CRH|=0X0000000B;	//���ù������ 
	
	TIM1->ARR=arr;			//�趨�������Զ���װֵ 
	TIM1->PSC=psc;			//Ԥ��Ƶ������
  
	TIM1->CCMR1|=0X0D<<3;  	//CH1���,PWM1ģʽ,Ԥװ��ʹ��			 
//	TIM1->CCMR1|=0X0D<<11; 	//CH2���,PWM1ģʽ,Ԥװ��ʹ��
//	TIM1->CCMR2|=0X0D<<3;  	//CH3���,PWM1ģʽ,Ԥװ��ʹ��			 
//	TIM1->CCMR2|=0X0D<<11; 	//CH4���,PWM1ģʽ,Ԥװ��ʹ��
							
 	TIM1->CCER|=0X0001;   	//OC1,2,3,4 ���ʹ�ܣ��������Ϊ�ߵ�ƽ��Ч	   
	TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��	   

	TIM1->CR1=0x0080;   	//���ϼ������Զ���װ��ʹ��
	
	TIM1->DIER|=1<<0;//ʹ���ж�	
	MY_NVIC_Init(3,2,TIM1_UP_IRQn,2);
	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1 
	
    TIM1->CCR1=22500;    	//��ʼװ��ֵ
//	TIM1->CCR2=5;    	//��ʼװ��ֵ
//	TIM1->CCR3=5;    	//��ʼװ��ֵ
//	TIM1->CCR4=500;    	//��ʼװ��ֵ
	//PWM���ģʽ1�£�ռ�ձȵļ��㷽��ΪCCR1/ARR*100%
	//PWM���ģʽ2�£�ռ�ձȵļ��㷽��Ϊ(1-CCR1/ARR)*100%
} 
	
void Set_Speed(u32 frequency)
{
	if(frequency>100000) frequency=100000;
	//if(frequency<1)  frequency=100;
	if(frequency>10000)
	{	
		TIM1->PSC=0;
		TIM1->ARR=72000000/frequency-1;
		TIM1->CCR1=TIM1->ARR/2;
	}else if(frequency>1000)
	{	
		TIM1->PSC=1;
		TIM1->ARR=36000000/frequency-1;
		TIM1->CCR1=TIM1->ARR/2;
	}else if(frequency>100)
	{
		TIM1->PSC=15;
		TIM1->ARR=4500000/frequency-1;
		TIM1->CCR1=TIM1->ARR/2;
	}
	else if(frequency>10)
	{
		TIM1->PSC=150;
		TIM1->ARR=450000/frequency-1;
		TIM1->CCR1=TIM1->ARR/2;
	}
	else if(frequency>=1)
	{
		TIM1->PSC=1500;
		TIM1->ARR=45000/frequency-1;
		TIM1->CCR1=TIM1->ARR/2;
	}else
	{
		TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
	}

}
//u16 count=0;
//u16 encoder=0;
//u8 flag=0;
u32 num=0;
void TIM1_UP_IRQHandler(void) //�������ж�
{
	if(TIM1->SR&0x0001)
	{
		if(num==0)
		{
			TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
			TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
		}else
			num--;
	}
	TIM1->SR&=~(1<<0);//����жϱ�־λ 
}		

void TIM3_PWM_Init(u16 arr,u16 psc)//arr���Զ���װֵ; psc��ʱ��Ԥ��Ƶ��
{		 					 
	RCC->APB1ENR|=1<<1; 	//TIM3ʱ��ʹ��  
	RCC->APB2ENR|=1<<2;		//IO�˿�A��ʱ��ʹ��
	RCC->APB2ENR|=1<<3;		//IO�˿�B��ʱ��ʹ��	
	GPIOA->CRL&=0X00FFFFFF;	//PA6,PA7
	GPIOA->CRL|=0XBB000000;	//���ù������ 	
	GPIOB->CRL&=0XFFFFFF00;	//PB0,PB1
	GPIOB->CRL|=0X000000BB;	//���ù������ 	
	
	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������
  
	TIM3->CCMR1|=0X0D<<3;  	//CH1���,PWM1ģʽ,Ԥװ��ʹ��			 
	TIM3->CCMR1|=0X0D<<11; 	//CH2���,PWM1ģʽ,Ԥװ��ʹ��
	TIM3->CCMR2|=0X0D<<3;  	//CH3���,PWM1ģʽ,Ԥװ��ʹ��			 
	TIM3->CCMR2|=0X0D<<11; 	//CH4���,PWM1ģʽ,Ԥװ��ʹ��
							
 	TIM3->CCER|=0X1111;   	//OC1,2,3,4 ���ʹ�ܣ��������Ϊ�ߵ�ƽ��Ч	   	   

	TIM3->CR1=0x0080;   	//���ϼ������Զ���װ��ʹ�� 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 
	
    TIM3->CCR1=3;    	//��ʼװ��ֵ
	TIM3->CCR2=10;    	//��ʼװ��ֵ
	TIM3->CCR3=10;    	//��ʼװ��ֵ
	TIM3->CCR4=99;    	//��ʼװ��ֵ
	//PWM���ģʽ1�£�ռ�ձȵļ��㷽��ΪCCR1/ARR*100%
	//PWM���ģʽ2�£�ռ�ձȵļ��㷽��Ϊ(1-CCR1/ARR)*100%
} 


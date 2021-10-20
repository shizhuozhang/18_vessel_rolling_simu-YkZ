#include "pwm.h"
#include "stdio.h"
#include "io.h"


void TIM1_PWM_Init(u16 arr,u16 psc)//arr：自动重装值; psc：时钟预分频数
{		 					 
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能  
	RCC->APB2ENR|=1<<2;		//IO端口A的时钟使能
	GPIOA->CRH&=0XFFFFFFF0;	//PA8,PA9,PA10,PA11
	GPIOA->CRH|=0X0000000B;	//复用功能输出 
	
	TIM1->ARR=arr;			//设定计数器自动重装值 
	TIM1->PSC=psc;			//预分频器设置
  
	TIM1->CCMR1|=0X0D<<3;  	//CH1输出,PWM1模式,预装载使能			 
//	TIM1->CCMR1|=0X0D<<11; 	//CH2输出,PWM1模式,预装载使能
//	TIM1->CCMR2|=0X0D<<3;  	//CH3输出,PWM1模式,预装载使能			 
//	TIM1->CCMR2|=0X0D<<11; 	//CH4输出,PWM1模式,预装载使能
							
 	TIM1->CCER|=0X0001;   	//OC1,2,3,4 输出使能，输出极性为高电平有效	   
	TIM1->BDTR|=1<<15;   	//高级定时器特有的寄存器，MOE 主输出使能	   

	TIM1->CR1=0x0080;   	//向上计数，自动重装载使能
	
	TIM1->DIER|=1<<0;//使能中断	
	MY_NVIC_Init(3,2,TIM1_UP_IRQn,2);
	TIM1->CR1|=0x01;    	//使能定时器1 
	
    TIM1->CCR1=22500;    	//初始装载值
//	TIM1->CCR2=5;    	//初始装载值
//	TIM1->CCR3=5;    	//初始装载值
//	TIM1->CCR4=500;    	//初始装载值
	//PWM输出模式1下，占空比的计算方法为CCR1/ARR*100%
	//PWM输出模式2下，占空比的计算方法为(1-CCR1/ARR)*100%
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
		TIM1->BDTR&=~(1<<15);   	//高级定时器特有的寄存器，MOE 主输出使能
		TIM1->CR1&=~(1<<0);    	//使能定时器1 
	}

}
//u16 count=0;
//u16 encoder=0;
//u8 flag=0;
u32 num=0;
void TIM1_UP_IRQHandler(void) //计数器中断
{
	if(TIM1->SR&0x0001)
	{
		if(num==0)
		{
			TIM1->BDTR&=~(1<<15);   	//高级定时器特有的寄存器，MOE 主输出使能
			TIM1->CR1&=~(1<<0);    	//使能定时器1 
		}else
			num--;
	}
	TIM1->SR&=~(1<<0);//清除中断标志位 
}		

void TIM3_PWM_Init(u16 arr,u16 psc)//arr：自动重装值; psc：时钟预分频数
{		 					 
	RCC->APB1ENR|=1<<1; 	//TIM3时钟使能  
	RCC->APB2ENR|=1<<2;		//IO端口A的时钟使能
	RCC->APB2ENR|=1<<3;		//IO端口B的时钟使能	
	GPIOA->CRL&=0X00FFFFFF;	//PA6,PA7
	GPIOA->CRL|=0XBB000000;	//复用功能输出 	
	GPIOB->CRL&=0XFFFFFF00;	//PB0,PB1
	GPIOB->CRL|=0X000000BB;	//复用功能输出 	
	
	TIM3->ARR=arr;			//设定计数器自动重装值 
	TIM3->PSC=psc;			//预分频器设置
  
	TIM3->CCMR1|=0X0D<<3;  	//CH1输出,PWM1模式,预装载使能			 
	TIM3->CCMR1|=0X0D<<11; 	//CH2输出,PWM1模式,预装载使能
	TIM3->CCMR2|=0X0D<<3;  	//CH3输出,PWM1模式,预装载使能			 
	TIM3->CCMR2|=0X0D<<11; 	//CH4输出,PWM1模式,预装载使能
							
 	TIM3->CCER|=0X1111;   	//OC1,2,3,4 输出使能，输出极性为高电平有效	   	   

	TIM3->CR1=0x0080;   	//向上计数，自动重装载使能 
	TIM3->CR1|=0x01;    	//使能定时器3 
	
    TIM3->CCR1=3;    	//初始装载值
	TIM3->CCR2=10;    	//初始装载值
	TIM3->CCR3=10;    	//初始装载值
	TIM3->CCR4=99;    	//初始装载值
	//PWM输出模式1下，占空比的计算方法为CCR1/ARR*100%
	//PWM输出模式2下，占空比的计算方法为(1-CCR1/ARR)*100%
} 


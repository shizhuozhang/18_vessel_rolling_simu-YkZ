/**
  * @attention
  *	IO口引脚配置文件
  * 一方面作为输出口，用于LED和蜂鸣器的驱动，一方面作为输入口，接收外部中断。
  * LED0作为系统运行指示灯，LED1为状态指示灯，BEEP为状态提醒
  * 
  ******************************************************************************
**/  

#include "IO.h"
#include "delay.h"

void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	GPIOA->CRL&=0X00FFFFFF;//IO状态设置
	GPIOA->CRL|=0X33000000;//IO状态设置 
	GPIOA->ODR|=3<<6;		//PA6,7
}



void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	GPIOA->CRH&=0XFFF0FFFF;//IO状态设置
	GPIOA->CRH|=0X00030000;//IO状态设置 
	GPIOA->ODR|=0<<12;		//PA12
}


void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	GPIOA->CRL&=0XFF00FF00;		  
	GPIOA->CRL|=0X00880088; //PA4.5设置成输入
	GPIOA->ODR|=1<<0;	//上拉
	GPIOA->ODR|=1<<1;	//上拉
	GPIOA->ODR|=1<<4;	//上拉
	GPIOA->ODR|=1<<5;	//上拉
} 	

//u8 KEY_Scan(void)
//{	 
////	if(KEY1==1||KEY2==0||KEY3==0||KEY4==0)
////	{
////		delay_ms(5);//去抖动 
////		if(KEY1==1)
////			return KEY1_PRES;
////		else if(KEY2==0)
////			return KEY2_PRES;
////		else if(KEY3==0)
////			return KEY3_PRES; 
////		else if(KEY4==0)
////			return KEY4_PRES; 
////	} 	     
////	return 0;// 无按键按下
//}


void ELSE_IO_Init(void)
{
	JTAG_Set(SWD_ENABLE);	//若使用到默认IO引脚，关闭JTAG,开启SWD
	
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	
	GPIOA->CRH&=0X0FFF0FFF;//IO状态设置
	GPIOA->CRH|=0X30003000;//IO状态设置 
	GPIOA->ODR&=~(1<<11);	//PA11默认低电平
	GPIOA->ODR&=~(1<<15);	//PA15默认低电平
	
	GPIOB->CRL&=0XFF0FFF00;//IO状态设置
	GPIOB->CRL|=0X00800088;//IO状态设置 
	GPIOB->ODR&=~(3<<0);	//PB0,1下拉
	GPIOB->ODR&=~(1<<5);	//PB5,下拉
	
	GPIOB->CRH&=0X000000FF;//IO状态设置
	GPIOB->CRH|=0X33338800;//IO状态设置 
	GPIOB->ODR&=~(3<<10);	//PB10,11下拉
	GPIOB->ODR&=~(0XF<<12);	//PB12,13,14,15输出低电平
}

//外部中断配置
void EXTIx_Init(void)		//直接调用sys.c对外部中断的配置
{
	Ex_NVIC_Config(GPIO_A,0,FTIR);//PA0，外部下降沿触发
	Ex_NVIC_Config(GPIO_A,1,FTIR);//PA1，外部下降沿触发	
	Ex_NVIC_Config(GPIO_A,4,FTIR);//PA4，外部下降沿触发
	Ex_NVIC_Config(GPIO_A,5,FTIR);//PA5，外部下降沿触发	
	Ex_NVIC_Config(GPIO_B,5,RTIR);//PB5，外部上升沿触发	
	MY_NVIC_Init(0,0,EXTI0_IRQn,2);//中断优先级设为最高
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);//中断优先级设为最高
	MY_NVIC_Init(1,0,EXTI4_IRQn,2);//中断优先级设为最高
	MY_NVIC_Init(1,1,EXTI9_5_IRQn,2);//中断优先级设为低 
}
//extern u8 t;
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	u16 i=0;
	for(i=0;i<0xffff;i++);//消抖8ms
	if(L_LIM==0)	//对应确认按键 
	{
//		MOTOR_DIR=1;
	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}
//外部中断1服务程序
void EXTI1_IRQHandler(void)
{			
	u16 i=0;
	for(i=0;i<0xffff;i++);//消抖8ms	
	
    if(R_LIM==0)	//对应取消按键
	{
		MOTOR_DIR=0;
	}
 	EXTI->PR=1<<1;     //清除LINE1上的中断标志位  
}
void EXTI4_IRQHandler(void)
{			
	u16 i=0;
	for(i=0;i<0xffff;i++);//消抖8ms	 
    if(KEY2==0)	//对应取消按键
	{
		LED1=~LED1;
//		num=60000;
		TIM1->BDTR|=1<<15;   	//高级定时器特有的寄存器，MOE 主输出使能
		TIM1->CR1=1<<0;    	//使能定时器1
	}
 	EXTI->PR=1<<4;     //清除LINE1上的中断标志位  
}


void EXTI9_5_IRQHandler(void)
{
	u16 i=0;
	for(i=0;i<0xffff;i++);//消抖8ms
    if(KEY3==0)	//对应取消按键
	{		
		TIM1->BDTR&=~(1<<15);   	//高级定时器特有的寄存器，MOE 主输出使能
		TIM1->CR1&=~(1<<0);    	//使能定时器1 
	}
 	EXTI->PR=1<<5;     //清除LINE1上的中断标志位 
}




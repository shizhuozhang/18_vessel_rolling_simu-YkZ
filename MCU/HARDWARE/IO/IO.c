/**
  * @attention
  *	IO�����������ļ�
  * һ������Ϊ����ڣ�����LED�ͷ�������������һ������Ϊ����ڣ������ⲿ�жϡ�
  * LED0��Ϊϵͳ����ָʾ�ƣ�LED1Ϊ״ָ̬ʾ�ƣ�BEEPΪ״̬����
  * 
  ******************************************************************************
**/  

#include "IO.h"
#include "delay.h"

void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	GPIOA->CRL&=0X00FFFFFF;//IO״̬����
	GPIOA->CRL|=0X33000000;//IO״̬���� 
	GPIOA->ODR|=3<<6;		//PA6,7
}



void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	GPIOA->CRH&=0XFFF0FFFF;//IO״̬����
	GPIOA->CRH|=0X00030000;//IO״̬���� 
	GPIOA->ODR|=0<<12;		//PA12
}


void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0XFF00FF00;		  
	GPIOA->CRL|=0X00880088; //PA4.5���ó�����
	GPIOA->ODR|=1<<0;	//����
	GPIOA->ODR|=1<<1;	//����
	GPIOA->ODR|=1<<4;	//����
	GPIOA->ODR|=1<<5;	//����
} 	

//u8 KEY_Scan(void)
//{	 
////	if(KEY1==1||KEY2==0||KEY3==0||KEY4==0)
////	{
////		delay_ms(5);//ȥ���� 
////		if(KEY1==1)
////			return KEY1_PRES;
////		else if(KEY2==0)
////			return KEY2_PRES;
////		else if(KEY3==0)
////			return KEY3_PRES; 
////		else if(KEY4==0)
////			return KEY4_PRES; 
////	} 	     
////	return 0;// �ް�������
//}


void ELSE_IO_Init(void)
{
	JTAG_Set(SWD_ENABLE);	//��ʹ�õ�Ĭ��IO���ţ��ر�JTAG,����SWD
	
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	
	GPIOA->CRH&=0X0FFF0FFF;//IO״̬����
	GPIOA->CRH|=0X30003000;//IO״̬���� 
	GPIOA->ODR&=~(1<<11);	//PA11Ĭ�ϵ͵�ƽ
	GPIOA->ODR&=~(1<<15);	//PA15Ĭ�ϵ͵�ƽ
	
	GPIOB->CRL&=0XFF0FFF00;//IO״̬����
	GPIOB->CRL|=0X00800088;//IO״̬���� 
	GPIOB->ODR&=~(3<<0);	//PB0,1����
	GPIOB->ODR&=~(1<<5);	//PB5,����
	
	GPIOB->CRH&=0X000000FF;//IO״̬����
	GPIOB->CRH|=0X33338800;//IO״̬���� 
	GPIOB->ODR&=~(3<<10);	//PB10,11����
	GPIOB->ODR&=~(0XF<<12);	//PB12,13,14,15����͵�ƽ
}

//�ⲿ�ж�����
void EXTIx_Init(void)		//ֱ�ӵ���sys.c���ⲿ�жϵ�����
{
	Ex_NVIC_Config(GPIO_A,0,FTIR);//PA0���ⲿ�½��ش���
	Ex_NVIC_Config(GPIO_A,1,FTIR);//PA1���ⲿ�½��ش���	
	Ex_NVIC_Config(GPIO_A,4,FTIR);//PA4���ⲿ�½��ش���
	Ex_NVIC_Config(GPIO_A,5,FTIR);//PA5���ⲿ�½��ش���	
	Ex_NVIC_Config(GPIO_B,5,RTIR);//PB5���ⲿ�����ش���	
	MY_NVIC_Init(0,0,EXTI0_IRQn,2);//�ж����ȼ���Ϊ���
	MY_NVIC_Init(0,0,EXTI1_IRQn,2);//�ж����ȼ���Ϊ���
	MY_NVIC_Init(1,0,EXTI4_IRQn,2);//�ж����ȼ���Ϊ���
	MY_NVIC_Init(1,1,EXTI9_5_IRQn,2);//�ж����ȼ���Ϊ�� 
}
//extern u8 t;
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	u16 i=0;
	for(i=0;i<0xffff;i++);//����8ms
	if(L_LIM==0)	//��Ӧȷ�ϰ��� 
	{
//		MOTOR_DIR=1;
	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{			
	u16 i=0;
	for(i=0;i<0xffff;i++);//����8ms	
	
    if(R_LIM==0)	//��Ӧȡ������
	{
		MOTOR_DIR=0;
	}
 	EXTI->PR=1<<1;     //���LINE1�ϵ��жϱ�־λ  
}
void EXTI4_IRQHandler(void)
{			
	u16 i=0;
	for(i=0;i<0xffff;i++);//����8ms	 
    if(KEY2==0)	//��Ӧȡ������
	{
		LED1=~LED1;
//		num=60000;
		TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
	}
 	EXTI->PR=1<<4;     //���LINE1�ϵ��жϱ�־λ  
}


void EXTI9_5_IRQHandler(void)
{
	u16 i=0;
	for(i=0;i<0xffff;i++);//����8ms
    if(KEY3==0)	//��Ӧȡ������
	{		
		TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
	}
 	EXTI->PR=1<<5;     //���LINE1�ϵ��жϱ�־λ 
}




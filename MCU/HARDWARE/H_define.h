#ifndef __H_DEFINE_H 
#define __H_DEFINE_H 

#include "sys.h"
#include "stdio.h"
#include  "math.h"  

/**************************���ܿ�**************************/

/**********��ʱ********/
#define		__DELAY  		1
#if __DELAY
	#include "delay.h"
#endif

/*************����*************/

#define		__USART  		0	

#if __USART
/*ע��Printf����ʱ�ܳ�������1��Ƶ72MHz�����������ֽ�֮���ʱ���Լ��2.5ms
����2��3����Ƶ��36MHz��ʱ���Լ��5ms,��ˣ������ж�ʱ��ܳ�������Ҫ���ж���ʹ��*/
	#define 	___USART_PRINTF	2				//��printf���������Ĵ��ں�,
												//ע��Ҫ������ʹ�ܵĴ���ƥ��
	#define 	__USART1 		1				//����1
	#define 	__USART2		1				//����2
												//ע�⴮��2��IO�ںͰ�����ͻ
	#define 	__USART3		0				//����3
	#include 	"usart.h"
	#include "DMA_USART.h"
#endif



/************IO��**************/
#define		__IO_PORT  		0
#if __IO_PORT
	#define		__LED  		1
	#define		__BEEP  	1
	#define		__KEY  		1		//������IO�ںʹ���2��ͻ
	#define		__EXTI		1
	#define		__ELSE  	1		//���������õ�IO�ڵ�����
	#include "IO.h"
#endif

/**************TIMER************/
#define		__TIM  		0
#if __TIM
	#define __SETITIM1		0       //TIM1��ʱ���ж�
	#define __SETITIM2		1       //TIM2��ʱ���ж�
	#define __SETITIM3		0       //TIM3��ʱ���ж�
	#define __SETITIM4		0       //TIM4��ʱ���ж�
	#define __SETITIM5		0       //TIM5��ʱ���ж�
	#define __SETITIM6		0       //TIM6��ʱ���ж�
	#define __SETITIM7		0       //TIM7��ʱ���ж�
	#define __SETITIM8		0       //TIM8��ʱ���ж�
	#include "timer.h"
#endif

/**************PWMOUT************/
#define		__PWMOUT 		0
#if __PWMOUT
	#define __TIM1_PWM		1   //TIM1��PWM�����//��ʱ��1��2��3ͨ���ʹ���1�����ų�ͻ											  
	#define __TIM2_PWM		0   //TIM2��PWM�����//TIM2�����ź�TIM5��ͻ��ʹ����ӳ������
//��ӳ�����ź�SWD���ų�ͻPB3������Jlink��ⲻ��оƬ
//�����ʹ��CH1��CH2������ʱҪ����״̬����
	#define __TIM3_PWM		0   //TIM3��PWM�����
	#define __TIM4_PWM		0   //TIM4��PWM�����											  
	#define __TIM5_PWM		0   //TIM5��PWM�����//ע��TIM5���źͰ��������Լ�����2��ͻ											  
	#define __TIM8_PWM		0  //TIM8��PWM�����
	#include "pwm.h"
#endif



/**************ENCODER************/
#define		__TIM_ENCODER 		0
#if __TIM_ENCODER
	#define __ENCODER1		0
	#define __ENCODER2		0
//��ӳ�����ź�SWD���ų�ͻPB3������Jlink��ⲻ��оƬ
//�����ʹ��CH1��CH2������ʱҪ����״̬����
	#define __ENCODER3		0
	#define __ENCODER4		1
	#define __ENCODER5		0
	#define __ENCODER8		0
	#include  "Encoder.h"

#endif
/******************ADC*********************/
//adcʹ��ʱע�⣺ADC��ͨ����ͨ�õ�ADC�����һ��ͨ�����ӵ���оƬ�ڲ����¶ȴ�����
//adc2�����ݴ����ADC1->DR�У����Բ�Ӧ�õ���ʹ��ADC2
#define __ADC  0
#if __ADC
	#define __ADC1   	0
	#define __ADC2      0
	#define __ADC3      0
	#include "adc.h"
	#define __DMA_ADC   0
	#if __DMA_ADC
	//adc_dma ע��  adc2�����ݷ���ADC1��DR�У�����û�е�����dmaͨ��
	//���ʹ��adc2��һ�����dma�жϣ���adc1��adc2�����ݷֿ�
		#define __DMA_ADC1   0
		#define __DMA_ADC3   0
		#include "dma_adc.h"
	#endif
#endif 


/**************************ͨ���ⲿ��������**************************/

//extern 	u8 uc_T1;		//����ʱ��,
//extern	s16 si_Z1;		//�����������
//extern 	u8 uc_T2;		//����ʱ��,
//extern	s16 si_Z2;		//�����������
extern u8 Tim_flag;
extern u8 Dma1_C3_Close;		//DMA1Channel3�ر��ź�
//extern u8 Dma1_C6_Close;		//DMA1Channel6�ر��ź�

///////////////////////////////////////////////////////////////////////////////////
/**********************����ʹ�ܳ�ʼ����������**************************/

void Init1(void);
void Init2(void);

#endif





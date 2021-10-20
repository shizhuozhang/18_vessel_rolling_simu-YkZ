#include "H_define.h"

void Init1(void)
{
    Stm32_Clock_Init(9); //ϵͳʱ������
	
	////////////��ʱ//////////////
	#if __DELAY
		delay_init(72);	     //��ʱ��ʼ��
	#endif
	/////////////////////////////
	
	////////////����//////////////
	#if __USART1
		usart1(72,115200);  //���ڳ�ʼ�� 
	#endif
	
	#if __USART2
		usart2(36,115200);  //���ڳ�ʼ�� 
//		DMA1_Channel6_Init(11,(u32)&USART2->DR,(u32)D_6050);

	#endif
	
	#if __USART3
		usart3(36,100000);  //���ڳ�ʼ�� 
		DMA1_Channel3_Init(25,(u32)&USART3->DR,(u32)SBUS_DATA);
	#endif
	
	/////////////////////////////
	
	////////////LED//////////////
	#if __LED
		LED_Init();
	#endif
	#if __BEEP
		BEEP_Init();
	#endif
	#if __KEY
		KEY_Init();
	#endif
	#if __EXTI
		EXTIx_Init();
	#endif
	#if __ELSE
		ELSE_IO_Init();
	#endif

	/////////////////////////////
	/*����������ϵͳʱ�ӷ�Ƶϵ��ʱ,APB1����Ƶ��APB2һ��Ƶ��AHB1��Ƶ
	�����еĶ�ʱ��1-8��ʱ��Ƶ�ʶ���72MHz*/
/*ע��Printf����ʱ�ܳ�������1��Ƶ72MHz�����������ֽ�֮���ʱ���Լ��2.5ms
	����2��3����Ƶ��36MHz��ʱ���Լ��5ms,��ˣ������ж�ʱ��ܳ�������Ҫ���ж���ʹ��*/
	////////////TIM//////////////
	#if __SETITIM1							//TIM1�������ж�
		TIM1_Init(9,71);
	#endif
	#if __SETITIM2							//TIM2�������ж�
		TIM2_Init(4999,7199);
	#endif
	#if __SETITIM3							//TIM3�������ж�
		TIM3_Init(99,35999);
	#endif
	#if __SETITIM4							//TIM4�������ж�
		TIM4_Init(9999,7199);
	#endif
	#if __SETITIM5							//TIM5�������ж�
		TIM5_Init(999,7199);
	#endif
	#if __SETITIM6							//TIM6�������ж�
		TIM6_Init(99,71);  					//10K����������Ƶ��
	#endif
	#if __SETITIM7							//TIM7�������ж�
		TIM7_Init(99,71); 					//10K����������Ƶ��
	#endif
	#if __SETITIM8							//TIM8�������ж�
		TIM8_Init(9999,7199);
	#endif
	/////////////////////////////
	
	/*********PWMOUT***********/
	//(3600,2)10KHz;;(3600,20)1KHz;;(7200,200)50Hz
	#if __TIM1_PWM
		TIM1_PWM_Init(3600-1,20-1);
	#endif
	#if __TIM2_PWM
		TIM2_PWM_Init(7199,9999);
	#endif
	#if __TIM3_PWM
		TIM3_PWM_Init(199,7199);
	#endif
	#if __TIM4_PWM
		TIM4_PWM_Init(3600-1,20-1);
	#endif
	#if __TIM5_PWM
		TIM5_PWM_Init(3600-1,20-1);
	#endif
	#if __TIM8_PWM
		TIM8_PWM_Init(3599,1);		//10K�������Ƶ��K
	#endif
	
	/*********TIM_ENCODER***********/
	#if __ENCODER1
		Encoder1_Init();
	#endif
	#if __ENCODER2
		Encoder2_Init();
	#endif
	#if __ENCODER3
		Encoder3_Init();
	#endif
	#if __ENCODER4
		Encoder4_Init();
	#endif
	#if __ENCODER5
		Encoder5_Init();
	#endif
	#if __ENCODER8
		Encoder8_Init();
	#endif
	

}

void Init2(void)
{
}
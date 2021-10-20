#include "H_define.h"

void Init1(void)
{
    Stm32_Clock_Init(9); //系统时钟设置
	
	////////////延时//////////////
	#if __DELAY
		delay_init(72);	     //延时初始化
	#endif
	/////////////////////////////
	
	////////////串口//////////////
	#if __USART1
		usart1(72,115200);  //串口初始化 
	#endif
	
	#if __USART2
		usart2(36,115200);  //串口初始化 
//		DMA1_Channel6_Init(11,(u32)&USART2->DR,(u32)D_6050);

	#endif
	
	#if __USART3
		usart3(36,100000);  //串口初始化 
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
	/*由于在设置系统时钟分频系数时,APB1两分频，APB2一分频，AHB1分频
	故所有的定时器1-8的时钟频率都是72MHz*/
/*注意Printf语句耗时很长，串口1主频72MHz，发送两个字节之间的时间大约是2.5ms
	串口2，3，主频是36MHz，时间大约是5ms,因此，除非中断时间很长，否则不要在中断中使用*/
	////////////TIM//////////////
	#if __SETITIM1							//TIM1计数器中断
		TIM1_Init(9,71);
	#endif
	#if __SETITIM2							//TIM2计数器中断
		TIM2_Init(4999,7199);
	#endif
	#if __SETITIM3							//TIM3计数器中断
		TIM3_Init(99,35999);
	#endif
	#if __SETITIM4							//TIM4计数器中断
		TIM4_Init(9999,7199);
	#endif
	#if __SETITIM5							//TIM5计数器中断
		TIM5_Init(999,7199);
	#endif
	#if __SETITIM6							//TIM6计数器中断
		TIM6_Init(99,71);  					//10K编码器驱动频率
	#endif
	#if __SETITIM7							//TIM7计数器中断
		TIM7_Init(99,71); 					//10K编码器驱动频率
	#endif
	#if __SETITIM8							//TIM8计数器中断
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
		TIM8_PWM_Init(3599,1);		//10K电机驱动频率K
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

#ifndef __H_DEFINE_H 
#define __H_DEFINE_H 

#include "sys.h"
#include "stdio.h"
#include  "math.h"  

/**************************功能块**************************/

/**********延时********/
#define		__DELAY  		1
#if __DELAY
	#include "delay.h"
#endif

/*************串口*************/

#define		__USART  		0	

#if __USART
/*注意Printf语句耗时很长，串口1主频72MHz，发送两个字节之间的时间大约是2.5ms
串口2，3，主频是36MHz，时间大约是5ms,因此，除非中断时间很长，否则不要在中断中使用*/
	#define 	___USART_PRINTF	2				//与printf函数关联的串口号,
												//注意要与下面使能的串口匹配
	#define 	__USART1 		1				//串口1
	#define 	__USART2		1				//串口2
												//注意串口2的IO口和按键冲突
	#define 	__USART3		0				//串口3
	#include 	"usart.h"
	#include "DMA_USART.h"
#endif



/************IO口**************/
#define		__IO_PORT  		0
#if __IO_PORT
	#define		__LED  		1
	#define		__BEEP  	1
	#define		__KEY  		1		//按键的IO口和串口2冲突
	#define		__EXTI		1
	#define		__ELSE  	1		//其他可能用到IO口的配置
	#include "IO.h"
#endif

/**************TIMER************/
#define		__TIM  		0
#if __TIM
	#define __SETITIM1		0       //TIM1计时器中断
	#define __SETITIM2		1       //TIM2计时器中断
	#define __SETITIM3		0       //TIM3计时器中断
	#define __SETITIM4		0       //TIM4计时器中断
	#define __SETITIM5		0       //TIM5计时器中断
	#define __SETITIM6		0       //TIM6计时器中断
	#define __SETITIM7		0       //TIM7计时器中断
	#define __SETITIM8		0       //TIM8计时器中断
	#include "timer.h"
#endif

/**************PWMOUT************/
#define		__PWMOUT 		0
#if __PWMOUT
	#define __TIM1_PWM		1   //TIM1，PWM波输出//定时器1的2，3通道和串口1的引脚冲突											  
	#define __TIM2_PWM		0   //TIM2，PWM波输出//TIM2的引脚和TIM5冲突，使用重映射引脚
//重映射引脚和SWD引脚冲突PB3，导致Jlink检测不到芯片
//因此若使用CH1，CH2，下载时要重启状态下载
	#define __TIM3_PWM		0   //TIM3，PWM波输出
	#define __TIM4_PWM		0   //TIM4，PWM波输出											  
	#define __TIM5_PWM		0   //TIM5，PWM波输出//注意TIM5引脚和按键引脚以及串口2冲突											  
	#define __TIM8_PWM		0  //TIM8，PWM波输出
	#include "pwm.h"
#endif



/**************ENCODER************/
#define		__TIM_ENCODER 		0
#if __TIM_ENCODER
	#define __ENCODER1		0
	#define __ENCODER2		0
//重映射引脚和SWD引脚冲突PB3，导致Jlink检测不到芯片
//因此若使用CH1，CH2，下载时要重启状态下载
	#define __ENCODER3		0
	#define __ENCODER4		1
	#define __ENCODER5		0
	#define __ENCODER8		0
	#include  "Encoder.h"

#endif
/******************ADC*********************/
//adc使用时注意：ADC的通道是通用的ADC的最后一个通道连接的是芯片内部的温度传感器
//adc2的数据存放在ADC1->DR中，所以不应该单独使用ADC2
#define __ADC  0
#if __ADC
	#define __ADC1   	0
	#define __ADC2      0
	#define __ADC3      0
	#include "adc.h"
	#define __DMA_ADC   0
	#if __DMA_ADC
	//adc_dma 注意  adc2的数据放在ADC1的DR中，所以没有单独的dma通道
	//如果使用adc2，一般会用dma中断，将adc1和adc2的数据分开
		#define __DMA_ADC1   0
		#define __DMA_ADC3   0
		#include "dma_adc.h"
	#endif
#endif 


/**************************通用外部变量定义**************************/

//extern 	u8 uc_T1;		//最终时长,
//extern	s16 si_Z1;		//最终脉冲个数
//extern 	u8 uc_T2;		//最终时长,
//extern	s16 si_Z2;		//最终脉冲个数
extern u8 Tim_flag;
extern u8 Dma1_C3_Close;		//DMA1Channel3关闭信号
//extern u8 Dma1_C6_Close;		//DMA1Channel6关闭信号

///////////////////////////////////////////////////////////////////////////////////
/**********************外设使能初始化函数声明**************************/

void Init1(void);
void Init2(void);

#endif





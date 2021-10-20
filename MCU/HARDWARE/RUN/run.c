#include "delay.h"
#include "stdio.h"
#include "timer.h"
#include "pwm.h"
#include "IO.h"
#include  "Encoder.h"
#include "run.h"

void Find_Center(void)
{
	TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
	TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
	MOTOR_DIR=RIGHT;
	num=0xffff;
	while(1)
	{
		if(R_LIM==0)
		{
			delay_ms(1);
			if(R_LIM==0)
				break;
		}
	}
	num=0;
	TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
	TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1 
	delay_ms(500);
	Roll_num=0;
	TIM4->CNT=0;
	TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
	TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
	MOTOR_DIR=LEFT;	
	num=0xffff;
	while(1)
	{
		if(L_LIM==0)
		{
			delay_ms(1);
			if(L_LIM==0)
				break;
		}
	}
	num=0;
	TIM1->BDTR&=~(1<<15);   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
	TIM1->CR1&=~(1<<0);    	//ʹ�ܶ�ʱ��1
	delay_ms(500);
	TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
	TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
	MOTOR_DIR=RIGHT;
	num=-TIM4->CNT/2-Roll_num*5000;
	while(num);
	delay_ms(500);
	Roll_num=0;
	TIM4->CNT=0;
}

void Center_Again(void)
{
	if(Roll_num==0)
	{		
		Set_Speed(3000);
		MOTOR_DIR=LEFT;
		num=TIM4->CNT;
		TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
		
	}else if(Roll_num==-1)
	{
		Set_Speed(3000);
		MOTOR_DIR=RIGHT;
		num=9999-TIM4->CNT;
		TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1
		
	}else
	{
		Set_Speed(5000);
		Find_Center();//5Khz���ٶ���ԭ��
	}
}

void Set_Exp(float time,float position)
{
	s32 step=0;
	u32 freq=0;
	u32 temp=time*1000;
	float delt_time=time-Time/1000.0;
	step=position*1000-(Roll_num*10000+TIM4->CNT);//���һת����10000���������ֵ����Ӧʵ�ʾ���10CM����1cm��Ӧ1000������ֵ��
												//��λ�����ص�����cmΪ��λ��ת��Ϊ����ֵ��1000	
	if(step>0) 
	{
		freq=step/delt_time+0.5;
		Set_Speed(freq);
		MOTOR_DIR=RIGHT;
		num=step;
		TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1	
	}else
	{
		freq=-step/delt_time+0.5;
		Set_Speed(freq);
		MOTOR_DIR=LEFT;
		num=-step;
		TIM1->BDTR|=1<<15;   	//�߼���ʱ�����еļĴ�����MOE �����ʹ��
		TIM1->CR1=1<<0;    	//ʹ�ܶ�ʱ��1		
	}
//	if((time<3))
//		printf("%d %d\r\n",step,freq);
//	if(temp<3000)
//		printf("%d %f %d %d,%d,%d\r\n",step,delt_time,Time,freq,num,temp);
	while(num>0&&Time<temp);
//	printf("%d %d\r\n",num,Time);
//	while(Time<temp)
//	{
//		if((temp-Time)<500)//0.5s
//		{
//			if(num>freq*0.5+50)
//			{
//				Set_Speed(2*num);//���µ�Ƶ���£�0.5S����num�����������Ƶ����2*num
//			}
//		}
//	}
//	printf("%d\r\n",Time);
}


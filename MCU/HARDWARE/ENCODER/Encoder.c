#include  "Encoder.h"
#include  "IO.h"

void Encoder4_Init(void)					////TIM4用于编码器模式
{
	RCC->APB1ENR|=1<<2;//tim4 enable
	RCC->APB2ENR|=1<<3;//GPIOB enable
    GPIOB->CRL&=0x00ffffff;
    GPIOB->CRL|=0x44000000;
	TIM4->ARR=9999;
	TIM4->CNT=0;
	TIM4->SMCR|=3<<0;//编码器模式3
	TIM4->CCMR1|=1<<0;//输入通道映射
	TIM4->CCMR1|=1<<8;//输入通道映射
	
	TIM4->DIER|=1<<0;//使能中断	
	MY_NVIC_Init(0,2,TIM4_IRQn,2);
	
	TIM4->CR1|=1<<0;//使能计数器
}

s8 Roll_num=0;
void TIM4_IRQHandler(void)
{
	if(TIM4->SR&0x0001)
	{
//		LED1=~LED1;	
		if(TIM4->CNT>=5000)			
			Roll_num-=1;
		else
			Roll_num+=1;
	}
	TIM4->SR&=~(1<<0);//清除中断标志位 
}
/*
入口参数
Line_num 编码器分辨率，即线数
X_KHz 定时器频率
用在定时器中断中
注意修改编码器所使用的定时器TIMx->CNT
*/
s16 M_T_Speed_measure(u16 Line_num,u16 X_KHz)//   	M/T测速算法
{
	static u8 Tc=100;   //固定时长
	static u8 t1=0 ;	//第一个脉冲前时长
    static u8 t2=0;	//最后一个脉冲前时长	   
	static u8 T=0;	//最终时长,
	static u16 cntL=0;	//编码器过去值
	static s16 Z=0;		//最终脉冲个数
	
	if(cntL==TIM4->CNT)
	{
		if(Tc==100)
		{
			t2++;					//最后一个脉冲前时长计数	
			if(t2>50)				//超时处理
			{
				T=Tc-t1+t2-1;
				Z=cntL-32760;			//编码器重装值是100
				Tc=0;
				t1=0;
				t2=0;
				TIM4->CNT=32760;		//编码器重装值是100
				cntL=TIM4->CNT;
			}
		}
		else
		{
			t1++;					//第一个脉冲前时长计数	
		}
	}
	else
	{
		if(Tc==100)					//一次计时结束
		{
			T=Tc-t1+t2;
			Z=cntL-32760;
			Tc=0;
			t1=0;
			t2=0;
			TIM4->CNT=32760;
			cntL=TIM4->CNT;
		}
	}
	if(Tc<100)		//固定时长计数
	{
		Tc++;
		if(Tc==100)
		{
			cntL=TIM4->CNT;
		}
	}
	return (s16)(60*1000*X_KHz*Z/4/Line_num/(float)T);//算出的速度单位是r/min
			/*			      60*Z*（定时器频率f）
			//最终速度为     --------------------   （r/min）
							 （4*编码器分辨率）*T
			*/
}

/*测试直流电机的机电时间常数的两种数字方法，
注意修改编码器所使用的定时器TIMx->CNT
*/
	/*方法1是使用1KHz的定时中断采样速度，然后画出速度时间曲线，
	0.632倍的最大速度对应的时间即为机电时间常数*/
////			if(Count_num>3000)
////			{
////				Count_num=0;
////				LED0=~LED0;
////				IO_PC4=1;
////				y=1;
////			}
////			
////			if(y==1)
////			{
////				if(i<800)
////				{
////					i++;
////					Speed_data[i]=TIM4->CNT-32760;
////					TIM4->CNT=32760;
////				}
////				else Print_Flag=1;
////			
////			}
	/*方法2
	使用时，中断定时频率是10HZ,测速所用编码器线数至少要300线
	这种计算电机机电时间常数的算法是通过测量电机在响应阶跃输入
	的第一个100MS内脉冲个数和第二100MS内的脉冲个数间接推算的*/
//	      s16 Z1=0;//外部全局变量
//	      s16 Z2=0;	//外部全局变量
//中断函数中内容
//	static u8 x=2,y=0;
//		if(TIM7->SR&0x0001)
//		{
//			if(y==1)
//			{
//				if(x==1)
//				{
//					Z2=TIM4->CNT-32760;
//					TIM4->CNT=32760;
//					x--;
//					Print_Flag=1;
//				}	
//				else if(x==2)
//				{
//					Z1=TIM4->CNT-32760;
//					TIM4->CNT=32760;
//					x--;
//				}
//				else if(x==0)
//					x=0;
//									
//			}
//			Count_num++;
//			if(Count_num>30)
//			{
//				Count_num=0;
//				LED0=~LED0;
//				IO_PC4=1;
//				y=1;
//			}
//			delay_ms(2);

			
	/*			        Z1(第一个100ms)
	//最终速度为 T -  --------------------  (单位是s)，
					  10*Z2（第二个100ms）
	  
	由于电机的机电时间常数一般在10ms左右，故测量的时间选取10倍（远大于）即0.1S
	使用该封装的函数时，中断定时频率必须是1KHZ，才能保证T=0.1S	
			*/














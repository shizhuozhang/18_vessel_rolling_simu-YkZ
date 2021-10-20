#include "usart.h"	
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "queue.h"
#include "timer.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////


//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
void usart1(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
	USART1->CR3|=1<<6;    //接收DMA
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
}



//cndtr:数据数量
//cpar:外设地址
//cmar:存储地址
void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar)//串口2 dam数据传输  通道6
{
	RCC->AHBENR|=1<<0;			//开启DMA1时钟
	delay_ms(5);				//等待DMA时钟稳定
	DMA1_Channel6->CPAR=cpar; 	 	//DMA1 外设地址 
	DMA1_Channel6->CMAR=cmar; 	//DMA1,存储器地址
	DMA1_Channel6->CNDTR=cndtr;    	//DMA1,传输数据量
	DMA1_Channel6->CCR=0X00000000;	//复位
	DMA1_Channel6->CCR|=0<<4;  		//从外设读
	DMA1_Channel6->CCR|=1<<5;  		//普通模式
	DMA1_Channel6->CCR|=0<<6; 		//外设地址非增量模式
	DMA1_Channel6->CCR|=1<<7; 	 	//存储器增量模式
	DMA1_Channel6->CCR|=0<<8; 	 	//外设数据宽度为8位
	DMA1_Channel6->CCR|=0<<10; 		//存储器数据宽度8位
	DMA1_Channel6->CCR|=3<<12; 		//最高等优先级
	DMA1_Channel6->CCR|=0<<14; 		//非存储器到存储器模式
	DMA1_Channel6->CCR|=1<<1;			//允许TC中断
	DMA1_Channel6->CCR|=1<<0;			//通道开启
	MY_NVIC_Init(3,2,DMA1_Channel6_IRQn,2);
}

u8 lstatue=STOP;//上次上位机状态
u8 statue=STOP;//上位机状态
//bit7 通道关闭
u8 SIGN_LEC=0,Zero_flag=0;
u16 sum=0;
u_SendData SendData,RXData;
void  DMA1_Channel6_IRQHandler()//dma1中断  usart3
{
	if(DMA1->ISR&(1<<21))//传输完成
	{
		DMA1->IFCR|=1<<20;//清除标志位
		if(0xfe==RXData.ca_SendData[2]&&0xef==RXData.ca_SendData[3])//帧头校验
		{
			sum=(u16)RXData.ca_SendData[2]+RXData.ca_SendData[3]+RXData.ca_SendData[4]+RXData.ca_SendData[5]+RXData.ca_SendData[6]+RXData.ca_SendData[7]+RXData.ca_SendData[8]+RXData.ca_SendData[9]+RXData.ca_SendData[10]+RXData.ca_SendData[11]+RXData.ca_SendData[12];//校验数据计算
			if((sum&0xff)==RXData.s_SendData.c_Verify)//校验值比较
			{
				switch(RXData.s_SendData.c_Order)
				{
					case STOP:
						lstatue=statue;
						statue=STOP;
					break;
					case PAUSE:
						lstatue=statue;
						statue=PAUSE;
					break;
					case RUNNING: //入队
						//在while里更新状态标志
						enqueue(RXData.s_SendData.u_PointData.s_PointData.f_Time,RXData.s_SendData.u_PointData.s_PointData.f_Position);
					break;
					case STARTRUNNING:
						if(lstatue==STOP&&Pause_Flag==1)
						{
							lstatue=RUNNING;
							statue=STOP;
						}else
						{
							lstatue=statue;
							statue=RUNNING;
						}
					break;
					case ZERO:
						Zero_flag=1;
						break;
					default:
						
					break;
				}
				
			}else
			{
				DMA1_Channel6->CCR&=~(1<<0);			//通道关闭
				SIGN_LEC|=1<<7;		//通道关闭标记
			}
		}else
		{
			DMA1_Channel6->CCR&=~(1<<0);			//通道关闭
			SIGN_LEC|=1<<7;			//通道关闭标记
		}
	}	
}


u8 res=0;
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))//接收到数据
	{
		res=USART2->DR;
		if((SIGN_LEC&1<<7)&&(0Xfe==res))	//检验第一个字节
		{
			SIGN_LEC&=0x7f;				//通道开启标记
			DMA1_Channel6->CCR|=1<<0;			//通道开启
		}	
	}
}





//cndtr:数据数量
//cpar:外设地址
//cmar:存储地址
void DMA1_Channel7_Init(u16 cndtr,u32 cpar,u32 cmar)//串口2 dam数据传输  通道7
{
	RCC->AHBENR|=1<<0;			//开启DMA1时钟
	delay_ms(5);				//等待DMA时钟稳定
	DMA1_Channel7->CPAR=cpar; 	 	//DMA1 外设地址 
	DMA1_Channel7->CMAR=cmar; 	//DMA1,存储器地址
	DMA1_Channel7->CNDTR=cndtr;    	//DMA1,传输数据量
	DMA1_Channel7->CCR=0X00000000;	//复位
	DMA1_Channel7->CCR|=1<<4;  		//从存储器读
	DMA1_Channel7->CCR|=0<<5;  		//不执行循环操作
	DMA1_Channel7->CCR|=0<<6; 		//外设地址非增量模式
	DMA1_Channel7->CCR|=1<<7; 	 	//存储器增量模式
	DMA1_Channel7->CCR|=0<<8; 	 	//外设数据宽度为8位
	DMA1_Channel7->CCR|=0<<10; 		//存储器数据宽度8位
	DMA1_Channel7->CCR|=2<<12; 		//高等优先级
	DMA1_Channel7->CCR|=0<<14; 		//非存储器到存储器模式
	DMA1_Channel7->CCR|=1<<1;			//允许TC中断
//	DMA1_Channel7->CCR|=1<<0;			//通道开启
	MY_NVIC_Init(3,1,DMA1_Channel7_IRQn,2);
}

u8 UATR2_COMP=0;
void  DMA1_Channel7_IRQHandler()//dma1中断  usart2
{
	
	if(DMA1->ISR&(1<<25))//传输完成
	{
		DMA1->IFCR|=1<<24;//清除标志位
		UATR2_COMP=0;
	}
}



void usart2(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
	mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF;//IO状态设置
	GPIOA->CRL|=0X00008B00;//IO状态设置 
	RCC->APB1RSTR|=1<<17;   //复位串口1
	RCC->APB1RSTR&=~(1<<17);//停止复位
	//波特率设置
	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	USART2->CR3|=1<<6;	//dma使能接受
	USART2->CR3|=1<<7;	//dma使能发送
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
}

//void USART2_IRQHandler(void)
//{
//	if(USART2->SR&(1<<5))//接收到数据
//	{	 
//		USART2->SR&=~(1<<5);	
////			if(0X55==USART2->DR&&1==Dma1_C6_Close)
////			{
////				Dma1_C6_Close=0;
////				DMA1_Channel6->CCR|=1<<0;			//通道开启
////			}
//	}

//} 
//	//cndtr:数据数量
////cpar:外设地址
////cmar:存储地址
//void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar)   //usart2
//{
//	RCC->AHBENR|=1<<0;			//开启DMA1时钟
//	delay_ms(5);				//等待DMA时钟稳定
//	DMA1_Channel6->CPAR=cpar; 	 	//DMA1 外设地址 
//	DMA1_Channel6->CMAR=cmar; 	//DMA1,存储器地址
//	DMA1_Channel6->CNDTR=cndtr;    	//DMA1,传输数据量
//	DMA1_Channel6->CCR=0X00000000;	//复位
//	DMA1_Channel6->CCR|=0<<4;  		//从外设读
//	DMA1_Channel6->CCR|=1<<5;  		//普通模式
//	DMA1_Channel6->CCR|=0<<6; 		//外设地址非增量模式
//	DMA1_Channel6->CCR|=1<<7; 	 	//存储器增量模式
//	DMA1_Channel6->CCR|=0<<8; 	 	//外设数据宽度为8位
//	DMA1_Channel6->CCR|=0<<10; 		//存储器数据宽度8位
//	DMA1_Channel6->CCR|=1<<12; 		//中等优先级
//	DMA1_Channel6->CCR|=0<<14; 		//非存储器到存储器模式
//	DMA1_Channel6->CCR|=1<<1;			//允许TC中断
//	DMA1_Channel6->CCR|=1<<0;			//通道开启
//	MY_NVIC_Init(1,3,DMA1_Channel6_IRQn,2);
//}

//void  DMA1_Channel6_IRQHandler()
//{
//	if(DMA1->ISR&(1<<21))//传输完成
//	{
//		DMA1->IFCR|=1<<21;//清除标志位

//	}
//	
//}

void usart3(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
	mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOB->CRH&=0XFFFF00FF;//IO状态设置
	GPIOB->CRH|=0X00008B00;//IO状态设置 
	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	USART3->CR3|=1<<6;	//dma使能接受
	//使能接收中断
//	USART3->CR1|=1<<8;    //PE中断使能
//	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
//	MY_NVIC_Init(3,2,USART3_IRQn,2);//组2，最低优先级 
}


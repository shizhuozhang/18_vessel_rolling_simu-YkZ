#include "usart.h"	
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "queue.h"
#include "timer.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////


//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
void usart1(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART1->CR3|=1<<6;    //����DMA
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
}



//cndtr:��������
//cpar:�����ַ
//cmar:�洢��ַ
void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar)//����2 dam���ݴ���  ͨ��6
{
	RCC->AHBENR|=1<<0;			//����DMA1ʱ��
	delay_ms(5);				//�ȴ�DMAʱ���ȶ�
	DMA1_Channel6->CPAR=cpar; 	 	//DMA1 �����ַ 
	DMA1_Channel6->CMAR=cmar; 	//DMA1,�洢����ַ
	DMA1_Channel6->CNDTR=cndtr;    	//DMA1,����������
	DMA1_Channel6->CCR=0X00000000;	//��λ
	DMA1_Channel6->CCR|=0<<4;  		//�������
	DMA1_Channel6->CCR|=1<<5;  		//��ͨģʽ
	DMA1_Channel6->CCR|=0<<6; 		//�����ַ������ģʽ
	DMA1_Channel6->CCR|=1<<7; 	 	//�洢������ģʽ
	DMA1_Channel6->CCR|=0<<8; 	 	//�������ݿ��Ϊ8λ
	DMA1_Channel6->CCR|=0<<10; 		//�洢�����ݿ��8λ
	DMA1_Channel6->CCR|=3<<12; 		//��ߵ����ȼ�
	DMA1_Channel6->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ
	DMA1_Channel6->CCR|=1<<1;			//����TC�ж�
	DMA1_Channel6->CCR|=1<<0;			//ͨ������
	MY_NVIC_Init(3,2,DMA1_Channel6_IRQn,2);
}

u8 lstatue=STOP;//�ϴ���λ��״̬
u8 statue=STOP;//��λ��״̬
//bit7 ͨ���ر�
u8 SIGN_LEC=0,Zero_flag=0;
u16 sum=0;
u_SendData SendData,RXData;
void  DMA1_Channel6_IRQHandler()//dma1�ж�  usart3
{
	if(DMA1->ISR&(1<<21))//�������
	{
		DMA1->IFCR|=1<<20;//�����־λ
		if(0xfe==RXData.ca_SendData[2]&&0xef==RXData.ca_SendData[3])//֡ͷУ��
		{
			sum=(u16)RXData.ca_SendData[2]+RXData.ca_SendData[3]+RXData.ca_SendData[4]+RXData.ca_SendData[5]+RXData.ca_SendData[6]+RXData.ca_SendData[7]+RXData.ca_SendData[8]+RXData.ca_SendData[9]+RXData.ca_SendData[10]+RXData.ca_SendData[11]+RXData.ca_SendData[12];//У�����ݼ���
			if((sum&0xff)==RXData.s_SendData.c_Verify)//У��ֵ�Ƚ�
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
					case RUNNING: //���
						//��while�����״̬��־
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
				DMA1_Channel6->CCR&=~(1<<0);			//ͨ���ر�
				SIGN_LEC|=1<<7;		//ͨ���رձ��
			}
		}else
		{
			DMA1_Channel6->CCR&=~(1<<0);			//ͨ���ر�
			SIGN_LEC|=1<<7;			//ͨ���رձ��
		}
	}	
}


u8 res=0;
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))//���յ�����
	{
		res=USART2->DR;
		if((SIGN_LEC&1<<7)&&(0Xfe==res))	//�����һ���ֽ�
		{
			SIGN_LEC&=0x7f;				//ͨ���������
			DMA1_Channel6->CCR|=1<<0;			//ͨ������
		}	
	}
}





//cndtr:��������
//cpar:�����ַ
//cmar:�洢��ַ
void DMA1_Channel7_Init(u16 cndtr,u32 cpar,u32 cmar)//����2 dam���ݴ���  ͨ��7
{
	RCC->AHBENR|=1<<0;			//����DMA1ʱ��
	delay_ms(5);				//�ȴ�DMAʱ���ȶ�
	DMA1_Channel7->CPAR=cpar; 	 	//DMA1 �����ַ 
	DMA1_Channel7->CMAR=cmar; 	//DMA1,�洢����ַ
	DMA1_Channel7->CNDTR=cndtr;    	//DMA1,����������
	DMA1_Channel7->CCR=0X00000000;	//��λ
	DMA1_Channel7->CCR|=1<<4;  		//�Ӵ洢����
	DMA1_Channel7->CCR|=0<<5;  		//��ִ��ѭ������
	DMA1_Channel7->CCR|=0<<6; 		//�����ַ������ģʽ
	DMA1_Channel7->CCR|=1<<7; 	 	//�洢������ģʽ
	DMA1_Channel7->CCR|=0<<8; 	 	//�������ݿ��Ϊ8λ
	DMA1_Channel7->CCR|=0<<10; 		//�洢�����ݿ��8λ
	DMA1_Channel7->CCR|=2<<12; 		//�ߵ����ȼ�
	DMA1_Channel7->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ
	DMA1_Channel7->CCR|=1<<1;			//����TC�ж�
//	DMA1_Channel7->CCR|=1<<0;			//ͨ������
	MY_NVIC_Init(3,1,DMA1_Channel7_IRQn,2);
}

u8 UATR2_COMP=0;
void  DMA1_Channel7_IRQHandler()//dma1�ж�  usart2
{
	
	if(DMA1->ISR&(1<<25))//�������
	{
		DMA1->IFCR|=1<<24;//�����־λ
		UATR2_COMP=0;
	}
}



void usart2(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
	mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF;//IO״̬����
	GPIOA->CRL|=0X00008B00;//IO״̬���� 
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ
	//����������
	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART2->CR3|=1<<6;	//dmaʹ�ܽ���
	USART2->CR3|=1<<7;	//dmaʹ�ܷ���
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//��2��������ȼ� 
}

//void USART2_IRQHandler(void)
//{
//	if(USART2->SR&(1<<5))//���յ�����
//	{	 
//		USART2->SR&=~(1<<5);	
////			if(0X55==USART2->DR&&1==Dma1_C6_Close)
////			{
////				Dma1_C6_Close=0;
////				DMA1_Channel6->CCR|=1<<0;			//ͨ������
////			}
//	}

//} 
//	//cndtr:��������
////cpar:�����ַ
////cmar:�洢��ַ
//void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar)   //usart2
//{
//	RCC->AHBENR|=1<<0;			//����DMA1ʱ��
//	delay_ms(5);				//�ȴ�DMAʱ���ȶ�
//	DMA1_Channel6->CPAR=cpar; 	 	//DMA1 �����ַ 
//	DMA1_Channel6->CMAR=cmar; 	//DMA1,�洢����ַ
//	DMA1_Channel6->CNDTR=cndtr;    	//DMA1,����������
//	DMA1_Channel6->CCR=0X00000000;	//��λ
//	DMA1_Channel6->CCR|=0<<4;  		//�������
//	DMA1_Channel6->CCR|=1<<5;  		//��ͨģʽ
//	DMA1_Channel6->CCR|=0<<6; 		//�����ַ������ģʽ
//	DMA1_Channel6->CCR|=1<<7; 	 	//�洢������ģʽ
//	DMA1_Channel6->CCR|=0<<8; 	 	//�������ݿ��Ϊ8λ
//	DMA1_Channel6->CCR|=0<<10; 		//�洢�����ݿ��8λ
//	DMA1_Channel6->CCR|=1<<12; 		//�е����ȼ�
//	DMA1_Channel6->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ
//	DMA1_Channel6->CCR|=1<<1;			//����TC�ж�
//	DMA1_Channel6->CCR|=1<<0;			//ͨ������
//	MY_NVIC_Init(1,3,DMA1_Channel6_IRQn,2);
//}

//void  DMA1_Channel6_IRQHandler()
//{
//	if(DMA1->ISR&(1<<21))//�������
//	{
//		DMA1->IFCR|=1<<21;//�����־λ

//	}
//	
//}

void usart3(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
	mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOB->CRH&=0XFFFF00FF;//IO״̬����
	GPIOB->CRH|=0X00008B00;//IO״̬���� 
	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART3->CR3|=1<<6;	//dmaʹ�ܽ���
	//ʹ�ܽ����ж�
//	USART3->CR1|=1<<8;    //PE�ж�ʹ��
//	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
//	MY_NVIC_Init(3,2,USART3_IRQn,2);//��2��������ȼ� 
}


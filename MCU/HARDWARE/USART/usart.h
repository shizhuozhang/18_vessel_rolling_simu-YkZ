#ifndef __USART_H
#define __USART_H
#include "sys.h"

typedef union _point    //������ݽṹ
{
    struct
    {
        float f_Time;
        float f_Position;
    }s_PointData;
    u8 ca_PointData[8];
}u_Point;

typedef union _senddata  //�������ݵ����ݽṹ
{
  struct
  {
	  u8  kong[2];//Ϊ����λƫ��
      u8  c_FH1;//֡ͷ
      u8  c_FH2;//֡ͷ
      u_Point u_PointData;
      u8  c_Order;//״̬
      u8  c_Verify;//У��
  }s_SendData;
  u8 ca_SendData[16];   //��Ϊ���ֽڶ���ʶ��ĸ�����ƫ����λ
}u_SendData;

extern u_SendData SendData,RXData;
extern u8 statue,lstatue,Zero_flag;
extern u8 UATR2_COMP;  //����2�������


#define STOP   0x10 //ֹͣ״̬����λ���յ����ź�Ӧ�ñ����Լ���ԭ�� 
#define PAUSE  0x20 //��ͣ״̬����λ���յ����ź�Ӧ�ñ��־�ֹ����ֹͣ��ʱ
#define RUNNING 0x00 //����״̬����λ���������У������������ݵ�����
#define STARTRUNNING 0x30 //����ͣ״̬ת������״̬
#define ZERO 0x40 //��λ��ִ�й������
#define REQUEST   0x01 //����״̬����λ�����ݲ��㣬�������ݣ�������������
#define ASK  0x02 //ѯ��״̬��������λ����ǰ״̬��������������
#define END  0x03 //����״̬��������λ��������������������
#define DATA_NUM 12

void usart1(u32 pclk2,u32 bound);
void usart2(u32 pclk1,u32 bound);
void usart3(u32 pclk1,u32 bound);

void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar);//����2 dam���ݴ���  ͨ��5
void DMA1_Channel7_Init(u16 cndtr,u32 cpar,u32 cmar);//����2 dam���ݴ���  ͨ��7





#endif	   

















#ifndef __USART_H
#define __USART_H
#include "sys.h"

typedef union _point    //点的数据结构
{
    struct
    {
        float f_Time;
        float f_Position;
    }s_PointData;
    u8 ca_PointData[8];
}u_Point;

typedef union _senddata  //发送数据的数据结构
{
  struct
  {
	  u8  kong[2];//为了两位偏移
      u8  c_FH1;//帧头
      u8  c_FH2;//帧头
      u_Point u_PointData;
      u8  c_Order;//状态
      u8  c_Verify;//校验
  }s_SendData;
  u8 ca_SendData[16];   //因为四字节对齐故多四个，需偏移两位
}u_SendData;

extern u_SendData SendData,RXData;
extern u8 statue,lstatue,Zero_flag;
extern u8 UATR2_COMP;  //串口2传输完成


#define STOP   0x10 //停止状态，下位机收到该信号应该保持自己在原点 
#define PAUSE  0x20 //暂停状态，下位机收到该信号应该保持静止，并停止计时
#define RUNNING 0x00 //运行状态，下位机正常运行，发送坐标数据到曲线
#define STARTRUNNING 0x30 //由暂停状态转到运行状态
#define ZERO 0x40 //下位机执行归零程序
#define REQUEST   0x01 //请求状态，下位机数据不足，发送数据，坐标数据正常
#define ASK  0x02 //询问状态，告诉下位机当前状态，坐标数据无用
#define END  0x03 //结束状态，告诉上位机结束，坐标数据无用
#define DATA_NUM 12

void usart1(u32 pclk2,u32 bound);
void usart2(u32 pclk1,u32 bound);
void usart3(u32 pclk1,u32 bound);

void DMA1_Channel6_Init(u16 cndtr,u32 cpar,u32 cmar);//串口2 dam数据传输  通道5
void DMA1_Channel7_Init(u16 cndtr,u32 cpar,u32 cmar);//串口2 dam数据传输  通道7





#endif	   

















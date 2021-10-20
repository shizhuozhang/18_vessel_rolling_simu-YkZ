#ifndef __IO_H
#define __IO_H	

#include "sys.h"

#define LED0 PAout(7)
#define LED1 PAout(6)

#define RIGHT 1  //位置增加
#define LEFT 0	//位置减小

#define L_LIM	PAin(0)		//左边的极限位置
#define R_LIM	PAin(1)		//右边的极限位置
#define  BEEP0   PAout(12)	

#define KEY2	PAin(4)		//板子上的按键，功能自定义
#define KEY3	PAin(5)		//板子上的按键，功能自定义
#define Alarm	PBin(0)		//报警检出
#define Ready	PBin(1)		//伺服准备好
#define Z_SIGN 	PBin(5)		//编码器Z相的信号
#define Emg	PBin(10)		//紧急停止检出
#define Preach	PBin(11)	//定位完成

#define SON 	PBout(12)		//驱动器使能
#define AlarmRst 	PBout(13)		//报警复位
#define ZeroLock 	PBout(14)		//零速钳位
#define Pclear 	PBout(15)		//位置偏差清楚

#define MOTOR_DIR 	PAout(11)		//电机方向控制
#define RELAY_ON 	PAout(15)		//继电器，用于电机驱动电源控制

//#define L_PRES		0
//#define R_PRES		1
//#define KEY2_PRES	2
//#define KEY3_PRES	3

void LED_Init(void);
void BEEP_Init(void);
void KEY_Init(void);
//u8 KEY_Scan(void);//按键扫描函数应配合定时器中断使用，以一定频率扫描
void ELSE_IO_Init(void);
void EXTIx_Init(void);

//根据需要在驱动器里设置，输入输出信号的类型
#endif


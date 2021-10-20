#ifndef __IO_H
#define __IO_H	

#include "sys.h"

#define LED0 PAout(7)
#define LED1 PAout(6)

#define RIGHT 1  //λ������
#define LEFT 0	//λ�ü�С

#define L_LIM	PAin(0)		//��ߵļ���λ��
#define R_LIM	PAin(1)		//�ұߵļ���λ��
#define  BEEP0   PAout(12)	

#define KEY2	PAin(4)		//�����ϵİ����������Զ���
#define KEY3	PAin(5)		//�����ϵİ����������Զ���
#define Alarm	PBin(0)		//�������
#define Ready	PBin(1)		//�ŷ�׼����
#define Z_SIGN 	PBin(5)		//������Z����ź�
#define Emg	PBin(10)		//����ֹͣ���
#define Preach	PBin(11)	//��λ���

#define SON 	PBout(12)		//������ʹ��
#define AlarmRst 	PBout(13)		//������λ
#define ZeroLock 	PBout(14)		//����ǯλ
#define Pclear 	PBout(15)		//λ��ƫ�����

#define MOTOR_DIR 	PAout(11)		//����������
#define RELAY_ON 	PAout(15)		//�̵��������ڵ��������Դ����

//#define L_PRES		0
//#define R_PRES		1
//#define KEY2_PRES	2
//#define KEY3_PRES	3

void LED_Init(void);
void BEEP_Init(void);
void KEY_Init(void);
//u8 KEY_Scan(void);//����ɨ�躯��Ӧ��϶�ʱ���ж�ʹ�ã���һ��Ƶ��ɨ��
void ELSE_IO_Init(void);
void EXTIx_Init(void);

//������Ҫ�������������ã���������źŵ�����
#endif


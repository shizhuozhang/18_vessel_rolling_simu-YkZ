#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

extern s8 Roll_num;

void Encoder4_Init(void);

s16 M_T_Speed_measure(u16 Line_num,u16 X_KHz);
s16 Motor_Time_measure(void);
#endif

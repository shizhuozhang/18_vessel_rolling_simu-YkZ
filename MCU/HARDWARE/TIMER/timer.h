#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern volatile u32 Time;
extern u8 Pause_Flag;

void TIM2_Init(u16 arr,u16 psc);//TIM2¼ÆÊýÆ÷ÖÐ¶Ï				
void Send_Position(float time,float position,u8 status);

#endif








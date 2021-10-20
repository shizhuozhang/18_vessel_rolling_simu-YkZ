#ifndef __QUEUE_H
#define __QUEUE_H

#include "sys.h"

u8 enqueue(float time,float poistion);
u8 dequeue(float *ptime,float *ppoistion);
u8 size(void);
u8 Qclear(void);
#endif







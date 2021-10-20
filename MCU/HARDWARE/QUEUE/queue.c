#include "queue.h"

float Queue[35][2];
u8 head=0,tail=0;
u8 isempty=1;
u8 enqueue(float time,float poistion)
{
	if(head==tail&&isempty==0)//队列已满，无法入队
	{
		return 0;
	}else
	{
		Queue[head][0]=time;
		Queue[head][1]=poistion;
		if(isempty==1) isempty=0;  //设置队列非空
		head++;
		if(head==35) head=0;
	}
	return 1;
}
u8 dequeue(float *ptime,float *ppoistion)
{
	if(isempty==1)//队列已空，无法出队
	{
		return 0;
	}else
	{
		*ptime=Queue[tail][0];
		*ppoistion=Queue[tail][1];
		tail++;
		if(tail==35) tail=0;
		if(head==tail) isempty=1;
	}
	return 1;
	
}
u8 size(void)
{
	if(head>tail)
	{
		return head-tail;
	}else if(head<tail)
	{
		return 35+head-tail;
	}else
	{
		if(isempty==1)
		{
			return 0;
		}else
		{
			return 35;
		}
	}
}

u8 Qclear(void)
{
	head=0;
	tail=0;
	isempty=1;
	return 1;
}

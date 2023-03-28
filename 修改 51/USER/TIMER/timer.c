#include "stc15w4k.h"
#include "timer.h"
#include <intrins.h>

bit timer_10ms;

void DelayMs( unsigned int delay_time)
{
unsigned int delay_cnt;
while(delay_time--)
       {
       for(delay_cnt=0;delay_cnt<200;delay_cnt++)
             {
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             }
       _nop_();
       }
_nop_();
}


void Timer1_Init(void)		//10毫秒@24MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TMOD |= 0x10;			//设置定时器模式
	TL1 = 0xE0;				//设置定时初始值
	TH1 = 0xB1;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
}


void Timer1_isr() interrupt 3
{
	TR1=0;//关闭定时器1
	TL1 = 0xE0;				//设置定时初始值
	TH1 = 0xB1;				//设置定时初始值
	TR1 = 1;				//定时器1开始计时
	if(timer_10ms)timer_10ms=1;
}


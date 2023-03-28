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


void Timer1_Init(void)		//10����@24MHz
{
	AUXR &= 0xBF;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TMOD |= 0x10;			//���ö�ʱ��ģʽ
	TL1 = 0xE0;				//���ö�ʱ��ʼֵ
	TH1 = 0xB1;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
}


void Timer1_isr() interrupt 3
{
	TR1=0;//�رն�ʱ��1
	TL1 = 0xE0;				//���ö�ʱ��ʼֵ
	TH1 = 0xB1;				//���ö�ʱ��ʼֵ
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	if(timer_10ms)timer_10ms=1;
}


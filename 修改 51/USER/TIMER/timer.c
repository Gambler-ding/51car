#include "stc15w4k.h"
#include "timer.h"
#include <intrins.h>

void Timer2_Init(void)		//10微秒@30MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0xE7;				//设置定时初始值
	T2H = 0xFF;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	INT_CLKO |= 0x04;			//使能定时器2中断
}




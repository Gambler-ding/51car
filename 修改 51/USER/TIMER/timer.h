#ifndef _TIMER_H_
#define _TIMER_H_
#include "stc15w4k.h"
extern bdata u16 timer_10ms;
#define TIMER1_START TR1



void Timer1_Init(void);
void DelayMs( unsigned int delay_time);
#endif


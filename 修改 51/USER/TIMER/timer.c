#include "stc15w4k.h"
#include "timer.h"
#include <intrins.h>

void Timer2_Init(void)		//10΢��@30MHz
{
	AUXR &= 0xFB;			//��ʱ��ʱ��12Tģʽ
	T2L = 0xE7;				//���ö�ʱ��ʼֵ
	T2H = 0xFF;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ
	INT_CLKO |= 0x04;			//ʹ�ܶ�ʱ��2�ж�
}




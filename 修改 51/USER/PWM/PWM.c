#include "PWM.h"
#include "stc15w4k.h"
#include "timer.h"
#define  CYCLE  0x07d0L//����PWM����(���ֵΪ32767) 2000
sbit PWM2=P2^7;
sbit PWM3=P2^1;
sbit PWM4=P2^2;
sbit PWM5=P2^3;
sbit PWM6=P0^7;
sbit PWM7=P0^6;

void PWM_config(void)
{
	P0 &= ~0xc0;			//����P0.6/.P0.7��ƽ
	P2 &=~0x8e;				//����P2.1/P2.2/P2.3/P2.7��ƽ
	P_SW2|=0x80;
	Timer2_Init();
	PWMCKS = 0x10;
	PWMC = CYCLE;			//����PWM����
	PWM2T1 = 1;
	PWM2T2 = 0;
	PWM2CR = 0x08;			//PWM2�����P2.7
	PWM3T1 = 1;
	PWM3T2 = 0;
	PWM3CR = 0x00;			//PWM3�����P2.1
	PWM4T1 = 1;
	PWM4T2 = 0;
	PWM4CR = 0x00;			//P WM4�����P2.2
	PWM5T1 = 1;
	PWM5T2 = 0;
	PWM5CR = 0x00;			//PWMS�����P2.3
	PWM6T1 = 1;
	PWM6T2 = 0;
	PWM6CR = 0x08;			//PWM6�����P0.7
	PWM7T1 = 1;
	PWM7T2 = 0;
	PWM7CR = 0x08;			//PWM7�����P0.6
	PWMCFG = 0x00;			//����PWM�������ʼ��ƽ
	PWMCR = 0x3f;			//ʹ��PWM�ź����
	PWMCR|=0x80;			//ʹ��PWMģ��
	P_SW2 &=~0x80;
}
void PWM2_2_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x01;
		PWM2 =0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x01;
		PWM2 =1;
	}
	else
	{
		P_SW2|=0x80;
		PWM2T1 = Wide;
		P_SW2 &=~0x80;
		PWMCR |=0x01;
	}
}
void PWM3_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x02;
		PWM3 = 0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x02;
		PWM3 = 1;
	}
	else
	{
		P_SW2|=0x80;
		PWM3T1 = Wide;
		P_SW2 &=~0x80;
		PWMCR |= 0x02;
	}
}
void PWM4_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x04;
		PWM4 = 0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x04;
		PWM4 = 1;
	}
	else
	{
		P_SW2|=0x80;
		PWM4T1 = Wide;
		P_SW2 &=~0x80;
		PWMCR |= 0x04;
	}
}
void PWM5_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x08;
		PWM5 = 0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x08;
		PWM5 = 1;
	}
	else
	{
		P_SW2|=0x80;
		PWM5T1 = Wide;
		P_SW2 &= ~0x80;
		PWMCR |= 0x08;
	}
}
void PWM6_2_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x10;
		PWM6 = 0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x10;
		PWM6 = 1;
	}
	else
	{
		P_SW2|=0x80;
		PWM6T1 = Wide;
		P_SW2 &=~0x80;
		PWMCR |=0x10;
	}
}
void PWM7_2_SetPwmWide(unsigned short Wide)
{
	if (Wide == 0)
	{
		PWMCR &=~0x20;
		PWM7 = 0;
	}
	else if (Wide == CYCLE)
	{
		PWMCR &=~0x20;
		PWM7 =- 1;
	}
	else
	{
		P_SW2|=0x80;
		PWM7T1 = Wide;
		P_SW2 &=~0x80;
		PWMCR |= 0x20;
	}
}

/********************
定时器0用于motor的pwm使用
定时器1用于串口一使用
若要开始
********************/


#include "stc15w4k.h"
#include "MOTOR.h"
#include "oled.h"
#include "timer.h"
#include "adc.h"
#include "action.h"
#include "uart.h"
#include "PWM.h"
#include <intrins.h>
sbit F1key 	= P5 ^ 4;
sbit F2key 	= P5 ^ 5;
sbit F3key	= P3 ^ 5;
sbit F4key	= P5 ^ 1;

int main()
{
	char Route1[] = "-2NL+2G+3p";
	char Route2[] = "-2NL+6p";
	char Route3[] = "-2NLL+2G+2R+4P";
	char Route4[] = "-2NLD+5r";
	P7M1 = 0x00;P7M0 = 0x00; /*P7 准双向口*/
	P6M1 = 0x00;P6M0 = 0x00; /*P6 准双向口*/
	P5M1 = 0x08;P5M0 = 0x08; /*P5.7-P5.6, 准双向口,P5.5/F2key,准双向口;P5.4/F1key,准双向口;P5.3:开漏输出(TXD4_2);P5.2:准双向口(RXD4_2);*/
						  /*P5.1/F4key,准双向口;P5.0:准双向口(RXD3_2)*/
	P4M1 = 0x00;P4M0 = 0xff; /*P4口:强推挽输出(驱动电机)*/
	P3M1 = 0x00;P3M0 = 0x00; /*P3 准双向口;F3key=P3 ^ 5;*/
	P2M1 = 0x00;P2M0 = 0x00; /*P2 准双向口*/
	P1M1 = 0x00;P1M0 = 0x00; /*P1 准双向口*/
	P0M1 = 0x00;P0M0 = 0x00; /*P0.7-P0.0:准双向口*/

	PWM_config();
	OLED_Init();			//初始化OLED
	Uart1Init();  		//初始化串口1
	OLED_Clear();
	OLED_ShowString(0,0,"yyds",16);_nop_();	
	motor_init();
	
	//while(SBUF!=0x33&&SBUF!=0x34)power_show();
	//test_track();

	start();
	//put_high();

	while(1)
	{
		power_show();
		TestIrStatus();
		if(!F1key)sw(Route1);
		if(!F2key)sw(Route2);
		if(!F3key)sw(Route3);
		if(!F4key)sw(Route4);
		/*
		if(SBUF==0x33)track_PID(40);
		else if(SBUF==0x34)track_PID(-40);
		*/
		//OLED_ShowNum(0,4,UartFlag>>4,4,16);
		/*if(UartFlag>>4)
		{
			UART1send_AString(UARTbuffer);
			UartFlag&=0x6f;
		}*/
	}
}


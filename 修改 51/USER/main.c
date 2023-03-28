/********************
定时器0用于motor的pwm使用
定时器1用于定时使用
********************/


#include "stc15w4k.h"
#include "MOTOR.h"
#include "oled.h"
#include "timer.h"
#include "adc.h"
#include "eeprom.h"
#include <intrins.h>

sbit F1key 	= P5 ^ 4;
sbit F2key 	= P5 ^ 5;
sbit F3key	= P3 ^ 5;
sbit F4key	= P5 ^ 1;

int main()
{
	
	//u8 dt1=0x1f;
	P4M1 = 0x00;P4M0 = 0xff; /*P4口:强推挽输出(驱动电机)*/
	
	OLED_Init();			//初始化OLED
	Timer1_Init();  		//初始化定时器1
	OLED_Clear();
	OLED_ShowString(0,0,"yyds",16);_nop_();	
	
	motor_init();
	
	//IapEraseSector(0x6600);
	//DelayMs(1000);
	//IapProgramByte(0x6600,dt1);
	OLED_ShowNum(0,6,IapReadByte(0x6600),4,16);
	

	
	/*DelayMs(2000);
	track_flag=1;*/
	while(1)
	{


		/*TestIrStatus();
		track_PID(40);
		power_show();*/
	}
	return 0;
}
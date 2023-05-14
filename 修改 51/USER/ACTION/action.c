#include "stc15w4k.h"
#include "action.h"
#include "motor.h"
#include "uart.h"
#include "PWM.h"
#include "oled.h"


#define track_line (mode?(frontPortData&0x20&&frontPortData&0x02):(backPortData&0x20&&backPortData&0x02))
#define track_angle  (frontPortData&0x08||frontPortData&0x04||frontPortData&0x10)

//test for track
//track eight lines 
bit idata mode;
//碰到为低电平
sbit touch_key=P1 ^ 4;
void track_PID(char pwm)
{
	
	static idata char Last_error;
	idata char error;
	idata char L_Pwm,R_Pwm;
	idata u8 D =pwm/2;
	code u8 P=4,limit_pwm=60;
	UART1send_Abyte(track_angle);
	pwm=(mode?1:-1)*pwm;
	switch(pwm<0?backPortData:frontPortData)
		{                                         //421 8421
			case 0x01:		error=8;	break;      //111 1110 0111 111
			case 0x03:		error=7;	break;      //111 1100 0011 111
			case 0x07:		error=6;	break;      //111 1000 0001 111
			case 0x02:		error=5;	break;      //111 1101 1011 111
			case 0x06:		error=4;	break;      //111 1001 1001 111
			case 0x0E:		error=3;	break;      //111 0001 1000 111
			case 0x04:		error=2;	break;      //111 1011 1101 111
			case 0x0C:		error=1;	break;      //111 0011 1100 111
			case 0x1C:		error=0;	break;      //110 0011 1100 011
			case 0x7f:		error=0;	break;      //000 0000 0000 000
			case 0x08:		error=0;	break;      //111 0111 1110 111
			case 0x18:		error=-1;	break;      //110 0111 1110 011
			case 0x38:		error=-2;	break;      //100 0111 1110 001
			case 0x10:		error=-3;	break;      //110 1111 1111 011
			case 0x30:		error=-4;	break;      //100 1111 1111 001
			case 0x70:		error=-5;	break;      //000 1111 1111 000
			case 0x20:		error=-6;	break;      //101 1111 1111 101
			case 0x60:		error=-7;	break;      //001 1111 1111 100
			case 0x40:		error=-8;	break;      //011 1111 1111 110
			/*case 0x00:		brake();	break;      //偏*/
			default :		//用与偏离后的措施
			{
				switch(pwm>0?backPortData:frontPortData)
				{
					case 0x01:		error=8;	break;      //111 1110 0111 111
					case 0x03:		error=7;	break;      //111 1100 0011 111
					case 0x07:		error=6;	break;      //111 1000 0001 111
					case 0x02:		error=5;	break;      //111 1101 1011 111
					case 0x06:		error=4;	break;      //111 1001 1001 111
					case 0x0E:		error=3;	break;      //111 0001 1000 111
					case 0x04:		error=2;	break;      //111 1011 1101 111
					case 0x0C:		error=1;	break;      //111 0011 1100 111
					case 0x1C:		error=0;	break;      //110 0011 1100 011
					case 0x7f:		error=0;	break;      //000 0000 0000 000
					case 0x08:		error=0;	break;      //111 0111 1110 111
					case 0x18:		error=-1;	break;      //110 0111 1110 011
					case 0x38:		error=-2;	break;      //100 0111 1110 001
					case 0x10:		error=-3;	break;      //110 1111 1111 011
					case 0x30:		error=-4;	break;      //100 1111 1111 001
					case 0x70:		error=-5;	break;      //000 1111 1111 000
					case 0x20:		error=-6;	break;      //101 1111 1111 101
					case 0x60:		error=-7;	break;      //001 1111 1111 100
					case 0x40:		error=-8;	break;      //011 1111 1111 110
					case 0x00:		brake();	break;      //偏
					default :		error=0;	return; 
				}
			}
		}
		
		R_Pwm = (pwm-(error*P+(error-Last_error)*D));
		L_Pwm = (pwm+(error*P+(error-Last_error)*D));
		
		Last_error = error;
		//限制
		if(L_Pwm > (pwm+limit_pwm))
			L_Pwm = (pwm+limit_pwm);
		if(R_Pwm > (pwm+limit_pwm))
			R_Pwm = (pwm+limit_pwm);
		if(L_Pwm <(pwm-limit_pwm))
			L_Pwm = (pwm-limit_pwm);
		if(R_Pwm < (pwm-limit_pwm))
			R_Pwm = (pwm-limit_pwm);
		motor_pwm_set(L_Pwm,R_Pwm);
		/*UART1send_AFloat(Integral_error*I);
		UART1send_AFloat(L_Pwm);
		UART1send_AFloat(R_Pwm);
		UART1send_AFloat(error);
		UART1send_Abyte(0x00);
		UART1send_Abyte(0x00);
		UART1send_Abyte(0x80);
		UART1send_Abyte(0x7f);*/
		motor_pwm_set(L_Pwm,R_Pwm);

}

void delay_ms(u16 i)
{
	t_sum=0;
	timer_10ms=1;
	while(t_sum<=i);
	timer_10ms=0;
	t_sum=0;
}
void jump_line()
{
	while(track_line)track_PID(20);
}
//just for test
void test_track()
{
	idata u8 n=9;
	t_sum=0;
	timer_10ms=0;
	while(n)
	{
		track_PID(40);
		TestIrStatus();
		if(track_line)
		{
			UART1send_AFloat(t_sum);
			UART1send_Abyte(0x00);
			UART1send_Abyte(0x00);
			UART1send_Abyte(0x80);
			UART1send_Abyte(0x7f);
			timer_10ms=1;
			n--;
			jump_line();
		}
	}
	timer_10ms=0;
	UART1send_AFloat(t_sum/8);
	UART1send_Abyte(0x00);
	UART1send_Abyte(0x00);
	UART1send_Abyte(0x80);
	UART1send_Abyte(0x7f);
	brake();
}
void turn_right()
{
	brake();
	motor_pwm_set(10,-10);
	delay_ms(30);
	while(track_angle);
	while(!track_angle);
	brake();
}
void turn_left()
{
	brake();
	motor_pwm_set(-10,10);
	delay_ms(30);
	while(track_angle);
	while(!track_angle);
	brake();
}
void r_in()
{
	brake();
	motor_pwm_set(10,-10);
	while(track_angle);
	motor_pwm_set(10,10);
	while(!track_angle);
	while(track_angle);
	brake();
	lr_up();
	motor_pwm_set(-10,-10);
	while(!track_angle);
	while(track_angle);
	while(!track_angle);
	brake();
	while(!track_line)track_PID(10);
	brake();
}
void l_in()
{
	brake();
	motor_pwm_set(-10,10);
	while(track_angle);
	motor_pwm_set(10,10);
	while(!track_angle);
	while(track_angle);
	brake();
	lr_up();
	motor_pwm_set(-10,-10);
	while(!track_angle);
	while(track_angle);
	while(!track_angle);
	brake();
	while(!track_line)track_PID(5);
	brake();
}
void straight_way(u8 i)
{
	u8 n=i;
	while(n)
	{
		if(n!=1&&n!=i)
		{
			track_PID(40);
		}
		else 
		{
			track_PID(10);
		}
		TestIrStatus();
		if(track_line)
		{
			n--;
			if(!n)break;
			jump_line();
		}
	}
	brake();
}
void sw(char r[])
{
	char* p=r;
	//mode==1 前进 mode==0 后退
	while( p[0] != '\0')
	{
		switch (*p)
		{
			case 'l':
				l_in();break;
			case 'r':
				r_in();break;
			case 'L':
				if(*(p-1)=='L')
				{
					mode=!mode;
					turn_left();
					mode=!mode;
					break;
				}
				else turn_left();break;
			case 'R':
				if(*(p-1)=='R')
				{
					mode=!mode;
					turn_right();
					mode=!mode;
					break;
				}
				else turn_right();break;
			case '+':
				mode=1;break;
			case '-':
				mode=0;break;
			case 'N':
				nomor();break;
			case 'p':
				put_low();break;
			case 'P':
				put_high();break;
			case 'G':
				get();break;
			case 'U':
				lr_up();break;
			case 'D':
				lr_down();break;
			default:
			{
				straight_way(*p-'0');
			}
		}
		p++;
	}
}
//PWM7_2左边
//PWM6_2右边
//50次-250次-2000次
void start()
{
	lr_up();
	PWM7_2_SetPwmWide(80);
	PWM6_2_SetPwmWide(100);
	PWM2_2_SetPwmWide(220);
	PWM5_SetPwmWide(70);
}
void nomor()
{
	//PWM7_2_SetPwmWide(125);
	PWM6_2_SetPwmWide(150);
	delay_ms(30);
	PWM2_2_SetPwmWide(250);
	delay_ms(30);
	PWM5_SetPwmWide(200);
	delay_ms(50);
}
void lr_down()
{
	PWM4_SetPwmWide(87);
	PWM3_SetPwmWide(202);
	delay_ms(40);
}
void lr_up()
{
	PWM4_SetPwmWide(150);
	PWM3_SetPwmWide(150);
	delay_ms(40);
}
void push()
{
	loosen();
	PWM6_2_SetPwmWide(150);
	delay_ms(50);
	PWM2_2_SetPwmWide(80);
	delay_ms(50);
	PWM5_SetPwmWide(80);
	delay_ms(80);
}
//5 越大越后面
void put_low()
{
	brake();
	PWM6_2_SetPwmWide(250);
	delay_ms(60);
	PWM2_2_SetPwmWide(200);
	delay_ms(60);
	PWM2_2_SetPwmWide(150);
	delay_ms(80);
	PWM2_2_SetPwmWide(100);
	delay_ms(80);
	PWM2_2_SetPwmWide(50);
	delay_ms(70);
	PWM5_SetPwmWide(175);
	delay_ms(100);
	loosen();
	delay_ms(50);
}
void put_high()
{
	mode=0;
	while(touch_key)
	{
		mode=1;
		track_PID(10);
		mode=0;
	}
	brake();
	
	PWM6_2_SetPwmWide(250);
	delay_ms(50);
	PWM2_2_SetPwmWide(70);
	delay_ms(50);
	PWM5_SetPwmWide(200);
	delay_ms(80);
	loosen();
	delay_ms(50);
	nomor();
}
void touch()
{
	PWM7_2_SetPwmWide(186);
}
void loosen()
{
	PWM7_2_SetPwmWide(125);
}
void get()
{
	mode=1;
	push();
	while(track_line)track_PID(20);
	while(!track_line)track_PID(40);
	touch();
	jump_line();
	brake();
	nomor();
}
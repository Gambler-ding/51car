#include "MOTOR.h"
#include "stc15w4k.h"
#include "oled.h"
#include "uart.h"
#include "adc.h"
#include <intrins.h>
#include "action.h"

#define FrontStatus P7
#define BackStatus  P6

bit idata timer_10ms=0;
u16 bdata t_sum=0;
char bdata motor_l_pwm;
char bdata motor_r_pwm;

u8 data frontPortData,frontPortDataA,frontPortDataB,frontPortDataC;  
u8 data backPortData,backPortDataA,backPortDataB,backPortDataC;

sbit MOTOR_LF_FRONT	= P4^5;
sbit MOTOR_LF_BACK	= P4^6;

sbit MOTOR_LB_FRONT	= P4^4;
sbit MOTOR_LB_BACK	= P4^3;

sbit MOTOR_RF_FRONT	= P4^7;
sbit MOTOR_RF_BACK	= P4^0;

sbit MOTOR_RB_FRONT	= P4^2;
sbit MOTOR_RB_BACK	= P4^1;

u8 idata motor_counter;

/*100us自动装载*/
void motor_tim0_init()
{
	motor_counter = 0;
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD =0x00;				//设置定时器模式
	TL0 = 0x48;				//设置定时初始值
	TH0 = 0xF4;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0	= 1;				//使能定时器中断
	EA = 1;					//总计时器
}

void motor_pwm_set(char pwml,char pwmr)
{
	if(pwml > 100)				//电机速度限幅
	{
		pwml = 100;
	}
    if (pwml < -100)		//电机速度限幅
    {
        pwml = -100;
    }  
	if(pwmr > 100)				//电机速度限幅
	{
		pwmr = 100;
	}
    if (pwmr < -100)		//电机速度限幅
    {
        pwmr = -100;
    }
	motor_l_pwm=pwml;
	motor_r_pwm=pwmr;
}
void motor_init()
{
	motor_pwm_set(0,0);	//电机PWM设置 0
	motor_tim0_init();   //模拟电机IO PWM波定时器初始化
}

//正反皆可 若为反则 pwm为负数

void TestIrStatus(void)
{
	u8 cnt_i,UserReg;
	u8 idata TestBuffer[11]="F:00000000";
	UserReg = frontPortData;
	for( cnt_i=2;cnt_i<10;cnt_i++)
			{
			if (UserReg & 0x80)   TestBuffer[cnt_i] ='1';
			else                  TestBuffer[cnt_i] ='0';
			UserReg <<= 1;
			}
	OLED_ShowString(0,0,TestBuffer,16);


	TestBuffer[0] = 'B';
	UserReg = backPortData;
	for( cnt_i=2;cnt_i<10;cnt_i++)
			{
			if (UserReg & 0x80)   TestBuffer[cnt_i] ='1';
			else                  TestBuffer[cnt_i] ='0';
			UserReg <<= 1;
			}
	OLED_ShowString(0,2,TestBuffer,16);
	_nop_();	
}
void brake()
{
	motor_pwm_set(0,0);
	t_sum=0;
	timer_10ms=1;
	//TODO写计时器延时
	while(t_sum<=30)
	{
		MOTOR_RF_FRONT = 1;
		MOTOR_RF_BACK = 1;      
		MOTOR_RB_FRONT = 1;
		MOTOR_RB_BACK = 1;
		MOTOR_LF_FRONT = 1;
		MOTOR_LF_BACK = 1;       
		MOTOR_LB_FRONT = 1;
		MOTOR_LB_BACK = 1;  
	}
	timer_10ms=0;
}
void motor_irp()	interrupt 1 using 1
{
	if(!timer_10ms)t_sum=0;
	motor_counter++;
    if (motor_counter >= 100)		//pwm波周期设置 周期= 100 * 100us = 10000us=10ms 即电机pwm波频率100Hz 周期10ms
    {
		if(timer_10ms)t_sum++;
        motor_counter = 0;
    }
	//左motor_pwm设置
	if (motor_l_pwm >= 0)
	{
		//左轮正转
		if(motor_counter < motor_l_pwm)  
		{
			MOTOR_LF_FRONT = 1;
			MOTOR_LF_BACK = 0;       
			MOTOR_LB_FRONT = 1;
			MOTOR_LB_BACK = 0;      
		}
		else
		{
			MOTOR_LF_FRONT = 0;
			MOTOR_LF_BACK = 0;      
			
			MOTOR_LB_FRONT = 0;
			MOTOR_LB_BACK = 0;    
		}
	}
	else
	{
		//左轮反转
		
		 if(motor_counter < -motor_l_pwm)
		{
			MOTOR_LF_FRONT = 0;
			MOTOR_LF_BACK = 1;      
			MOTOR_LB_FRONT = 0;
			MOTOR_LB_BACK = 1;      
		}
		else
		{
			MOTOR_LF_FRONT = 0;
			MOTOR_LF_BACK = 0;      
			
			MOTOR_LB_FRONT = 0;
			MOTOR_LB_BACK = 0;    
		}
	}
	
	if (motor_r_pwm >= 0)
	{
		//右轮正转
		
		if(motor_counter < motor_r_pwm)  
		{
			MOTOR_RF_FRONT = 1;
			MOTOR_RF_BACK = 0;      
			MOTOR_RB_FRONT = 1;
			MOTOR_RB_BACK = 0;      
		}
		else
		{
			MOTOR_RF_FRONT = 0;
			MOTOR_RF_BACK = 0;      
			
			MOTOR_RB_FRONT = 0;
			MOTOR_RB_BACK = 0;    
		}
	}
	else
	{
		//右轮反转
		
		 if(motor_counter < -motor_r_pwm)
		{
			MOTOR_RF_FRONT = 0;
			MOTOR_RF_BACK = 1;      
			MOTOR_RB_FRONT = 0;
			MOTOR_RB_BACK = 1;      
		}
		else
		{
			MOTOR_RF_FRONT = 0;
			MOTOR_RF_BACK = 0;      
			
			MOTOR_RB_FRONT = 0;
			MOTOR_RB_BACK = 0;    
		}
	}
/*------3中取二规则读取七路传感器--------
/*前端传感器数据循环更新*/
	frontPortDataA = frontPortDataB;
	frontPortDataB = frontPortDataC;
	frontPortDataC = FrontStatus & 0x7f;	
	frontPortData = (frontPortDataB & ( frontPortDataA | frontPortDataC)) | (frontPortDataA & frontPortDataC);
/*后端传感器数据循环更新*/
	backPortDataA = backPortDataB;
	backPortDataB = backPortDataC;
	backPortDataC = BackStatus & 0x7f;
	backPortData = (backPortDataB & ( backPortDataA | backPortDataC)) | (backPortDataA & backPortDataC);
	
/*------3中取二规则------*/	
	_nop_();
}
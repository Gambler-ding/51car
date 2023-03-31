#include "MOTOR.h"
#include "stc15w4k.h"
#include "oled.h"
#include <intrins.h>

#define FrontStatus P7
#define BackStatus  P6

bdata float motor_l_pwm;
bdata float motor_r_pwm;

data u8 frontPortData,frontPortDataA,frontPortDataB,frontPortDataC;  
data u8 BackPortData,BackPortDataA,BackPortDataB,BackPortDataC;

sbit MOTOR_LF_FRONT	= P4^5;
sbit MOTOR_LF_BACK	= P4^6;

sbit MOTOR_LB_FRONT	= P4^4;
sbit MOTOR_LB_BACK	= P4^3;

sbit MOTOR_RF_FRONT	= P4^7;
sbit MOTOR_RF_BACK	= P4^0;

sbit MOTOR_RB_FRONT	= P4^2;
sbit MOTOR_RB_BACK	= P4^1;

u8 motor_counter;

/*1000us*/
void motor_tim1_init()
{
	motor_counter = 0;
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模z式
	TL0 = 0xA0;				//设置定时初始值
	TH0 = 0xF6;				//设置定时初始值
	ET0 = 1;				//定时器0允许中断
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA = 1;					//总计时器
}

void motor_pwm_set(float pwml,float pwmr)
{
	if(pwml > 100)				//电机速度限幅
	{
		pwml = 100;
	}
    else if (pwmr < -100)		//电机速度限幅
    {
        pwmr = -100;
    }  
	motor_l_pwm=pwml;
	motor_r_pwm=pwmr;
}
void motor_init()
{
	motor_pwm_set(0,0);	//电机PWM设置 0
	motor_tim1_init();   //模拟电机IO PWM波定时器初始化
}

//正反皆可 若为反则 pwm为负数
void track_PID(int pwm)
{
	static float Integral_error,Last_error,error = 0;
	int L_Pwm,R_Pwm;
	code float P=4.5,I = 0.000001,D =9;
	code u8 limit_pwm=50;
	switch(frontPortData)
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
			default :			error=0;	break;
		}
		//error=error<0?-error:error;
		//OLED_ShowNum(20,4,frontPortData,2,16);
		
		Integral_error+=error;
		
		R_Pwm = (pwm-(error*P+Integral_error*I+(error-Last_error)*D));
		L_Pwm = (pwm+(error*P+Integral_error*I+(error-Last_error)*D));
		
		Last_error = error;
		
		if(pwm > 0)
		{
			if(L_Pwm > (pwm+limit_pwm))
				L_Pwm = (pwm+limit_pwm);
			if(R_Pwm > (pwm+limit_pwm))
				R_Pwm = (pwm+limit_pwm);
			if(L_Pwm <=(pwm-limit_pwm))
				L_Pwm = (pwm-limit_pwm);
			if(R_Pwm <= (pwm-limit_pwm))
				R_Pwm = (pwm-limit_pwm);
		}
		 motor_pwm_set(L_Pwm,R_Pwm);
		OLED_ShowNum(0,4,L_Pwm,2,16);
		OLED_ShowNum(16,4,R_Pwm,2,16);
}
void brake()
{
	//TODO写计时器延时
		MOTOR_RF_FRONT = 1;
		MOTOR_RF_BACK = 1;      
		MOTOR_RB_FRONT = 1;
		MOTOR_RB_BACK = 1;
		MOTOR_LF_FRONT = 1;
		MOTOR_LF_BACK = 1;       
		MOTOR_LB_FRONT = 1;
		MOTOR_LB_BACK = 1;  
}
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
	UserReg = BackPortData;
	for( cnt_i=2;cnt_i<10;cnt_i++)
			{
			if (UserReg & 0x80)   TestBuffer[cnt_i] ='1';
			else                  TestBuffer[cnt_i] ='0';
			UserReg <<= 1;
			}
	OLED_ShowString(0,2,TestBuffer,16);
	_nop_();	
}
void motor_irp()	interrupt 1
{
	TR0=0;
	TL0 = 0xA0;				//设置定时初始值
	TH0 = 0xF6;				//设置定时初始值
	TR0=1;
	motor_counter++;
	//OLED_ShowString(0,4,"yyds",16);
    if (motor_counter >= 100)		//pwm波周期设置 周期= 100 * 100us = 10000us=10ms 即电机pwm波频率100Hz 周期10ms
    {
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
	BackPortDataA = BackPortDataB;
	BackPortDataB = BackPortDataC;
	BackPortDataC = BackStatus & 0x7f;
	BackPortData = (BackPortDataB & ( BackPortDataA | BackPortDataC)) | (BackPortDataA & BackPortDataC);
	
/*------3中取二规则------*/	

	_nop_();
}
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

/*100us�Զ�װ��*/
void motor_tim0_init()
{
	motor_counter = 0;
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD =0x00;				//���ö�ʱ��ģʽ
	TL0 = 0x48;				//���ö�ʱ��ʼֵ
	TH0 = 0xF4;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0	= 1;				//ʹ�ܶ�ʱ���ж�
	EA = 1;					//�ܼ�ʱ��
}

void motor_pwm_set(char pwml,char pwmr)
{
	if(pwml > 100)				//����ٶ��޷�
	{
		pwml = 100;
	}
    if (pwml < -100)		//����ٶ��޷�
    {
        pwml = -100;
    }  
	if(pwmr > 100)				//����ٶ��޷�
	{
		pwmr = 100;
	}
    if (pwmr < -100)		//����ٶ��޷�
    {
        pwmr = -100;
    }
	motor_l_pwm=pwml;
	motor_r_pwm=pwmr;
}
void motor_init()
{
	motor_pwm_set(0,0);	//���PWM���� 0
	motor_tim0_init();   //ģ����IO PWM����ʱ����ʼ��
}

//�����Կ� ��Ϊ���� pwmΪ����

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
	//TODOд��ʱ����ʱ
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
    if (motor_counter >= 100)		//pwm���������� ����= 100 * 100us = 10000us=10ms �����pwm��Ƶ��100Hz ����10ms
    {
		if(timer_10ms)t_sum++;
        motor_counter = 0;
    }
	//��motor_pwm����
	if (motor_l_pwm >= 0)
	{
		//������ת
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
		//���ַ�ת
		
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
		//������ת
		
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
		//���ַ�ת
		
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
/*------3��ȡ�������ȡ��·������--------
/*ǰ�˴���������ѭ������*/
	frontPortDataA = frontPortDataB;
	frontPortDataB = frontPortDataC;
	frontPortDataC = FrontStatus & 0x7f;	
	frontPortData = (frontPortDataB & ( frontPortDataA | frontPortDataC)) | (frontPortDataA & frontPortDataC);
/*��˴���������ѭ������*/
	backPortDataA = backPortDataB;
	backPortDataB = backPortDataC;
	backPortDataC = BackStatus & 0x7f;
	backPortData = (backPortDataB & ( backPortDataA | backPortDataC)) | (backPortDataA & backPortDataC);
	
/*------3��ȡ������------*/	
	_nop_();
}
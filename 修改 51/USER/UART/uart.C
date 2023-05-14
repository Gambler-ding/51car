#include "stc15w4k.h"
#include "uart.h"
#include "adc.h"
#include <intrins.h>
/*----------------series define------------------------------*/

/*定时器3用作串口3的波特率发生器,定时器4用作串口4的波特率发生器*/
//#define FOSC 11059200L          //系统频率
#define FOSC 24000000L //18432000L
#define BAUD 9600               //串口波特率
#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验
#define PARITYBIT EVEN_PARITY    //定义校验位

#define S3RI  0x01              //S3CON.0
#define S3TI  0x02              //S3CON.1
#define S3RB8 0x04              //S3CON.2
#define S3TB8 0x08              //S3CON.3
#define S3_S0 0x02              //P_SW2.1

#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define S4RB8 0x04              //S4CON.2
#define S4TB8 0x08              //S4CON.3
#define S4_S0 0x04              //P_SW2.2


u8 bdata UartFlag;
sbit Uart1BusyStatus = UartFlag ^ 0;
sbit Uart2BusyStatus = UartFlag ^ 1;
sbit Uart3BusyStatus = UartFlag ^ 2;
sbit Uart4BusyStatus = UartFlag ^ 3;
sbit UartFirstByte   = UartFlag ^ 4;
sbit HostCommand     = UartFlag ^ 5;

u8 idata UART_cnt;
u8 idata UARTbuffer[10];    /*10byte*/

sbit Uart1RXD         = P3 ^ 0;    /*PIN13,??1??*/
sbit Uart1TXD         = P3 ^ 1;    /*PIN14,??1??*/

void uartDelayMs( u16 delay_time)
{
u8 idata delay_cnt;
while(delay_time--)
       {
       for(delay_cnt=0;delay_cnt<200;delay_cnt++)
             {
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
             }
       _nop_();
       }
_nop_();
}
/*----------------------------------------------------------------------------------*/
void UART1(void) interrupt 4           /*中断接受数据*/
{
	u8 idata character;
	if (RI)
	{
		HostCommand = 1; 				//设置标志
		RI = 0;							//清除RI位
		character = SBUF;
		if(character==0x3f)			{IAP_CONTR = 0x60;SBUF=0x0a;}//复位到IAP;
		if(UartFirstByte == 1)   	{UARTbuffer[UART_cnt++] = character; }
		if(character==0xaa)     	{UartFirstByte = 1;UART_cnt = 0;}
		if(character==0xbb)      	{UartFirstByte = 0;HostCommand = 0;}
		
	}
	if (TI)
	{
		TI=0; 							//清除TI位
		
	}
	HostCommand = 0; 				//清忙标志
	//UART1send_Abyte(character);
}

void UART1send_Abyte(u8 character)	/*发送u8数据*/
{
	while(HostCommand);
	ACC=character;											//获取校验位P(PSW.0)
	if(P)
	{
		#if (PARITYBIT==ODD_PARITY)
		TB8=0;               	 //设置校验位为0
		#elif (PARITYBIT==EVEN_PARITY)
		TB8 = 1;               //设置校验位为1
		#endif
	}
	else
	{
		#if (PARITYBIT == ODD_PARITY)
		TB8 = 1;             		//设置校验位为1
		#elif (PARITYBIT==EVEN_PARITY)
		TB8 = 0;              	//设置校验位为0
		#endif
	}
	HostCommand=1;
	SBUF=ACC;
}
void UART1send_AString(u8 *p)
{
	while(*p)
	{
		UART1send_Abyte(*p);
		p++;
	}
}
void UART1send_AFloat(float f)
{
	u8 idata  i;
	char farray[4] = {0};
	*(float*)farray = f;
	for(i=0;i<4;i++)
	UART1send_Abyte(farray[3-i]);
}
void Uart1Init(void)		//9600bps@30MHz 0.03%误差
{
	#if (PARITYBIT == NONE_PARITY)
	SCON = 0x50;																		//8位可变波特率
	#elif (PARITYBIT==ODD_PARITY)||(PARITYBIT==EVEN_PARITY)||(PARITYBIT == MARK_PARITY)
	SCON = 0xd0; 				//9位可变波特率，校验位初始为1
	#elif (PARITYBIT==SPACE_PARITY)
	SCON = 0xd2; 																	//9位可变波特率，校验位初始为0
	#endif
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xF3;			//设置定时初始值
	TH1 = 0xFC;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	UartFirstByte = 0;
	ES = 1;
	EA = 1;
}















#include "stc15w4k.h"
#include "uart.h"
#include <intrins.h>
/*----------------series define------------------------------*/

/*定时器3用作串口3的波特率发生器,定时器4用作串口4的波特率发生器*/
//#define FOSC 11059200L          //系统频率
#define FOSC 24000000L //18432000L
#define BAUD 115200               //串口波特率
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

u8 data UART_cnt;
u8 idata UARTbuffer[10];    /*10byte*/

sbit Uart1RXD         = P3 ^ 0;    /*PIN13,??1??*/
sbit Uart1TXD         = P3 ^ 1;    /*PIN14,??1??*/

void uartDelayMs( u16 delay_time)
{
u16 delay_cnt;
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
	u8 character;
	RI = 0;character = SBUF;

	if(UartFirstByte == 1)   {UARTbuffer[UART_cnt++] = character; }

	if(character==0xaa)      {UartFirstByte = 1;UART_cnt = 0;HostCommand=1}

	if(character==0xbb)      {UartFirstByte = 0;HostCommand = 0;}

}

void UART1send_Abyte(u8 character)	/*发送u8数据*/
{
	while(!HostCommand);
	HostCommand = 1;
	ES = 0;							/*关闭串口保证设置*/
	SBUF = character;				/*给SBUF赋值*/
	while(!TI) ;					/*有中断时才可以运行*/
	TI = 0;							/*中断TI请求开启*/
	ES = 1;							/*开启串口保证设置*/
	uartDelayMs(1);
	HostCommand = 0;
}
void UART1send_AString(u8 *p)
{
	while(!HostCommand);
	HostCommand = 1;
	while(*p)
	{
		UART1send_Abyte(*p);
		p++;
	}
	HostCommand = 0;
}
void Uart1Init(void)		//9600bps@24MHz
	{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x8F;			//设置定时初始值
	TH1 = 0xFD;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	UartFirstByte = 0;
	ES = 1;
	}

 /*-----------------------------------------------------------------------*/     
      

void Uart3( void ) interrupt 17       /*------------UART3 中断服务程序----------------------*/
    {
    if (S3CON & S3RI)
       {
        S3CON &= ~S3RI;         //清除S3RI位
       }
    if (S3CON & S3TI)
       {
        S3CON &= ~S3TI;         //清除S3TI位
        Uart3BusyStatus = 0;               //清忙标志
       }
    }


void Uart3SendData(u8 dat)      /*---------------发送串口数据--------------------------*/
{
    while (Uart3BusyStatus);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
#if (PARITYBIT == ODD_PARITY)
        S3CON &= ~S3TB8;        //设置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        S3CON |= S3TB8;         //设置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S3CON |= S3TB8;         //设置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        S3CON &= ~S3TB8;        //设置校验位为0
#endif
    }
    Uart3BusyStatus = 1;
//    S3BUF = ACC;                //写数据到UART2数据寄存器20200625


//    if (S3CON & S3TI)
//        {
//        S3CON &= ~S3TI;         //清除S3TI位
//        }

    S3BUF = dat;

//    while(S3CON & S3TI) ;
//    S3CON &= ~S3TI;         //清除S3TI位
}
void Uart3Command( u8 *p)
    {
    while(*p != '\0' )
        {
        Uart3SendData( *p);_nop_();uartDelayMs(1);

        p ++;
        }
    Uart3SendData(0x0d);_nop_();uartDelayMs(1);Uart3SendData(0x0a);_nop_();uartDelayMs(1);
    }

void InitUart3(void)      /*----------------初始化UART3----------------------*/
    {
//P_SW2 &= ~S3_S0;_nop_();            //S3_S0=0 (P0.0/RxD3, P0.1/TxD3)
P_SW2 |= S3_S0;_nop_();             //S3_S0=1 (P5.0/RxD3_2, P5.1/TxD3_2)

#if (PARITYBIT == NONE_PARITY)
    S3CON = 0x50;_nop_();               //8位可变波特率

#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S3CON = 0xda;_nop_();               //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    S3CON = 0xd2;_nop_();               //9位可变波特率,校验位初始为0
#endif

    T3L = (65536 - (FOSC/4/BAUD));_nop_();   //设置波特率重装值
    T3H = (65536 - (FOSC/4/BAUD))>>8;_nop_();

    T4T3M |= 0x02;_nop_();              //定时器3为1T模式
    T4T3M |= 0x08;_nop_();              //定时器3开始计时

    IE2 = 0x08;_nop_();                 //使能串口3中断
    }

/*----------------UART3 程序over----------------------*/

/*----------------UART4 程序--------------------------*/
void Uart4() interrupt 18
    {
    if (S4CON & S4RI)
        {
        S4CON &= ~S4RI;         //??S4RI?
        }
    if (S4CON & S4TI)
        {
        S4CON &= ~S4TI;         //??S4TI?
        Uart4BusyStatus = 0;               //????
        }
    }


void Uart4SendData(u8 dat)
{
    while (Uart4BusyStatus);               //???????????
    ACC = dat;                  //?????P (PSW.0)
    if (P)                      //??P??????
    {
#if (PARITYBIT == ODD_PARITY)
        S4CON &= ~S4TB8;        //??????0
#elif (PARITYBIT == EVEN_PARITY)
        S4CON |= S4TB8;         //??????1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S4CON |= S4TB8;         //??????1
#elif (PARITYBIT == EVEN_PARITY)
        S4CON &= ~S4TB8;        //??????0
#endif
    }
    Uart4BusyStatus = 1;
    S4BUF = ACC;                //????UART4?????
}


void Uart4SendString(char *s)
{
    while (*s)                  //?????????
    {
        Uart4SendData(*s++);uartDelayMs(10);
    }
}

void InitUart4(void)
{
//P_SW2 &= ~S4_S0;            //S4_S0=0 (P0.2/RxD4, P0.3/TxD4)
P_SW2 |= S4_S0;             //S4_S0=1 (P5.2/RxD4_2, P5.3/TxD4_2)

#if (PARITYBIT == NONE_PARITY)
    S4CON = 0x50;               //8??????
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S4CON = 0xda;               //9??????,??????1
#elif (PARITYBIT == SPACE_PARITY)
    S4CON = 0xd2;               //9??????,??????0
#endif

    T4L = (65536 - (FOSC/4/BAUD));   //????????
    T4H = (65536 - (FOSC/4/BAUD))>>8;
    T4T3M |= 0x20;              //???4?1T??
    T4T3M |= 0x80;              //???4????
    IE2 = 0x10;                 //????4??

}
/*----------------UART4 程序over----------------------*/














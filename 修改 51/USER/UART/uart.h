#ifndef _UART_H_
#define _UART_H_
extern unsigned char UartFlag;
extern unsigned char idata UARTbuffer[10];    /*10byte*/
void UART1send_AString(u8 *p);		/*发送一个字符串*/
void UART1send_Abyte(u8 character);	/*发送一个字符*/
void Uart1Init(void);		//9600bps@18.432MHz
void Uart3SendData(u8 dat);      /*---------------发送串口数据--------------------------*/
void Uart3Command( u8 *p);
void InitUart3(void);      /*----------------初始化UART3----------------------*/
void Uart4SendData(u8 dat);
void Uart4SendString(char *s);
void InitUart4(void);
#endif


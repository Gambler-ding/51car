//  文 件 名   : oled.h
//  版 本 号   : v1.0
//  作    者   : cl
//  完成日期   : 2019-0518
#ifndef _OLED_H_
#define _OLED_H_

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define OLED_SCLK_Clr() OLED_SCL=0;_nop_();;_nop_();_nop_();_nop_();_nop_()
#define OLED_SCLK_Set() OLED_SCL=1;_nop_();_nop_();_nop_();_nop_();_nop_()

#define OLED_SDIN_Clr() OLED_SDIN=0;_nop_();_nop_();_nop_();_nop_();_nop_()
#define OLED_SDIN_Set() OLED_SDIN=1;_nop_();_nop_();_nop_();_nop_();_nop_()
void delay_ms(unsigned int ms);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
//OLED相关函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
void floattochar(float value,char* cSendBuff,int Decimals);
extern const unsigned char code F6x8[][6];
extern const unsigned char code F8x16[];
extern const unsigned char code Hzk[][32];
extern const unsigned char code BMP1[];
extern const unsigned char code BMP2[];

#endif /*_OLED_H_*/



/*signal INPUT PIN is 5,ADC4   */
#include "stc15w4k.h"
#include "oled.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include <intrins.h>

#define  uchar unsigned char
#define  uint unsigned int

#define ADC_POWER 0x80   /*ADC power control bit*/
#define ADC_FLAG  0x10   /*ADC complete flag*/
#define ADC_START 0x08   /*ADC start control bit*/
#define ADC_SPEEDHH 0x60 /*90 clocks*/
#define ADC_SPEEDH  0x40 /*180 clocks*/
#define ADC_SPEEDL  0x20 /*360 clocks*/
#define ADC_SPEEDLL 0x00 /*540 clocks*/
u8 idata asc_buffer[10];
u8 AdcCounter = 0;
u8 bdata AdcFlag;  
u16 data AdcValue;			  
sbit AdcOver = AdcFlag ^ 0;

void ADC_isr(void) interrupt 5  using 1  /*get ADC result ,中断向量地址002BH*/
     {
     ADC_CONTR &= ~ADC_FLAG;   /*clear ADC complte flag*/
	 AdcValue = ADC_RES;AdcValue &= 0x0003;AdcValue *= 256;
     AdcValue += ADC_RESL ;

     AdcOver = 1;
     }

void StartADC( uchar ADC_ch)
     {
     ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | ADC_ch;
     }

void initialADC( uchar ADC_ch)
{

/*--------端口模式设置-------*/
/*  PxM1  PxM0       模式    */
/*    0     0     准双向口   */
/*    0     1     强推挽输出 */
/*    1     0     高阻输入   */
/*    1     1     开漏输出   */
/*---------------------------*/

	 switch(ADC_ch)
	 	{
		case 0:P1M1 = 0x01;P1M0 = 0x00;break; /*P1.7-P1.1准双向口,P1.0高阻输入*/
		case 1:P1M1 = 0x02;P1M0 = 0x00;break; /*P1.7-P1.2准双向口,P1.1高阻输入,P1.0准双向口*/
		case 2:P1M1 = 0x04;P1M0 = 0x00;break; /*P1.7-P1.3准双向口,P1.2高阻输入,P1.1-P1.0准双向口*/
		case 3:P1M1 = 0x08;P1M0 = 0x00;break; /*P1.7-P1.4准双向口,P1.3高阻输入,P1.2-P1.0准双向口*/
		case 4:P1M1 = 0x10;P1M0 = 0x00;break; /*P1.7-P1.5准双向口,P1.4高阻输入,P1.3-P1.0准双向口*/
		case 5:P1M1 = 0x20;P1M0 = 0x00;break; /*P1.7-P1.6准双向口,P1.5高阻输入,P1.4-P1.0准双向口*/
		case 6:P1M1 = 0x40;P1M0 = 0x00;break; /*P1.7准双向口,P1.6高阻输入,P1.5-P1.0准双向口*/
		case 7:P1M1 = 0x80;P1M0 = 0x00;break; /*P1.7高阻输入,P1.6-P1.0准双向口*/
		default:P1M1 = 0xff;P1M0 = 0x00;	  /*P1.7-P1.0高阻输入*/
		}
     P1 = 0xff;     /*20190831*/
	 switch(ADC_ch)
	 	{
		case 0:P1ASF = 0x01;break; /*P1.0口作为模拟输入口*/
		case 1:P1ASF = 0x02;break; /*P1.1口作为模拟输入口*/
		case 2:P1ASF = 0x04;break; /*P1.2口作为模拟输入口*/
		case 3:P1ASF = 0x08;break; /*P1.3口作为模拟输入口*/
		case 4:P1ASF = 0x10;break; /*P1.4口作为模拟输入口*/
		case 5:P1ASF = 0x20;break; /*P1.5口作为模拟输入口*/
		case 6:P1ASF = 0x40;break; /*P1.6口作为模拟输入口*/
		case 7:P1ASF = 0x80;break; /*P1.7口作为模拟输入口*/
		default:P1ASF = 0xff;	   /*P1.7-P1.0作为模拟输入口*/
		}

//     ADC_RES =0;
     ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | ADC_ch;   /*enable ADC*/
     CLK_DIV |= 0x20;      /*ADRJ=1,2bit+8bit ADC数据格式*/
     EADC = 1;  /*ADC转换中断 允许*/
	 AdcOver = 0;

}
void f_to_a(float x )
        {
	unsigned char data i,j,p;
	unsigned char data user_buffer[8];
        unsigned long data long_register;
        p=0;
        if(x < 0.0) {asc_buffer[p] = '-';x *= -1;}
        else         {asc_buffer[p] = '+';}

        p++;

        long_register=x;

        x=x-long_register;

        j=0;
        while(1)
                {
                user_buffer[j]=long_register%10;

                if(long_register == 0)
                       {
                       if(j==0)  { j++;}
                       break;
                       }

                long_register /= 10;
                 j++;
                }
        for(i=j;i<8;i++)
                {
                user_buffer[i]=x*10;
                x *= 10;
                x -= user_buffer[i];
                }
	for(i=0;i<j;i++)
	        {
		asc_buffer[p] = user_buffer[j-i-1]+'0';
                p++;
	        }
        asc_buffer[p] = '.';p++;
        for(i=j;i<8;i++)
                {
                asc_buffer[p] = user_buffer[i]+'0';p++;
                }
         }

void power_show()
{
		u16 idata powerAdcBuffer[8];
	    u8 cnt_i=0xff;
		float floatReg=0.0,powerVolts;
		initialADC( 7 );
	  	StartADC( 7 );
	  	while ( AdcOver == 0 )  {_nop_();}
	  	AdcOver = 0;
		powerAdcBuffer[AdcCounter] = AdcValue;
	  	
		AdcValue = 0;
      	for(cnt_i=0;cnt_i<8;cnt_i++)
	  		{
	  		AdcValue += powerAdcBuffer[cnt_i];
	  		}
		AdcValue /= 8; 
		floatReg = AdcValue;floatReg *= 5;floatReg /= 512;

		powerVolts = floatReg;
		f_to_a(floatReg);
	  for(cnt_i=1;cnt_i<5;cnt_i++)
	  		{
	  		OLED_ShowChar(cnt_i*8,6,asc_buffer[cnt_i],16);DelayMs(1);
	  		}
	 	_nop_();
		OLED_ShowString(40,6,"V/",16);

		
		AdcCounter ++;AdcCounter &= 0x07;
		/*for test*/
		//UART1send_Abyte(AdcCounter + '0');UART1send_Abyte(0x0d);UART1send_Abyte(0x0a);
}









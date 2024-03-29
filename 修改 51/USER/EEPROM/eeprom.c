#include "eeprom.h"
#include <intrins.h>

#define CMD_IDLE 	0	//空闲模式
#define CMD_READ 	1	//IAP字节读命令
#define CMD_PROGRAM 2	//IAP字节编程命令
#define CMD_ERASE 	3	//IAP扇区擦除命令

#define ENABLE_IAP 0x80

#define IAP_ADRRESS 0x0400

/****************
关闭IAP
*****************/
void IapIdle(void)
{
	IAP_CONTR=0;		//关闭IAP功能
	IAP_CMD=0;			//清除命令寄存器
	IAP_TRIG=0;			//清除触发寄存器
	IAP_ADDRH=0x80;		//将地址设置到非IAP区域
	IAP_ADDRL=0;		
}

/****************
从ISP/IAP/EEPROM区域读取一个字节
****************/

u8 IapReadByte(u16 addr)
{
	u8 dat;
	
	IAP_CONTR=ENABLE_IAP;	//使能IAP
	IAP_CMD=CMD_READ;		//设置IAP命令
	IAP_ADDRL=addr;			//设置IAP低地址
	IAP_ADDRH=addr>>8;		//设置IAP高地址
	IAP_TRIG=0x5a;			//写触发命令(0x5a)
	IAP_TRIG=0xa5;			//写触发命令(0x5a)
	_nop_();				//等待ISP/IAP/EEPROM操作完成
	dat=IAP_DATA;			//读ISP/IAP/EEPROM数据
	IapIdle();				//关闭IAP功能
	
	return dat;				//返回
}

/***************
写一字节数据到ISP/IAP/EEPROM区域
***************/
void IapProgramByte(u16 addr,u8 dat)
{
	IAP_CONTR=ENABLE_IAP;	//使能IAP
	IAP_CMD=CMD_PROGRAM;	//设置IAP命令
	IAP_ADDRL=addr;			//设置IAP低地址
	IAP_ADDRH=addr>>8;		//设置IAP高地址_
	IAP_DATA=dat;			//写ISP/IAP/EEPROM数据
	IAP_TRIG=0x5a;			//写触发命令(0x5a)
	IAP_TRIG=0xa5;			//写触发命令(0x5a)
	_nop_();				//等待ISP/IAP/EEPROM操作完成
	IapIdle();				//关闭IAP功能
}
/***************
扇区擦除
***************/
void IapEraseSector(u16 addr)
{
	IAP_CONTR=ENABLE_IAP;	//使能IAP
	IAP_CMD=CMD_ERASE;	//设置IAP命令
	IAP_ADDRL=addr;			//设置IAP低地址
	IAP_ADDRH=addr>>8;		//设置IAP高地址_
	IAP_TRIG=0x5a;			//写触发命令(0x5a)
	IAP_TRIG=0xa5;			//写触发命令(0x5a)
	_nop_();				//等待ISP/IAP/EEPROM操作完成
	IapIdle();				//关闭IAP功能
}
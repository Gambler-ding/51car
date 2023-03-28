#include "eeprom.h"
#include <intrins.h>

#define CMD_IDLE 	0	//����ģʽ
#define CMD_READ 	1	//IAP�ֽڶ�����
#define CMD_PROGRAM 2	//IAP�ֽڱ������
#define CMD_ERASE 	3	//IAP������������

#define ENABLE_IAP 0x80

#define IAP_ADRRESS 0x0400

/****************
�ر�IAP
*****************/
void IapIdle(void)
{
	IAP_CONTR=0;		//�ر�IAP����
	IAP_CMD=0;			//�������Ĵ���
	IAP_TRIG=0;			//��������Ĵ���
	IAP_ADDRH=0x80;		//����ַ���õ���IAP����
	IAP_ADDRL=0;		
}

/****************
��ISP/IAP/EEPROM�����ȡһ���ֽ�
****************/

u8 IapReadByte(u16 addr)
{
	u8 dat;
	
	IAP_CONTR=ENABLE_IAP;	//ʹ��IAP
	IAP_CMD=CMD_READ;		//����IAP����
	IAP_ADDRL=addr;			//����IAP�͵�ַ
	IAP_ADDRH=addr>>8;		//����IAP�ߵ�ַ
	IAP_TRIG=0x5a;			//д��������(0x5a)
	IAP_TRIG=0xa5;			//д��������(0x5a)
	_nop_();				//�ȴ�ISP/IAP/EEPROM�������
	dat=IAP_DATA;			//��ISP/IAP/EEPROM����
	IapIdle();				//�ر�IAP����
	
	return dat;				//����
}

/***************
дһ�ֽ����ݵ�ISP/IAP/EEPROM����
***************/
void IapProgramByte(u16 addr,u8 dat)
{
	IAP_CONTR=ENABLE_IAP;	//ʹ��IAP
	IAP_CMD=CMD_PROGRAM;	//����IAP����
	IAP_ADDRL=addr;			//����IAP�͵�ַ
	IAP_ADDRH=addr>>8;		//����IAP�ߵ�ַ_
	IAP_DATA=dat;			//дISP/IAP/EEPROM����
	IAP_TRIG=0x5a;			//д��������(0x5a)
	IAP_TRIG=0xa5;			//д��������(0x5a)
	_nop_();				//�ȴ�ISP/IAP/EEPROM�������
	IapIdle();				//�ر�IAP����
}
/***************
��������
***************/
void IapEraseSector(u16 addr)
{
	IAP_CONTR=ENABLE_IAP;	//ʹ��IAP
	IAP_CMD=CMD_ERASE;	//����IAP����
	IAP_ADDRL=addr;			//����IAP�͵�ַ
	IAP_ADDRH=addr>>8;		//����IAP�ߵ�ַ_
	IAP_TRIG=0x5a;			//д��������(0x5a)
	IAP_TRIG=0xa5;			//д��������(0x5a)
	_nop_();				//�ȴ�ISP/IAP/EEPROM�������
	IapIdle();				//�ر�IAP����
}
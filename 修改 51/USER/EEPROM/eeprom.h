#ifndef _EEPROM_H_
#define _EEPROM_H_
#include "stc15w4k.h"

u8 IapReadByte(u16 addr);	//������
void IapProgramByte(u16 addr,u8 dat);	//д������
void IapEraseSector(u16 addr);	//��������

#endif


#ifndef _EEPROM_H_
#define _EEPROM_H_
#include "stc15w4k.h"

u8 IapReadByte(u16 addr);	//读数据
void IapProgramByte(u16 addr,u8 dat);	//写入数据
void IapEraseSector(u16 addr);	//擦除数据

#endif


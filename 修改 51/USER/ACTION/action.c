#include "stc15w4k.h"
#include "action.h"
#include "eeprom.h"
#include "motor.h"
//test for track
//track eight lines
void jump_line()
{
	while(frontPortData==0x7f)track_PID(50);
}
void test_track()
{
	u8 n=8;
	while(n)
	{
		track_PID(50);
		if(frontPortData==0x7f)
		{
			n--;
			jump_line();
		}
	}
	/*IapEraseSector(0x6600);
	OLED_ShowNum(0,6,IapReadByte(0x6600),4,16);*/
}

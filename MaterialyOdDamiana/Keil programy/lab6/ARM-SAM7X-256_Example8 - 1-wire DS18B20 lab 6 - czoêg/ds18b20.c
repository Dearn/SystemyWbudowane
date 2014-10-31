#include "common.h"
#include "one_wire.h"
#include "ds18b20.h"
//#include "lcd.h"



//function commands
#define ONE_WIRE_CMD_CONVERT_T          0x44
#define ONE_WIRE_CMD_READ_SCRATCHPAD    0xBE
#define ONE_WIRE_CMD_WRITE_SCRATCHPAD   0x4E
#define ONE_WIRE_CMD_COPY_SCRATCHPAD    0x48

unsigned char COUNT_REMAIN;
unsigned char COUNT_PER;

void ds18b20ConvertT(void)
{
	if(owPresence()==OK)
	{
		owWrite(ONE_WIRE_CMD_SKIP_ROM);
		owWrite(ONE_WIRE_CMD_CONVERT_T);
		owStrongPullupOn();
	}
}






short ds18b20ReadTempNoRom(void)
{
	owStrongPullupOff();
	if(owPresence())
	{
		unsigned char tempL;
		unsigned char tempH;
		volatile unsigned char Th;
		volatile unsigned char Tl;
		volatile unsigned char res1;
		volatile unsigned char res2;
		volatile unsigned char crc;
		owWrite(ONE_WIRE_CMD_SKIP_ROM);
		owWrite(ONE_WIRE_CMD_READ_SCRATCHPAD);
		tempL = owRead();
		tempH = owRead();
		Th = owRead();
		Tl = owRead();
		res1 = owRead();
		res2 = owRead();
		COUNT_REMAIN = owRead();
		COUNT_PER = owRead();
		crc = owRead();
		owPresence();
//		COUNT_REMAIN = COUNT_REMAIN/2;
//		COUNT_PER = (COUNT_PER-COUNT_REMAIN)/COUNT_PER);
//		LCDPutunsigned char(0x30+((COUNT_PER-COUNT_REMAIN)/COUNT_PER)*10000, 40, 10, LARGE, BLACK, YELLOW);

		{short temperature = ( (unsigned short)tempH << 8) | tempL;
		return(temperature);
		}
	}
	return(0x7FFF);
}


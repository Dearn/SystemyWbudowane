#include "common.h"
#include "one_wire.h"

//ustawienie pina do podlaczenia zewnetrznego czujnika temperatury
#define ONE_WIRE_PIO					AT91C_BASE_PIOA
#define ONE_WIRE_MASK					AT91C_PIO_PA28


#define ONE_WIRE_INIT					{ONE_WIRE_PIO->PIO_PER = ONE_WIRE_MASK;\
										ONE_WIRE_PIO->PIO_SODR = ONE_WIRE_MASK;\
										ONE_WIRE_PIO->PIO_MDER = ONE_WIRE_MASK;\
										ONE_WIRE_PIO->PIO_OER =  ONE_WIRE_MASK;\
										ONE_WIRE_PIO->PIO_PPUER = ONE_WIRE_MASK; \
										}


#define ONE_WIRE_IS_HIGH				(ONE_WIRE_PIO->PIO_PDSR & ONE_WIRE_MASK)
#define ONE_WIRE_SET_HIGH				ONE_WIRE_PIO->PIO_SODR = ONE_WIRE_MASK;
#define ONE_WIRE_SET_LOW				ONE_WIRE_PIO->PIO_CODR = ONE_WIRE_MASK;
#define ONE_WIRE_STRONG_PULLUP_ON		{ONE_WIRE_SET_HIGH; ONE_WIRE_PIO->PIO_MDDR = ONE_WIRE_MASK;}
#define ONE_WIRE_STRONG_PULLUP_OFF		{ONE_WIRE_PIO->PIO_MDER = ONE_WIRE_MASK;}


char owCrcUpdate(char crc, char data);


char owCheckCrc(char *rom)
{
	char crc=0, i;
	for(i=0;i<7;i++)
	{
		crc = owCrcUpdate(crc,rom[i]);
	}
	if(crc == rom[7]) return(OK); else return(FAIL);
}


char owCrcUpdate(char crc, char data)
{
	char i;
	crc = crc ^ data;
	for(i=0;i<8;i++)
	{
		if(crc & 0x01)
			crc = (crc >> 1) ^ 0x8C;
		else
			crc >>= 1;
	}
	return(crc);
}



char owPresence(void)
{
    ONE_WIRE_SET_LOW;
    delay_us(500);
    ONE_WIRE_SET_HIGH;
    delay_us(70);
    if(ONE_WIRE_IS_HIGH)
    {
        return(FAIL);
    }
    else
    {
        delay_us(150);
        return(OK);
    }
}




void owWrite(char dat)
{
    char i;
    for(i=0;i<8;i++)
    {
        if(dat&0x01) //if least significant bit is one
        {
            //"write 1"
            ONE_WIRE_SET_LOW;        //master pulls down
            delay_us(2);       //minimum 1 us
            ONE_WIRE_SET_HIGH;        //release bus
            delay_us(70);    //60 to 120 us delay
        }
        else //least significant bit is zero
        {
            //"write 0"
            ONE_WIRE_SET_LOW;        //master pulls down
            delay_us(70);    //rest of the time slot (sum is 60-120 us)
            ONE_WIRE_SET_HIGH;        //master releases the bus
            delay_us(4);       //minimum 1 us
        }
        dat=dat>>1; //shift dat right
    }
    delay_us(20);
}



char owRead(void)
{
    char dat=0,i;
    for(i=0;i<8;i++)
    {
        dat=dat>>1;				//shift temporary value right
        ONE_WIRE_SET_LOW;		//initiate time slot
        delay_us(4);			//pull low for at least 1us
        ONE_WIRE_SET_HIGH;		//release bus to allow read
        delay_us(9);			//for 15us after time slot init data is valid
        if(ONE_WIRE_IS_HIGH)	//read current bit
        {
            dat+=0x80;			//if bit is 1 add to the most significant bit 1
        }
        delay_us(70);			//finish the time slot
    }
    delay_us(20);
    return(dat);
}


char owReadRom(char *rom)
{
	char i;
	if(owPresence())
	{
		owWrite(ONE_WIRE_CMD_READ_ROM);
		for(i=0;i<8;i++)
		{
			rom[i] = owRead();
		}
		owPresence();
		if(owCheckCrc(rom)==OK)
		{
			return(OK);
		}
		else
		{
			return(FAIL);
		}
	}
	else
	{
		return(FAIL);
	}
}

void owStrongPullupOn(void)
{
	ONE_WIRE_STRONG_PULLUP_ON;
}

void owStrongPullupOff(void)
{
	ONE_WIRE_STRONG_PULLUP_OFF;
}

void owInit(void)
{
	ONE_WIRE_INIT;
}


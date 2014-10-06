/******************************************************************************/
// Program utworzony do potrzeb laboratorium IISI Systemy Wbudowane
// do wykorzystania w celach dydaktycznych podczas zajec laboratoryjnych
/******************************************************************************/



#include "common.h"
#include <stdio.H>                        //standard I/O
#include <math.H>                        //math library
#include "lcd.h"
#include "delay.h"
#include "one_wire.h"
#include "adc.h"
#include "adc.c"

extern void InitSpi(void);
extern void InitLcd(void);
extern void LCDClearScreen(void);

extern void WriteSpiCommand(unsigned int data);
extern void WriteSpiData(unsigned int data);

extern void LCDClearScreen(void);
extern void LCDSetXY(int x, int y);
extern void LCDSetPixel(int  x, int  y, int  color);
extern void LCDSetLine(int x1, int y1, int x2, int y2, int color);
extern void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
extern void LCDSetCircle(int x0, int y0, int radius, int color);
extern void LCDPutChar(char c, int  x, int  y, int size, int fcolor, int bcolor);
extern void LCDPutString (char *lcd_string, const char *font_style, unsigned char x, unsigned char y,
                   unsigned char fcolor, unsigned char bcolor);
extern void LCDPutStr(char *pString, int  x, int  y, int Size, int fColor, int bColor);

char ciag[];
static volatile unsigned long status_IRQ;


volatile short temperatura_ADC;
float temperatura,wart_adc,temp;
char rom[8]; //tablica 8 bajtów, które beda wypelnione poprzez dokonanie odczytu z termometru
//glosnik
#define LED_PIO		AT91C_BASE_PIOB			//adres kontrolera PIOB

#define BUZ_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20|AT91C_PIO_PB19) //inicjowanie glosnika
#define BUZ_INIT	{LED_PIO->PIO_PER = BUZ_MASK; LED_PIO->PIO_OER = BUZ_MASK; LED_PIO->PIO_OWER = BUZ_MASK;}

#define BUZ_ON  AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB19; // jest dzwiek 
#define BUZ_OFF AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB19; //  nie ma dzwieku

//
//  Main Program
// 
int main(void) {
	static int ye;			
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB);

BUZ_INIT;	//inicjacja do glosnika
	
	// ARM7X256 Leds Definition


  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
					 
	
  
  // Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();

	// clear the screen
   LCDClearScreen();

	owInit(); //inicjzacja interfejsu OneWire
	
	



  // draw a filled box
  //LCDSetRect(10, 10, 40, 40, FILL, RED);
  //LCDSetRect(10, 50, 40, 80, FILL, GREEN);
  //LCDSetRect(10, 90, 40, 120, FILL, BLUE);


  // draw a string
  //LCDPutStr("Hello world", 15, 15, LARGE, PINK, BLACK);


	
	while(1) 
	{
		//Temperatura wewnetrznego czujnika
		wart_adc=GetAdcChanel(ADC_CHN_6);
		temp=(wart_adc*0.305)/1023;
		if(((temp*10/0.305)+20)>=25)
			{		
				LCDPutStr("ALARM - POZAR",20,10,SMALL,BLACK,RED);
				BUZ_ON;
				delay_us(1000); //Generuje dzwiek
				BUZ_OFF;
				delay_us(1000);
			}
		sprintf(ciag,"Temp=%3.2f",(temp*10/0.305)+20);
		LCDPutStr(ciag,30,10,MEDIUM,BLACK,MAGENTA);
		LCDClearScreen();
	};
	
  }
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

//char ciag[];
//static volatile unsigned long status_IRQ;
int a,dr,i,sz;
int szyb;
float U;
char s[30];

//sterowanie
        #define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
				#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)

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
	
	a=0;
  szyb=1;
  dr=2;
	i=0;

  // draw a filled box
  //LCDSetRect(10, 10, 40, 40, FILL, RED);
  //LCDSetRect(10, 50, 40, 80, FILL, GREEN);
  //LCDSetRect(10, 90, 40, 120, FILL, BLUE);


  // draw a string
  //LCDPutStr("Hello world", 15, 15, LARGE, PINK, BLACK);

	while(1)
	{
	LCDSetRect(104, a+10, 129, a+40, NOFILL, GREEN);
		LCDSetRect(104, a+11, 129, a+16, NOFILL, GREEN);
		LCDSetRect(104, a+34, 129, a+39, NOFILL, GREEN);
  LCDSetRect(104, a+23, 84, a+27, NOFILL, GREEN);
		
	if(LEFT_KEY_DOWN && a>=10)
	{
			a=a-10;
	}
	if(RIGHT_KEY_DOWN && a<=70)
	{
       a=a+10;             
  }
	
	U=GetAdcChanel(ADC_CHN_7);
	U=((U*3.3)/1023);
		if(U>=0 && U<=1)
			{		
				szyb=2;
				BUZ_ON;
				delay_us(100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(100);
			}
	 if(U>=1 && U<=2)
			{		
				szyb=4;
				BUZ_ON;
				delay_us(600); //Generuje dzwiek
				BUZ_OFF;
				delay_us(600);
			}
	if(U>=2 && U<=3.3)
			{		
				szyb=6;
				BUZ_ON;
				delay_us(1100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(1100);
			}
	sprintf(s, "speed+%i", szyb);
	LCDPutStr(s, 1, 0, SMALL, WHITE, BLACK);
			
	dr=3*szyb;
	i=i+5;
	if(i>=131){i=0;}
	if(dr+i>=0 && dr+i<=20)
		{		
				LCDSetRect(0, 0, 20, 2, FILL, CYAN);
				LCDSetRect(0, 127, 20, 129, FILL, CYAN);
		}
  if(dr+i>=21 && dr+i<=40)
		{		
				LCDSetRect(0, 0, 40, 2, FILL, CYAN);
				LCDSetRect(0, 127, 40, 129, FILL, CYAN);
		}
	if(dr+i>=41 && dr+i<=60)
		{		
				LCDSetRect(20, 0, 60, 2, FILL, CYAN);
				LCDSetRect(20, 127, 60, 129, FILL, CYAN);
		}
  if(dr+i>=61 && dr+i<=80)
		{		
				LCDSetRect(40, 0, 80, 2, FILL, CYAN);
				LCDSetRect(40, 127, 80, 129, FILL, CYAN);
		}
  if(dr+i>=81 && dr+i<=100)
		{		
				LCDSetRect(60, 0, 100, 2, FILL, CYAN);
				LCDSetRect(60, 127, 100, 129, FILL, CYAN);
		}
  if(dr+i>=101 && dr+i<=120)
		{		
				LCDSetRect(80, 0, 120, 2, FILL, CYAN);
				LCDSetRect(80, 127, 120, 129, FILL, CYAN);
		}
	if(dr+i>=121 && dr+i<=131)
		{		
				LCDSetRect(100, 0, 120, 2, FILL, CYAN);
				LCDSetRect(100, 127, 120, 129, FILL, CYAN);
		}
	//sz=rand()
	
	
		
		
		
	LCDClearScreen();
	}
	
  }
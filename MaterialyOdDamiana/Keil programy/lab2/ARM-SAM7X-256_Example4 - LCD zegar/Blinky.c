/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
// Program zmodyfikowany do potrzeb laboratorium IISI Systemy Wbudowane
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#define uint16_t  unsigned int;

#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
#include "lcd.h"
#include  "stdio.h" //biblioteka z konwersja
 

void delay_1ms(void) {
  unsigned int n;

  for (n = 0; n < 10000; n++);

}
void delay (int val) {
  unsigned int n;

  for (n = 0; n < val; n++)
		delay_1ms();
}

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
//extern void Delay (unsigned long a);




	
	// ARM7X256 Leds Definition


//maska dla pinow do ktorych podlaczone sa diody, którymi bedziemy sterowac
#define LEDS_MASK	(AT91C_PIO_PB20)
	

//konfiguracja wybranych linii PIO
												  //linie pracuja jako GPIO sterowane przez uzutkownika (a nie przez peryferia) = PIO_PER
												  //ustawienie pinow jako wyjscia = PIO_OER
												  //odblokowanie mozliwosc zapisu za pomoca rejestru ODSR = PIO_OWER
#define LED_INIT	{AT91C_BASE_PIOB->PIO_PER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OWER = LEDS_MASK;}


//definicja ustawiania stanu niskiego wyjscia PB20 = zgaszenie podswietlenia LED wyswietlacza LCD 
#define LED_BCK_OFF		AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB20
//definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD 
#define LED_BCK_ON		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB20

	
 
#define LEDS_OFF		AT91C_BASE_PIOB->PIO_CODR = LEDS_MASK
//definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD 
#define LEDS_ON		AT91C_BASE_PIOB->PIO_SODR = LEDS_MASK
	




//Inputs
#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)
#define LEFT_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB7)==0)

#define INPUTS_INIT {AT91C_BASE_PIOB->PIO_PER = (AT91C_PIO_PB24|AT91C_PIO_PB25); AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); }


//
//
//Zmienne do zegarka
//
int sek=0,min=0,godz=0,n=1000;
char* ws;
char* wm;
char* wg;

//
//  Main Program
// 
int main (void) {

						 
						 
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
	LED_INIT;

	AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); //keys SW1 i SW2 - jako wejscia
						 
						 

	
	
	
  
  // Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();

  // Voltage control (contrast setting)
	WriteSpiCommand(VOLCTR);
	WriteSpiData(-20);    // P1 = 32  volume value (experiment with this value to get the best contrast)
  WriteSpiData(3);   // P2 = 3   resistance ratio (only value that works)

	// clear the screen
   LCDClearScreen();




  // set a few pixels
	/*
  LCDSetPixel(30, 120, RED);
  LCDSetPixel(34, 120, GREEN);
  LCDSetPixel(38, 120, BLUE);
  LCDSetPixel(40, 120, WHITE);
*/

    //  endless loop
    //while (1) 
		//{}


//
//
//           ZEGAREK
//
//
	while(1){
		
		for(;godz<24;godz++)
		{
		for(;min<60;min++)
		{
		for(;sek<60;sek++)
		{
			LCDClearScreen();//czyszczenie ekranu
		
			sprintf(ws, "%d", sek);
			sprintf(wm, "%d", min);
			sprintf(wg, "%d", godz);
			
			LCDPutStr(wg, 60, 5, SMALL, WHITE, BLACK);
			LCDPutStr(wm, 65, 5, SMALL, WHITE, BLACK);
			LCDPutStr(ws, 70, 5, SMALL, WHITE, BLACK);
			delay(n);
		}
		sek=0;
		}
		min=0;
		}
		godz=0;
					}

/*
int TempColor[11] = { WHITE, BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, 
 YELLOW, BROWN, ORANGE, PINK }; 
char *TempChar[11] = { "White", "Black", "Red", "Green", "Blue", "Cyan", 
 "Magenta", "Yellow", "Brown", "Orange", "Pink" }; 


 for (int j = 0; j < 11; j++) {

    // draw a filled box
    LCDSetRect(120, 10, 25, 120, FILL, TempColor[j]);

    // label the color
    LCDPutStr("        ", 5, 40, LARGE, BLACK, BLACK);
    LCDPutStr(TempChar[j], 5, 40, LARGE, YELLOW, BLACK);

    // wait a bit
    delay(100);
   }
*/

/*
void LCDWrite130x130bmp(void);

LCDClearScreen();

   LCDWrite130x130bmp();

    LCDPutStr("This guy is nuts", 115, 2, LARGE, BLACK, CYAN);

    // draw a filled box
    LCDSetRect(90, 70, 75, 120, FILL, YELLOW);

    LCDPutStr("HELP!", 80, 80, SMALL, BLACK, YELLOW);
*/









  // draw some characters
  //LCDPutStr("Czestochowa", 10, 10, SMALL, WHITE, BLACK);

  /*/ draw a string
  LCDPutStr("Hello World", 60, 5, SMALL, WHITE, BLACK);
  LCDPutStr("Hello World", 40, 5, MEDIUM, ORANGE, BLACK);
  LCDPutStr("Hello World", 20, 5, LARGE, PINK, BLACK);

  // draw a filled box
  LCDSetRect(120, 60, 80, 80, FILL, BROWN);

  // draw a empty box
  LCDSetRect(120, 85, 80, 105, NOFILL, CYAN);
  // draw some lines
  LCDSetLine(120, 10, 120, 50, YELLOW);
  LCDSetLine(120, 50, 80, 50, YELLOW);
  LCDSetLine(80, 50, 80, 10, YELLOW);
  LCDSetLine(80, 10, 120, 10, YELLOW);

  LCDSetLine(120, 85, 80, 105, YELLOW);
  LCDSetLine(80, 85, 120, 105, YELLOW);

  // draw a circle
  LCDSetCircle(65, 100, 10, RED);


   Delay(1000);

   // ***************************************************************
    // *  bmp display test - display the Olimex photograph
   // ***************************************************************

   LCDClearScreen();


    LCDPutStr("Hello world", 115, 2, LARGE, BLACK, CYAN);

    // draw a filled box
    LCDSetRect(90, 70, 75, 120, FILL, YELLOW);

    LCDPutStr("Text.", 80, 80, SMALL, BLACK, YELLOW);




   // *****************************
    // *  endless blink loop
   // *****************************
    while (1) {
  
   }*/
  }

/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  

        #define uint16_t  unsigned int;

        #include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
        #include "lcd.h"
        #include <stdio.h>
        #include <math.h>

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
        extern void Delay (unsigned long a);



           
           // ARM7X256 Leds Definition


        //maska dla pinow do ktorych podlaczone sa diody, kt�rymi bedziemy sterowac
        #define LEDS_MASK   (AT91C_PIO_PB20)
           

        //konfiguracja wybranych linii PIO
                                              //linie pracuja jako GPIO sterowane przez uzutkownika (a nie przez peryferia) = PIO_PER
                                              //ustawienie pinow jako wyjscia = PIO_OER
                                              //odblokowanie mozliwosc zapisu za pomoca rejestru ODSR = PIO_OWER
        #define LED_INIT   {AT91C_BASE_PIOB->PIO_PER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OWER = LEDS_MASK;}


        //definicja ustawiania stanu niskiego wyjscia PB20 = zgaszenie podswietlenia LED wyswietlacza LCD
        #define LED_BCK_OFF      AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB20
        //definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD
        #define LED_BCK_ON      AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB20

           
         
        #define LEDS_OFF      AT91C_BASE_PIOB->PIO_CODR = LEDS_MASK
        //definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD
        #define LEDS_ON      AT91C_BASE_PIOB->PIO_SODR = LEDS_MASK
        //Inputs
        #define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
        #define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)
        #define PUSH_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PA14)==0)
        #define INPUTS_INIT {AT91C_BASE_PIOB->PIO_PER = (AT91C_PIO_PB24|AT91C_PIO_PB25); AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); }

        #define PI 3.1415




        //
        //  Main Program
        //
        int main (void) {

           
           
                         
          // Enable the Clock of the PIO
          AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
           
           LED_INIT;

           AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25|AT91C_PIO_PA14); //keys SW1 i SW2 - jako wejscia
                         
                         

           
           
           
         
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





            //  endless loop
            while (1)
              {
								LCDSetLine(0, 65, 130, 65, WHITE);
              if(LEFT_KEY_DOWN)
                   {
                      LCDSetPixel(rand() % 131, rand() % 66, BLUE);
                      LCDSetPixel(rand() % 131, rand() % 66, PINK);
                      LCDSetPixel(rand() % 131, rand() % 66, RED);
                      LCDSetPixel(rand() % 131, rand() % 66, GREEN);
                      LCDSetPixel(rand() % 131, rand() % 66, YELLOW);
    }
              if(RIGHT_KEY_DOWN)
                   {
                      LCDSetPixel(rand() % 131, rand() % 65 + 66, BLUE);
                      LCDSetPixel(rand() % 131, rand() % 65 + 66, PINK);
                      LCDSetPixel(rand() % 131, rand() % 65 + 66, RED);
                      LCDSetPixel(rand() % 131, rand() % 65 + 66, GREEN);
                      LCDSetPixel(rand() % 131, rand() % 65 + 66, YELLOW);
    }
               if(PUSH_STICK)
                    {
    LCDClearScreen();
                    }                  
              }
          }



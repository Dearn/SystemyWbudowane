/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
//#include "..\AT91SAM7X-EK.h"

 
void delay_us(int val) {
  unsigned int n;
  for (n = 0; n < (10*val); n++);
} 

void delay_1ms(void) {
  unsigned int n;

  for (n = 0; n < 10000; n++);

}
void delay (int val) {
  unsigned int n;

  for (n = 0; n < val; n++)
		delay_1ms();
}
  

//
//  Main Program
// 
int main (void) {
		
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB);

	
	
	// ARM7X256 Leds Definition

#define LED_PIO		AT91C_BASE_PIOB			//adres kontrolera PIOB

//#define LED_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20|AT91C_PIO_PB30) //maska dla pinow do ktorych podlaczone sa diody
#define LED_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20|AT91C_PIO_PB19) //bez blue

	
//przejecie kontroli przez PIO
												  //nad liniami wyjscia
												  //ustawienie pinow jako wyjscia
												  //odblokowanie mozliwosc zapisu
												  //za pomoca rejestru ODSR

#define LED_INIT	{LED_PIO->PIO_PER = LED_MASK; LED_PIO->PIO_OER = LED_MASK; LED_PIO->PIO_OWER = LED_MASK;}
#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
#define LED_BCK_TGL (AT91C_BASE_PIOB->PIO_ODSR=AT91C_BASE_PIOB->PIO_ODSR ^ AT91C_PIO_PB20)


#define LED_BCK_OFF		LED_PIO->PIO_CODR = AT91C_PIO_PB20
#define LED_BCK_ON		LED_PIO->PIO_SODR = AT91C_PIO_PB20
//PA7, PA8, PA9, PA14 i PA23
	
	LED_INIT;
  
	


while(1)
{
if (LEFT_KEY_DOWN)
{
LED_BCK_TGL;
delay(10); //opóznienie po wykryciu wcisniecia
//(zabezpieczenie na „zboczu opadajacy” sygnalu – rys. 2a)
//oczekiwanie na puszczenie klawisza
while (LEFT_KEY_DOWN)
{};
delay(10);
//opóznienie po wykryciu puszczenia klawisza
//(zabezpieczenie na „zboczu narastajacym” sygnalu – rys. 2b)
};
};

}

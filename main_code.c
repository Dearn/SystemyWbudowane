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
//int a,dr,i,sz;
//int szyb;
float U;
//char s[30];
int czekaj=200;
int t=1;
int i=0;
int dd=0;

//zrobic timer

int krot=220;
int dlug=520;
int czek=3800;

//sterowanie
#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)

//char rom[8]; //tablica 8 bajtów, które beda wypelnione poprzez dokonanie odczytu z termometru
//glosnik
#define LED_PIO		AT91C_BASE_PIOB			//adres kontrolera PIOB

#define BUZ_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20|AT91C_PIO_PB19) //inicjowanie glosnika
#define BUZ_INIT	{LED_PIO->PIO_PER = BUZ_MASK; LED_PIO->PIO_OER = BUZ_MASK; LED_PIO->PIO_OWER = BUZ_MASK;}

#define BUZ_ON  AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB19; // jest dzwiek 
#define BUZ_OFF AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB19; //  nie ma dzwieku

//Funkcje
void Voice(int t,int dd)
{
				i=0;
				t=t*dd;
				while(i<t)
				{	
					i++;
					U=GetAdcChanel(ADC_CHN_7);
					U=((U*3.3)/1023);
					
					czekaj=U*200;
					
					BUZ_ON;
					delay_us(czekaj); //Generuje dzwiek
					BUZ_OFF;
					delay_us(czekaj); 
				}
}

void puste()
{
					U=GetAdcChanel(ADC_CHN_7);
					U=((U*3.3)/1023);
					
					czekaj=U*czek;					
					delay_us(czekaj);
}

//Przy kazdym znaku nastepuje spacja

void spacja()
{
		puste();
		puste();
		puste();
		puste();
		puste();
		puste();
}

void krotkie()
{
				dd=krot;
				Voice(t,dd);
				puste();
}

void dlugie()
{
				dd=dlug;
				Voice(t,dd);
				puste();
}

//Alfabet

void _A()
{
				krotkie();
				dlugie();
				spacja();
}

void _B()
{				
				dlugie();
				krotkie();
				krotkie();
				krotkie();
				spacja();
}

void _C()
{
				dlugie();
				krotkie();
				dlugie();
				krotkie();
				spacja();
}

void _D()
{
				dlugie();
				krotkie();
				krotkie();
				spacja();
}

void _E()
{
				krotkie();
				spacja();
}

void _F()
{
				krotkie();
				krotkie();
				dlugie();
				krotkie();
				spacja();
}

void _G()
{
				dlugie();
				dlugie();
				krotkie();
				spacja();
}

void _H()
{
				krotkie();
				krotkie();
				krotkie();
				krotkie();
				spacja();
}

void _I()
{
				krotkie();
				krotkie();
				spacja();
}

void _J()
{
				krotkie();
				dlugie();
				dlugie();
				dlugie();
				spacja();
}

void _K()
{
				dlugie();
				krotkie();
				dlugie();
				spacja();
}

void _L()
{
				krotkie();
				dlugie();
				krotkie();
				krotkie();
				spacja();
}

void _M()
{
				dlugie();
				dlugie();
				spacja();
}

void _N()
{
				dlugie();
				krotkie();
				spacja();
}

void _O()
{
				dlugie();
				dlugie();
				dlugie();
				spacja();
}

void _P()
{
				krotkie();
				dlugie();
				dlugie();
				krotkie();
				spacja();
}

void _Q()
{
				dlugie();
				dlugie();
				krotkie();
				dlugie();
				spacja();
}

void _R()
{
				krotkie();
				dlugie();
				krotkie();
				spacja();
}

void _S()
{
				krotkie();
				krotkie();
				krotkie();
				spacja();
}

void _T()
{
				dlugie();
				spacja();
}

void _U()
{
				krotkie();
				krotkie();
				dlugie();
				spacja();
}

void _V()
{
				krotkie();
				krotkie();
				krotkie();
				dlugie();
				spacja();
}

void _W()
{
				krotkie();
				dlugie();
				dlugie();
				spacja();
}

void _X()
{
				dlugie();
				krotkie();
				krotkie();
				dlugie();
				spacja();
}

void _Y()
{
				dlugie();
				krotkie();
				dlugie();
				dlugie();
				spacja();
}

void _Z()
{
				dlugie();
				dlugie();
				krotkie();
				krotkie();
				spacja();
}

//znaki narodowe

void _om()
{							
				krotkie();
				dlugie();
				krotkie();
				dlugie();
				spacja();
}

void _ci()
{
				dlugie();
				krotkie();
				dlugie();
				krotkie();
				krotkie();
				spacja();
}

void _em()
{
				krotkie();
				krotkie();
				dlugie();
				krotkie();
				krotkie();
				spacja();
}

void _ch()
{
				dlugie();
				dlugie();
				dlugie();
				dlugie();
				spacja();
}

void _ni()
{
				dlugie();
				dlugie();
				krotkie();
				dlugie();
				dlugie();
				spacja();
}

void _ly()
{
				krotkie();
				dlugie();
				krotkie();
				krotkie();
				dlugie();
				spacja();
}

void _uu()
{
				dlugie();
				dlugie();		
				dlugie();
				krotkie();
				spacja();
}

void _si()
{
				krotkie();
				krotkie();
				krotkie();
				dlugie();
				krotkie();
				krotkie();
				krotkie();
				spacja();
}

void _zy()
{
				dlugie();
				dlugie();
				krotkie();
				krotkie();
				dlugie();
				krotkie();
				spacja();
}

void _zi()
{
				dlugie();
				dlugie();
				krotkie();
				krotkie();
				dlugie();
				spacja();
}

//Cyfry

void _jeden()
{
			krotkie();
			dlugie();
			dlugie();
			dlugie();
			dlugie();
			spacja();
}

void _dwa()
{
			krotkie();
			krotkie();
			dlugie();
			dlugie();
			dlugie();
			spacja();
}

void _trzy()
{
			krotkie();
			krotkie();
			krotkie();
			dlugie();
			dlugie();
			spacja();
}

void _cztery()
{
			krotkie();
			krotkie();
			krotkie();
			krotkie();
			dlugie();
			spacja();
}

void _piec()
{
			krotkie();
			krotkie();
			krotkie();
			krotkie();
			krotkie();
			spacja();
}

void _szesc()
{
			dlugie();
			krotkie();
			krotkie();
			krotkie();
			krotkie();
			spacja();
}

void _siedem()
{
			dlugie();
			dlugie();
			krotkie();
			krotkie();
			krotkie();
			spacja();
}

void _osiem()
{
			dlugie();
			dlugie();
			dlugie();
			krotkie();
			krotkie();
			spacja();
}

void _dziewiec()
{
			dlugie();
			dlugie();
			dlugie();
			dlugie();
			krotkie();
			spacja();
}

void _zero()
{
			dlugie();
			dlugie();
			dlugie();
			dlugie();
			dlugie();
			spacja();
}

//znaki interpunkcyjne i symbole

void _kropka()
{
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _przecinek()
{
		dlugie();
		dlugie();
		krotkie();
		krotkie();
		dlugie();
		dlugie();
		spacja();
}

void _pojedynczy_apostrof()
{
		krotkie();
		dlugie();
		dlugie();
		dlugie();
		dlugie();
		krotkie();
		spacja();
}

void _cudzyslow()
{
		krotkie();
		dlugie();
		krotkie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _podkreslenie()
{
		krotkie();
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _dwukropek()
{
		dlugie();
		dlugie();
		dlugie();
		krotkie();
		krotkie();
		krotkie();
		spacja();
}

void _srednik()
{
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _znak_zapytania()
{
		krotkie();
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		krotkie();
		spacja();
}

void _wykrzyknik()
{
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		dlugie();
		spacja();
}

void _myslnik()
{
		dlugie();
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		dlugie();
		spacja();
}

void _plus()
{
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _ukosnik()
{
		dlugie();
		krotkie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _nawias_otwierajacy()
{
		dlugie();
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		spacja();
}

void _nawias_zamykajacy()
{
		dlugie();
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _znak_rownosci()
{
		dlugie();
		krotkie();
		krotkie();	
		krotkie();
		dlugie();
		spacja();
}

void _malpa()
{
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

//komendy specjalne

void _znak_rozdzialu()
{
		krotkie();
		dlugie();
		krotkie();
		krotkie();
		dlugie();
		spacja();
}

void _poczatek_kontaktu()
{
		krotkie();
		krotkie();	
		krotkie();
		dlugie();
		krotkie();
		krotkie();	
		krotkie();
		dlugie();
		krotkie();
		krotkie();	
		krotkie();
		dlugie();
		spacja();
}

void _poczatek_nadawania()
{
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _koniec_nadawania()
{
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _blad()
{
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		krotkie();
		spacja();
}

void _prosba_o_powtorzenie()
{
		krotkie();
		krotkie();
		dlugie();
		dlugie();
		krotkie();
		krotkie();
		spacja();
}

void _zrozumiano()
{
		krotkie();
		krotkie();
		krotkie();
		dlugie();
		krotkie();
		spacja();
}

void _czekaj()
{
		krotkie();
		dlugie();
		krotkie();	
		krotkie();
		krotkie();
		spacja();
}

void _wezwanie()
{
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _koniec_kontaktu()
{
		krotkie();
		krotkie();
		krotkie();
		dlugie();
		krotkie();
		dlugie();
		spacja();
}

void _miedzynarodowy_sygnal_alarmowy_SOS()
{
		krotkie();
		krotkie();
		krotkie();
		dlugie();
		dlugie();
		dlugie();
		krotkie();
		krotkie();
		krotkie();
		spacja();
}


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
	
	//----------------------MORS------------------
		
	while(1)
	{
		if(LEFT_KEY_DOWN)
		{
				_miedzynarodowy_sygnal_alarmowy_SOS();
				//dd=600;
				//Voice(t,dd);
		}
		if(RIGHT_KEY_DOWN)
		{
				_D();
				_A();
				_M();
				_I();
				_A();
				_N();
				//dd=120;
				//Voice(t,dd);
		}
	
	
	
	}
  // draw a filled box
  //LCDSetRect(10, 10, 40, 40, FILL, RED);
  //LCDSetRect(10, 50, 40, 80, FILL, GREEN);
  //LCDSetRect(10, 90, 40, 120, FILL, BLUE);


  // draw a string
  //LCDPutStr("Hello world", 15, 15, LARGE, PINK, BLACK);

/*	while(1)
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
	
	
	*/	
		
		
	LCDClearScreen();
//	}
	
  }
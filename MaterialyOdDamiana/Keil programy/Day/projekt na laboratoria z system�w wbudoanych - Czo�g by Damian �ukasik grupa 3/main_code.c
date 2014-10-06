/******************************************************************************/
// Program utworzony do potrzeb laboratorium IISI Systemy Wbudowane
// do wykorzystania w celach dydaktycznych podczas zajec laboratoryjnych
/******************************************************************************/


//Wszystkie potrzebne biblioteki
#include "common.h"
#include <stdio.H>                        //standard I/O
#include <math.H>                        //math library
#include "lcd.h"
#include "delay.h"
#include "one_wire.h"
#include "adc.h"
#include "adc.c"
#include <stdlib.h>
#include <time.h>

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

//potrzebne do przerwan.
static volatile unsigned int status_IRQ;

//wszystkie potrzebne zmiennne
int aa,dr,sz,szyb,przeszkoda,stop,h,strzal;
float U,ob;
int granie;
char s[30],ciag[30];

//zmienne zycia, punkty oraz zmienne do budowania srodowiska gry.
int life=3;
int over=0;
int i=0;
int ii=0;
int punkty=0;

//funkcja kolizji.
#define KOLIZJA(void){life=life-1;strzal=1;if(life==0){punkty=0;granie=0;over=1;}}

//sterowanie
#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)
#define LEFT_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB7)==0)

char rom[8]; //tablica 8 bajtów, które beda wypelnione poprzez dokonanie odczytu z termometru

//glosnik
#define LED_PIO		AT91C_BASE_PIOB			//adres kontrolera PIOB

#define BUZ_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20|AT91C_PIO_PB19) //inicjowanie glosnika
#define BUZ_INIT	{LED_PIO->PIO_PER = BUZ_MASK; LED_PIO->PIO_OER = BUZ_MASK; LED_PIO->PIO_OWER = BUZ_MASK;}

#define BUZ_ON  AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB19; // jest dzwiek 
#define BUZ_OFF AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB19; //  nie ma dzwieku


// Timer0 ISR
__irq void tim0_isr (void)  {

  volatile int dummy = AT91C_BASE_TC0->TC_SR;      //  Interrupt Ack - odczytanie rejestru statusu kasuje flage zgloszenia komparacji timera
  AT91C_BASE_AIC->AIC_ICCR  = (1 << AT91C_ID_TC0);        //  Interrupt Clear Command Register - skasowanie flagi zgloszenia przerwania w AIC
	
//..................................
//tu mozemy wstawic swoje instrukcje	
	
U=GetAdcChanel(ADC_CHN_7);//wczytywanie z kanalu 7

	//te instrukcje sa potrzebne do generowania dzwieku, z racji, ze generowany dzwiek jest w timerze
	//dzwiek bedzie buczal, a nie "stukal"
	U=((U*3.3)/1023);//zamiana danej zmiennej X na wartosc od 0 do 3.3
		if(U>=0 && U<=1 && granie==1)//warutenk dla 1 typu dzwieku i predkosci
			{		
				szyb=2;
				BUZ_ON;
				delay_us(100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(100);
			}
	 if(U>=1 && U<=2 && granie==1)//warutenk dla 2 typu dzwieku i predkosci
			{		
				szyb=4;
				BUZ_ON;
				delay_us(600); //Generuje dzwiek
				BUZ_OFF;
				delay_us(600);
			}
	if(U>=2 && U<=3.3 && granie==1)//warutenk dla 3 typu dzwieku i predkosci
			{		
				szyb=6;
				BUZ_ON;
				delay_us(1100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(1100);
			}
//..................................

//tak zakonczamy przerwanie
  *AT91C_AIC_EOICR = 0;                                   // End of Interrupt
}

__irq void pioIsr2(void) 
{
		//kopiujemy zawartosc rejestru PIO_ISR do zmiennej status
		status_IRQ = AT91C_BASE_PIOB->PIO_ISR;
		//teraz flagi w PIO_ISR sa juz skasowane
		
	//..................................
	//kod uzytkownika

		
		//przechodzimy do analizy zawartosci zmiennej status
		//przykladowo:

	if (status_IRQ & (1<<24)) //jesli PA24 wywolalo przerwanie... (dowolna zmiana stanu na linii PA24 = wcisniecie lub puszczenie klawisza)
		{
			//kod dla zmiany PA24
			if (LEFT_KEY_DOWN && aa>=10) //dodatkowo sprawdzamy, czy aktualny stan to wcisniety klawisz? (czyli zdarzenie, które wystapilo to wcisniecie)
				{
				  aa=aa-10;//dla przycisku w lewo pojazd porusza sie w lewo

				}
		}
		if (status_IRQ & (1<<25)) //jesli PA25 wywolalo przerwanie... (dowolna zmiana stanu na linii PA25 = wcisniecie lub puszczenie klawisza)
		{
			if (RIGHT_KEY_DOWN && aa<=70)//oczywiscie pojazd nie moze poruszac sie poza przedzialem 10-70.
				{
					aa=aa+10;//dla przycisku w prawo, pojazd porusza sie w prawo.

				}
		}
	//..................................


 //tak zakonczamy przerwanie
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
}

//
//  Main Program
// 
int main(void) {
	
		static int ye;	
	const unsigned char PIO_IRQ_PRIORITY = 5;
	 AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);

	AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); //keys SW1 i SW2 - jako wejscia
	
	//************************************************************************************************************
	//************* konfiguracja przerwan zglaszanych od jednostki PIOA ******************************************
	//************************************************************************************************************
	
	//przerwanie od PIOA - bedzie zglaszane przy zmianie stanu wybranych wejsc (wychylenie joysticka w lewo lub w prawo) - czyli zmianie zawartosci rejestru kontrolera AT91C_BASE_PIOA->PIO_PDSR
	AT91C_BASE_PIOA->PIO_IDR = 0xffffffff; //na wszelki wypadek wylaczenie zezwolen przerwan od wszyskich linii PIOA (gdyby przypadkowo byly wczesniej zezwolone)

	AT91C_BASE_PIOA->PIO_IER = (1<<7) | (1<<14); //przerwania beda generowane w reakcji na zmiane stanu linii PA7 i PA14
	//PA7 = left, PA8 = down, PA9 = UP, PA14 = PRAWO, AP15 = CLICK
	
	//przerwanie od PIOA - bedzie zglaszane przy zmianie stanu wybranych wejsc (wychylenie joysticka w lewo lub w prawo) - czyli zmianie zawartosci rejestru kontrolera AT91C_BASE_PIOA->PIO_PDSR
	AT91C_BASE_PIOB->PIO_IDR = 0xffffffff; //na wszelki wypadek wylaczenie zezwolen przerwan od wszyskich linii PIOA (gdyby przypadkowo byly wczesniej zezwolone)

	AT91C_BASE_PIOB->PIO_IER = (1<<24) | (1<<25); //przerwania beda generowane w reakcji na zmiane stanu linii PA24 i PA25

	//************************************************************************************************************

	//przerwanie zostanie "podlaczone" do kontolera AIC za pomoca pierwszego sposobu, tj. z wykorzystaniem procedur biblioteki "lib_AT91SAM7X256"
	//funkcja biblioteki "lib_AT91SAM7X256" podlaczajaca procedure obslugi przerwania do systemu przerwan AIC

		//przerwanie zostanie "podlaczone" do kontolera AIC za pomoca pierwszego sposobu, tj. z wykorzystaniem procedur biblioteki "lib_AT91SAM7X256"
	//funkcja biblioteki "lib_AT91SAM7X256" podlaczajaca procedure obslugi przerwania do systemu przerwan AIC
	AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC , AT91C_ID_PIOB ,	PIO_IRQ_PRIORITY , 
		AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL , (void*) pioIsr2);


	status_IRQ = AT91C_BASE_PIOA->PIO_ISR; //odczyt statusu PIOA - spowoduje to skasowanie ew. zgloszonych wczesniej przerwan
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC); //potwierdzenie ew. wczesniej zgloszonych przerwan w AIC
	
	AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_PIOA); //zezwolenie na generacje przerwan dla calego PIOA

  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_PIOB); //zezwolenie na generacje przerwan dla calego PIOA

	//************************************************************************************************************
	//************************************************************************************************************
	//************************************************************************************************************



	//************************************************************************************************************
	//********* Konfiguracja timera TC0 oraz przerwan zglaszanych przy doliczeniu do wartosci TC_RC  *************
	//************************************************************************************************************

	//**************************************************************
	//***************** Konfiguracja Timera ************************
	//**************************************************************
  
  AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_TC0);  // Enable Clock for TIM0
  AT91C_BASE_TC0->TC_CCR    = AT91C_TC_CLKEN | AT91C_TC_SWTRG; //clk enable oraz Software trigger (softwareowy restart licznika)
  AT91C_BASE_TC0->TC_CMR    = 2 | AT91C_TC_CPCTRG; //2 - sterowany przez Timer_CLOCK3 = Master Clock MCK/32 ; 
	//czyli czestotliwosc zegara napedzajacego timer = 1.4975MHz (wynika to z wartosci 18.432MHz (czestotliwosc kwarcu) / 32

		//AT91C_TC_CPCTRG = compare trigger enable
  AT91C_BASE_TC0->TC_RC     = 14975;   // period is 10ms - 14975 cykli po 0.6677 us kazdy = 10000us
	
	
	AT91C_BASE_TC0->TC_IER    = AT91C_TC_CPCS; //zglaszanie przerwan Timera w reakcji na wykrycie stanu COMPARE 
	//czyli w reakcji na zrownanie sie wartosci licznika z rejestrem TC_RC

	
	//**************************************************************
	//przerwanie zostanie "podlaczone" do kontolera AIC za pomoca drugiego sposobu, tj. BEZ wykorzystania procedur biblioteki "lib_AT91SAM7X256"
	
	// Konfiguracja przerwan zglaszanych przez timer: 1. tryb pracy, 2. ustawienie wektora przerwania oraz 3. zezwolenie na przerwanie
  // TIM0 Interrupt: Mode and Vector with Lowest Priority and Enable 
	//wszystko to ustawia sie w AIC (Advanced Interrupt Controller)
  
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | AT91C_AIC_PRIOR_HIGHEST;
  //tryb przerwania - wewnetrzne, wyzwalane zboczem (od pojawienia sie stanu przepelnienia), poziom priorytetu

	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (unsigned long) tim0_isr; 
	//wektor przerwania TC0 wskazuje teraz na NASZA procedure obslugi 'tim0_isr'
  
	AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_TC0); 
	//zezwolenie na przerwanie od Timera TC0 (przerwanie zglaszane jest gdy timer doliczy do wartosci compare)

	//************************************************************************************************************
	//************************************************************************************************************
	//************************************************************************************************************
	
	
		
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB);

BUZ_INIT;	//inicjacja do glosnika
	
	// ARM7X256 Leds Definition


  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
					 
	
  
  // Inicjalizacja interfejsu SPI do LCD
	InitSpi();

  // Inicjalizacja LCD
   InitLcd();

	// czysci ekran
   LCDClearScreen();

	owInit(); //inicjzacja interfejsu OneWire
	
	//poczatkowe wartosci zmiennych.
	aa=0;
  szyb=1;
  dr=2;
	ob=0;
	granie=1;
	stop=0;
	przeszkoda=0;
	h=0;
	strzal=0;

	while(1)//niekonczonca sie petla
	{
		
	if(i==20 && granie==1){punkty+=1;}	//jesli gracz moze grac i gra co 26 sekund dolicany jest 1 punkt wiecej.
	sprintf(ciag, "pkt=%d  ", punkty);//konwersja liczby punktów na ciag znakow
	LCDPutStr(ciag, 1, 55, SMALL, YELLOW, BLACK);//rysowanie ciagu znakow wyswietlajac punkty
	sprintf(ciag, "szanse=%d  ", life);//konwersja liczby szans na ciag znakow
	LCDPutStr(ciag, 10, 55, SMALL, RED, BLACK);//rysowanie ciagu znakow wyswietlajac szanse
		
	//rysowanie czolgu
	LCDSetRect(104, aa+10, 129, aa+40, NOFILL, GREEN);
  LCDSetRect(104, aa+11, 129, aa+16, NOFILL, GREEN);
	LCDSetRect(104, aa+34, 129, aa+39, NOFILL, GREEN);
  LCDSetRect(104, aa+23, 84, aa+27, NOFILL, GREEN);
	
	//Ten fragment kodu nie bylby skomentowany gdyby nie bylo przerwan.
	/*
	if(LEFT_KEY_DOWN && aa>=10)
	{
			aa=aa-10;
	}
	if(RIGHT_KEY_DOWN && aa<=70)
	{
       aa=aa+10;             
  }
	*/
	
	sprintf(s, "speed+%i", szyb);//konwersja liczby oznaczajacej szybkosc na ciag znakow
	LCDPutStr(s, 1, 0, SMALL, WHITE, BLACK);//rysowanie ciagu znakow wyswietlajac predkosc
	
	//funkcja dzieki ktorej gra sie konczy po straceniu 3 szans.
	if(over==1){
	LCDPutStr("GAME OVER", 38, 21, MEDIUM, RED, YELLOW);//ang. Koniec gry
	life=0;//zerowanie szans
	przeszkoda=0;//przeszkody juz sie nie pojawiaja
	}
			
	dr=3*szyb;//zmienna dr jest potrzebna do dzialania rysownaia drogi.
	
	i=i+5;//naliczanie i oraz ii
	ii=ii+10;
	if(i>=131){i=0;ii=0;}//zerowanie aby mialo swoj koniec i zaczelo od poczatku.
	
	//Ten fragment kodu nie bylby skomentowany gdyby nie bylo przerwan.
	/*
	U=GetAdcChanel(ADC_CHN_7);

	U=((U*3.3)/1023);
		if(U>=0 && U<=1 && granie==1)
			{		
				szyb=2;
				BUZ_ON;
				delay_us(100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(100);
			}
	 if(U>=1 && U<=2 && granie==1)
			{		
				szyb=4;
				BUZ_ON;
				delay_us(600); //Generuje dzwiek
				BUZ_OFF;
				delay_us(600);
			}
	if(U>=2 && U<=3.3 && granie==1)
			{		
				szyb=6;
				BUZ_ON;
				delay_us(1100); //Generuje dzwiek
				BUZ_OFF;
				delay_us(1100);
			}
	*/
	
	//funkcja dzieki ktorej rysowana jest "iluzja" drogi.
	if(dr+i>=0 && dr+i<=20)
		{						
				LCDSetRect(0, 0, 20, 2, FILL, CYAN);
				LCDSetRect(0, 127, 20, 129, FILL, CYAN);
			
				LCDSetRect(0, 0, 20, 2, FILL, CYAN);
				LCDSetRect(0, 127, 20, 129, FILL, CYAN);
			  
			  if(przeszkoda==1)//razem z przeszkoda
					{			
							LCDSetRect(30, sz+0, 40, sz+10, NOFILL, RED);
					}else{}
		}
  if(dr+i>=21 && dr+i<=40)
		{		
				LCDSetRect(0, 0, 40, 2, FILL, CYAN);
				LCDSetRect(0, 127, 40, 129, FILL, CYAN);
			
			  LCDSetRect(0, 0, 40, 2, FILL, CYAN);
				LCDSetRect(0, 127, 40, 129, FILL, CYAN);
			
				if(przeszkoda==1)
					{			
							LCDSetRect(40, sz+0, 50, sz+10, NOFILL, RED);
					}else{}
		}
	if(dr+i>=41 && dr+i<=60)
		{		
				LCDSetRect(20, 0, 60, 2, FILL, CYAN);
				LCDSetRect(20, 127, 60, 129, FILL, CYAN);
				
			  LCDSetRect(20, 0, 60, 2, FILL, CYAN);
				LCDSetRect(20, 127, 60, 129, FILL, CYAN);
			
							if(przeszkoda==1)
					{			
							LCDSetRect(60, sz+0, 70, sz+10, NOFILL, RED);
					}else{}
		}
  if(dr+i>=61 && dr+i<=80)
		{		
				LCDSetRect(40, 0, 80, 2, FILL, CYAN);
				LCDSetRect(40, 127, 80, 129, FILL, CYAN);
			
							if(przeszkoda==1)
					{			
							LCDSetRect(70, sz+0, 80, sz+10, NOFILL, RED);
					}else{}
		}
  if(dr+i>=81 && dr+i<=100)
		{		
				LCDSetRect(60, 0, 100, 2, FILL, CYAN);
				LCDSetRect(60, 127, 100, 129, FILL, CYAN);
			
							if(przeszkoda==1)
					{			
							LCDSetRect(80, sz+0, 90, sz+10, NOFILL, RED);
					}else{}
		}
  if(dr+i>=101 && dr+i<=120)
		{		
				LCDSetRect(80, 0, 120, 2, FILL, CYAN);
				LCDSetRect(80, 127, 120, 129, FILL, CYAN);
			
							if(przeszkoda==1)
					{			
							LCDSetRect(90, sz+0, 100, sz+10, NOFILL, RED);
					}else{}
		}
	if(dr+i>=121 && dr+i<=131)
		{		
				LCDSetRect(100, 0, 120, 2, FILL, CYAN);
				LCDSetRect(100, 127, 120, 129, FILL, CYAN);
			
							if(przeszkoda==1)
					{			
							LCDSetRect(100, sz+0, 110, sz+10, NOFILL, RED);
						
						if(i==130)//dzieki tej instrukcji przeszkoda jest usuwana ze swiata gry i losowana od nowa
							{
						  przeszkoda=0;
							}else{}
					}else{}
		}
		
	if(aa+10 <=sz+0 && aa+40>=sz+10)//jesli przeszkoda na pewnej wysokosci miesci sie w przedziale szerokosci czolgu nastepuje...
	{
	if(i==120 && strzal==0){KOLIZJA();przeszkoda=0;}else{strzal=0;}//...kolizja
	}
	else{}
		
	//przeszkody	
	if(i==0 && ii==0){
	sz=rand()%90;//losowanie polozenia przeszkody
	przeszkoda=1;//dodanie przeszkody do swiata gry.
	}

		
	LCDClearScreen();//czyszczenie ekranu aby obrazy sie nie nakladaly.
	}
	
  }
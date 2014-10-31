#include "delay.h"


/*static __inline void delay_us_core(void)
{
	unsigned int i;
	for(i=0;i<7;i++) __asm { nop };
}
*/
static __inline void delay_us_core(void)
{
	unsigned int i;
	
	/*dla Flash musi byc mniej (=7 dla RAM, =3 dla Flash)
	wynika to z faktu, ze program wykonywany bezposrednio z pamieci Flash dziala wolniej - pamiec Flash jest wolniejsza od pamieci RAM
	wlasciwa czesc kodu zostanie skompilowana a pozostala pominieta
	slowo "Flash" zostalo zawarte jedynie w konfiguracji SAM7X Flash - ULINK w miejscu:
	opcje projektu (PPM na projekcie) -> ZakBadka C/C++ -> Okienko Define w "Prerocessor Symbols"
	*/
	#ifdef Flash
	for(i=0;i<3;i++) __asm { nop }; 
	#else
	for(i=0;i<7;i++) __asm { nop }; 
	#endif

}

void delay_ms(uint16_t ms)
{
	uint16_t i;
	for(i=0;i<ms;i++)
	{
		delay_us(980);
	}
}


void delay_s(uint16_t s)
{
	uint16_t i;
	for(i=0;i<s;i++)
	{
		delay_ms(1000);
	}
}


void delay_us(uint16_t us)
{
	uint16_t i;
	for(i=0;i<us;i++)
	{
		delay_us_core();
	}
}



/********************************************************************************
* main.c: Implementering av ett system innehållande interrupts och AD-omvandling.
*         En lysdiod ansluten till pin 8 (PORTB0) styrs via PWM-generering, där
*         en potentiometer ansluten till analog pin A0 (PORTC0) läses av för
*         att justera ljusstyrkan. Detta sker då lysdioden är aktiverad, annars
*         hålls lysdioden släckt.
*
*         Toggling av lysdioden sker via nedtryckning av en tryckknapp ansluten
*         till pin 13 (PORTB5). Ett PCI-avbrott används för att vid nedtryckning
*         toggla aktivering av lysdioden via toggling av den globala variabeln
*         led1_enabled.
********************************************************************************/
#include "header.h"

/********************************************************************************
* main: Initierar mikrodatorn vid start. Om lysdioden är aktiverad styrs denna
*       med PWM-generering. Då läses potentiometern av, där PWM-värden, alltså
*       on- och off-tid för aktuell PWM-period, beräknas. Dessa värden används
*       för att köra PWM-generering en period, alltså en blinkning med on- och
*       off-tiden som blinkhastighet.
********************************************************************************/
int main(void)
{
	setup();

	while (1)
	{
		pwm_run(POT1, 1000);
		
	}

	return 0;
}


/********************************************************************************
* interrupts.c: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.h"

/********************************************************************************
* ISR (PCINT2_vect): Avbrottsrutin f�r PCI-avbrott p� I/O-port D, som anropas
*                    vid uppsl�ppning eller nedtryckning av tryckknappar
*                    Om n�gon av
*                    dessa knappar trycks ned s� uppdateras tillst�ndsmaskinen.
********************************************************************************/
ISR(PCINT2_vect)
{
	if (read(PIND, BUTTON1))
	{
		led1_enabled = !led1_enabled;
	}
	else if (read(PIND, BUTTON2))
	{
		led2_enabled = !led2_enabled;
	}
	return;
}

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin f�r PCI-avbrott p� I/O-port B, som anropas
*                    vid uppsl�ppning eller nedtryckning av tryckknappar
*                    Om n�gon av
*                    dessa knappar trycks ned s� uppdateras tillst�ndsmaskinen.
********************************************************************************/
ISR(PCINT0_vect)
{
	if (read(PINB, BUTTON3))
	{
		led3_enabled = !led3_enabled;
	}
	else if (read(PINB, BUTTON4))
	{
		led4_enabled = !led4_enabled;
	}
	else if (read(PINB, BUTTON5))
	{
		led5_enabled = !led5_enabled;
	}
	return;
}

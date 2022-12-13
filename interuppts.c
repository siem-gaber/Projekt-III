/********************************************************************************
* interrupts.c: Innehåller avbrottsrutiner.
********************************************************************************/
#include "header.h"

/********************************************************************************
* ISR (PCINT2_vect): Avbrottsrutin för PCI-avbrott på I/O-port D, som anropas
*                    vid uppsläppning eller nedtryckning av tryckknappar
*                    Om någon av
*                    dessa knappar trycks ned så uppdateras tillståndsmaskinen.
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
* ISR (PCINT0_vect): Avbrottsrutin för PCI-avbrott på I/O-port B, som anropas
*                    vid uppsläppning eller nedtryckning av tryckknappar
*                    Om någon av
*                    dessa knappar trycks ned så uppdateras tillståndsmaskinen.
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

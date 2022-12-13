/********************************************************************************
* setup.c: Innehåller initieringsrutiner.
********************************************************************************/
#include "header.h"

/* Statiska funktioner: */
static inline void init_ports(void);
static inline void init_interrupts(void);

/* Definition av globala variabler: */
volatile bool led1_enabled = false; /* Lysdiod1 inaktiverad vid start. */
volatile bool led2_enabled = false; /* Lysdiod2 inaktiverad vid start. */
volatile bool led3_enabled = false; /* Lysdiod3 inaktiverad vid start. */
volatile bool led4_enabled = false; /* Lysdiod4 inaktiverad vid start. */
volatile bool led5_enabled = false; /* Lysdiod5 inaktiverad vid start. */

/********************************************************************************
* setup: Initierar mikrodatorns I/O-portar, PCI-avbrott samt AD-omvandlare.
*
*        1. Vi initierar mikrodatorns I/O-portar, där lysdioder pins
*           sätts till utport, medan den interna pullup-resistorn på
*           tryckknapparnas pins ettställs för att insignalen alltid
*           ska bli hög eller låg, inget flytande där emellan.
*
*        2. Vi aktiverar PCI-avbrott för att direkt kunna toggla aktivering
			av lysdioden vid nedtryckning.

*        3. Vi initierar AD-omvandlaren analoga kretsar genom att köra en
*           testomvandling. Eftersom vi inte är intresserade av resultatet
*           (returvärdet) så typomvandlas denna i detta fall till void.
********************************************************************************/
void setup(void)
{
	init_ports();
	init_interrupts();
	(void)adc_read(POT1);
	return;
}

/********************************************************************************
* init_ports: Initerar mikrodatorns I/O-portar.
*
*             1. Vi sätter lysdiodens pin till utport genom att
*                ettställa motsvarande bit i datariktningsregister DDRD/B.
*
*             2. Vi aktiverar den interna pullup-resistorn på tryckknappens
*                pin genom att ettställa motsvarande bit i register
*                PORTB. Detta görs för att insignalen alltid ska bli 0 eller 1
*                (låg eller hög), inget flytande däremellan.
*
*             3. Eftersom potentiometern medför en analog insignal så aktiverar
*                vi inte den interna pullup-resistorn i detta fall, då detta
*                hade medfört en insignal som antingen hade blivit 0 eller 1
*                såsom sker på en digital inport.
********************************************************************************/
static inline void init_ports(void)
{
	set(DDRD, LED1);
	set(PORTD, BUTTON1);
	
	set(DDRD, LED2);
	set(PORTD, BUTTON2);
	
	set(DDRB, LED3);
	set(PORTB, BUTTON3);
	
	set(DDRB, LED4);
	set(PORTB, BUTTON4);
	
	set(DDRB, LED5);
	set(PORTB, BUTTON5);
	return;
}

/********************************************************************************
* init_interrupts: Aktiverar avbrott  på tryckknappens pin .
*
*                  1. Vi aktiverar avbrott globalt genom att ettställa
*                     I-flaggan i statusregister SREG via instruktionen
*                     SEI (Set Interrupt Flag).
*
*                  2. Vi aktiverar avbrottsvektor PCINT0, dvs. PCI-avbrott på
*                     I/O-port B/D, via ettställning av biten PCIE0 (PIN Change
*                     Interrupt Enable 0) i kontrollregistret PCICR (PIN Change
*                     Interrupt Control Register).
*
*                  3. Aktiverar specifikt avbrott på tryckknappens pin
*                     via ettställning av motsvarande bit
*                     i maskregistret PCMSK0 (PIN Change Mask Register 0).
********************************************************************************/
static inline void init_interrupts(void)
{
	set(PCICR, PCIE2);
	set(PCMSK2, BUTTON1);
	set(PCMSK2, BUTTON2);
	
	set(PCICR, PCIE0);
	set(PCMSK0, BUTTON3);
	set(PCMSK0, BUTTON4);
	set(PCMSK0, BUTTON5);
	
	asm("SEI");
	return;
}
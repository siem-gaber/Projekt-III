/********************************************************************************
* header.h: Innehåller diverse definitioner och deklarationer för implementering
*           av det inbyggda systemet.
********************************************************************************/
#ifndef HEADER_H_
#define HEADER_H_

/* Klockfrekvens (behövs för fördröjningsrutiner): */
#define F_CPU 16000000UL /* 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h>        /* Innehåller information om I/O-register såsom PORTB. */
#include <avr/interrupt.h> /* Innehåller information om avbrottsvektorer såsom PCINT0_vect. */
#include <util/delay.h>    /* Innehåller fördröjningsrutiner såsom _delay_ms. */

/* Makrodefinitioner: */
#define LED1 PORTD6   /* Lysdiod 1 ansluten till pin 6 (PORTD6). */
#define LED2 PORTD7   /* Lysdiod 2 ansluten till pin 7 (PORTD7). */
#define LED3 PORTB0	  /* Lysdiod 3 ansluten till pin 8 (PORTB0). */
#define LED4 PORTB1   /* Lysdiod 4 ansluten till pin 9 (PORTB1). */
#define LED5 PORTB2   /* Lysdiod 5 ansluten till pin 10 (PORTB2). */

#define POT1 PORTC0    /* Potentiometer ansluten till analog pin A0 (PORTC0). */

#define BUTTON1 PORTD2 /* Tryckknapp ansluten till pin 2 (PORTB5). */
#define BUTTON2 PORTD3 /* Tryckknapp ansluten till pin 3 (PORTB5). */
#define BUTTON3 PORTB3/* Tryckknapp ansluten till pin 13 (PORTB5). */
#define BUTTON4 PORTB4/* Tryckknapp ansluten till pin 13 (PORTB5). */
#define BUTTON5 PORTB5/* Tryckknapp ansluten till pin 13 (PORTB5). */

/********************************************************************************
* set: Ettställer bit i angivet register.
*
*      - reg: Registret som skrivning ska ske till.
*      - bit: Biten som ska ettställas.
********************************************************************************/
#define set(reg, bit) reg |= (1 << bit)

/********************************************************************************
* clr: Nollställer bit i angivet register.
*
*      - reg: Registret som skrivning ska ske till.
*      - bit: Biten som ska nollställas.
********************************************************************************/
#define clr(reg, bit) reg &= ~(1 << bit)

/********************************************************************************
* read: Läser av angiven pin och returnerar ifall denna är hög eller låg.
*
*       - reg: Registret som ska läsas av.
*       - bit: Den bit som ska kontrolleras.
********************************************************************************/
#define read(reg, bit) (reg & (1 << bit))

/* Enumerationer: */
typedef enum { false, true } bool; /* Realiserar datatypen bool, false = 0, true = 1. */

/********************************************************************************
* setup: Initierar mikrodatorns I/O-portar, PCI-avbrott samt AD-omvandlare.
********************************************************************************/
void setup(void);

/********************************************************************************
* adc_read: Läser av insignalen på angiven analog pin och returnerar motsvarande
*           digitala värde mellan 0 - 1023.
*
*           - pin: Den analoga pin som ska läsas av.
********************************************************************************/
uint16_t adc_read(const uint8_t pin);

/********************************************************************************
* adc_get_pwm_values: Läser av angiven analog pin och beräknar sedan on- och
*                     off-tid för PWM-generering via angiven periodtid.
*                     Beräknade värden sparas på angivna adresser och indikerar
*                     tiden som lysdioden ska vara tänd respektive släckt.
*
*                     - pin          : Den analoga pin A0 - A5 som ska läsas av.
*                     - pwm_period_us: Periodtid för PWM i mikrosekunder.
*                     - pwm_on_us    : Pekare till variabel där on-tiden ska
*                                      lagras i mikrosekunder.
*                     - pwm_off_us   : Pekare till variabel där off-tiden ska
*                                      lagras i mikrosekunder.
********************************************************************************/
void adc_get_pwm_values(const uint8_t pin,
const uint16_t pwm_period_us,
uint16_t* pwm_on_us,
uint16_t* pwm_off_us);

/********************************************************************************
* pwm_run: Kontrollerar ljusstyrkan på en lysdiod med PWM-generering under en
*          period via avläsning av angiven potentiometer.
*
*          - pot_pin  : Analog pin A0 - A5 som potentiometern är ansluten till.
*          - period_us: Periodtid för PWM i mikrosekunder.
********************************************************************************/
void pwm_run(const uint8_t pot_pin, const uint16_t period_us);

/********************************************************************************
* delay_us: Genererar fördröjning mätt i mikrosekunder.
*
*           - delay_time_us: Pekare till fördröjningstiden i mikrosekunder.
********************************************************************************/
void delay_us(const volatile uint16_t* delay_time_us);


/* declaration av globala variabler: */
extern volatile bool led1_enabled; /* Indikerar ifall lysdioden är aktiverad eller inte. */
extern volatile bool led2_enabled; /* Indikerar ifall lysdioden är aktiverad eller inte. */
extern volatile bool led3_enabled; /* Indikerar ifall lysdioden är aktiverad eller inte. */
extern volatile bool led4_enabled; /* Indikerar ifall lysdioden är aktiverad eller inte. */
extern volatile bool led5_enabled; /* Indikerar ifall lysdioden är aktiverad eller inte. */

#endif /* HEADER_H_ */
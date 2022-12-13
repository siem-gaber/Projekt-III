/********************************************************************************
* adc.h: Innehåller funktionalitet för AD-omvandling och PWM-generering.
********************************************************************************/
#include "header.h"

/********************************************************************************
* adc_read: Läser av insignalen på angiven analog pin och returnerar motsvarande
*           digitala värde mellan 0 - 1023.
*
*           1. Vi väljer att använda intern matningsspänning på 5 V. Vi väljer
*              också vilken analog pin som ska kopplas till AD-omvandlaren.
*
*           2. Vi aktiverar AD-omvandlaren (ADEN = ADC Enable). Sedan startar
*              vi en ny AD-omvandling (ADSC = ADC Start Conversion). Vi använder
*              lägsta möjliga frekvens (16M / 128 = 125 kHz) för att
*              AD-omvandlaren ska kunna göra så ackurat omvandling som möjligt,
*              alltså bästa möjliga resultat (ADPS[2:0] sätter denna frekvens).
*
*           3. Vi väntar in att AD-omvandlingen blir färdig. Detta indikeras
*              via biten ADIF (ADC Interrupt Flag), som då blir ettställd.
*              Därmed väntar vi via en loop tills ADIF blir ettställd.
*              Vi väntar via en while-sats, som kör så länge ADIF är lika med 0.
*              Semikolonet betyder "gör ingenting" varje varv i while-satsen.
*              Vi hade kunnat använda måsvingar.
*
*           4. Vi återställer ADIF inför nästa omvandling genom att skriva
*              en etta till denna. ADIF nollställs då av hårdvaran och kommer
*              kunna användas vid nästa AD-omvandling.
*
*           5. Resultatet ligger nu i register ADC. Egentligen är det två
*              8-bitars register ADCH och ADCL (ADC High och ADC Low), men
*              detta behöver ni bara tänka på i assembler, här får ni alla
*              bitar genom att skriva ADC. Vi returnerar därmed innehållet
*              från register ADC, vilket är ett osignerat heltal mellan
*              0 - 1023.
*
*           - pin: Den analoga pin som ska läsas av.
********************************************************************************/
uint16_t adc_read(const uint8_t pin)
{
	ADMUX = (1 << REFS0) | pin;
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	while ((ADCSRA & (1 << ADIF)) == 0);
	ADCSRA = (1 << ADIF);
	return ADC;
}

/********************************************************************************
* adc_get_pwm_values: Läser av angiven analog pin och beräknar sedan on- och
*                     off-tid för PWM-generering via angiven periodtid.
*                     Beräknade värden sparas på angivna adresser och indikerar
*                     tiden som lysdioden ska vara tänd respektive släckt.
*
*                     1. Vi beräknar duty cycle, (hur stor andel av periodtiden
*                        som lysdioden ska vara tänd) som ett tal mellan 0 - 1
*                        genom att dividera avläst värde från potentiometern med
*                        högsta möjliga värde 1023. Vi beräknar då hur mycket av
*                        max som potentiometern har vridits mellan 0 - 100 %,
*                        fast här i stället som ett tal mellan 0 - 1.
*
*                     2. Vi beräknar on-tiden, alltså tiden som lysdioden ska
*                        vara tänd under aktuell PWM-period, som periodtiden
*                        multiplicerat med beräknad duty cycle. Som exempel,
*                        för en duty cycle på 60 % ska on-tiden sättas till
*                        60 % av periodtiden, alltså periodtiden * 0.6.
*
*                     3. Vi beräknar off-tiden, alltså tiden lysdioden ska vara
*                        släckt under aktuell PWM-period, genom att beräkna
*                        resterande periodtid, dvs. periodtiden minus on-tiden.
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
uint16_t* pwm_off_us)
{
	const double duty_cycle = adc_read(pin) / 1023.0;
	*pwm_on_us = (uint16_t)(pwm_period_us * duty_cycle + 0.5);
	*pwm_off_us = pwm_period_us - *pwm_on_us;
	return;
}

/********************************************************************************
* pwm_run: Kontrollerar ljusstyrkan på en lysdiod med PWM-generering under en
*          period via avläsning av angiven potentiometer.
*
*          1. Först läses potentiometern av för att beräkna on- och off-tid
*             för aktuell PWM-period.
*
*          2. Därefter hålls lysdioden tänd under on-tiden.
*
*          3. Slutligen hålls lysdioden släckt under off-tiden.
*
*          - pot_pin  : Analog pin A0 - A5 som potentiometern är ansluten till.
*          - period_us: Periodtid för PWM i mikrosekunder.
********************************************************************************/
void pwm_run(const uint8_t pot_pin, const uint16_t period_us)
{
	uint16_t on_time_us;
	uint16_t off_time_us;

	adc_get_pwm_values(pot_pin, period_us, &on_time_us, &off_time_us);

	if (led1_enabled) set(PORTD, LED1);
	if (led2_enabled) set(PORTD, LED2);
	if (led3_enabled) set(PORTB, LED3);
	if (led4_enabled) set(PORTB, LED4);
	if (led5_enabled) set(PORTB, LED5);
	delay_us(&on_time_us);
	
	clr(PORTD, LED1);
	clr(PORTD, LED2);
	clr(PORTB, LED3);
	clr(PORTB, LED4);
	clr(PORTB, LED5);
	delay_us(&off_time_us);
	
	return;
}

/********************************************************************************
* delay_us: Genererar fördröjning mätt i mikrosekunder. Detta sker genom
*           uppräkning till angiven fördröjningstid, där 1 us fördröjning
*           genereras varje varv via anrop av funktionen _delay_us från
*           biblioteket util/delay.h
*
*           - delay_time_us: Pekare till fördröjningstiden i mikrosekunder.
********************************************************************************/
void delay_us(const volatile uint16_t* delay_time_us)
{
	for (uint16_t i = 0; i < *delay_time_us; ++i)
	{
		_delay_us(1);
	}

	return;
}
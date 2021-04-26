/*
 * Tuner.c
 *
 * Created: 26.04.2021 20:07:46
 * Author : Nevtod
 */ 

#define F_CPU 1

#include <avr/io.h>
#include <avr/interrupt.h>

void init_timer0(unsigned mcs)
{
	cli();
	OCR0 = mcs / F_CPU / 8 - 1;   //border
	TCCR0 |= (1 << WGM01);  //mode 2
	TCCR0 |= (1 << CS01); //division into 8
	
	TIMSK |= (1 << OCIE0) // border interrupt enable
	
	sei();
}

void init_ADC()
{
	switch(F_CPU)
	{
		case 1:
			ADCSRA |= (1 << ADPS1) + (1 << ADPS0);
			break;
		case 16:
			ADCSRA |= (1 << ADPS1) + (1 << ADPS0) + (1 << ADPS2);
	}
	
}

int main(void)
{
	init_timer0(512);
	
	
	
	
    while (1) 
    {
    }
}

ISR(TIMER0_COMP_vect)
{
	
	//ADC Processing
}


/*
 * Tuner.c
 *
 * Created: 26.04.2021 20:07:46
 * Author : Nevtod
 */ 

#define F_CPU 8
//  -fuse bits -U lfuse:w:0xD4:m	-U hfuse:w:0xD9:m   turn CPU to 8 MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


void init_timer0(unsigned int mcs)
{
	cli();
	
	TCCR0 |= 3 + (1 << WGM01);
	
	OCR0 = 128;//mcs / 8 - 1;   //border
	//TCCR0 |= 3;//(1 << CS00);// (1 << CS01) +  //division into 8
	//TCCR0 |= (1 << WGM01);
	
	TIMSK |= (1 << OCIE0); // border interrupt enable
	  //mode 2
	sei();
}

void init_ADC()
{
	DDRA = 0; //ADC use only
	ADCSRA |= 1 << ADEN;  //ADC enabled
	ADCSRA |= 1 << ADIE;   //Interrupt enabled
	
	ADMUX = 0;    //ADC0 port used
	//prescaler
	switch(F_CPU)
	{
		case 1:
			ADCSRA |= (1 << ADPS1) + (1 << ADPS0); break;
		case 8:
			ADCSRA |= (1 << ADPS2) + (1 << ADPS1); break;
		case 16:
			ADCSRA |= (1 << ADPS1) + (1 << ADPS0) + (1 << ADPS2); break;
	}
		
}

void start_ADC()
{
	ADCSRA |= 1 << ADSC;
}

void init_UART()
{
	
	
	UBRRL = 51;    //9600 bit/sec for 1MHz
	//UCSRA |= (1 << U2X)
	UCSRB |= (1 << RXEN) | (1 << TXEN);   //only transmition
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

unsigned int cnt = 0;
unsigned int cnt2 = 0;


void wait_Serial()   // wait ready of port
{
	while(!(UCSRA & (1 << UDRE))){};
}

register unsigned char i asm("r28");

int write_Serial(char* str)
{
	for(i = 0; i < strlen(str); i++)
	{
		wait_Serial();
		UDR = str[i];
	}
	return 0;
}


int main(void)
{
	init_timer0(1024);
	init_UART();
	init_ADC();
	
	
	//TCCR0 |= 1 << WGM01;
	
	DDRC |= 1 << 2;
    while (1) 
    {
		//if (!TCNT0)
			//cnt = (cnt + 1) % 10;

		//if (UCSRA & UDRE)
    }
}



ISR(TIMER0_COMP_vect)
{
	//write_Serial("Any plus Vladik = love\n");
	start_ADC();
	cnt2 = (cnt2 + 1) % 10;
	cnt++;
	if (cnt == 1000)
	{
		PORTC ^= 1 << 2;
		cnt = 0;
		//write_Serial("Any plus Vladik = love\n");
	}
	/*
	cnt = (cnt + 1) % 10;
	if (!cnt)
		PORTC ^= (1 << 2);
	*/			
			

	//ADC Processing
}


ISR(ADC_vect)
{
	cli();
	wait_Serial();
	UDR = ADCL;
	wait_Serial();
	UDR = ADCH;
	sei();
}


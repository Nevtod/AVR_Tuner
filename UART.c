#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define F_CPU 8
void init_UART()
{
	
	
	UBRRL = 51;    //19200 bit/sec for 1MHz
	//UCSRA |= (1 << U2X)
	UCSRB |= (1 << RXEN) | (1 << TXEN);   //only transmition
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}


void wait_Serial()   // wait ready of port
{
	while(!(UCSRA & (1 << UDRE))){};
}


int write_Serial(char* str)
{
	for(unsigned char i = 0; i < strlen(str); i++)
	{
		wait_Serial();
		UDR = str[i];
	}
	return 0;
}


void int_to_str(unsigned int x, char* str)
{
    unsigned char i = 0;
	
	while (x > 0)
	{
	    unsigned char dig = x % 10;
	    x /= 10;
	    str[i] = dig + 48;
	    i++;
	}
	str[i] = 0;
	unsigned char j = 0;
	i--;
	while (j < i)
	{
	    char tmp = str[i];
	    str[i] = str[j];
	    str[j] = tmp;
	    j++;
	    i--;
	}
}



void init_timer0(unsigned int mcs)   //ADC sampling
{
	cli();
	
	TCCR0 |= 3 + (1 << WGM01); //mode 2, divide into 8 mode
	
	OCR0 = mcs / 64 - 1;   //border
	
	TIMSK |= (1 << OCIE0); // border interrupt enable
	
	sei();
}


void init_ADC()
{
    cli();
	ADMUX |= (1 << REFS1) + (1 << REFS0); //internal 2.56V
	DDRA = 0; //ADC use only
	ADCSRA |= 1 << ADEN;  //ADC enabled
	ADCSRA |= 1 << ADIE;   //Interrupt enabled
	
	ADMUX |= 0;    //ADC0 port used
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
	sei();
}


void start_ADC()
{
	ADCSRA |= 1 << ADSC;
}



unsigned int cnt = 0;

int main(void)
{
	init_timer0(1024);
	init_UART();
	init_ADC();
	
	
	DDRC |= 1 << 2;
    while (1) 
    {
    }
}




ISR(TIMER0_COMP_vect)
{
    start_ADC();   
}




ISR(ADC_vect)
{
	cli();
    	write_Serial("v==");
	
	wait_Serial();
	UDR = ADCL;
	
	wait_Serial();
	UDR = ADCH;
	
	sei();
	
}

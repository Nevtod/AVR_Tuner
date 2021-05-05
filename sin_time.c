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
#include <math.h>
#include <string.h>

void init_timer1()
{
	cli();
	
	TCCR1B |= 1; //mode 0, / 64
	//TIMSK |= (1 << OCIE0); // border interrupt enable
	//OCR0 = mcs / 8;   //border

	sei();
}


void init_UART()
{
	
	
	UBRRL = 51;    //
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


unsigned int cnt = 0;
char str[15];
int main(void)
{
	init_UART();
	init_timer1();
	unsigned int res;
	double val;

	TCNT1 = 1;
	while (TCNT1 < 50000);
	write_Serial("before calc\n");
	
	res = 65535;
	int_to_str(res, str);
	write_Serial("res=");
	write_Serial(str);


	TCNT1 = 1;
	unsigned int start = TCNT1;
	val = sqrt(TCNT1);
	//res = floor(fabs(val) * 1000);
	//int_to_str(res, str);
	//write_Serial("\nsin = 0.");
	//write_Serial(str);
	
	cli();
	res = TCNT1 - start;// * 8;
	sei();

    int_to_str(res, str);
	write_Serial("\nmcs = ");
	write_Serial(str);
	
    res = floor(fabs(val) * 1000);
	int_to_str(res, str);
	write_Serial("\nsin = 0.");
	write_Serial(str);
	
	
	
	DDRC |= 1 << 2;
    while (1) 
    {
    }
}




ISR(TIMER0_COMP_vect)
{
    cli();
	cnt++;
	write_Serial("F");
	sei();
}

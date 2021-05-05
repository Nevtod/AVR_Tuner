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

}



void init_timer0()
{
	cli();
	DDRB |= 1 << PB3; //OC0 is out
	TCCR0 |= 4 + (1 << WGM00); //mode 0, / 8
	TCCR0 |= (1 << COM01); //PWM port out enabled
	OCR0 = 128;   //border
    
	sei();
}

void set_brightness(unsigned char val)   //0 to 255
{
    OCR0 = val;
}



int main(void)
{

	init_timer0();
	unsigned cnt = 0;
	while(1)
	{
    	cnt++;
	    if (!cnt)
	        OCR0 += 1;
	}
}



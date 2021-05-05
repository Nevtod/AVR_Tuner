/*
 * Tuner.c
 *
 * Created: 26.04.2021 20:07:46
 * Author : Nevtod
 */ 


//  -fuse bits -U lfuse:w:0xD4:m	-U hfuse:w:0xD9:m   turn CPU to 8 MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define FREQUENCY 110.0
//#define FREQUENCY 146.83
#define F_CPU 8
#define N 6
#define T 700    // samples for one period
#define TIME_BLOCK  T / 100
#define EPS 3
#define ST  1024  //sample time length
#define DELTA_T  0.001024
const unsigned int limits[N] = {400, 1200, 1500, 1900, 2500, 3200};
//const unsigned int limits[N] = {100, 200, 300, 400, 500, 1000};

double sumStory[100] = {0, };


void init_lights()
{
    DDRD = 0xff;
    PORTD = 0;
}


void init_timer1(unsigned int ms)   //updating indication
{
    
    OCR1A = ms * 125;
    TCCR1B |= 3 + (1 << WGM12); //64, CTC mod (4)
    TIMSK |= 1 << OCIE1A; //Interrupt border enabled
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
	//ADCSRA |= 1 << ADIE;   //Interrupt enabled
	
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


unsigned char z = 0;
int main(void)
{
	init_timer0(ST);  //mcs
	init_timer1(250);  //ms. Time between ligts updating
	init_ADC();
    init_lights();	
    
	double t = 0;
    double sum = 0;
    unsigned int i = 0;   //id of sumStore element that's now is acting out
    double signal;
    double msin;
    //we are to find shift of signal from zero in average
    double mid = 0;//, midTmp = 0;
    
    while (t < 2000)	//calibration time
    {
        if (ADIF)
        {
            t++;
            mid += ADC;
        }
    }
    t = 0;

    mid /= 2000;
    
    
    while (1) 
    {
        if (ADIF)
        {
            cli();
            signal = ADC;
            msin = sin(2 * M_PI * FREQUENCY * t);
            sum += (signal - mid) * msin;
             
            t += DELTA_T;
            
            if (i % TIME_BLOCK == 0)
            {
                if (i == T)
                {
                     i = 0;
                }
                
                
                if (z)
                {
                    double mSum = fabs(sum - sumStory[i / TIME_BLOCK]);
                    unsigned char signalPower = 0;
                    for (int j = 0; j < N; j++)
                        if (mSum >= limits[j]) 
                            signalPower |= 1 << j;
                        else
                            break;  
 
                    PORTD = signalPower;
                    z = 0;
                }
                
                sumStory[i / TIME_BLOCK] = sum;
            }
            i++;
            sei();   
        }
        
        if (fabs(sum) > 25000)
        {
            cli();
            t = msin / 6 / FREQUENCY;
            sum = 0;
            memset(sumStory, 0, sizeof(double) * 100);        
            sei();           
        }
    }
}




ISR(TIMER0_COMP_vect)
{
    start_ADC();
}


ISR(TIMER1_COMPA_vect)
{
    z = 1;
    //PORTD ^= 0xff;
    /*
    double mSum = sum;//fabs(sum - sumStory[i]);
    unsigned char signalPower = 0;
    for (int j = 0; j < N; j++)
        if (fabs(mSum) >= limits[j]) 
            signalPower |= 1 << j;
        else
            break;
    
    PORTD = signalPower ;//+ ((PORTD & (1 << 5)) ^ (1 << 5));
    */
}











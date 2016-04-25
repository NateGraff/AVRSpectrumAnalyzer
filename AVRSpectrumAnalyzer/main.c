/*
 * AVRSpectrumAnalyzer.c
 *
 * Created: 2016-04-23 6:47:57 PM
 * Author : natil
 */ 

// FHT Library Defines
#define LOG_OUT 1
#define FHT_N 16

#include <avr/io.h>
#include <avr/interrupt.h>
#include "FHT/FHT.h"

volatile uint8_t sample;

void store_sample(uint8_t high_byte, uint8_t low_byte)
{
	int value = (high_byte << 8) | low_byte;
	value -= 0x0200; // make sample signed
	value <<= 6; // 16b
	fht_input[sample] = value;
}

int main(void)
{
    // ADC Configuration
	ADMUX  = 0x00; // AREF, ADC0
	ADCSRB = 0x00; // Free Running Mode
	ADCSRA |= (1<<ADEN)|(1<<ADIE); // Enable ADC, ADC interrupt
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // prescaler 128 TODO: minimize
	DIDR0  |= (1<<ADC0D); // disable digital input
	
	sample = 0;
	
	sei();
	ADCSRA |= (1<<ADSC); // Start ADC

    while (1) 
    {
		// TODO: render display from fht_log_out
    }
}

ISR(ADC_vect)
{
	if(sample < FHT_N)
	{
		store_sample(ADCH, ADCL);
	}
	else
	{
		fht_window();
		fht_reorder();
		fht_run();
		fht_mag_log();
		
		sample = 0;
	}
}
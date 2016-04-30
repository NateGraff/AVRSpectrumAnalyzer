/*
 * AVRSpectrumAnalyzer.c
 *
 * Created: 2016-04-23 6:47:57 PM
 * Author : natil
 */

#define F_CPU 16000000UL

// FHT Library Defines
#define LOG_OUT 1
#define FHT_N 16

#include "FHT/FHT.h"
#include "display.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
	
	// SPI and Display Configuration
	init_SPI();
	send_cmd(DECODE_MODE_ADDR, DECODE_MODE_NO_DECODE);
	send_cmd(INTENSITY_ADDR, INTENSITY_MAX);
	send_cmd(SCAN_LIMIT_ADDR, SCAN_LIMIT_MAX);
	blank_display();
	
	// Display test mode
	send_cmd(DISPLAY_TEST_ADDR, DISPLAY_TEST_ON);
	_delay_ms(1000);
	send_cmd(DISPLAY_TEST_ADDR, DISPLAY_TEST_OFF);
	_delay_ms(1000);
	
	sample = 0;
	
	sei();
	ADCSRA |= (1<<ADSC); // Start ADC

    while(1);
}

ISR(ADC_vect)
{
	if(sample < FHT_N)
	{
		store_sample(ADCH, ADCL);
		++sample;
	}
	else
	{
		fht_window();
		fht_reorder();
		fht_run();
		fht_mag_log();
		
		normalize_spectrum();
		encode_display();
		send_display();
		
		sample = 0;
	}
}
/*
 * AVRSpectrumAnalyzer.c
 *
 * Created: 2016-04-23 6:47:57 PM
 * Author : natil
 */

#define F_CPU 16000000UL

// FHT Library Defines
#define LOG_OUT 1
#define FHT_N 64

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

void blank_vram()
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		vram[i] = 0x00;
		normalized_spectrum[i] = 0;
	}	
}

int main(void)
{
	// ADC Configuration
	ADMUX  = 0x00; // AREF, ADC0
	ADCSRB = 0x00; // Free Running Mode
	ADCSRA |= (1<<ADEN)|(1<<ADIE); // Enable ADC, ADC interrupt
	ADCSRA |= (1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0); // prescaler 32
	DIDR0  |= (1<<ADC0D); // disable digital input
	
	// SPI and Display Configuration
	init_SPI();
	send_cmd(SHUTDOWN_ADDR, SHUTDOWN_DISPLAY_ON);
	send_cmd(DECODE_MODE_ADDR, DECODE_MODE_NO_DECODE);
	send_cmd(INTENSITY_ADDR, INTENSITY_MAX);
	send_cmd(SCAN_LIMIT_ADDR, SCAN_LIMIT_MAX);
	blank_display();
	
	// Test all pixels at startup
	send_cmd(DISPLAY_TEST_ADDR, DISPLAY_TEST_ON);
	_delay_ms(500);
	send_cmd(DISPLAY_TEST_ADDR, DISPLAY_TEST_OFF);
	_delay_ms(500);
	
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
		// Run the FHT
		fht_window();
		fht_reorder();
		fht_run();
		fht_mag_log();
		
		// Process and display the FHT results
		normalize_spectrum();
		encode_display();
		send_display();
		
		sample = 0;
	}
	ADCSRA |= (1<<ADSC); // restart the ADC
}
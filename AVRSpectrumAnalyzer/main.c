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

void blank_vram()
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		vram[i] = 0x00;
	}	
}

int main(void)
{
	// SPI and Display Configuration
	init_SPI();
	send_cmd(SHUTDOWN_ADDR, 0x01);
	send_cmd(DECODE_MODE_ADDR, DECODE_MODE_NO_DECODE);
	send_cmd(INTENSITY_ADDR, INTENSITY_MAX);
	send_cmd(SCAN_LIMIT_ADDR, SCAN_LIMIT_MAX);
	blank_display();
	
	sample = 0;

    while(1)
	{
		uint8_t row, column;
		
		// turn on row by row
		for(row = 0; row < 8; row++)
		{
			blank_vram();
			vram[row] = 0xFF;
			
			send_display();
			_delay_ms(500);
		}
		
		// turn on column by column
		for(column = 0; column < 8; column++)
		{
			blank_vram();
			for(row = 0; row < 8; row++)
			{
				vram[row] = (1<<column);
			}
			
			send_display();
			_delay_ms(500);
		}
	}
}
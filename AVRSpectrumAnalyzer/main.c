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
		normalized_spectrum[i] = 0;
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
	
	uint8_t column_list[8] = {1, 5, 7, 3, 0, 4, 6, 2};
	uint8_t row_list[8] = {6, 1, 5, 0, 3, 7, 2, 4};

    while(1)
	{
		uint8_t column, row;
		for(column = 0; column < 8; column++)
		{
			blank_vram();
			vram[column_list[column]] = 0xFF;
			send_display();
			_delay_ms(50);
		}
		
		for(row = 0; row < 8; row++)
		{
			blank_vram();
			for(column = 0; column < 8; column++)
			{
				vram[column_list[column]] = (1<<row_list[row]);
			}
			send_display();
			_delay_ms(50);
		}
	}
}
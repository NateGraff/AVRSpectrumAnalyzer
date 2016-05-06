
#include "display.h"
#include <avr/io.h>

volatile uint8_t normalized_spectrum[8];
volatile uint8_t vram[8];

extern uint8_t fht_log_out[8];
void normalize_spectrum()
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		normalized_spectrum[i] = fht_log_out[i] / 32; // 256 / 8
	}
}

#define DBC(A, B, C) ((normalized_spectrum[A] <= B) << C)
/*
 * DBC - Display Bit Check
 *   Checks if column A is less than or equal to B, and then left-shifts it by C
 *   Row order, top to bottom of the display:
 *     1 5 7 3 0 4 6 2
 *   Byte order of segments, MSB to LSB:
 *     DP A B C D E F G
 *   Column order (bit shift):
 *     6 1 5 0 3 7 2 4
 */

const uint8_t bar_value[9] = {0x00, 0x40, 0x42, 0x62, 0x63, 0x6B, 0xEB, 0xEF, 0xFF};
const uint8_t column_order[8] = {1, 5, 7, 3, 0, 4, 6, 2};

void encode_display()
{
    /*
     * Encode Display
     *   Transforms fht_log_out[column] = {0-8}
     *   Into vram[column] = BYTE
     *   Reorders vram rows and columns to match layout of MAX7219CNG
     */
	uint8_t column;
	for(column = 0; column < 8; column++)
	{
		if(normalized_spectrum[column] > 8)
		{
			vram[column_order[column]] = 0xFF;
		}
		else
		{
			vram[column_order[column]] = bar_value[normalized_spectrum[column]];
		}
	}
}

void init_SPI()
{
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);
	
	SPCR = (1<<SPE)|(1<<MSTR);
	SPCR &= ~(1<<CPHA);
	SPCR &= ~(1<<CPOL);
	
	PORTB |= (1<<SS);
}

void send_cmd(uint8_t addr, uint8_t data)
{
	PORTB &= ~(1<<SS);
	
	SPDR = (addr & 0x0F);
	while(!(SPSR & (1<<SPIF)));
	
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	
	PORTB |= (1<<SS);
}

void send_display()
{
	uint8_t addr;
	for(addr = 0x01; addr < 0x09; ++addr)
	{
		send_cmd(addr, vram[addr-1]);
	}
}

void blank_display()
{
	uint8_t addr;
	for(addr = 0x01; addr < 0x09; ++addr)
	{
		send_cmd(addr, 0x00);
	}	
}

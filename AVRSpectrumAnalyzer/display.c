
#include "display.h"
#include <avr/io.h>

volatile uint8_t vram[8];

extern uint8_t fht_log_out[8];
#define DBC(A, B, C) ((fht_log_out[A] <= B) << C)
/*
 * DBC - Display Bit Check
 *   Checks if column A is less than or equal to B, and then left-shifts it by C
 *   Row order, top to bottom of the display:
 *     1 5 7 3 2 6 4 0
 *   Byte order of segments, MSB to LSB:
 *     DP A B C D E F G
 *   Column order (bit shift):
 *     6 1 5 0 4 2 7 3
 */

void encode_display()
{
    /*
     * Encode Display
     *   Transforms fht_log_out[column] = {0-8}
     *   Into vram[row] = BYTE
     *   Reorders vram rows and columns to match layout of MAX7219CNG
     */
	vram[1] = DBC(0, 8, 6) | DBC(1, 8, 1) | DBC(2, 8, 5) | DBC(3, 8, 0) |
			  DBC(4, 8, 4) | DBC(5, 8, 2) | DBC(6, 8, 7) | DBC(7, 8, 3); 
	
	vram[5] = DBC(0, 7, 6) | DBC(1, 7, 1) | DBC(2, 7, 5) | DBC(3, 7, 0) |
			  DBC(4, 7, 4) | DBC(5, 7, 2) | DBC(6, 7, 7) | DBC(7, 7, 3);
	
	vram[7] = DBC(0, 6, 6) | DBC(1, 6, 1) | DBC(2, 6, 5) | DBC(3, 6, 0) |
			  DBC(4, 6, 4) | DBC(5, 6, 2) | DBC(6, 6, 7) | DBC(7, 6, 3);

	vram[3] = DBC(0, 5, 6) | DBC(1, 5, 1) | DBC(2, 5, 5) | DBC(3, 5, 0) |
			  DBC(4, 5, 4) | DBC(5, 5, 2) | DBC(6, 5, 7) | DBC(7, 5, 3);
	
	vram[2] = DBC(0, 4, 6) | DBC(1, 4, 1) | DBC(2, 4, 5) | DBC(3, 4, 0) |
			  DBC(4, 4, 4) | DBC(5, 4, 2) | DBC(6, 4, 7) | DBC(7, 4, 3);
	
	vram[6] = DBC(0, 3, 6) | DBC(1, 3, 1) | DBC(2, 3, 5) | DBC(3, 3, 0) |
			  DBC(4, 3, 4) | DBC(5, 3, 2) | DBC(6, 3, 7) | DBC(7, 3, 3);
	
	vram[4] = DBC(0, 2, 6) | DBC(1, 2, 1) | DBC(2, 2, 5) | DBC(3, 2, 0) |
			  DBC(4, 2, 4) | DBC(5, 2, 2) | DBC(6, 2, 7) | DBC(7, 2, 3);
	
	vram[0] = DBC(0, 1, 6) | DBC(1, 1, 1) | DBC(2, 1, 5) | DBC(3, 1, 0) |
			  DBC(4, 1, 4) | DBC(5, 1, 2) | DBC(6, 1, 7) | DBC(7, 1, 3);
}

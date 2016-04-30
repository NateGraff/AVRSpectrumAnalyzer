#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>

extern volatile uint8_t vram[8];

void encode_display();

#endif // DISPLAY_H

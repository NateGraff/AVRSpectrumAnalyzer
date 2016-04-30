#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>

// Control register addresses
#define DECODE_MODE_ADDR 0x09
#define INTENSITY_ADDR 0x0A
#define SCAN_LIMIT_ADDR 0x0B
#define SHUTDOWN_ADDR 0x0C
#define DISPLAY_TEST_ADDR 0x0F

// Control register configuration
#define DECODE_MODE_NO_DECODE 0x00
#define INTENSITY_MAX 0x0F
#define SCAN_LIMIT_MAX 0x07
#define DISPLAY_TEST_ON 0x01
#define DISPLAY_TEST_OFF 0x00

extern volatile uint8_t normalized_spectrum[8];
extern volatile uint8_t vram[8];

void normalize_spectrum();
void encode_display();

void init_SPI();
void send_byte(uint8_t data);
void send_cmd(uint8_t addr, uint8_t data);
void send_display();
void blank_display();

#endif // DISPLAY_H

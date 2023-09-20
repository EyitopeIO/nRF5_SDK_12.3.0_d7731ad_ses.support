#ifndef ADAFRUIT_CONTROL_H
#define ADAFRUIT_CONTROL_H

#include <cstdint>

void adafruit_test( void );
void adafruit_init( void );
void adafruit_spiwrite (uint8_t c);
void adafruit_print( char c );
void adafruit_longwrite(uint8_t *c, uint16_t len);
void adafruit_print(const char *c, uint16_t len, int x, int y);

#endif
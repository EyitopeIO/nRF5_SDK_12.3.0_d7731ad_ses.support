#ifndef ADAFRUIT_CONTROL_H
#define ADAFRUIT_CONTROL_H

#include <cstdint>

#define MAX_STATIC_BUFFER_LENGTH 4032

/**
 * Clear the screen if clear is true
 *
 */
inline void clear_screen(bool clear);

/**
 * Run a test on the adafruit display
 *
 * This is the demo code from the adafruit library. It does a
 * few tricks to show off the display.
 */
void adafruit_test( void );


/**
 * Initialize the adafruit display
 *
 * This function initializes the display and sets up the
 * SPI interface.
 */
void adafruit_init( void );

/**
 * Write a single byte to the display
 *
 * @param c The byte to write
 */
void adafruit_spiwrite (uint8_t c);


/**
 * Write a buffer of bytes to the display
 *
 * @param c The buffer to write
 * @param len The length of the buffer
 */
void adafruit_longwrite(uint8_t *c, uint16_t len);

/**
 * Print a string to the display
 *
 * @param c The string to print
 * @param clear If true, clear the display before printing
 */
void adafruit_print( char c, bool clear = false);

/**
 * Print a string to the display
 *
 * @param c The string to print
 * @param clear If true, clear the display before printing
 */
void adafruit_print(const char *c, bool clear = false);

/**
 * Print a string to the display
 *
 * @param c The string to print
 * @param len The length of the string
 * @param x The x coordinate to print at
 * @param y The y coordinate to print at
 * @param clear If true, clear the display before printing
 */
void adafruit_print(const char *c, uint16_t len, int x, int y, bool clear = false);

#endif
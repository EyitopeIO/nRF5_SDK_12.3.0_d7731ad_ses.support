#ifndef ADAFRUIT_CONTROL_H
#define ADAFRUIT_CONTROL_H

#include <cstdint>

/**
 * @brief Clear the screen
 * 
 * @param clear If true, clear the screen
 */
void clear_screen(bool clear);

/**
 * @brief Run a test on the adafruit display
 *
 * @details This is the demo code from the adafruit library. It does a
 *          few tricks to show off the display.
 */
void adafruit_test( void );

/**
 * @brief Tells us if the display is busy
 *
 * @details A busy display could mean that a write operation is on-going, we are showing something on the page
 *          that should not be overwritten for whatever reason, waiting for input, etc.
 * @return true if busy
 */
bool adafruit_busy(void);

/**
 * @brief Initialise the adafruit display objects and test the display
 *
 * @details It initialises the SPI instance and the PCD8544 C++ object used to control the display.
 */
void adafruit_init( void );

/**
 * @brief Write a single byte to the display. 
 *
 * @details You normally should write using @ref adafruit_print since that would go through the display buffer. Let
            any synchronisation or scheduling happen there. Avoid using this directly. 
 *
 * @param c The byte to write
 */
void adafruit_spiwrite (uint8_t c);

/**
 * @brief Write a buffer of bytes to the display
 *
 * @param c The buffer to write
 * @param len The length of the buffer
 */
void adafruit_print(const uint8_t *c, bool clear = true);

/**
 * @brief Print a string to the display
 *
 * @param c The string to print
 * @param len The length of the string
 * @param x The x coordinate to print at
 * @param y The y coordinate to print at
 * @param clear If true, clear the display before printing
 */
void adafruit_print(const uint8_t *c, uint16_t len, int x, int y, bool clear = false);

/**
 * @brief Print a number to the display
 *
 * @param n The number to print
 */
void adafruit_print(uint32_t n, bool clear = true);


#endif
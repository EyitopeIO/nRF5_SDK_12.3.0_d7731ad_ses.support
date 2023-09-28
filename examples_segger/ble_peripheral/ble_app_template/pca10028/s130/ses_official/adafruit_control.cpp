/**
 * @file adafruit_control.cpp
 *
 * @brief Logic for controlling the adafruit display
 */


#include "adafruit_control.h"
#include "peripherals.h"
#include "common.h"
#include "PCD8544.h"


/// @brief The display object
PCD8544 nokia = PCD8544(DISPLAY_CLK, DISPLAY_DIN, DISPLAY_DC, DISPLAY_CE, DISPLAY_RESET);


void clear_screen(bool clear) {
  if (clear) {
    nokia.clear();
    nokia.display();
  }
}

void adafruit_init( void  )
{
  adafruit_test();
}

void adafruit_spiwrite (uint8_t c)
{
  spi_write(&c, 1);
}

void adafruit_longwrite (uint8_t *c, uint16_t len)
{
  spi_write(c, len);
}

void adafruit_print(const char *c, uint16_t len, int x, int y, bool clear)
{
  clear_screen(clear);
  nokia.print(c, x, y);
  nokia.display();
}

void adafruit_print(const char *c, bool clear) {
  clear_screen(clear);
  nokia.print(c);
  nokia.display();
}

void adafruit_print(char c, bool clear)
{
  clear_screen(clear);
  app_uart_put(c);
}

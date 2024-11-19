/**
 * @file adafruit_control.cpp
 *
 * @brief Logic for controlling the adafruit display
 */

#include "nrf_drv_spi.h"
#include "app_timer_appsh.h"
#include "bsp.h"
#include "adafruit_control.h"
#include "PCD8544.h"
#include <stdlib.h>


//#define DEBUG_MODE_PRINTING

#define DISPLAY_SPI_INSTANCE  0 /**< SPI instance index. */
#define SPI0_INSTANCE_INDEX 0
#define SPI1_INSTANCE_INDEX SPI0_INSTANCE_INDEX+SPI0_ENABLED
#define DISPLAY_BACKLIGHT_OFF bsp_board_led_on(bsp_board_pin_to_led_idx(DISPLAY_BACKLIGHT))
#define DISPLAY_BACKLIGHT_ON bsp_board_led_off(bsp_board_pin_to_led_idx(DISPLAY_BACKLIGHT))
#define DISPLAY_TX_BUF_LENGTH ((84 * 48) / 8) /**< SPI transaction buffer length. */

typedef enum {
    DISPLAY_RESET = 19,
    DISPLAY_CE = 20,
    DISPLAY_DC = 14,
    DISPLAY_DIN = 10,
    DISPLAY_CLK = 6,
    DISPLAY_BACKLIGHT = LED_DISPLAY_BACKLIGHT
} DISPLAY_pin_t;

/// @brief The display object
PCD8544 nokia = PCD8544(DISPLAY_CLK, DISPLAY_DIN, DISPLAY_DC, DISPLAY_CE, DISPLAY_RESET);

static const nrf_drv_spi_t spi_object = NRF_DRV_SPI_INSTANCE(DISPLAY_SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done = false;  /**< Flag used to indicate that SPI instance completed the transfer. */

APP_TIMER_DEF(m_refresh_display_timer_id);


void spi_write( uint8_t *buffer, size_t size)
{
    spi_xfer_done = false;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_object, buffer, size, NULL, 1));
    while (!spi_xfer_done)
        __WFE();
}

static void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    UNUSED_PARAMETER(p_event);
    spi_xfer_done = true;
}

static void pp_spi_init( void )
{
    DISPLAY_BACKLIGHT_ON;
    nrf_delay_ms(500);
    nrf_drv_spi_config_t DISPLAY_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    DISPLAY_spi_config.ss_pin = DISPLAY_CE;
    DISPLAY_spi_config.mosi_pin = DISPLAY_DIN;
    DISPLAY_spi_config.sck_pin  = DISPLAY_CLK;
    nrf_gpio_cfg_output(DISPLAY_RESET);
    nrf_gpio_cfg_output(DISPLAY_DC);
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_object, &DISPLAY_spi_config, spi_event_handler));
    //APP_ERROR_CHECK(nrf_drv_spi_init(&spi_object, &DISPLAY_spi_config, NULL));
    DISPLAY_BACKLIGHT_OFF;
}


void clear_screen(bool clear) {
  if (clear) {
    nokia.clear();
    nokia.display();
  }
}

void adafruit_spiwrite (uint8_t c)
{
  spi_write(&c, 1);
}

void adafruit_print(uint32_t n, bool clear)
{
  clear_screen(clear);
  nokia.print(n);
  nokia.display();
}

void adafruit_print(const uint8_t *c, uint16_t len, int x, int y, bool clear)
{
  clear_screen(clear);
  nokia.print(c, x, y);
  nokia.display();
}

void adafruit_print(const uint8_t *c, bool clear) {
  clear_screen(clear);
  nokia.print(c);
  nokia.display();
}

void adafruit_print(uint8_t c, bool clear)
{
  clear_screen(clear);
  app_uart_put(c);
}

bool adafruit_busy(void)
{
  return spi_xfer_done ? true : false;
}

static void refresh_display_timer_cb(void *p_context)
{
  if (adafruit_busy())
    return;
  UNUSED_PARAMETER(p_context);
  static uint32_t n_refresh = 1;
  adafruit_print(n_refresh);
  n_refresh += 1;
}

void adafruit_init( void  )
{
  uint32_t err_code;

  pp_spi_init();

  adafruit_test();

  err_code = app_timer_create(&m_refresh_display_timer_id, APP_TIMER_MODE_REPEATED, refresh_display_timer_cb);
  APP_ERROR_CHECK(err_code);
  //err_code = app_timer_start(m_refresh_display_timer_id, APP_TIMER_TICKS(1000,0), nullptr);
  //APP_ERROR_CHECK(err_code);
}
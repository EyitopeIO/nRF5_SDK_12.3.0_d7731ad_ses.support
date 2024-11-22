#include "adafruit_control.h"
#include "nrf_log_ctrl.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nordic.h"
#include "scanner.h"
#include "bsp.h"
#include "view.h"

#define APP_SCHED_QUEUE_SIZE 32

#define APP_TIMER_PRESCALER             0
#define APP_TIMER_OP_QUEUE_SIZE         32   // Enough for all timers used


extern "C" void push_btn_handler(bsp_event_t event)
{
  switch(event)
  {
    case BSP_EVENT_KEY_0:
      app_sched_event_put(nullptr, 0, begin_scan);
      break;
    case BSP_EVENT_KEY_1:
      app_sched_event_put(nullptr, 0, begin_scan);
      break;
    default:
      break;
  }
}


/**
 * @brief Function for application main entry.
 */
int main( void )
{
    uint32_t err_code;

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    APP_SCHED_INIT(APP_TIMER_SCHED_EVT_SIZE, APP_SCHED_QUEUE_SIZE);

    /* fRTC [kHz] = 32.768 / (PRESCALER + 1 )
     * nrf51822 has 2 RTC's.
     */
    APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

    /* Initialise the display */
    adafruit_init();

    static_pages_init();

    /* Initialise the PCA10026 for its primary function as an advertiser */
    board_begin();

    /* Initialise the added BLE functionality as a scanner */
    scanner_init();

    for (;;)
    {
      app_sched_execute();

      if (NRF_LOG_PROCESS() == false)
      {
          uint32_t err_code = sd_app_evt_wait();
          APP_ERROR_CHECK(err_code);
      }
    }
    return 0;
}

#include "adafruit_control.h"
#include "nrf_log_ctrl.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "ser_conn_handlers.h"
#include "app_timer_appsh.h"
#include "nordic.h"
#include "Arduino.h"

#define APP_SCHED_QUEUE_SIZE 8

#define APP_TIMER_PRESCALER             0
#define APP_TIMER_OP_QUEUE_SIZE         8   // Enough for all timers used

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

    /* Initialise the PCA10026 for its primary function as an advertiser */
    board_begin();

    /* Initialise the display */
    adafruit_init();

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

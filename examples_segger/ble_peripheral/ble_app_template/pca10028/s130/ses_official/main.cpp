#include "peripherals.h"
#include "view_control.h"
#include "witmotion_control.h"
#include "adafruit_control.h"
#include "nrf_log_ctrl.h"
#include "app_error.h"
#include "nordic.h"
#include "Arduino.h"
#include "common.h"



extern "C" void main_loop_forever( void );
extern void push_button_handler(bsp_event_t event);


void main_loop_forever( void )
{

  for (;;)
  {
    if (NRF_LOG_PROCESS() == false)
    {
        uint32_t err_code = sd_app_evt_wait();
        APP_ERROR_CHECK(err_code);
    }
    
    wit_make_data_request(); 

    /* There's inherent latency due to the bus speed and processing time. This delay 
     * was from experimenting with the Windows application provided by WIT.
     */
    delay(WIT_TIME_TO_WAIT_FOR_DATA); 
    
    if (wit_check_data_ready() != WIT_REQ_SUCCESS) {
      set_next_page_to_error_page();
    } else {
      wit_read_data();
    }

    update_display_info();
  }
}


/**
 * @brief Function for application main entry.
 */
int main( void )
{
    board_begin(push_button_handler);

    pp_spi_init(); // todo: move to view_init()
    adafruit_init();
    initialise_view();

    witmotion_init( 1 );
    main_loop_forever();
    return 0;
}


/**
 * @}
 */

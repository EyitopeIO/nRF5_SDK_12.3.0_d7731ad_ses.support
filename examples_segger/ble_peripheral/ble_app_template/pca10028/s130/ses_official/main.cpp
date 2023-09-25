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
unsigned int miss_fire = 0;
witmotion_data *sensor_data = nullptr;


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

    /* For whatever reason, the sensor may not update after we send data,
     * so we Could be stuck waiting endlessly for it. This is delay was from
     * experimenting with the Windows application provided by WIT
     */
    delay(WIT_TIME_TO_WAIT_FOR_DATA); 
    
    int status = wit_check_data_ready();
    
    if (status == WIT_DATA_READY) {
      sensor_data = wit_read_data();
    }
    else {
      notify(NOTIFICATION_MISSFIRE_OCCURRED);
      miss_fire += 1;
    }
  
    update_information();
  }
}


/**
 * @brief Function for application main entry.
 */
int main( void )
{
    board_begin();

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

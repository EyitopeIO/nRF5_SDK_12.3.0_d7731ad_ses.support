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
unsigned int miss_fire = 0;

/** \brief The sensor data that is being displayed on the screen
 * \details This is a pointer to the sensor data that is being displayed on
 *          the screen. It is defined main.cpp
 */
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
      show_error_page();
      miss_fire += 1;
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

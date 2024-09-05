#include "witmotion_control.h"
#include <wit_c_sdk.h>
#include <nrf_error.h>
#include <bsp_config.h>
#include <nrf_delay.h>
#include <app_uart.h>
#include <app_timer.h>
#include <common.h>
#include <Arduino.h>


#define ACC_UPDATE    0x01
#define GYRO_UPDATE   0x02
#define ANGLE_UPDATE  0x04
#define MAG_UPDATE    0x08
#define READ_UPDATE   0x80

#define MODBUS_ADDR   0x50

#define UART_BUFFER_SIZE 64

/**
 * \brief Contains data read by the sensor
 */
volatile witmotion_data wit_data = { 99.99 };

/**
 * \brief Number of times we had a UART communication error
 */
unsigned int uart_communication_errors = 0;

/**
 * \brief Becomes 1 when WIT data available. Must be reset to 0
 */
volatile char s_cDataUpdate = 0;

static void transmit_data_cb( uint8_t *p_data, uint32_t uiSize );
static void data_ready_cb( uint32_t uiReg, uint32_t uiRegNum );
static void wit_uart_event_handler( app_uart_evt_t *p_event );
static void pp_uart_init( void );


// APP_TIMER_DEF(witmotion_polling_id);


static inline void reset_wit_data_status(void)
{
  s_cDataUpdate = 0;
}


void witmotion_init( int start_or_stop )
{
  if (start_or_stop) {
    pp_uart_init();
    WitInit(WIT_PROTOCOL_MODBUS, MODBUS_ADDR);
    WitSerialWriteRegister(transmit_data_cb);
    WitRegisterCallBack(data_ready_cb);
    WitDelayMsRegister(nrf_delay_ms);
    //app_timer_create(&witmotion_polling_id,
    //                APP_TIMER_MODE_SINGLE_SHOT,
    //                wit_request_timeout_handler);
  }
}


void wit_make_data_request( void )
{
  s_cDataUpdate = 0;
  WitReadReg(AX, 12); // read 12 registers starting from AX

  //app_timer_start(witmotion_polling_id,
  //                            APP_TIMER_TICKS(WIT_DATA_REQUEST_TIMEOUT,0),
  //                            NULL);
  //
}


int wit_check_data_ready( void )
{
  enum wit_data_status state;
  if (s_cDataUpdate) {
    state = WIT_REQ_SUCCESS;
  } else {
    state = WIT_REQ_FAILED;
    uart_communication_errors += 1;
  }
  return state;
}


//static void show_data( void )
//{
//  printf("a:%.2f %.2f %.2f\r\n",wit_data.acceleration[1],wit_data.acceleration[2],wit_data.acceleration[3]);
//  printf("w:%.2f %.2f %.2f\r\n",wit_data.angular_velocity[1],wit_data.angular_velocity[2],wit_data.angular_velocity[3]);
//  printf("<:%.1f %.1f %.1f\r\n",wit_data.angle[0],wit_data.angle[1],wit_data.angle[2]);
//}

/**
 * \brief Load sensor data into struct, ready to be displayed
 */
void wit_read_data( void )
{
  for (int i=0; i<3; i++) {
    wit_data.acceleration[i] = ((float)sReg[AX+i]/32768.0f) * 16.0f;
    wit_data.angular_velocity[i] = ((float)sReg[GX+i]/32768.0f) * 2000.0f;
    wit_data.angle[i] = ((float)sReg[Roll+i]/32768.0f) * 180.0f;
  }
  reset_wit_data_status();
}

/**
 * \brief Called by WIT library to transmit data
 */
static void transmit_data_cb(uint8_t *p_data, uint32_t uiSize)
{
  uint32_t i,err_code;
  for ( i=0,err_code=0; i<uiSize; i++ ) app_uart_put(p_data[i]); // add to fifo
}
/**
 * \brief Called by WIT library when valid data has been received
 */
static inline void data_ready_cb(uint32_t uiReg, uint32_t uiRegNum)
{
  s_cDataUpdate = 1;  // set to 0 when new data requested
}


static void pp_uart_init( void )
{
    uint32_t err_code;

    const app_uart_comm_params_t comm_params =
    {
        .rx_pin_no = RX_PIN_NUMBER, // p024
        .tx_pin_no = TX_PIN_NUMBER, // p022
        .cts_pin_no = CTS_PIN_NUMBER,
        .rts_pin_no = RTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
        .baud_rate = UART_BAUDRATE_BAUDRATE_Baud9600
    };

    /* uart module weirdly does not configure rx pin as input
       alternative way to enable pullup:
        NRF_GPIO->PIN_CNF[RX_PIN_NUMBER] |=3<<2 ;
    */
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);

    APP_UART_FIFO_INIT(&comm_params,
                      UART_BUFFER_SIZE,
                      UART_BUFFER_SIZE,
                      wit_uart_event_handler,
                      APP_IRQ_PRIORITY_LOWEST,
                      err_code);
    //APP_UART_INIT(&comm_params,
    //               uart_event_handler,
    //               APP_IRQ_PRIORITY_LOWEST,
    //               err_code);
    APP_ERROR_CHECK(err_code);
}


static void wit_uart_event_handler( app_uart_evt_t *p_event )
{
    static uint8_t c = 0;
    uint32_t err_code;
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
          err_code = app_uart_get(&c);
          WitSerialDataIn(c);
          break;
        case APP_UART_FIFO_ERROR:
          break;
        case APP_UART_COMMUNICATION_ERROR:
          digitalWrite(LED_RGB_BLUE, LOW);
          break;
        case APP_UART_TX_EMPTY:
          break;
        case APP_UART_DATA:
        default:
            break;
    }
}
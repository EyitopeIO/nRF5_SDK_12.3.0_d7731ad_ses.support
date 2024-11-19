#include "softdevice_handler.h"
#include "app_scheduler.h"
#include "scanner.h"
#include "view.h"
#include "bsp.h"
#include "nordic.h"


#define SCAN_INTERVAL               0x0064  // 62.5ms
#define SCAN_WINDOW                 0x0050  // 50.0ms

static ble_gap_scan_params_t m_scan_param;    



void begin_scan(void * p_event_data, uint16_t event_size)
{
  UNUSED_PARAMETER(p_event_data);
  UNUSED_PARAMETER(event_size);

  uint32_t err;

  goto_page(PAGE_ADVERTISEMENT_STOPPING, nullptr);
  err = sd_ble_gap_adv_stop();

  APP_ERROR_CHECK(err);

  err = sd_ble_gap_scan_start(&m_scan_param);
  if (!err)
  {
    goto_page(PAGE_SCAN_BEGIN, nullptr);
    err = bsp_indication_set(BSP_INDICATE_SCANNING);
    APP_ERROR_CHECK(err);
  } 
  else
  {
    goto_page(PAGE_SCAN_FAILED, nullptr);
  }
}

static void bluetooth_event_handle(ble_evt_t *p_ble_event)
{
  uint32_t err;
  const ble_gap_evt_t *p_gap_evt = &(p_ble_event->evt.gap_evt);

  switch (p_ble_event->header.evt_id)
  {
      case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
      case BLE_GAP_EVT_ADV_REPORT:
      case BLE_GAP_EVT_RSSI_CHANGED:
      case BLE_GAP_EVT_SCAN_REQ_REPORT:
        break;
      
      case BLE_GAP_EVT_TIMEOUT:   // resume advertising at next opportunity
        if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
        {
          goto_page(PAGE_SCAN_FINISHED, nullptr);
          app_sched_event_put(nullptr, 0, advertising_init);
          app_sched_event_put(nullptr, 0, advertising_start);
        }
        break;

      default:
        break;
  }
}

void scanner_init(void)
{
    uint32_t err_code;
    err_code = softdevice_ble_evt_handler_set(bluetooth_event_handle);
    APP_ERROR_CHECK(err_code);

    m_scan_param.active   = 1;
    m_scan_param.selective = 0;
    m_scan_param.interval = SCAN_INTERVAL;
    m_scan_param.window   = SCAN_WINDOW;
    m_scan_param.timeout = SCAN_TIMEOUT;
}
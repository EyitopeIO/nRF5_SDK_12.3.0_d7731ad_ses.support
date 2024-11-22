#include "softdevice_handler.h"
#include "app_scheduler.h"
#include "ble_advdata.h"
#include "scanner.h"
#include "view.h"
#include "bsp.h"
#include "nordic.h"
#include <cstring>



#define SCAN_INTERVAL               0x0064  // 62.5ms
#define SCAN_WINDOW                 0x0050  // 50.0ms

static ble_gap_scan_params_t m_scan_param;

ble_gap_evt_adv_report_t advertisement_reports[MAX_REPORTS] = {0};
uint32_t n_advertisements_seen = 0;


/**
 * @brief Reset the scan report, discarding all previous scan information
 */
static void reset_scan_report(void)
{
  n_advertisements_seen = 0;
  advertisement_reports[0] = {0};
  advertisement_reports[1] = {0};
}

/**
 * @brief Helper function to compare peer addresses
 */
static bool same_device(const uint8_t* a, const uint8_t* b)
{
  for (uint8_t i = 0; i < BLE_GAP_ADDR_LEN; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Save the device that responded to a scan request
 * 
 * @param sd_adv_report Pointer to @ref ble_gap_evt_scan_req_report_t in the SoftDevice event
 */
static void add_adv_report(const ble_gap_evt_adv_report_t *sd_adv_report)
{
  if (n_advertisements_seen < MAX_REPORTS) {
    advertisement_reports[n_advertisements_seen] = *sd_adv_report;
    n_advertisements_seen++;
    return ;
  }

  if (sd_adv_report->rssi >= advertisement_reports[FIRST_ON_LIST].rssi) {
    advertisement_reports[FIRST_ON_LIST] = *sd_adv_report;
  } else if ((sd_adv_report->rssi < advertisement_reports[FIRST_ON_LIST].rssi)
      && (!same_device(advertisement_reports[FIRST_ON_LIST].peer_addr.addr, sd_adv_report->peer_addr.addr))) {
    advertisement_reports[SECOND_ON_LIST] = *sd_adv_report;
  }

  if (same_device(advertisement_reports[FIRST_ON_LIST].peer_addr.addr, advertisement_reports[SECOND_ON_LIST].peer_addr.addr)) {
    advertisement_reports[SECOND_ON_LIST] = {0};
  }

  n_advertisements_seen++;
}

void begin_scan(void * p_event_data, uint16_t event_size)
{
  UNUSED_PARAMETER(p_event_data);
  UNUSED_PARAMETER(event_size);

  uint32_t err;

  reset_scan_report();

  goto_static_page(PAGE_ADVERTISEMENT_STOPPING);
  err = sd_ble_gap_adv_stop();

  APP_ERROR_CHECK(err);

  err = sd_ble_gap_scan_start(&m_scan_param);
  if (!err)
  {
    goto_static_page(PAGE_SCAN_BEGIN);
    err = bsp_indication_set(BSP_INDICATE_SCANNING);
    APP_ERROR_CHECK(err);
  } 
  else
  {
    goto_static_page(PAGE_SCAN_FAILED);
  }
}

static void bluetooth_event_handle(ble_evt_t *p_ble_event)
{
  uint32_t err;
  const ble_gap_evt_t *p_gap_evt = &(p_ble_event->evt.gap_evt);

  switch (p_ble_event->header.evt_id)
  {
      case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
      case BLE_GAP_EVT_RSSI_CHANGED:
      case BLE_GAP_EVT_SCAN_REQ_REPORT:
        break;

      case BLE_GAP_EVT_ADV_REPORT: {
        add_adv_report(&p_gap_evt->params.adv_report);
        if (get_current_page() != PAGE_SCAN_RESPONSES) {
          goto_dynamic_page(PAGE_SCAN_RESPONSES);
        }
        break;
      }

      case BLE_GAP_EVT_TIMEOUT:   // resume advertising at next opportunity
        if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
        {
          goto_static_page(PAGE_SCAN_FINISHED);
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
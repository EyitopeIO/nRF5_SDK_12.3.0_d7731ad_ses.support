
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "view.h"
#include "adafruit_control.h"
#include "scanner.h"
#include "nrf_delay.h"
#include "app_scheduler.h"



#define V_SCANNING_L  23
static uint8_t v_scanning[V_SCANNING_L] = {0};

static const uint8_t v_scan_failed[] = "Scan\nfailed\nto start";
static const uint8_t v_scan_begin[] = "Scan\nstarted\n";
static const uint8_t v_scan_finished[] = "Scan\nfinished\n";
static const uint8_t v_advertiser_stopping[] = "Stopping advertisment...\r\n";
static const uint8_t v_advertiser_running[] = "Advertising...\r\n";
static const uint8_t v_null_page[] = "Lolz...\nNull page\r\n";
static const char v_advertisements[] = "%02X%02X%02X%02X%02X%02X\ns:%d\r\n\n";

static volatile bool v_is_changing_page = 0;
static page_t v_current_page;


void static_pages_init(void)
{
  std::snprintf(reinterpret_cast<char*>(v_scanning), V_SCANNING_L, "Scanning for\n%us...", SCAN_TIMEOUT);
}

/**
 * @brief Format a buffer to print on PAGE_SCAN_RESPONSES 
 */
static void do_page_scan_responses(void) {
  ble_gap_evt_adv_report_t *report;
  uint8_t* buffer = adafruit_get_cleared_buffer();
  uint8_t count = 0;
  
  if (n_advertisements_seen) {
    report = &advertisement_reports[FIRST_ON_LIST];
    count = std::snprintf(
      reinterpret_cast<char*>(buffer),
      BLE_GAP_ADV_MAX_SIZE + 4,   // 4 for the rssi including the negative sign
      v_advertisements, 
      report->peer_addr.addr[0],
      report->peer_addr.addr[1],
      report->peer_addr.addr[2],
      report->peer_addr.addr[3],
      report->peer_addr.addr[4],
      report->peer_addr.addr[5],
      report->rssi
    );

    report = &advertisement_reports[SECOND_ON_LIST];
    if (report->rssi != 0) {
      std::snprintf(
        reinterpret_cast<char*>(buffer + count - 1),
        BLE_GAP_ADV_MAX_SIZE + 4,
        v_advertisements, 
        report->peer_addr.addr[0],
        report->peer_addr.addr[1],
        report->peer_addr.addr[2],
        report->peer_addr.addr[3],
        report->peer_addr.addr[4],
        report->peer_addr.addr[5],
        report->rssi
      );
    }
  }
  adafruit_print(buffer);
}

void refresh_display_timer_cb(void *p_context)
{
  switch(v_current_page)
  {
    case PAGE_SCAN_RESPONSES:
      do_page_scan_responses();
      break;

    default:
      break;
  }
}

/**
 * @brief Callback for all page exit events. See also @ref on_page_entry_cb
 */
static void on_page_exit_cb(void)
{
  switch(v_current_page)
  {
    case PAGE_SCAN_RESPONSES:
      adafruit_stop_refresh_timer();
      break;

    default:
      break;
  }
}

/**
 * @brief Format data to be shown on display and send to display.
 * 
 * @details Note that this function will be executed in the main thread context, so any blocking operation will block the main thread and prevent you
 *          from processing other events. Do not call this function directly. Use @ref goto_page instead. In @ref page_data_t , the p_data member is
 *          what becomes p_event_data here. In this function, you add the logic to use the data in p_data to format the string you would have already
 *          created like the v_array examples above.
 * 
 * @param p_event_data Pointer to the event data. This is a @ref page_data_t
 * @param event_size Size of the event data
 */
static void on_static_page_entry(void *p_event_data, uint16_t event_size)
{
  page_t* tmp = reinterpret_cast<page_t*>(p_event_data);
  v_is_changing_page = 1;

  switch(*tmp)
  {
    case PAGE_NULL:
      adafruit_print(v_null_page);
      nrf_delay_ms(1000);     // Timer and scheduer queue may fill up before this returns, triggering the app error handler
      break;

    case PAGE_SCAN_BEGIN:
      adafruit_print(v_scanning);
      break;
  
    case PAGE_SCAN_FAILED:
      adafruit_print(v_scan_failed);
      break;

    case PAGE_SCAN_FINISHED:  // This page may not be displayed long enough for you to see it.
      adafruit_print(v_scan_finished);
      break;
  
    case PAGE_ADVERTISEMENT_STOPPING:
      adafruit_print(v_advertiser_stopping);
      break;
    
    case PAGE_ADVERTISING:
      adafruit_print(v_advertiser_running);
      break;

    default:
      break;
  }
  v_current_page = *tmp;
  v_is_changing_page = 0;
}

void goto_static_page(const page_t page)
{
  page_t pg = page;
  on_page_exit_cb();
  app_sched_event_put(&pg, sizeof(pg), on_static_page_entry);
}

void goto_dynamic_page(const page_t page)
{
  on_page_exit_cb();
  v_current_page = page;

  /* From here on, the page will now be refreshed by the timer callback */
  adafruit_start_refresh_timer();
}

page_t get_current_page(void)
{
  return v_current_page;
}

bool view_is_occupied(void)
{
  return (adafruit_busy() || v_is_changing_page) ? true : false;
}
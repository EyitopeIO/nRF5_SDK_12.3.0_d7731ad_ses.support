
#include <cstdint>
#include <cstdio>
#include "view.h"
#include "adafruit_control.h"
#include "scanner.h"
#include "nrf_delay.h"
#include "app_scheduler.h"


#define V_SCANNING_L  28  // 23 + two digits + 's' + miscellaneous
static uint8_t v_scanning[V_SCANNING_L] = {0};

static const uint8_t v_scan_failed[] = "Scan\nfailed\nto start";
static const uint8_t v_scan_begin[] = "Scan\nstarted\n";
static const uint8_t v_scan_finished[] = "Scan\nfinished\n";
static const uint8_t v_advertiser_stopping[] = "Stopping advertisment...\r\n";
static const uint8_t v_advertiser_running[] = "Advertising...\r\n";
static const uint8_t v_null_page[] = "Lolz...\nNull page\r\n";

static volatile bool v_is_changing_page = 0;

/**
 * @brief Page data structure
 * 
 * @details To change the page, you specify the page to change to and the data to be displayed on that page when
 *          @ref goto_page is called. This structure is used to pass the page and the data to the application scheduler,
 *          which will then call the function to change the page.
 */
typedef struct {
  page_t page;
  void *p_data;
} page_data_t;

void static_pages_init(void)
{
  std::snprintf(reinterpret_cast<char*>(v_scanning), V_SCANNING_L, "Scanning...\nTimeout: %us", SCAN_TIMEOUT);
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
static void do_goto_page(void *p_event_data, uint16_t event_size)
{
  page_data_t *p_data = reinterpret_cast<page_data_t*>(p_event_data);
  page_t page = p_data->page;

  v_is_changing_page = 1;
  switch(page)
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
  
    case PAGE_SCAN_FINISHED:
      adafruit_print(v_scan_finished);
      // nrf_delay_ms(1000);
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
  v_is_changing_page = 0;
}

void goto_page(const page_t page, void *p_page_data)
{
  page_data_t pd = { .page = page, .p_data = p_page_data };
  app_sched_event_put(&pd, static_cast<uint16_t>(sizeof(pd)), do_goto_page);
}


bool view_is_occupied(void)
{
  return (adafruit_busy() || v_is_changing_page) ? true : false;
}


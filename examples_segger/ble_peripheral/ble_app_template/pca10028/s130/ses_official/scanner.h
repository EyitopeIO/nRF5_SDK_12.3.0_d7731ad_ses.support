#ifndef SCANNER_H
#define SCANNER_H

#include <cstdint>
#include "ble_gap.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SCAN_TIMEOUT                0x000F  // 15s
#define FIRST_ON_LIST               0
#define SECOND_ON_LIST              1
#define MAX_REPORTS                 2

extern ble_gap_evt_adv_report_t advertisement_reports[];
extern uint32_t n_advertisements_seen;

/**
 * @brief Initialize the scanner
 */
void scanner_init(void);

/**
 * @brief Start scanning for devices.  
 */
void begin_scan(void * p_event_data, uint16_t event_size);



#ifdef __cplusplus
}
#endif

#endif // SCANNER_H
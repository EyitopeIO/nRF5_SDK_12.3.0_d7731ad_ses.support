#ifndef SCANNER_H
#define SCANNER_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define SCAN_TIMEOUT                0x000F  // 30s

/**
 * @brief Initialize the scanner
 */
void scanner_init(void);

/**
 * @brief Start scanning for devices
 */
void begin_scan(void * p_event_data, uint16_t event_size);



#ifdef __cplusplus
}
#endif

#endif // SCANNER_H
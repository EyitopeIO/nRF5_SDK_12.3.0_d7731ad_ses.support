#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CENTRAL_LINK_COUNT              1                                 /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           0                                 /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

void board_begin(void);

/**
 * @brief Function for starting advertising.
 */
void advertising_start(void *p_event_data, uint16_t event_size);

/**
 * @brief Reinitialise stack for advertisement
 */
void advertising_init(void *p_event_data, uint16_t event_size);

#endif

#ifdef __cplusplus
}
#endif
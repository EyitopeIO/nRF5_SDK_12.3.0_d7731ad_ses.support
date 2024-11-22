#ifndef VIEW_H
#define VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Unique identifiers for each page
 *
 * @details Each page has a corresponding array of characters that will be written to the display when the page is active.
 *          Pages with dynamic content will have format specifiers in the strings.
 */
typedef enum {
  PAGE_NULL = 0,    /**< For debug purposes to identify a null pointer */
  PAGE_SCAN_BEGIN = 2,
  PAGE_SCAN_FAILED,
  PAGE_SCAN_FINISHED,
  PAGE_SCAN_RESPONSES,
  PAGE_SCAN_WAITING_FOR_FIRST,
  PAGE_ADVERTISEMENT_STOPPING,
  PAGE_ADVERTISING,
} page_t;

/**
 * @brief Switch to a page with static content on the display
 * 
 * @details This actually schedules the page change and doesn't change immediately. The page will change in the
 *          next iteration of the main loop i.e. the application context.
 *
 * @param page The page to switch to.
 */
void goto_static_page(const page_t page);

/**
 * @brief Switch to a page with dynamic content on the display
 */
void goto_dynamic_page(const page_t page);

/**
 * @brief Check if the display is busy
 * 
 * @return true if there is no on-going operation on the display, false otherwise
 */
bool view_is_occupied(void);

/**
 * @brief Returns the current page the display is showing
 */
page_t get_current_page(void);

/**
 * @brief Initialise all static pages
 */
void static_pages_init(void);

/**
 * @brief Display refresh timer callback
 * 
 * @details When we are on a dynamic, a timer ticks every second to refresh the display.
 *          After each tick, the display is updated using this callback. This is where you
 *          add logic to update the content of your dynamic page. Avoid long running operations.
 */
void refresh_display_timer_cb(void *p_context);


#ifdef __cplusplus
}
#endif

#endif // VIEW_H
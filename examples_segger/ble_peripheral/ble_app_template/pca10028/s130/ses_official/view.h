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
  PAGE_ADVERTISEMENT_STOPPING,
  PAGE_ADVERTISING
} page_t;

/**
 * @brief Switch to a given page on the display
 * 
 * @note This actually schedules the page change and doesn't change immediately.
 *
 * @param page The page to switch to.
 * @param p_page_data  Pointer to arbitrary data used to update changing values on the page
 *                     For each page, the function treats the data differently. The caller is
 *                     responsible for making sure that the page data is valid. When you change
 *                     the page, @ref view_is_occupied returns true until the function returns.
 */
void goto_page(const page_t page, void *p_page_data);

/**
 * @brief Check if the display is busy
 * 
 * @return true if there is no on-going operation on the display, false otherwise
 */
bool view_is_occupied(void);

/**
 * @brief Initialise all static pages
 */
void static_pages_init(void);

#ifdef __cplusplus
}
#endif

#endif // VIEW_H
/**
 * @file view_control.h
 *
 * @brief View control
 *
 * This file contains the view control functions and definitions
 *
 */



#ifndef _VIEW_CONTROL_H
#define _VIEW_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <witmotion_control.h>


#ifdef __cplusplus
}
#endif
#endif

/**
 * \brief Set vars so that error page shows on next display update
 */
void set_next_page_to_error_page();

/**
 * \brief Initialise all display state
 */
void initialise_view();

void update_display_info();

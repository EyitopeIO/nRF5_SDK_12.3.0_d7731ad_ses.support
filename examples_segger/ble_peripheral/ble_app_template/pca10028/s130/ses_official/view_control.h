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
 * Trigger a notification
 *
 * Other parts of the code may take action depending on what notification
 * has just occured. It is important that notification handlers should
 * consider how they affect other processes.
 *
 * @param note The notification that has just occurred
 *
 */
void show_error_page();



/**
 * Initialise all view objects and assets
 *
 * All initialisation should be done here. Ensure initial state of all view
 * objects are known in this function
 *
 */
void initialise_view();
void update_display_info();

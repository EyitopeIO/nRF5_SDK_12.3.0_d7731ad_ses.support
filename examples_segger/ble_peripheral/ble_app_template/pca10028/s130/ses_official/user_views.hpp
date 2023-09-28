#ifndef  _USER_VIEWS_H
#define  _USER_VIEWS_H


#define NUMBER_OF_NORMAL_USER_VIEWS 3

/* USER VIEWS
 *
 * This enum represents the different views that could possible be on
 * the user interface. It must be kept in sync with @ref array_of_userviews
 */
enum user_views {
  VIEW_0_ACCELERATION_ONLY = 0,
  VIEW_1_ANGLE_ONLY,
  VIEW_2_ANGULARVELOCITY_ONLY,
  VIEW_3_NOTIFICATION,
  VIEW_NULL
};



#endif
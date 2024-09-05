#include "view_control.h"
#include "adafruit_control.h"
#include "witmotion_control.h"
#include <bsp.h>
#include <Arduino.h>
#include <cstdio>
#include <cstring>
#include <cassert>


extern witmotion_data wit_data;

/**
 * \brief It must be kept in sync with \ref array_of_userviews
 */
enum user_views {
  VIEW_ACCELERATION = 0,
  VIEW_ANGLE,
  VIEW_ANGULARVELOCITY,
  VIEW_ERROR,
  VIEW_LAST
};

#define N_SENSOR_DATA_VIEWS VIEW_LAST - 1

/**
 * \brief Working information about the current view that the user is seeing.
 * \details This struct is used to keep track of the current view that the
 *          user is seeing. It also keeps track of the previous view so that
 *          the user can be taken back to the previous view after a
 *          notification has been triggered.
 */
struct current_view_t {
  user_views view;
  user_views prev;
};

/** \brief The maximum length of a string that can be printed to the screen
 * \details It servers as a working buffer process anything that would be
 *          printed to the screen. It does not make sense for it to be
 *          larger than the screen size itself.
 */
static char processed_string[MAX_STATIC_BUFFER_LENGTH] = {0};


static current_view_t current_view;
extern unsigned int uart_communication_errors;
extern witmotion_data *sensor_data;


static const char *view_acceletation_text =
"Acc.     \n"
"x: %.2f  \n"
"y: %.2f  \n"
"z: %.2f  \n";

static const char *view_angle_text =
"Angle    \n"
"x: %.2f  \n"
"y: %.2f  \n"
"z: %.2f  \n";

static const char *view_angularvelocity_text =
"rad/s    \n"
"x: %.2f  \n"
"y: %.2f  \n"
"z: %.2f  \n";

static const char *view_error_text =
"ERROR    \n"
"_____    \n"
"misfire: %d\n";


/** \brief Array of user views and must be kept in sync with \ref user_views
 */
const char *array_of_userviews[] = {
  view_acceletation_text,
  view_angle_text,
  view_angularvelocity_text,
  view_error_text,
  nullptr
};


void set_next_page_to_error_page()
{
  current_view.prev = current_view.view;
  current_view.view = VIEW_ERROR;
}


void initialise_view(void)
{
  current_view.view = VIEW_ACCELERATION;
  current_view.prev = VIEW_ACCELERATION;
}


void update_display_info()
{
  std::memset(const_cast<char*>(processed_string), 0, MAX_STATIC_BUFFER_LENGTH );

  switch(current_view.view)
  {
    case VIEW_ACCELERATION:
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_ACCELERATION],
        wit_data.acceleration[1],
        wit_data.acceleration[2],
        wit_data.acceleration[3]
      );
      break;

    case VIEW_ANGLE:
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_ANGLE],
        wit_data.angle[1],
        wit_data.angle[2],
        wit_data.angle[3]
      );
      break;

    case VIEW_ANGULARVELOCITY:
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_ANGULARVELOCITY],
        wit_data.angular_velocity[1],
        wit_data.angular_velocity[2],
        wit_data.angular_velocity[3]
      );
      break;

    case VIEW_ERROR:
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_ERROR],
        uart_communication_errors
      );
      adafruit_print(processed_string, true);
      delay(2000);
      clear_screen(true);
      current_view.view = current_view.prev;
      return;

    default:
      break;
  }

  adafruit_print(processed_string, std::strlen(processed_string), 0, 0, false);
}


user_views operator++(user_views& v, int n)
{
  user_views tmp = v;
  if ((static_cast<int>(tmp) + 1) == static_cast<int>(N_SENSOR_DATA_VIEWS))
  {
    tmp = VIEW_ACCELERATION;
  }
  else {
    tmp = static_cast<user_views>(static_cast<int>(v) + 1);
  }
  v = tmp;
  return tmp;  
}


user_views operator--(user_views& v, int n)
{
  user_views tmp = v;
  if ((static_cast<int>(v) - 1) < static_cast<int>(VIEW_ACCELERATION))
  {
    tmp = static_cast<user_views>(static_cast<int>(N_SENSOR_DATA_VIEWS) - 1);
  }
  else {
    tmp = static_cast<user_views>(static_cast<int>(v) - 1);
  }
  v = tmp;
  return tmp;
}


void push_button_handler(bsp_event_t event)
{
  current_view.prev = current_view.view;
  switch(event)
  {
    case BSP_EVENT_KEY_0:
      (current_view.view)++; // see overloaded operator++ defined above
      break;
    case BSP_EVENT_KEY_1:
      (current_view.view)--; // see overloaded operator-- defined above
      break;
    default:
      break;
  }
}
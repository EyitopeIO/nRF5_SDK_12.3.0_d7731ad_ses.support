#include "user_views.hpp"
#include "view_control.h"
#include "adafruit_control.h"
#include <Arduino.h>
#include <cstdio>
#include <cstring>
#include <cassert>


/// @brief Working information about the current view that the user is seeing.
/// @details This struct is used to keep track of the current view that the
///          user is seeing. It also keeps track of the previous view so that
///          the user can be taken back to the previous view after a
///          notification has been triggered.
struct current_view_t {
  user_views view;    ///< The current view that the user is seeing
  user_views prev;    ///< The previous view that the user was seeing
  notification notf_triggered;  ///< The notification that has been triggered
  bool view_change;  ///< Whether we have just changed views
};


/// @brief The maximum length of a string that can be printed to the screen
/// @details It servers as a working buffer process anything that would be
///          printed to the screen. It does not make sense for it to be
///          larger than the screen size itself.
static char processed_string[MAX_STATIC_BUFFER_LENGTH] = {0};

static int last_write_n = 1;
static current_view_t current_view;
extern const char *array_of_userviews[];
extern unsigned int miss_fire;

/// @brief The sensor data that is being displayed on the screen
/// @details This is a pointer to the sensor data that is being displayed on
///          the screen. It is defined main.cpp
extern witmotion_data *sensor_data;


/// @brief The text that is displayed when a notification is triggered
/// @details This array must be kept in sync with @ref notification
static const char *notification_texts[] = {
  "%d miss fires have occurred\n"
};


void notify(notification note) {
  current_view.notf_triggered = note;
  current_view.prev = current_view.view;
  current_view.view = VIEW_4_NOTIFICATION;
  current_view.view_change = true;
}


void initialise_view(void) {
  current_view.view = VIEW_1_ACCELERATION_ONLY;
  current_view.prev = VIEW_1_ACCELERATION_ONLY;
  current_view.view_change = true;
  current_view.notf_triggered = NOTIFICATION_NONE;
}


void update_information() {

  if (current_view.view_change) {
    current_view.view_change = false;
  }

  // We absolutely want to null terminate the string. Not taking chances.
  std::memset(const_cast<char*>(processed_string), 0, MAX_STATIC_BUFFER_LENGTH );

  switch(current_view.view) {

    case VIEW_1_ACCELERATION_ONLY:
      if (sensor_data == nullptr) {
        return;
      }
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_1_ACCELERATION_ONLY],
        sensor_data->acceleration[1],
        sensor_data->acceleration[2],
        sensor_data->acceleration[3]
      );
      break;

    case VIEW_4_NOTIFICATION:
      switch(current_view.notf_triggered) {

        case NOTIFICATION_MISSFIRE_OCCURRED:
          std::sprintf(
            processed_string,
            notification_texts[NOTIFICATION_MISSFIRE_OCCURRED],
            miss_fire
          );
          adafruit_print(processed_string, true);
          delay(5000);
          clear_screen(true);
          break;

        default:  // e.g. NOTIFICATION_NONE
          break;
      }
      current_view.view = current_view.prev;
      return;

    default:
      break;
  }

  last_write_n = std::strlen(processed_string);
  printf("last_write_n: %d\n", last_write_n);
  printf("s: %s\n", processed_string);
  adafruit_print(processed_string, std::strlen(processed_string), 0, 0, false);
}

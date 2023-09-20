#include "user_views.hpp"
#include "view_control.h"
#include "adafruit_control.h"
#include <cstdio>
#include <cstring>


struct current_view_t {
  user_views view;
};

/*
* The display is 84 * 48. It is unlikely we will ever pass this limit
*/
static char processed_string[4032] = {0};

static int last_write_n = 1;

static current_view_t current_view;
extern const char *array_of_userviews[];


void initialise_view(void) {
  current_view.view = VIEW_1_ACCELERATION_ONLY;
}

void update_information(witmotion_data *data) {

  std::memset(const_cast<char*>(processed_string), 0, last_write_n );

  switch(current_view.view) {

    case VIEW_1_ACCELERATION_ONLY:
      std::sprintf(
        processed_string,
        array_of_userviews[VIEW_1_ACCELERATION_ONLY],
        data->acceleration[1],
        data->acceleration[2],
        data->acceleration[3]
      );
      break;
    
    default:
      break;
  }

  last_write_n = std::strlen(processed_string);
  adafruit_print(processed_string, std::strlen(processed_string), 0, 0);
}

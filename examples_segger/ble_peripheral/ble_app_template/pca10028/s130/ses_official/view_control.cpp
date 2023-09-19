#include "user_views.hpp"
#include "view_control.h"
#include "adafruit_control.h"
#include <cassert>
#include <cstdio>
#include <new>


static page_info_t page_info = { nullptr, VIEW_1_ACCELERATION_ONLY };

void view_init (void) {
  page_info.current_view_object = new(std::nothrow) User_View_1();
  assert(page_info.current_view_object != nullptr);
}

void view_update( witmotion_data *data ) {
  switch (page_info.current_view_type) {
    
    case VIEW_1_ACCELERATION_ONLY:
      User_View_1* obj = static_cast<User_View_1*>(page_info.current_view_object);
      obj->update_information(
        data->acceleration[1],
        data->acceleration[2],
        data->acceleration[3]
      );
      break;
  }
}
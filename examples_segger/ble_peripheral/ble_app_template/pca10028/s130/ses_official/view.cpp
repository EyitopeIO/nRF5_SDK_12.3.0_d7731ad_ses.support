#include "view.hpp"
#include "adafruit_control.h"


GenericUserView::GenericUserView() {
  previous_page_object = nullptr;
  current_page_object = nullptr;
  write = adafruit_longwrite;
}

GenericUserView::~GenericUserView() {};



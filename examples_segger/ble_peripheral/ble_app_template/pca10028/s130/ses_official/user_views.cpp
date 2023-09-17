#include "user_views.hpp"
#include "page.hpp"
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <cstdlib>


static const char *const view_1_acceletation_only = 
  "Acceleration **\n\n"
  "x: %.2f\n"
  "y: %.2f\n"
  "z: %.2f\n";


User_View_1::User_View_1() {
  this->text_page_object = TextOnlyPage(view_1_acceletation_only);
}

User_View_1::~User_View_1() {};

void User_View_1::update_information(float x, float y, float z) {
  std::size_t n = strlen( this->text_page_object.get_static_text() ) + 21; // strlen("abcd.xy") * 3 = 21
  char s[n];
  std::sprintf(s, this->text_page_object.get_static_text() ,x ,y, z);
  this->write(reinterpret_cast<uint8_t*>(s), strlen(s));
}

#include "page.hpp"
#include <string>
#include <cstdlib>
#include <cstddef>
#include <cstring>

TextOnlyPage::TextOnlyPage(const char* text) {
  this->static_text = text;
}

TextOnlyPage::TextOnlyPage() {};

TextOnlyPage::~TextOnlyPage() {}; 

const char *const TextOnlyPage::get_static_text(void) {
  return this->static_text;
}
#include "page.hpp"

void TextPage_init(SimpleTextPage_t *textpage, const char *const text) {
  if (textpage == nullptr || text == nullptr)
    return;

  textpage->text=text;
}




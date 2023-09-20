#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <cstdint>


enum page_t {
  TEXT_ONLY_PAGE
};


typedef struct {
  const page_types page_type;
  void *previous_page;
} BasePage_t;


typedef struct {
  const char *const text;
  const BasePage_t basepage;
} SimpleTextPage_t;   


/* **************************************************************************/

#endif
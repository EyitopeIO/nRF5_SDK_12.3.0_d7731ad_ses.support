#ifndef _PAGE_HPP_
#define _PAGE_HPP_


enum page_types {
  TEXT_ONLY_PAGE
};


class BasePage {

  protected:
    page_types page_t;
};


class TextOnlyPage: public BasePage {
  
  public:
    TextOnlyPage(const char *const text);
    TextOnlyPage();
    ~TextOnlyPage();
    const char *const get_static_text(void);

  protected:
    const char* static_text;
};



/* **************************************************************************/

#endif
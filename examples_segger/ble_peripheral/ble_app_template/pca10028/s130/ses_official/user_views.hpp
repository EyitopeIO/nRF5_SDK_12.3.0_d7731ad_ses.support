#ifndef  _USER_VIEWS_H
#define  _USER_VIEWS_H


/*
* Represents the actual customisable user content that is displayed on the
* screen, and how the content logically connects to other menus This is the user defines
* logic for the
*/


#include "view.hpp"
#include "page.hpp"


class User_View_1 : public GenericUserView {

  public:
    User_View_1();
    ~User_View_1();
    void update_information(float x, float y, float z);

  private:
    TextOnlyPage text_page_object;
};


#endif
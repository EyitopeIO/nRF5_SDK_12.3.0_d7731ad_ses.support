#ifndef  _VIEW_H
#define  _VIEW_H

#include <cstdint>

class GenericUserView {

  public:
    GenericUserView();
    ~GenericUserView();
    void (*write)(uint8_t* c, uint16_t len);

  protected:
    
    /* page object could be anything and we don't know this ahead of time;
     * hence, it will be void* and casted as needed
    */
    void *current_page_object;
  
    void *previous_page_object;
};

#endif
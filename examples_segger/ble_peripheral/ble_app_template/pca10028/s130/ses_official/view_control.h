#ifndef _VIEW_CONTROL_H
#define _VIEW_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <witmotion_control.h>

enum user_views {
  VIEW_1_ACCELERATION_ONLY,
  VIEW_2_ANGULARVELOCITY_ONLY,
  VIEW_3_ANGLE_ONLY
};

typedef struct page_info_t {
  void* current_view_object;
  user_views current_view_type;
};
 

#ifdef __cplusplus
}
#endif
#endif


void view_init( void );
void view_update ( witmotion_data *data );

/*
 * @file user_views.cpp
 *
 * @brief This file contains the text for the different views that could
 *       be displayed on the user interface.
 */



static const char *view_0_acceletation_only =
  "Acceleration\n\n"
  "x: %.2f       \n"
  "y: %.2f       \n"
  "z: %.2f       \n";

static const char *view_1_angle_only =
  "Angle       \n\n"
  "x: %.2f       \n"
  "y: %.2f       \n"
  "z: %.2f       \n";

static const char *view_2_angularvelocity_only =
  "rad/s        \n\n"
  "x: %.2f        \n"
  "y: %.2f        \n"
  "z: %.2f        \n";

static const char *view_3_notification =
  "NOTICE       \n\n %s";


/// @brief Array of user views
/// @details This array must be kept in sync with @ref user_views
const char *array_of_userviews[] = {
  view_0_acceletation_only,
  view_1_angle_only,
  view_2_angularvelocity_only,
  view_3_notification,
  nullptr
};

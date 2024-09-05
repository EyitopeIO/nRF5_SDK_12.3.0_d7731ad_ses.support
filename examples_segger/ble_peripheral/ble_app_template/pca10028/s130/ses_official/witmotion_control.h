
#ifndef _WITMOTION_CONTROL_H
#define _WITMOTION_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define WIT_MAX_PERIOD 50  // max frequency = 200Hz
#define WIT_TIME_TO_WAIT_FOR_DATA 100
#define WIT_DATA_REQUEST_TIMEOUT 200

typedef struct
{
  float acceleration[3];
  float angular_velocity[3];
  float angle[3];
} witmotion_data;

enum wit_data_status {
  WIT_REQ_SUCCESS = 0,
  WIT_REQ_FAILED,
};

/**
 * @brief Function for initializing the witmotion module.
 *
 * @param[in] start_or_stop 1 to start, 0 to stop
 */
void witmotion_init( int start_or_stop );


/**
 * @brief Function for making a data request to the witmotion module.
 *
 * This function simply sets all variable in preparation for a request
 * and then makes the actual request. It doesn't care about anuthing else.
 */
void wit_make_data_request( void );

/**
 * @brief Function for checking if data is ready.
 *
 * After making a request for data with @ref wit_make_data_request, this
 * function is be used to check if the data is ready for reading.
 *
 * @return WIT_DATA_READY if data is ready, WIT_DATA_NOT_READY if not.
 */
int wit_check_data_ready( void );

/**
 * @brief Function for reading the data.
 *
 * This function returns a pointer to the data structure containing the
 * data from the witmotion module. The data is updated every time a new
 * request is made and the data is ready.
 *
 * @return Pointer to the data structure containing the data.
 */
void wit_read_data( void );


void show_stats( void );
void show_data( void );


#ifdef __cplusplus
}
#endif

#endif
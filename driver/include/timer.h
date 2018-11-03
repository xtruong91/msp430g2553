/*
 * timer.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_TIMER_H_
#define DRIVER_INCLUDE_TIMER_H_

#include "type.h"

/* Time structure */
struct time
{
    unsigned int sec;
    unsigned int ms;
};
/**
 * \brief Initialize the timer module
 * \return TRUE on success, -1 otherwise
 */
BOOL timer_init(void);

/**
 * \brief Create and start a timer
 * \param[in] timeout_ms - the time timeout in ms
 * \param[in] periodic - non-zero for a periodic timer,  0 for single shot
 * \param[in] callback - timer callback function
 * \param[in] arg - callback function private data
 * \return non-negative integer - the timer handle - on success, -1 otherwise
 */
BOOL timer_create(uint16_t timeout_ms, int16_t periodic, void (*callback)(void *), void *arg);

/**
 * \brief Delete a timer
 * \param[in] handle - the timer handle to delete
 * \return 0 if the handle is valid, -1 otherwise
 */
BOOL timer_delete(int16_t handle);

/**
 * \brief Capture the current value of the timer
 * \param[out] time - the time structure to fill with captured time
 * \return 0 on success, -1 otherwise
 */
int timer_capture(struct time *time);


#endif /* DRIVER_INCLUDE_TIMER_H_ */

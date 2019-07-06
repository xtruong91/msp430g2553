/*
 * ring_buffer.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef UTILS_INCLUDE_RING_BUFFER_H_
#define UTILS_INCLUDE_RING_BUFFER_H_

#include "type.h"
#include <stdint.h>
#include <stdlib.h>

/* Ring buffer descriptor */
typedef unsigned int rbd_t;

/* User defined ring buffer attributes */
typedef struct {
    size_t s_elem;
    size_t n_elem;
    void *buffer;
} rb_attr_t;

/**
 * \brief Initialize a ring buffer
 * \param[out] rb - pointer to a ring buffer descriptor
 * \param[in] attr - ring buffer attributes
 * \return 0 on success, -1 otherwise
 *
 * The attributes must contain a ring buffer which is sized
 * to an even power of 2. This should be reflected by the
 * attribute n_elem.
 */
BOOL ring_buffer_init(rbd_t *rbd, rb_attr_t *attr);

/**
 * \brief Add an element to the ring buffer
 * \param[in] rb - the ring buffer descriptor
 * \param[in] data - the data to add
 * \return 0 on success, -1 otherwise
 */
BOOL ring_buffer_put(rbd_t rbd, const void *data);

/**
 * \brief Get (and remove) an element from the ring buffer
 * \param[in] rb - the ring buffer descriptor
 * \param[in] data - pointer to store the data
 * \return 0 on success, -1 otherwise
 */
BOOL ring_buffer_get(rbd_t rbd, void *data);

#endif /* UTILS_INCLUDE_RING_BUFFER_H_ */

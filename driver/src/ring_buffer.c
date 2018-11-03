/*
 * ring_buffer.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#include "ring_buffer.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define RING_BUFFER_MAX  1

struct ring_buffer
{
    size_t s_elem;
    size_t n_elem;
    uint8_t *buf;
    volatile size_t head;
    volatile size_t tail;
};

static struct ring_buffer _rb[RING_BUFFER_MAX];

static int16_t _ring_buffer_full(struct ring_buffer *rb);
static int16_t _ring_buffer_empty(struct ring_buffer *rb);

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
BOOL ring_buffer_init(rbd_t *rbd, rb_attr_t *attr)
{
    static int idx = 0;
    int err = -1;

    if ((idx < RING_BUFFER_MAX) && (rbd != NULL) && (attr != NULL)) {
        if ((attr->buffer != NULL) && (attr->s_elem > 0)) {
            /* Check that the size of the ring buffer is a power of 2 */
            if (((attr->n_elem - 1) & attr->n_elem) == 0) {
                /* Initialize the ring buffer internal variables */
                _rb[idx].head = 0;
                _rb[idx].tail = 0;
                _rb[idx].buf = attr->buffer;
                _rb[idx].s_elem = attr->s_elem;
                _rb[idx].n_elem = attr->n_elem;

                *rbd = idx++;
                err= 0;
            }
        }
    }

    return err;
}

/**
 * \brief Add an element to the ring buffer
 * \param[in] rbd - the ring buffer descriptor
 * \param[in] data - the data to add
 * \return 0 on success, -1 otherwise
 */
BOOL ring_buffer_put(rbd_t rbd, const void *data)
{
    int err = 0;

    if ((rbd < RING_BUFFER_MAX) && (_ring_buffer_full(&_rb[rbd]) == 0)) {
        const size_t offset = (_rb[rbd].head & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
        memcpy(&(_rb[rbd].buf[offset]), data, _rb[rbd].s_elem);
        _rb[rbd].head++;
    } else {
        err = -1;
    }

    return err;
}

/**
 * \brief Get (and remove) an element from the ring buffer
 * \param[in] rb - the ring buffer descriptor
 * \param[in] data - pointer to store the data
 * \return 0 on success, -1 otherwise
 */
BOOL ring_buffer_get(rbd_t rbd, void *data)
{
    int err = 0;
    if ((rbd < RING_BUFFER_MAX) && (_ring_buffer_empty(&_rb[rbd]) == 0)) {
        const size_t offset = (_rb[rbd].tail & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
        memcpy(data, &(_rb[rbd].buf[offset]), _rb[rbd].s_elem);
        _rb[rbd].tail++;
    } else {
        err = -1;
    }

    return err;
}

static int16_t _ring_buffer_full(struct ring_buffer *rb)
{
    return ((rb->head - rb->tail) == rb->n_elem) ? 1 : 0;
}

static int16_t _ring_buffer_empty(struct ring_buffer *rb)
{
    return ((rb->head - rb->tail) == 0U) ? 1 : 0;
}





/*
 * isr.h
 *
 *  Created on: Sep 25, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_ISR_H_
#define DRIVER_INCLUDE_ISR_H_

#include "ring_buffer.h"
#include "type.h"

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define ENDMARKED     '.'

// ring-buffer for usca1 module
extern rbd_t g_rbuart;
extern rb_attr_t g_rbuartAttr;


//ring buffer for usca2 module;
extern rbd_t g_rbusca2;
extern rb_attr_t g_rbusca2Attr;

extern CallBack ISRCallback;



#endif /* DRIVER_ISR_H_ */

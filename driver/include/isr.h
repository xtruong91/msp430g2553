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

// ring-buffer for usca1 module
extern rbd_t g_rbd1;
extern char g_rbmem1[BUFFER_LENGTH];

//ring buffer for usca2 module;
extern rbd_t g_rbd2;
extern char g_rbmem2[BUFFER_LENGTH];

typedef enum{
    RX_UART,
    RX_SPI,
    RX_I2C
}EModule;

typedef struct {
    EModule module;
    void (*cbFunction)(void *arg);
} isr_config;

void subscribe(const isr_config *config);
void unsubscribe(const isr_config *config);


#endif /* DRIVER_ISR_H_ */

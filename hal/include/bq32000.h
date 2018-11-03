/*
 * bq32000.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#ifndef HAL_INCLUDE_BQ32000_H_
#define HAL_INCLUDE_BQ32000_H_

#include "type.h"

#define BQ32000 0x68


typedef struct {
    int8_t second;
    int8_t minute;
    int8_t hour;
    int8_t day;
    int8_t month;
    int8_t year;
    int8_t control;
} DateTime;


BOOL BQ32000_init();
int8_t BQ32000_readData(int8_t *buffer);



#endif /* HAL_INCLUDE_BQ32000_H_ */

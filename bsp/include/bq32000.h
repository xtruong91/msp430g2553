/*
 * bq32000.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#ifndef BSP_INCLUDE_BQ32000_H_
#define BSP_INCLUDE_BQ32000_H_

#include "type.h"

#define BQ32000 0x68

typedef enum{
    SEC_REG = 0x00, //Clock seconds and STOP bit
    MIN_REG = 0x01, // Clock minutes
    HOUR_REG = 0x02, // Clock hours, century, and CENT_EN bit
    DAY_REG = 0x03, // Clock day
    DATE_REG = 0x04, // Clock date
    MONTH_REG = 0x05, //Clock month
    YEAR_REG = 0x06, //Clock years
    CALCFG1_REG = 0x07, //Calibration and configuration
    TCH2_REG = 0x08, //Trickle charge enable
    CFG2_REG = 0x09, //Configuration 2
    SFKEY1_REG = 0x20, //Special function key 1
    SFKEY2_REG = 0x21, //Special function key 2
    SFR_REG = 0x22 //Special function register
}EAddrReg;

typedef struct{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
}SDateTime;

void BQ32000_init();
SDateTime BQ32000_readDateTime();

#endif /* BSP_INCLUDE_BQ32000_H_ */

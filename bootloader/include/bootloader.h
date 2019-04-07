/*
 * bootloader.h
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 */

#ifndef BOOTLOADER_INCLUDE_BOOTLOADER_H_
#define BOOTLOADER_INCLUDE_BOOTLOADER_H_

#include "msp430g2553.h"

#define APP_ROM_START   ((unsigned char *)(0xCA00))
#define APP_ROM_END     ((unsigned char *)(0xFDFF))

#define APP_SEGMENT_START   APP_ROM_START
#define APP_SEGMENT_END     (APP_ROM_END - 512 + 1)

typedef enum
{
    START_STATE = 0,
    MENU_STATE,
    CLEAR_STATE,
    DOWNLOAD_STATE,
    START_APPLICATION_STATE,
    END_STATE
} Bootloader_State_t;

typedef enum
{
    START_FLAG = 0,
    SUCCESS_FLAG,
    FINISH_FLAG
} Bootloader_Flag_t;

#endif /* BOOTLOADER_INCLUDE_BOOTLOADER_H_ */

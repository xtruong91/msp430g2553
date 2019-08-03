/*
 * lebmatrix.h
 *
 *  Created on: Jun 28, 2019
 *      Author: truongtx
 */

#ifndef APP_INCLUDE_LEBMATRIX_H_
#define APP_INCLUDE_LEBMATRIX_H_


#include "gpio.h"
#include "helper.h"
#include "type.h"

#define LATCH   P1_0

// row and column  of LED martix
uint8 row[] = {1, 2, 4, 8, 16, 32, 64, 128};
uint8 column[] = {128, 64, 32, 16, 8, 4, 2, 1};

uint8 characterHEX[][8] = {
    {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x66},//A
    {0x78,0x64,0x68,0x78,0x64,0x66,0x66,0x7C},//B
    {0x3C,0x62,0x60,0x60,0x60,0x62,0x62,0x3C},//C
    {0x78,0x64,0x66,0x66,0x66,0x66,0x64,0x78}//D
};

uint8 offLED[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 onLED[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

uint8 character[] = {'A','B','C'};

extern Ret LedMatrix_init();

extern Ret LedMatrix_run();

/*
 * @param: image- a specific character
 * @times: number blink
 * @on: effect duration when turning on led.
 * @off: effect duration when turning off led.
 * */
void blinkImage(uint8 image[],int times, int on, int off);

/*
 * Clear image on led matrix
 * @param: duration-time for displaying.
 * */
void clearImage(int duration);
/*
 * show image on led matrix
 * */
void displayImage(uint8 image[],int duration);
/*
 * scroll image on led matrix
 * */
void scrollImage(uint8 image[]);

void initImage();

#endif /* APP_INCLUDE_LEBMATRIX_H_ */

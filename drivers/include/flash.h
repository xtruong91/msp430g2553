/*
 * flash.h
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 * Flash memory divide into Segments, it contains flash main memory and flash information memory.
 *
 * flash main memory: 32Kb or 64 segment (1 segment = 512 byte) 0x0F000 -> 0xFFFF
 * flash information memory: 512 byte or 4 segment (1 segment = 128 byte) 0x01000 -> 0x010FF
 * Segment A has private lock due to DCO data  which LOCKA/FCTL1: 1 : restrict write or clear data, 0 allow clear all datas.
 *  Segment size is 512B
 */

#ifndef DRIVERS_INCLUDE_FLASH_H_
#define DRIVERS_INCLUDE_FLASH_H_

#include "type.h"

typedef enum
{
    Segment_A = 0x10C0,
    Segment_B = 0x1080,
    Segment_C = 0x1040,
    Semegnt_D = 0x1000,
    Segment_Main = 0xFFFF
}SegmentAddr;

void flash_init(void);
/*
 * Clear specify segment.
 * @param: address of segment.
 * */
void flash_eraseSegment(SegmentAddr segment);
/*
 * Clear both main memory and information memory
 * */
void flash_eraseAll(void);

/*
 * Clear all segment in main memory
 * */
void flash_earseMainMemory(void);

int flash_writeBlock(SegmentAddr segment, const char* data, unsigned int length);

int flash_readBlock(SegmentAddr segment, char* data, unsigned int length);

#endif /* DRIVERS_INCLUDE_FLASH_H_ */

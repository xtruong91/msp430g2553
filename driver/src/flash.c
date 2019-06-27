/*
 * flash.c
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 */

#include "flash.h"
#include "msp430g2553.h"

void flash_init(void)
{
    /*
     * FWKEY: Flash key for write
     * FSSEL_2: Select source clock
     * */
    FCTL2 = FWKEY + FSSEL_2 + FN3;
}

/* Segment size is 512B */
void flash_eraseSegment(SegmentAddr segment)
{
    char *ptr;
    ptr = (char*)segment; // Initialize flash pointer
    FCTL1 = FWKEY + ERASE; // set Erase bit
    FCTL3 = FWKEY;  // Clear Lock bit

    *ptr = 0xFF; // Dummy write to erase Flash segment

    while (FCTL3 & BUSY);

    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

void flash_eraseAll(void)
{
    char* baseOffSetMain =(char*)(Segment_Main);

    // delete all;
    int i;
    for(i = 0; i < 64; i++)
    {
        baseOffSetMain = (char*)(Segment_Main + i);
        *baseOffSetMain = 0xFF;
    }

}

int flash_writeBlock(SegmentAddr segment, const char* data, unsigned int length)
{
    if(data == NULL || length == 0)
    {
        return -1;
    }

    unsigned char *ptr = (unsigned char*)segment; // Flash pointer

    FCTL1 = FWKEY + WRT; // Set WRT bit for write operation
    FCTL3 = FWKEY; //Clear Lock bit

    while (FCTL3 & BUSY);
    int i;
    for(i = 0; i < length; i++)
    {
        *ptr = data[i];
    }


    FCTL1 = FWKEY; // Clear WRT bit
    FCTL3 = FWKEY + LOCK; // Set LOCK bit
    return i;
}

int flash_readBlock(SegmentAddr segment, char* data, unsigned int length)
{
    if(data == NULL || length == 0)
        return -1;
    char *Flash_ptr = (char*)segment;              // Segment C pointer

    int i;
    for(i = 0; i < length; i++)
    {
        data[i] = *Flash_ptr;// copy value segment C to variable
        Flash_ptr += i; // Jump to desired address
    }
    return i;
}




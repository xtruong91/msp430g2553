/*
 * compile.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 *
 *      build application depend product type
 *
 */

#ifndef COMPILEOPTION_H_
#define COMPILEOPTION_H_

// Select a board for building
#define BOARD_VERSION_LoRaMASTER    1
#define BOARD_VERSION_LoRaSLAVE    2
#define BOARD_VERSION_MASTER1   3
#define BOARD_VERSION_MASTER2   4
#define BOARD_VERSION_SERVO     5
#define BOARD_VERSION_LORA     6 // used for test LoRa network

// ----------------------------------- Board selection --------------------------------------------//
#define BOARD_VERSION           BOARD_VERSION_LoRaMASTER

#if (BOARD_VERSION == BOARD_VERSION_LoRaMASTER)
    #define MASTER_VERSION      1
#elif (BOARD_VERSION == BOARD_VERSION_LoRaSLAVE)
    #define SLAVE2_VERSION      1
#elif(BOARD_VERSION == BOARD_VERSION_MASTER1)
    #define MASTER1_VERSION      1
#elif (BOARD_VERSION == BOARD_VERSION_MASTER2)
    #define MASTER2_VERSION     1
#elif (BOARD_VERSION == BOARD_VERSION_LORA)
    #define LORA_VERSION       1
#elif (BOARD_VERSION == BOARD_VERSION_SERVO)
    #define SERVO_VERSION       1
#else
    #error Select complied board invalid
#endif
// ----------------------------------- TransIBC protocol selection --------------------------------------------//
#define TRANSISC_EN                                 1   // Enable IBC transmission library
#define TRANSISC_LIVE_MONITOR_EN                    0   // Enable live monitoring function
// ----------------------------------- Debug module ----------------------------------------------- //
#define     DEBUG_EN                                1   // Enable Debug library
#define     PERFORMANCE_COUNTER_EN                  0   // Enable performance counter
#define     DEBUG_SAMPLE_DATA_EN                    0   // Enable sampling data for testing purpose


// ----------------------------------- Sleep mode ------------------------------------------------- //
#define     PWR_SAV_MODE_EN                         1   // Enable Sleep mode


// ----------------------------------- Data logger ------------------------------------------------ //
#define     DATA_LOGGER_EN                          1   // Enable data logger
// -------------------------------------- Working mode -------------------------------------------- //
#define     STATION_ALONE                           1   // Enable Stand-alone working mode which is
                                                        // without cooperating with Master
#define     RECOVERY_MODE_EN                        1   // Enable Recovery mode. To fully enable this module
                                                        // we also need to enabling WDT & Traps inside Recovery.h file
#endif /* COMPILEOPTION_H_ */

/*
 * board.h
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include <stdint.h>

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3

#define TIMER_FREQUENCY_HZ (1000u)

void BSP_Init(void);

void BSP_CommutationTimerCallback(void);
void BSP_LowFrequencyTimerCallback(void);
void BSP_ConversionCompleteCallback(void);
void BSP_ButtonCallback(void);
void BSP_StartConversion(uint8_t channel);
void BSP_SetCommutationTimerPeriod(uint32_t period);
void BSP_SetIndicator(uint8_t led, uint8_t state);

#endif /* INCLUDE_BOARD_H_ */

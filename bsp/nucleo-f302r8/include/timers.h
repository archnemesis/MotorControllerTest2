/*
 * timers.h
 *
 *  Created on: Mar 6, 2022
 *      Author: Robin
 */

#ifndef NUCLEO_F302R8_INCLUDE_TIMERS_H_
#define NUCLEO_F302R8_INCLUDE_TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim16;

/**
 * Initialize TIM1 used for three-phase PWM output.
 */
void BSP_TIM1_Init(void);

/**
 * Initialize TIM2 used for high-frequency tasks.
 */
void BSP_TIM2_Init(void);

/**
 * Initialize TIM16 used for current reference PWM output.
 */
void BSP_TIM16_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* NUCLEO_F302R8_INCLUDE_TIMERS_H_ */

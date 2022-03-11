/*
 * adc.h
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#ifndef NUCLEO_F302R8_INCLUDE_ADC_H_
#define NUCLEO_F302R8_INCLUDE_ADC_H_

#include "stm32f3xx_hal.h"

extern ADC_HandleTypeDef hadc1;


void BSP_ADC_Init(void);

#endif /* NUCLEO_F302R8_INCLUDE_ADC_H_ */

/*
 * config.h
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#ifndef NUCLEO_F302R8_INCLUDE_CONFIG_H_
#define NUCLEO_F302R8_INCLUDE_CONFIG_H_

#include "stm32f3xx_hal.h"

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define CURR_SENS_Pin GPIO_PIN_1
#define CURR_SENS_GPIO_Port GPIOC
#define TEMP_SENS_Pin GPIO_PIN_2
#define TEMP_SENS_GPIO_Port GPIOC
#define BEMF1_Pin GPIO_PIN_3
#define BEMF1_GPIO_Port GPIOC
#define VBUS_SENS_Pin GPIO_PIN_1
#define VBUS_SENS_GPIO_Port GPIOA
#define BEMF3_Pin GPIO_PIN_7
#define BEMF3_GPIO_Port GPIOA
#define BEMF2_Pin GPIO_PIN_0
#define BEMF2_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOB
#define DEBUG1_Pin GPIO_PIN_12
#define DEBUG1_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define L6230_EN1_Pin GPIO_PIN_10
#define L6230_EN1_GPIO_Port GPIOC
#define L6230_EN2_Pin GPIO_PIN_11
#define L6230_EN2_GPIO_Port GPIOC
#define L6230_EN3_Pin GPIO_PIN_12
#define L6230_EN3_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

#endif /* NUCLEO_F302R8_INCLUDE_CONFIG_H_ */

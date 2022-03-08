/*
 * L6230 Driver Configuration
 *
 *  Created on: Feb 28, 2022
 *      Author: Robin
 */

#ifndef INCLUDE_L6230_CONFIG_H_
#define INCLUDE_L6230_CONFIG_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_gpio.h"

/* Output Enable Pins (OUTPUTS) */
#define L6230_EN1_PORT	GPIOC
#define L6230_EN1_PIN	GPIO_PIN_10
#define L6230_EN2_PORT	GPIOC
#define L6230_EN2_PIN	GPIO_PIN_11
#define L6230_EN3_PORT	GPIOC
#define L6230_EN3_PIN	GPIO_PIN_12

/* Output Command Pins (OUTPUTS) */
#define L6230_IN1_PORT	GPIOA
#define L6230_IN1_PIN	GPIO_PIN_8
#define L6230_IN2_PORT	GPIOA
#define L6230_IN2_PIN	GPIO_PIN_9
#define L6230_IN3_PORT	GPIOA
#define L6230_IN3_PIN	GPIO_PIN_10

#endif /* INCLUDE_L6230_CONFIG_H_ */

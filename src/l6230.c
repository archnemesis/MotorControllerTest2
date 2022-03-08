/*
 * l6230.c
 *
 *  Created on: Feb 28, 2022
 *      Author: Robin
 */

#include "l6230.h"
#include "l6230_config.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_tim.h"

static TIM_HandleTypeDef htim1;
static TIM_HandleTypeDef htim16;
static TIM_HandleTypeDef htim2;


static status_t timer1_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init.Pin = GPIO_PIN_12;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_init.Alternate = GPIO_AF11_TIM1;

	HAL_GPIO_Init(GPIOA, &gpio_init);

	return STATUS_OK;
}


static status_t timer1_init(void)
{
	TIM_ClockConfigTypeDef clock_src_config = {0};
	TIM_ClearInputConfigTypeDef clear_input_config = {0};
	TIM_MasterConfigTypeDef master_config = {0};
	TIM_OC_InitTypeDef oc_config = {0};
	TIM_BreakDeadTimeConfigTypeDef break_deadtime_config = {0};

	__HAL_RCC_TIM1_CLK_ENABLE();

	//
	// Basic timer settings
	//
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0; // TODO: config file
	htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
	htim1.Init.Period = 639; // TODO: config file
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	//
	// Clock source is INTERNAL clock
	//
	clock_src_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim1, &clock_src_config) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	//
	// ETR allows us to turn off the PWM outputs when the current limit is
	// reached.
	//
	clear_input_config.ClearInputState = ENABLE;
	clear_input_config.ClearInputSource = TIM_CLEARINPUTSOURCE_ETR;
	clear_input_config.ClearInputPolarity = TIM_CLEARINPUTPOLARITY_NONINVERTED;
	clear_input_config.ClearInputPrescaler = TIM_CLEARINPUTPRESCALER_DIV1;
	clear_input_config.ClearInputFilter = 0;

	if (HAL_TIM_ConfigOCrefClear(&htim1, &clear_input_config, TIM_CHANNEL_1) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_ConfigOCrefClear(&htim1, &clear_input_config, TIM_CHANNEL_2) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_ConfigOCrefClear(&htim1, &clear_input_config, TIM_CHANNEL_3) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	//
	// This timer will be a master to the ADC timer
	//
	master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
	master_config.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &master_config) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	//
	// Output Compare Configuration
	//
	oc_config.OCMode = TIM_OCMODE_PWM1;
	oc_config.Pulse = 0;
	oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	oc_config.OCFastMode = TIM_OCFAST_DISABLE;
	oc_config.OCIdleState = TIM_OCIDLESTATE_RESET;
	oc_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &oc_config, TIM_CHANNEL_1) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &oc_config, TIM_CHANNEL_2) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &oc_config, TIM_CHANNEL_3) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	//
	// Break/Dead-Time Configuration
	//
	break_deadtime_config.OffStateRunMode = TIM_OSSR_DISABLE;
	break_deadtime_config.OffStateIDLEMode = TIM_OSSI_DISABLE;
	break_deadtime_config.LockLevel = TIM_LOCKLEVEL_OFF;
	break_deadtime_config.DeadTime = 0;
	break_deadtime_config.BreakState = TIM_BREAK_DISABLE;
	break_deadtime_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;
	break_deadtime_config.BreakFilter = 0;
	break_deadtime_config.Break2State = TIM_BREAK2_DISABLE;
	break_deadtime_config.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	break_deadtime_config.Break2Filter = 0;
	break_deadtime_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &break_deadtime_config) != HAL_OK)
	{
		return STATUS_ERR_HAL;
	}

    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

	return STATUS_OK;
}


static status_t timer2_init(void)
{
	TIM_ClockConfigTypeDef clock_config = {0};

	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 63;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 999;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	clock_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim2, &clock_config) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	return STATUS_OK;
}


static status_t timer16_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_init.Pin = GPIO_PIN_4;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_init.Alternate = GPIO_AF1_TIM16;

	HAL_GPIO_Init(GPIOB, &gpio_init);

	return STATUS_OK;
}


static status_t timer16_init(void)
{
	TIM_OC_InitTypeDef oc_config = {0};

	__HAL_RCC_TIM16_CLK_ENABLE();

	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 63;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 999;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	oc_config.OCMode = TIM_OCMODE_PWM1;
	oc_config.Pulse = 0;
	oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	oc_config.OCFastMode = TIM_OCFAST_DISABLE;
	oc_config.OCIdleState = TIM_OCIDLESTATE_RESET;
	oc_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_PWM_Init(&htim16) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim16, &oc_config, TIM_CHANNEL_1) != HAL_OK) {
		return STATUS_ERR_HAL;
	}

    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

	return STATUS_OK;
}


status_t l6230_init()
{
	GPIO_InitTypeDef init = {
			.Pull = GPIO_NOPULL,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Speed = GPIO_SPEED_FREQ_HIGH
	};

	init.Pin = L6230_EN1_PIN;
	HAL_GPIO_Init(L6230_EN1_PORT, &init);

	init.Pin = L6230_EN2_PIN;
	HAL_GPIO_Init(L6230_EN2_PORT, &init);

	init.Pin = L6230_EN3_PIN;
	HAL_GPIO_Init(L6230_EN3_PORT, &init);

	init.Pin = L6230_IN1_PIN;
	HAL_GPIO_Init(L6230_IN1_PORT, &init);

	init.Pin = L6230_IN2_PIN;
	HAL_GPIO_Init(L6230_IN2_PORT, &init);

	init.Pin = L6230_IN3_PIN;
	HAL_GPIO_Init(L6230_IN3_PORT, &init);

	// TODO: init ADCs
	// TODO: init PWM
	// TODO: init timers

	return STATUS_OK;
}

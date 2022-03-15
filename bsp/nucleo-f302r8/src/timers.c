/*
 * tim.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Robin
 */

#include "timers.h"
#include "error.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_tim.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim16;


static void BSP_TIM1_PostInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*
	 * PA11 -> TIM1_CH1N
	 * PB14 -> TIM1_CH2N
	 * PB15 -> TIM1_CH3N
	 */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*
	 * PA8  -> TIM1_CH1
	 * PA9  -> TIM1_CH2
	 * PA10 -> TIM1_CH3
	 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
 * TIM1 is used to generate the 100kHz PWM signals that control the motor. This
 * timer controls all 3 phases using the special 3-phase PWM output mode. ETR is
 * enabled for current limiting.
 */
void BSP_TIM1_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_ClearInputConfigTypeDef sClearInputConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	__HAL_RCC_TIM1_CLK_ENABLE();

	//
	// Basic timer settings
	//
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0; // TODO: config file
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 719; // TODO: config file
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}

	//
	// Clock source is INTERNAL clock
	//
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}

	//
	// ETR allows us to turn off the PWM outputs when the current limit is
	// reached.
	//
	sClearInputConfig.ClearInputState = ENABLE;
	sClearInputConfig.ClearInputSource = TIM_CLEARINPUTSOURCE_ETR;
	sClearInputConfig.ClearInputPolarity = TIM_CLEARINPUTPOLARITY_NONINVERTED;
	sClearInputConfig.ClearInputPrescaler = TIM_CLEARINPUTPRESCALER_DIV1;
	sClearInputConfig.ClearInputFilter = 0;

	if (HAL_TIM_ConfigOCrefClear(&htim1, &sClearInputConfig, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_ConfigOCrefClear(&htim1, &sClearInputConfig, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_ConfigOCrefClear(&htim1, &sClearInputConfig, TIM_CHANNEL_3) != HAL_OK) {
		Error_Handler();
	}

	//
	// This timer will be a master to the ADC timer
	//
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}

	//
	// Output Compare Configuration
	//
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
		Error_Handler();
	}

	//
	// Break/Dead-Time Configuration
	//
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 25;		// DeadTime * (1/t_fCLK)
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}

    HAL_TIMEx_ConfigCommutEvent(&htim1, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);

    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    BSP_TIM1_PostInit();
}


static void BSP_TIM16_PostInit(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*
	 * PB4 -> TIM16_CH1
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM16;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/**
 * TIM16 is used to generate the current limit reference voltage via PWM and a
 * low-pass filter. PWM frequency is 1kHz.
 */
void BSP_TIM16_Init(void)
{
	TIM_OC_InitTypeDef sConfigOC = {0};

	__HAL_RCC_TIM16_CLK_ENABLE();

	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 71;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 999;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_PWM_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    BSP_TIM16_PostInit();
}


/**
 * TIM2 is the commutation timer. While running, it's period will be equal to
 * 60 electrical degrees, or one commutation period.
 */
void BSP_TIM2_Init(void)
{
	TIM_ClockConfigTypeDef clock_config = {0};

	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 71;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 999;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}

	clock_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim2, &clock_config) != HAL_OK) {
		Error_Handler();
	}

    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef * htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*
		 * PA12 -> TIM1_ETR
		 */
	    GPIO_InitStruct.Pin = GPIO_PIN_12;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF11_TIM1;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
	}
	else if (htim->Instance == TIM16) {
		__HAL_RCC_TIM16_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/*
		 * PB4 -> TIM16_CH1
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM16;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 0);
	    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}
	else if (htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();

	    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void TIM1_UP_TIM16_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
	HAL_TIM_IRQHandler(&htim16);
}


void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

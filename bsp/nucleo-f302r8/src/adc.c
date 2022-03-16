/*
 * adc.c
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#include "adc.h"
#include "config.h"
#include "error.h"

ADC_HandleTypeDef hadc1;

void BSP_ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
	sConfig.Offset = 0;
	sConfig.OffsetNumber = 0;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}


void HAL_ADC_MspInit(ADC_HandleTypeDef * hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (hadc->Instance == ADC1) {
		__HAL_RCC_ADC1_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/**
		 * ADC1 GPIO Configuration
		 * PC1 -> ADC1_IN7
		 * PC2 -> ADC1_IN8
		 * PC3 -> ADC1_IN9
		 * PA1 -> ADC1_IN2
		 * PA7 -> ADC1_IN15
		 * PB0 -> ADC1_IN11
		 */
		GPIO_InitStruct.Pin = CURR_SENS_Pin|TEMP_SENS_Pin|BEMF1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = VBUS_SENS_Pin|BEMF3_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = BEMF2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(BEMF2_GPIO_Port, &GPIO_InitStruct);

	    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(ADC1_IRQn);
	}
}


void ADC1_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc1);
}


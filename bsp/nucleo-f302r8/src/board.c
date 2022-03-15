/*
 * board.c
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#include "board.h"
#include "config.h"
#include "gpio.h"
#include "timers.h"
#include "adc.h"
#include "l6230.h"


void BSP_Init(void)
{
	SysTick_Config (SystemCoreClock / TIMER_FREQUENCY_HZ);

	BSP_GPIO_Init();
	BSP_ADC_Init();
	BSP_L6230_Init();

	BSP_TIM1_Init();
	BSP_TIM2_Init();
	BSP_TIM16_Init();
}


void BSP_StartConversion(uint8_t channel)
{
	hadc1.Instance->CR |= ADC_CR_ADSTP;
	while (hadc1.Instance->CR & ADC_CR_ADSTP);
	hadc1.Instance->SQR1 &= ~ADC_SQR1_RK(ADC_SQR2_SQ5, 1);
	hadc1.Instance->SQR1 |= ADC_SQR1_RK(channel, 1);
	hadc1.Instance->CR |= ADC_CR_ADSTART;
}


void BSP_SetIndicator(uint8_t led, uint8_t state)
{
	switch (led) {
	case 0:
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (state == 1 ? GPIO_PIN_SET : GPIO_PIN_RESET));
		break;
	default:
		break;
	}
}


void BSP_ToggleIndicator(uint8_t led)
{
    switch (led) {
    case 0:
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        break;
    default:
        break;
    }
}


void BSP_SetOutputChannelState(uint32_t channel, uint8_t state, uint32_t pwm)
{
	switch (state) {
	case CHANNEL_STATE_HIGH:
		HAL_TIMEx_PWMN_Start(&htim1, channel);
		__HAL_TIM_SET_COMPARE(&htim1, channel, pwm);
		break;
	case CHANNEL_STATE_LOW:
		HAL_TIMEx_PWMN_Start(&htim1, channel);
		__HAL_TIM_SET_COMPARE(&htim1, channel, 0);
		break;
	case CHANNEL_STATE_FLOAT:
        HAL_TIMEx_PWMN_Stop(&htim1, channel);
        __HAL_TIM_SET_COMPARE(&htim1, channel, 0);
        break;
	}
}


/**
 * Forward the timer interrupts to the appropriate application callbacks.
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM16) {
		BSP_LowFrequencyTimerCallback();
	}
	else if (htim->Instance == TIM2) {
		BSP_CommutationTimerCallback();
	}
}


/**
 * Forward the ADC conversion complete interrupt to the application callback.
 * @param hadc
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	UNUSED(hadc);
	BSP_ConversionCompleteCallback();
}


void SysTick_Handler(void)
{
	HAL_IncTick();
}


__weak void BSP_LowFrequencyTimerCallback(void)
{
	/*
	 * DO NOT EDIT THIS METHOD DIRECTLY - It should be redefined in the
	 * main application code.
	 */
}


__weak void BSP_CommutationTimerCallback(void)
{
	/*
	 * DO NOT EDIT THIS METHOD DIRECTLY - It should be redefined in the
	 * main application code.
	 */
}


__weak void BSP_ConversionCompleteCallback(void)
{
	/*
	 * DO NOT EDIT THIS METHOD DIRECTLY - It should be redefined in the
	 * main application code.
	 */
}

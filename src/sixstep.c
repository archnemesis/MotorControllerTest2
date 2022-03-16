/*
 * sixstep.c
 *
 *  Created on: Mar 5, 2022
 *      Author: Robin
 */

#include "sixstep.h"
#include "board.h"
#include "timers.h"
#include "adc.h"

#define PWM_VAL 500

static uint8_t m_step = 0;


void SIXSTEP_Start(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

    __HAL_TIM_SET_COUNTER(&htim1, 719);

    HAL_TIM_Base_Start(&htim1);

    while (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK);
    HAL_ADC_Start_IT(&hadc1);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    HAL_TIM_Base_Start_IT(&htim2);
}


void SIXSTEP_Stop(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIM_Base_Stop(&htim1);
}


void BSP_CommutationTimerCallback(void)
{
    BSP_ToggleIndicator(1);

    switch (m_step)
    {
    case 0:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_HIGH, PWM_VAL);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_LOW, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_FLOAT, 0);

        break;
    case 1:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_HIGH, PWM_VAL);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_FLOAT, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_LOW, 0);

        break;
    case 2:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_FLOAT, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_HIGH, PWM_VAL);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_LOW, 0);

        break;
    case 3:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_LOW, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_HIGH, PWM_VAL);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_FLOAT, 0);

      break;
    case 4:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_LOW, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_FLOAT, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_HIGH, PWM_VAL);

      break;
    case 5:
    	BSP_SetOutputChannelState(TIM_CHANNEL_1, CHANNEL_STATE_FLOAT, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_2, CHANNEL_STATE_LOW, 0);
    	BSP_SetOutputChannelState(TIM_CHANNEL_3, CHANNEL_STATE_HIGH, PWM_VAL);

        break;
    }

    if (++m_step == 6) {
        m_step = 0;
    }

    HAL_TIM_GenerateEvent(&htim1, TIM_EVENTSOURCE_COM);
}


void BSP_ConversionCompleteCallback(void)
{
	BSP_ToggleIndicator(0);
}

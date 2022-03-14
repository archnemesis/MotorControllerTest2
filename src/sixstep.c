/*
 * sixstep.c
 *
 *  Created on: Mar 5, 2022
 *      Author: Robin
 */

#include "sixstep.h"
#include "board.h"
#include "timers.h"

#define PWM_VAL 100

static uint8_t m_step = 6;


void SIXSTEP_Start(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 150);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

    HAL_TIMEx_ConfigCommutEvent(&htim1, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    BSP_CommutationTimerCallback();

    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);
}

void commutate(void)
{

}

void BSP_CommutationTimerCallback(void)
{
    BSP_ToggleIndicator(0);

    switch (m_step)
    {
    case 0:
        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_VAL);      // CH1/CH1N are complementary

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);            // CH2/CH2N are complementary

        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);            // CH3/CH3N are inactive

        break;
    case 1:
        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_VAL);    // CH1/CH1N are complementary

        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);          // CH2/CH2N are inactive

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);          // CH3/CH3N are complementary

        break;
    case 2:
        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);          // CH1/CH1N are inactive

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_VAL);    // CH2/CH2N are complementary

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);          // CH3/CH3N are complementary

        break;
    case 3:
        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);          // CH1/CH1N are complementary

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_VAL);    // CH2/CH2N are complementary

        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);            // CH3/CH3N are inactive
      break;
    case 4:
        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);          // CH1/CH1N are complementary

        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);          // CH2/CH2N are inactive

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_VAL);    // CH3/CH3N are complementary
      break;
    case 5:
        //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);          // CH1/CH1N are inactive

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);            // CH2/CH2N are complementary

        //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_VAL);    // CH3/CH3N are complementary
        break;
    }

    if (++m_step == 6) {
        m_step = 0;
    }

    HAL_TIM_GenerateEvent(&htim1, TIM_EVENTSOURCE_COM);
}

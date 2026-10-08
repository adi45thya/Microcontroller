/**
  ******************************************************************************
  * @file           : servo.c
  * @brief          : Driver implementation for SG90 Servo Motor on TIM1_CH1.
  ******************************************************************************
  */

#include "servo.h"
#include "tim.h"

/* Initialize the servo PWM output */
void Servo_Init(void)
{
    /* Start PWM generation on TIM1 Channel 1 (PA8) */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    
    /* Ensure valve starts in CLOSED position */
    Valve_Close();
}

/* Set servo position by angle (0 to 180 degrees) */
void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180)
    {
        angle = 180;
    }

    /* Convert angle (0 - 180 deg) to pulse width (1000 - 2000 us) */
    uint32_t pulse = SERVO_MIN_PULSE + (((uint32_t)angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180);

    /* Update TIM1 Capture/Compare Register 1 */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse);
}

/* Open the mechanical valve */
void Valve_Open(void)
{
    Servo_SetAngle(VALVE_OPEN_ANGLE);
}

/* Close the mechanical valve */
void Valve_Close(void)
{
    Servo_SetAngle(VALVE_CLOSED_ANGLE);
}

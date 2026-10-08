/**
  ******************************************************************************
  * @file           : servo.h
  * @brief          : Header file for SG90 Servo Motor Valve Controller.
  ******************************************************************************
  */

#ifndef __SERVO_H
#define __SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* Servo pulse limits in microseconds (1 tick = 1 us with PSC=71, ARR=19999) */
#define SERVO_MIN_PULSE     1000   /* 1.0 ms pulse for 0 degrees */
#define SERVO_MAX_PULSE     2000   /* 2.0 ms pulse for 180 degrees */

/* Valve angle configurations */
#define VALVE_CLOSED_ANGLE  0      /* 0 degrees = Closed valve */
#define VALVE_OPEN_ANGLE    90     /* 90 degrees = Open valve */

/* Public Functions */
void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);
void Valve_Open(void);
void Valve_Close(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H */

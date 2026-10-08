/**
  ******************************************************************************
  * @file           : sensors.h
  * @brief          : Header file for Analog Sensors (Soil Moisture & Raindrop).
  ******************************************************************************
  */

#ifndef __SENSORS_H
#define __SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdbool.h>

/* Calibration constants for Soil Moisture Sensor (YL-69 + LM393) */
/* (In dry air, reading is high ~3900. In pure water/wet soil, reading is low ~1300) */
#define SOIL_DRY_RAW        3900
#define SOIL_WET_RAW        1300

/* Raindrop sensor threshold (MH-RD module) */
/* When dry, reading ~4000. When water drops hit pad, reading drops below 2500 */
#define RAIN_THRESHOLD      2500

/* Public Sensor Functions */
uint32_t Sensors_ReadADC(uint32_t channel);
uint8_t Sensors_GetSoilMoisturePercent(void);
bool Sensors_IsRaining(void);

#ifdef __cplusplus
}
#endif

#endif /* __SENSORS_H */

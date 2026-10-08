/**
  ******************************************************************************
  * @file           : dht11.h
  * @brief          : Header file for DHT11 Temperature & Humidity Sensor driver.
  ******************************************************************************
  */

#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* Return status codes */
typedef enum {
    DHT11_OK = 0,
    DHT11_ERROR_TIMEOUT,
    DHT11_ERROR_CHECKSUM
} DHT11_StatusTypeDef;

/* Structure to hold sensor readings */
typedef struct {
    uint8_t temperature; /* Temperature in Celsius */
    uint8_t humidity;    /* Relative Humidity in % */
} DHT11_Data_t;

/* Public API Functions */
void DHT11_Init(void);
DHT11_StatusTypeDef DHT11_Read(DHT11_Data_t *data);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */

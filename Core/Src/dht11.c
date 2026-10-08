/**
  ******************************************************************************
  * @file           : dht11.c
  * @brief          : Driver implementation for DHT11 sensor using DWT microsecond timer.
  ******************************************************************************
  */

#include "dht11.h"

/* Initialize DWT (Data Watchpoint and Trace) cycle counter for microsecond delays */
void DHT11_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* Cycle-accurate microsecond delay based on 72 MHz system clock */
void delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < ticks);
}

/* Set DHT11 data pin as Output */
static void DHT11_SetPin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DATA_GPIO_Port, &GPIO_InitStruct);
}

/* Set DHT11 data pin as Input with Pull-up */
static void DHT11_SetPin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_DATA_GPIO_Port, &GPIO_InitStruct);
}

/* Read 1 byte (8 bits) from DHT11 */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t value = 0;
    for (int i = 0; i < 8; i++)
    {
        /* Wait for pin to go HIGH (start of bit) with timeout */
        uint32_t timeout = 1000;
        while (!HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
        {
            if (--timeout == 0) return 0;
            delay_us(1);
        }

        /* Wait ~40 microseconds to sample pulse length */
        delay_us(40);

        if (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
        {
            /* Line is still HIGH after 40us -> bit is '1' */
            value |= (1 << (7 - i));
            
            /* Wait for line to go LOW before next bit */
            timeout = 1000;
            while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
            {
                if (--timeout == 0) break;
                delay_us(1);
            }
        }
        /* If line already went LOW, bit is '0' (already 0 in value) */
    }
    return value;
}

/* Read Temperature and Humidity from DHT11 */
DHT11_StatusTypeDef DHT11_Read(DHT11_Data_t *data)
{
    uint8_t raw_bytes[5] = {0};
    uint32_t timeout;

    /* Step 1: Send Start Signal (Pull LOW for 18ms, then HIGH for 30us) */
    DHT11_SetPin_Output();
    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET);
    HAL_Delay(18); // 18 milliseconds

    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET);
    delay_us(30);

    /* Step 2: Switch to Input and wait for DHT11 response pulse */
    DHT11_SetPin_Input();

    /* Wait for DHT11 to pull line LOW (approx 20-40us) */
    timeout = 100;
    while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
    {
        if (--timeout == 0) return DHT11_ERROR_TIMEOUT;
        delay_us(1);
    }

    /* Wait for DHT11 to release line HIGH (approx 80us) */
    timeout = 100;
    while (!HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
    {
        if (--timeout == 0) return DHT11_ERROR_TIMEOUT;
        delay_us(1);
    }

    /* Wait for DHT11 to pull line LOW before transmission begins (approx 80us) */
    timeout = 100;
    while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin))
    {
        if (--timeout == 0) return DHT11_ERROR_TIMEOUT;
        delay_us(1);
    }

    /* Step 3: Read 5 Bytes (40 bits total) */
    for (int i = 0; i < 5; i++)
    {
        raw_bytes[i] = DHT11_ReadByte();
    }

    /* Step 4: Verify Checksum (Byte 0 + Byte 1 + Byte 2 + Byte 3 == Byte 4) */
    uint8_t checksum = raw_bytes[0] + raw_bytes[1] + raw_bytes[2] + raw_bytes[3];
    if (checksum != raw_bytes[4])
    {
        return DHT11_ERROR_CHECKSUM;
    }

    /* Success: Store data */
    data->humidity = raw_bytes[0];
    data->temperature = raw_bytes[2];

    return DHT11_OK;
}

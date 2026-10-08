/**
  ******************************************************************************
  * @file           : sensors.c
  * @brief          : Implementation for Soil Moisture and Raindrop sensor readings.
  ******************************************************************************
  */

#include "sensors.h"
#include "adc.h"

/* Read averaged ADC value from specific channel */
uint32_t Sensors_ReadADC(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

    /* Ensure single conversion mode in regular sequence */
    hadc1.Instance->SQR1 &= ~(0x000F0000U); // 1 conversion (L = 0)
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        return 0;
    }

    uint32_t total = 0;
    const int samples = 8; // Average 8 readings for stable signal

    for (int i = 0; i < samples; i++)
    {
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
        {
            total += HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);
        HAL_Delay(2);
    }

    return (total / samples);
}

/* Calculate Soil Moisture percentage (0% = completely dry, 100% = fully saturated) */
uint8_t Sensors_GetSoilMoisturePercent(void)
{
    uint32_t raw = Sensors_ReadADC(ADC_CHANNEL_0);

    if (raw >= SOIL_DRY_RAW)
    {
        return 0;
    }
    if (raw <= SOIL_WET_RAW)
    {
        return 100;
    }

    /* Linear mapping between dry and wet raw levels */
    uint32_t percent = ((SOIL_DRY_RAW - raw) * 100) / (SOIL_DRY_RAW - SOIL_WET_RAW);
    
    return (uint8_t)percent;
}

/* Check if rain is detected on the MH-RD sensor pad */
bool Sensors_IsRaining(void)
{
    uint32_t raw = Sensors_ReadADC(ADC_CHANNEL_1);
    return (raw < RAIN_THRESHOLD);
}

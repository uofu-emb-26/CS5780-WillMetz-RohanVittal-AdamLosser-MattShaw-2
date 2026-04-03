#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>
#include "stm32f0xx.h"

/*
 * Microphone input configuration.
 */
#define MIC_ADC_GPIO                  GPIOA
#define MIC_ADC_GPIO_CLOCK_ENABLE()   do { RCC->AHBENR |= RCC_AHBENR_GPIOAEN; } while (0)
#define MIC_ADC_PIN_INDEX             4u
#define MIC_ADC_PIN_MASK              (1u << MIC_ADC_PIN_INDEX)
#define MIC_ADC_CHANNEL_SELECT        ADC_CHSELR_CHSEL4

/*
 * Peak-to-peak measurement settings.
 */
#define MIC_SAMPLE_WINDOW             64u
#define MIC_NOISE_FLOOR_COUNTS        120u
#define MIC_LED1_THRESHOLD            200u
#define MIC_LED2_THRESHOLD            500u
#define MIC_LED3_THRESHOLD            1000u
#define MIC_LED4_THRESHOLD            1800u

/*
 *   MIC_NOISE_FLOOR_COUNTS = 220u
 *   MIC_LED1_THRESHOLD     = 400u
 *   MIC_LED2_THRESHOLD     = 850u
 *   MIC_LED3_THRESHOLD     = 1500u
 *   MIC_LED4_THRESHOLD     = 2600u
 */

/* UI update rate and smoothing. */
#define APP_UPDATE_PERIOD_MS          25u
#define APP_SMOOTHING_NUMERATOR       7u
#define APP_SMOOTHING_DENOMINATOR     8u

#endif /* APP_CONFIG_H */

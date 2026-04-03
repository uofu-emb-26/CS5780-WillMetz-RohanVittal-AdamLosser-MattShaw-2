#include "adc_mic.h"
#include "app_config.h"
#include "stm32f0xx.h"

static uint16_t adc_read_blocking(void)
{
    ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOSMP | ADC_ISR_EOSEQ | ADC_ISR_OVR;

    ADC1->CR |= ADC_CR_ADSTART;
    while ((ADC1->ISR & ADC_ISR_EOC) == 0u)
    {
    }

    return (uint16_t)(ADC1->DR & 0x0FFFu);
}

void mic_init(void)
{
    MIC_ADC_GPIO_CLOCK_ENABLE();
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    /* Configure selected pin as analog input. */
    MIC_ADC_GPIO->MODER &= ~(3u << (MIC_ADC_PIN_INDEX * 2u));
    MIC_ADC_GPIO->MODER |=  (3u << (MIC_ADC_PIN_INDEX * 2u));
    MIC_ADC_GPIO->PUPDR &= ~(3u << (MIC_ADC_PIN_INDEX * 2u));

    /* ADC is disabled before calibration. */
    if ((ADC1->CR & ADC_CR_ADEN) != 0u)
    {
        ADC1->CR |= ADC_CR_ADDIS;
        while ((ADC1->CR & ADC_CR_ADEN) != 0u)
        {
        }
    }

    /* Single-channel, right-aligned, software-triggered, 12-bit conversions. */
    ADC1->CFGR1 = 0u;
    ADC1->CHSELR = MIC_ADC_CHANNEL_SELECT;

    /* longest sampling time */
    ADC1->SMPR = ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;

    ADC1->CR |= ADC_CR_ADCAL;
    while ((ADC1->CR & ADC_CR_ADCAL) != 0u)
    {
    }

    ADC1->ISR = ADC_ISR_ADRDY;
    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0u)
    {
    }

    (void)adc_read_blocking();
}

uint16_t mic_measure_peak_to_peak(uint32_t sample_count)
{
    uint16_t min_value = 0x0FFFu;
    uint16_t max_value = 0u;

    for (uint32_t i = 0; i < sample_count; ++i)
    {
        uint16_t sample = adc_read_blocking();

        if (sample < min_value)
        {
            min_value = sample;
        }

        if (sample > max_value)
        {
            max_value = sample;
        }
    }

    return (uint16_t)(max_value - min_value);
}

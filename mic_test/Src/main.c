#include <stdint.h>
#include "adc_mic.h"
#include "app_config.h"
#include "board.h"
#include "stm32f0xx.h"

static uint16_t apply_noise_floor(uint16_t level)
{
    if (level <= MIC_NOISE_FLOOR_COUNTS)
    {
        return 0u;
    }

    return (uint16_t)(level - MIC_NOISE_FLOOR_COUNTS);
}

static uint8_t level_to_led_count(uint16_t level)
{
    if (level >= MIC_LED4_THRESHOLD)
    {
        return 4u;
    }
    if (level >= MIC_LED3_THRESHOLD)
    {
        return 3u;
    }
    if (level >= MIC_LED2_THRESHOLD)
    {
        return 2u;
    }
    if (level >= MIC_LED1_THRESHOLD)
    {
        return 1u;
    }
    return 0u;
}

volatile uint16_t g_mic_peak_to_peak_raw = 0u;
volatile uint16_t g_mic_peak_to_peak_filtered = 0u;
volatile uint16_t g_mic_level_smoothed = 0u;
volatile uint8_t g_led_count = 0u;

int main(void)
{
    uint16_t smoothed_level = 0u;

    board_init();
    mic_init();

    while (1)
    {
        uint16_t raw_level = mic_measure_peak_to_peak(MIC_SAMPLE_WINDOW);
        uint16_t filtered_level = apply_noise_floor(raw_level);

        smoothed_level = (uint16_t)(((uint32_t)smoothed_level * APP_SMOOTHING_NUMERATOR +
                                     (uint32_t)filtered_level) /
                                    APP_SMOOTHING_DENOMINATOR);

        g_mic_peak_to_peak_raw = raw_level;
        g_mic_peak_to_peak_filtered = filtered_level;
        if (filtered_level > g_mic_level_smoothed) {
            g_mic_level_smoothed = filtered_level;
        } else {
            g_mic_level_smoothed = (uint16_t)(((3u * g_mic_level_smoothed) + filtered_level) / 4u);
        }
        g_led_count = level_to_led_count(smoothed_level);

        board_led_bar_set(g_led_count);
        board_delay_ms(APP_UPDATE_PERIOD_MS);
    }
}

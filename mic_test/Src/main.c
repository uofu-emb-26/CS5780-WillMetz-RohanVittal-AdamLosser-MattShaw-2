#include <stdint.h>
#include "adc_mic.h"
#include "app_config.h"
#include "board.h"
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "uart.h"
#include "nixie_drivers.h"

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

    HAL_Init();
    uart_init();
    init_tube_gpios();

    uint8_t previous_percentage = 0; // hold the previously transmitted value


    

    while (1)
    {

        uint8_t percentage_buffer[BUFFER_SIZE] = {0};

        for (uint32_t buffer_count = 0; buffer_count < BUFFER_SIZE; buffer_count++) // loop to fill buffer with 20 values before transmitting
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

        //board_led_bar_set(g_led_count);

        // convert filtered level into the range 0 to 100 (a loudness percentage)
            uint32_t interpolation_input = (uint32_t)filtered_level; // convert to a 32 bit integer to prevent overflow when doing operations

            // cap our inputs so they always result in an output in the range 0 to 100
            if (interpolation_input > 1800)
            {
                interpolation_input = 1800;
            }

            if (interpolation_input < 64)
            {
                interpolation_input = 64;
            }

            // calculate the percentage
            uint8_t percentage = (uint8_t)(((interpolation_input - 64)*100)/1736);
            percentage_buffer[buffer_count] = percentage;

        }

        // get the average of the percentage buffer
        uint8_t average_percentage = get_array_average(percentage_buffer,BUFFER_SIZE);

        // check if the difference between the current percentage and the previous percentage is large enough to warrant transmitting
        int32_t difference = (int32_t)previous_percentage - (int32_t)average_percentage;

        // get the absolute value of the difference
        if (difference < 0)
        {
            difference = -1 * difference;
        }

        if (difference > UPDATE_THRESHOLD)
        {

            // transmit the loudness percentage via UART
            transmit_byte(average_percentage);
            write_int_to_tubes((uint16_t) average_percentage);

            // store the current percentage as the last transmitted percentage
            previous_percentage = average_percentage;

        }

        board_delay_ms(APP_UPDATE_PERIOD_MS);

    }

}



uint8_t get_array_average(uint8_t array[],uint32_t array_size)
{
    uint32_t sum = 0;
    uint32_t avg = 0;
    for (uint32_t i = 0; i < array_size; i++)
    {
        sum += array[i];
    }

    avg = sum/array_size;

    return (uint8_t)avg;
}



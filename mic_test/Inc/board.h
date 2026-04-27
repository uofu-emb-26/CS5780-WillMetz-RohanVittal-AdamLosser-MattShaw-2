#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void board_init(void);
void board_delay_ms(uint32_t ms);
void board_led_bar_set(uint8_t led_count);
void board_systick_handler(void);
uint8_t get_array_average(uint8_t array[],uint32_t array_size);

#endif /* BOARD_H */

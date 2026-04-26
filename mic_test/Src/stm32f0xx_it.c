#include "board.h"
#include "stm32f0xx_it.h"

void SysTick_Handler(void)
{
    board_systick_handler();
}

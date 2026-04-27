#include "stm32f0xx_hal.h"
#include "uart.h"

UART_HandleTypeDef huart3;

void uart_init(void)
{
    // initialize port PC10 and configure it to alternate function mode
  __HAL_RCC_GPIOC_CLK_ENABLE(); // enable the GPIOC clock

    GPIO_InitTypeDef initPins = {GPIO_PIN_10,
                                  GPIO_MODE_AF_PP,
                                  GPIO_NOPULL,GPIO_SPEED_FREQ_LOW};

    HAL_GPIO_Init(GPIOC, &initPins);


    // congigure PC10's alternate function to USART3TX (alt funtion 1)
    GPIOC->AFR[1] |= (1 << 8);


    // enable the clock for USART3
    RCC->APB1ENR |= (1 << 18);


    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX;

    HAL_UART_Init(&huart3);
}

void transmit_byte(uint8_t byte)
{
  HAL_UART_Transmit(&huart3, &byte, 1, HAL_MAX_DELAY);
}
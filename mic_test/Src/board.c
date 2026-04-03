#include "board.h"
#include "stm32f0xx.h"

#define LED_RED_PIN       (1u << 6)
#define LED_BLUE_PIN      (1u << 7)
#define LED_ORANGE_PIN    (1u << 8)
#define LED_GREEN_PIN     (1u << 9)
#define ALL_LED_PINS      (LED_RED_PIN | LED_BLUE_PIN | LED_ORANGE_PIN | LED_GREEN_PIN)

static volatile uint32_t g_ms_ticks = 0u;

static void leds_init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~((3u << (6u * 2u)) |
                      (3u << (7u * 2u)) |
                      (3u << (8u * 2u)) |
                      (3u << (9u * 2u)));
    GPIOC->MODER |=  ((1u << (6u * 2u)) |
                      (1u << (7u * 2u)) |
                      (1u << (8u * 2u)) |
                      (1u << (9u * 2u)));

    GPIOC->OTYPER &= ~(ALL_LED_PINS);
    GPIOC->OSPEEDR &= ~((3u << (6u * 2u)) |
                        (3u << (7u * 2u)) |
                        (3u << (8u * 2u)) |
                        (3u << (9u * 2u)));
    GPIOC->PUPDR &= ~((3u << (6u * 2u)) |
                      (3u << (7u * 2u)) |
                      (3u << (8u * 2u)) |
                      (3u << (9u * 2u)));

    GPIOC->BRR = ALL_LED_PINS;
}

void board_init(void)
{
    SystemCoreClockUpdate();
    (void)SysTick_Config(SystemCoreClock / 1000u);
    leds_init();
    board_led_bar_set(0u);
}

void board_delay_ms(uint32_t ms)
{
    uint32_t start = g_ms_ticks;
    while ((g_ms_ticks - start) < ms)
    {
        __WFI();
    }
}

void board_led_bar_set(uint8_t led_count)
{
    uint32_t set_mask = 0u;

    if (led_count > 0u) { set_mask |= LED_RED_PIN; }
    if (led_count > 1u) { set_mask |= LED_ORANGE_PIN; }
    if (led_count > 2u) { set_mask |= LED_GREEN_PIN; }
    if (led_count > 3u) { set_mask |= LED_BLUE_PIN; }

    GPIOC->BSRR = ((ALL_LED_PINS & ~set_mask) << 16u) | set_mask;
}

void board_systick_handler(void)
{
    g_ms_ticks++;
}

#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_rcc.h"
#include "nixie_drivers.h"

// function to write integer to nixie tubes
void write_int_to_tubes(uint16_t integer_to_write)
{

    // the mapping of digits is messed up, this is a lookup table
  static const uint8_t digit_map[10] = {
      0b1101, // 0
      0b0001, // 1
      0b1001, // 2
      0b0000, // 3
      0b1110, // 4
      0b0110, // 5
      0b1010, // 6
      0b0010, // 7
      0b1000, // 8
      0b0100  // 9
  };

  // initialize an array to hold the value of each digit we want to display (one
  // per nixie tube)
  uint16_t digits[4] = {0};

  // initialize loop varaibles
  uint16_t quotient = integer_to_write;
  uint32_t loop_count = 0;

  /* While the quotient is greater than 10, we modulus divide by 10 to get the
   next digit. As soon as the quotient is less than 10, we have reached the last
   digit, so we store the last digit and break out of the loop. */
  while (1) {

    if (quotient < 10)
    {
        digits[loop_count] = quotient;
        break;
    }

    uint16_t remainder = quotient % 10;
    quotient = quotient / 10;

    digits[loop_count] = remainder;

    loop_count++;

   }

   // NOTE: tube 4 is the "most significant tube" and tube 1 is the "least significant tube". Thus, if we display the number 56, 
   // tubes 4 & 3 will display "0", tube 2 will display "5", and tube 1 will display "6".
   uint16_t tube_four_val = digits[3];
   uint16_t tube_three_val = digits[2];
   uint16_t tube_two_val = digits[1];
   uint16_t tube_one_val = digits[0];

   uint16_t tube_four_mapped = digit_map[tube_four_val];
   uint16_t tube_three_mapped = digit_map[tube_three_val];
   uint16_t tube_two_mapped = digit_map[tube_two_val];
   uint16_t tube_one_mapped = digit_map[tube_one_val];

   /* The tubes are controlled with 4 bits each, which represent the binary digit (from 0 to 9) that the tube displays.
      These bits are written to sets of 4 GPIO pins, which act as 4-bit "control registers" for the tubes. The pin maps are as follows:
      
      TUBE 4: PC9 (MSB), PC8, PC7, PC6 (LSB)
      TUBE 3: PA11 (MSB), PA10, PA9, PA8 (LSB)
      TUBE 2: PB11 (MSB), PB10, PB9, PB8 (LSB)
      TUBE 1: PB7 (MSB), PB6, PB5, PB4 (LSB)

   */

   // We can write directly to the GPIO pins using the GPIOx_BSRR register

   // set the bits for tube 4
   GPIOC->BSRR = (0xF << 22);
   GPIOC->BSRR = (tube_four_mapped << 6);

   // set the bits for tube 3
   GPIOA->BSRR = (0xF << 24) | (0x1 << 16);
   GPIOA->BSRR = ((tube_three_mapped << 8) | (tube_three_val >> 3));

   // set the bits for tube 2
   GPIOB->BSRR = (0xF << 24);
   GPIOB->BSRR = (tube_two_mapped << 8);

   // set the bits for tube 1
   GPIOB->BSRR = (0xF << 20);
   GPIOB->BSRR = (tube_one_mapped << 4);
    
}

// function to initialize all the GPIO pins that interface with the tubes
void init_tube_gpios(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable the GPIOB clock
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable the GPIOC clock



   // initialize Tube 4 GPIOs
    GPIO_InitTypeDef initTubeFourGPIOs = {GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_7 | GPIO_PIN_6,
                                  GPIO_MODE_OUTPUT_PP,
                                  GPIO_NOPULL,GPIO_SPEED_FREQ_LOW};

    HAL_GPIO_Init(GPIOC, &initTubeFourGPIOs);

    // initialize Tube 3 GPIOs
    GPIO_InitTypeDef initTubeThreeGPIOs = {GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_0,
                                  GPIO_MODE_OUTPUT_PP,
                                  GPIO_NOPULL,GPIO_SPEED_FREQ_LOW};

    HAL_GPIO_Init(GPIOA, &initTubeThreeGPIOs);

    // initialize Tube 2 GPIOs
    GPIO_InitTypeDef initTubeTwoGPIOs = {GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8,
                                  GPIO_MODE_OUTPUT_PP,
                                  GPIO_NOPULL,GPIO_SPEED_FREQ_LOW};

    HAL_GPIO_Init(GPIOB, &initTubeTwoGPIOs);

    // initialize Tube 1 GPIOs
    GPIO_InitTypeDef initTubeOneGPIOs = {GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4,
                                  GPIO_MODE_OUTPUT_PP,
                                  GPIO_NOPULL,GPIO_SPEED_FREQ_LOW};

    HAL_GPIO_Init(GPIOB, &initTubeOneGPIOs);

}

// function that cycles all tubes through all values (0-9 for each tube)
void test_tubes(void)
{
    uint32_t val = 9999;
        while (1)
        {
            if (val == 0)
            {
                write_int_to_tubes(val);
                for (volatile uint32_t i = 0; i < 2000000; i++);

                break;
            }
            write_int_to_tubes(val);
            val -= 1111;
            for (volatile uint32_t i = 0; i < 2000000; i++);


        }
}






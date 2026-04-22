cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME mic_led_meter)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(gcc-arm-none-eabi)

project(${CMAKE_PROJECT_NAME})
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

enable_language(C ASM)
include(flash_stm32)

add_executable(${CMAKE_PROJECT_NAME}
    Src/main.c
    Src/board.c
    Src/adc_mic.c
    Src/stm32f0xx_it.c
    Src/runtime_stubs.c
    system_stm32f0xx.c
    startup_stm32f072xb.s
    Src/uart.c
    Src/nixie_drivers.c
    STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
    STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c
    STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
    STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
    STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_PROJECT_NAME}.map")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Inc
    CMSIS/Core/Include
    CMSIS/Device/ST/STM32F0xx/Include
    STM32F0xx_HAL_Driver/Inc
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    STM32F072xB
    USE_HAL_DRIVER
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wundef
    -Wdouble-promotion
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    m
)

flash_target(${CMAKE_PROJECT_NAME})

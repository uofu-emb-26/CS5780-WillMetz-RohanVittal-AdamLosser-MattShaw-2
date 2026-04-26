function(flash_target target)
    find_program(OPENOCD openocd)
    if(OPENOCD)
        add_custom_target(flash_${target}
            ${OPENOCD}
                -f interface/stlink.cfg
                -f target/stm32f0x.cfg
                -c 'program "$<TARGET_FILE:${target}>" verify reset exit'
            DEPENDS ${target}
        )
    else()
        message(WARNING "Could not find openocd; flash target will not be available")
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target}>
        VERBATIM
    )
endfunction()

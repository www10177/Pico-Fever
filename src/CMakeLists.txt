

add_executable(Pico_Game_Controller
        pico_game_controller.cpp
        # hid/hid.c
        )

pico_generate_pio_header(Pico_Game_Controller ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio)

include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR})
include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR}/hid)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/config)


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/hid)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gamepad)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/pico-ssd1306)

target_sources(Pico_Game_Controller PRIVATE pico_game_controller.cpp)


target_link_libraries(Pico_Game_Controller PRIVATE
        pico_multicore
        pico_stdlib
        tinyusb_device
        tinyusb_board
        hardware_pio
        hardware_dma
        hardware_irq
        hardware_i2c
        hid
        config
        gamepad
        pico_ssd1306
        )


pico_add_extra_outputs(Pico_Game_Controller)

add_custom_command(TARGET Pico_Game_Controller
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 ${PROJECT_SOURCE_DIR}/build_uf2/Pico_Game_Controller.uf2
)

set(PICO_DEST "H:/")  
# Configure this to your pico drive, comment out to disable auto upload

if (DEFINED PICO_DEST)
        message("Auto Upload Firmware to ${PICO_DEST}")
    add_custom_command(TARGET Pico_Game_Controller
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 ${PICO_DEST}
        )
endif (DEFINED PICO_DEST)


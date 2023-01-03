

add_executable(Pico_Game_Controller
        pico_game_controller.cpp
        # hid/hid.c
        )

pico_generate_pio_header(Pico_Game_Controller ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio)

include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR})
include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR}/hid)
include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR}/config)


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
        hid
        gamepad
        pico_ssd1306
        hardware_i2c
        )


pico_add_extra_outputs(Pico_Game_Controller)

add_custom_command(TARGET Pico_Game_Controller
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 ${PROJECT_SOURCE_DIR}/build_uf2/Pico_Game_Controller.uf2
)

add_custom_command(TARGET Pico_Game_Controller
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 E:/Pico_Game_Controller.uf2
    )

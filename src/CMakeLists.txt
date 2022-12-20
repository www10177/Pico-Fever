# add_library(usb_descriptors STATIC ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors/descriptorHelper.cpp)
# add_library(tusb_config STATIC ${CMAKE_CURRENT_LIST_DIR}/tusb_config.h)

# add_library(descriptorHelper STATIC usb_descriptors/descriptorHelper.cpp)
# target_link_libraries(descriptorHelper tinyusb_device )#tusb_config)
# target_link_libraries(Pico_Game_Controller descriptorHelper)#tusb_config)

# include_directories(descriptorHelper)
# link_directories(descriptorHelper)


add_executable(Pico_Game_Controller
        pico_game_controller.cpp
        usb_driver/descriptorHelper.c
        )
pico_generate_pio_header(Pico_Game_Controller ${CMAKE_CURRENT_LIST_DIR}/rotary-encoder/encoders.pio)
pico_generate_pio_header(Pico_Game_Controller ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio)

include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_sources(Pico_Game_Controller PRIVATE pico_game_controller.cpp)
# target_link_libraries(usb_descriptors tinyusb_device )#tusb_config)

target_link_libraries(Pico_Game_Controller PRIVATE
        pico_multicore
        pico_stdlib
        tinyusb_device
        tinyusb_board
        hardware_pio
        hardware_dma
        hardware_irq
        )


pico_add_extra_outputs(Pico_Game_Controller)

add_custom_command(TARGET Pico_Game_Controller
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 ${PROJECT_SOURCE_DIR}/build_uf2/Pico_Game_Controller.uf2
)

add_custom_command(TARGET Pico_Game_Controller
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/Pico_Game_Controller.uf2 E:/Pico_Game_Controller.uf2
          COMMENT "Upload to Pico @ E drive")

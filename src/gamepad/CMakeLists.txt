#Buttons
add_library(gamepad STATIC gamepad.cpp rotaryencoder.cpp)
pico_generate_pio_header(gamepad ${CMAKE_CURRENT_LIST_DIR}/encoders.pio)
target_include_directories(gamepad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../hid/)
target_include_directories(gamepad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../config)
target_link_libraries(gamepad 
        hardware_pio
        hardware_dma
        tinyusb_board
        tinyusb_device
        pico_stdlib
        hid
)
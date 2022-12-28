#Buttons
add_library(gamepad STATIC gamepad.cpp)
target_link_libraries(gamepad PRIVATE 
        # hardware_gpio
        pico_stdlib
        # tinyusb_device
        # tinyusb_board
)
# include_directories(gamepad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/gamepad)
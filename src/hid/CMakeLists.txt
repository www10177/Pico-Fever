# USB protocol Helper 
add_library(hid STATIC hid.c HIDDescriptors.cpp KeyboardDescriptors.cpp SwitchDescriptors.cpp XInputDescriptors.cpp)
# include_directories(${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(hid PRIVATE 
        tinyusb_device
        tinyusb_board
        pico_stdlib
)
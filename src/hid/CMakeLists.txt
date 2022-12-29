# USB protocol Helper 
add_library(hid STATIC ${CMAKE_CURRENT_LIST_DIR}/hid.c)
include_directories(${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(hid PRIVATE 
        tinyusb_device
        tinyusb_board
        pico_stdlib
)
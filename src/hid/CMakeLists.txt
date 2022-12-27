# USB protocol Helper 
add_library(hid STATIC ${CMAKE_CURRENT_LIST_DIR}/hid.c)
target_link_libraries(hid PRIVATE 
        tinyusb_device
        tinyusb_board
        pico_stdlib
)
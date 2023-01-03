#Buttons
add_library(config 
general_config.h
Profiles.h)
target_include_directories(config PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(config PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../hid)
target_link_libraries(config
        pico_stdlib
        tinyusb_device
)
# add_library(rotary-encoder STATIC rotaryencoder.cpp)
# pico_generate_pio_header(rotary-encoder encoders.pio)
# include_directories(Pico_Game_Controller PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../)

# target_link_libraries(rotary-encoder PRIVATE
#         pico_stdlib
#         hardware_pio
#         hardware_dma
#         hardware_irq
#         )


#Rotary Encoders 
add_library(rotary STATIC rotaryencoder.cpp )
pico_generate_pio_header(rotary ${CMAKE_CURRENT_LIST_DIR}/encoders.pio)
include_directories(PICO_GAME_CONTROLLER PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(rotary PRIVATE 
        hardware_pio
        hardware_dma
        pico_stdlib
        tinyusb_device
        tinyusb_board
)
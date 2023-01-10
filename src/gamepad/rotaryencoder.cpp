#include "rotaryencoder.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include <functional>
#include <iterator>
#include "encoders.pio.h"
#include "pico/time.h"
#include "general_config.h"

RotaryInput::RotaryInput()//uint8_t enc_gpio_size, const uint8_t * enc_gpio, bool enc_rev, int debounce_count)
{
    // Copy Configurations  
    this->enc_gpio_size = ENC_GPIO_SIZE;
    this->enc_gpio = new uint8_t[enc_gpio_size];
    std::copy(ENC_GPIO, ENC_GPIO+ enc_gpio_size, this->enc_gpio);
    this->enc_rev = ENC_REV;
    this->debounce_count = ENC_DEBOUNCE_COUNT;


    // Initialize Variables
    enc_val = new uint32_t[enc_gpio_size]();
    delta= new uint32_t[enc_gpio_size]();
    prev_enc_val = new uint32_t[enc_gpio_size]();
    dir_debounced = new uint8_t [enc_gpio_size]();
    dir_debouncing_counter = new uint32_t[enc_gpio_size]();
    last_dir_changed_time= new uint32_t [enc_gpio_size]();
    last_direction = new uint8_t[enc_gpio_size]();
    for (int i = 0; i < enc_gpio_size; i++)
    {
        enc_val[i] = 0, delta[i]=0;
        enc_val[i]=0, prev_enc_val[i] = 0;
        dir_debounced[i]=1, dir_debouncing_counter[i] = 1;
        last_dir_changed_time[i] = 0, last_direction[i] = 1;
    }

    // Setup Encoders
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &encoders_program);
    for (int i = 0; i < enc_gpio_size; i++)
    {
        encoders_program_init(pio, i, offset, enc_gpio[i], debounce_count> 0);

        dma_channel_config c = dma_channel_get_default_config(i);
        channel_config_set_read_increment(&c, false);
        channel_config_set_write_increment(&c, false);
        channel_config_set_dreq(&c, pio_get_dreq(pio, i, false));

        dma_channel_configure(i, &c,
                              &enc_val[i],  // Destination pointer
                              &pio->rxf[i], // Source pointer
                              0x10,         // Number of transfers
                              true          // Start immediately
        );
        irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
        irq_set_enabled(DMA_IRQ_0, true);
        dma_channel_set_irq0_enabled(i, true);
    }
}
RotaryInput::~RotaryInput()
{
    delete enc_val,delta, prev_enc_val,last_dir_changed_time,dir_debounced,dir_debouncing_counter;
}

void RotaryInput::update()
{
    for (int i = 0; i < enc_gpio_size; i++)
    {
        uint8_t direction = 1;
        delta[i] = (enc_val[i] - prev_enc_val[i]) * (enc_rev? 1 : -1);
        if (enc_val[i] > prev_enc_val[i])
            direction = enc_rev? 2 : 0;
        else if (enc_val[i] < prev_enc_val[i])
            direction = enc_rev? 0 : 2;

        // Ugly if-else statement to eliminate bouncing(?) of encoder
        // Bouncing only happens when the encoder is turning
        if (direction == 1)
        {
            if (last_direction[i] == 1 ){
                dir_debounced[i] = 1;
            }
            else {
                if (time_us_32() - last_dir_changed_time[i] > ENC_DEBOUNCE_TIME_US) {
                    dir_debounced[i] = 1;
                    last_direction[i] = 1;
                }
                else {
                    dir_debounced[i] = last_direction[i];
                }
            }
        }
        else {
            last_dir_changed_time[i] = time_us_32();
            last_direction[i] = direction;
            dir_debounced[i] = direction;
        }

        prev_enc_val[i] = enc_val[i];
    }
    // gamepad->state.lx = (uint16_t)(dir_debounced[0] * 65535 / 2);
    // if (ENC_GPIO_SIZE > 1)
    // {
    //     gamepad->state.rx = (uint16_t)(dir_debounced[1] * 65535 / 2);
    // }
}
void RotaryInput::dma_handler()
{
    // helper function
    PIO pio = pio0; //  Force using pio1
    uint i = 1;
    int interrupt_channel = 0;
    while ((i & dma_hw->ints0) == 0)
    {
        i = i << 1;
        ++interrupt_channel;
    }
    dma_hw->ints0 = 1u << interrupt_channel;
    if (interrupt_channel < 4)
    {
        dma_channel_set_read_addr(interrupt_channel, &pio->rxf[interrupt_channel], true);
    }
}

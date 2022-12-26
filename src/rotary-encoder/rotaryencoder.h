#ifndef RotaryEncoder_H_
#define RotaryEncoder_H_

// #include "gpaddon.h"
// #include "BoardConfig.h"
#include "hardware/pio.h"
#include "general_config.h"

// #include "GamepadEnums.h"

class RotaryInput  {
public:
    RotaryInput();
    ~RotaryInput();

	bool available();   // GPAddon available
	void setup();       // Analog Setup
	void process();     // Analog Process
    uint8_t *dir_debounced ;
    uint32_t *delta;
private:
    uint8_t enc_gpio_size ;
    uint32_t *enc_val;
    uint32_t *prev_enc_val;
    uint8_t *direction ;
    uint32_t *dir_debouncing_counter ;
    static void dma_handler() ;
    // uint32_t getMillis() ;
    
};

#endif   // _RotaryEncoder_H_
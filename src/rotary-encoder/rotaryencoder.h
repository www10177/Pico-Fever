#ifndef RotaryEncoder_H_
#define RotaryEncoder_H_


#include "hardware/pio.h"

class RotaryInput  {
public:
    RotaryInput(uint8_t enc_size, const uint8_t *enc_gpio, bool enc_rev, int debounce_count);
    ~RotaryInput();

	void update();     // Analog Process
    uint8_t *dir_debounced ; // 1 for non-spin; 0 for ccw; 2 for cw ; used for digital output
    uint32_t *delta; // delta compared to previous time; used for analog 
private:
    //Config 
    uint8_t enc_gpio_size ;
    uint8_t *enc_gpio;
    bool enc_rev;
    int debounce_count;
    
    //Encoder values 
    uint32_t *enc_val;
    uint32_t *prev_enc_val;
    uint8_t *direction ;
    uint32_t *dir_debouncing_counter ;
    static void dma_handler() ;
    // uint32_t getMillis() ;
    
};

#endif   // _RotaryEncoder_H_
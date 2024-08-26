#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>

class Joypad
{
    public:
        void set_selection(uint8_t value); // write to the top two selection bits
        void set_dpad(uint8_t value);
        void set_buttons(uint8_t value);

        uint8_t get_dpad(); // get the lower read only 4 bits
        uint8_t get_buttons(); // get the lower read only 4 bits
        uint8_t get_selection(); // get the upper 2 selection bits
        
    private:
        uint8_t selection_ = 0x30; // start joypad with all bits set to 1 (all deselected)
        uint8_t dpad_ = 0xf; // the lower 4 bits if reading directional keys
        uint8_t buttons_ = 0xf; // the lower 4 bits if reading SsBA buttons 
};


#endif
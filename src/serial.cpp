#include "serial.h"
#include <cstdint>
#include <iostream>

void Serial::write_sb(uint8_t value)
{
    sb_ = value;
    return;
}

void Serial::write_sc(uint8_t value)
{
    sc_ = value;
    
    if (value == 0x81) {
        char letter = read_sb();    
        std::cout << letter << '\n';
    }
    return;
}

uint8_t Serial::read_sb()
{
    return sb_;
}
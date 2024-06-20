#include "bus.h"
#include <cstdint>

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
    }
}

void Bus::write(uint16_t address) 
{
    if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
    }
}
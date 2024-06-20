#include "ram.h"

uint8_t RAM::read(uint16_t address)
{
    return ram_[address];
}

void RAM::write(uint16_t address, uint8_t value)
{
    ram_[address] = value;
}
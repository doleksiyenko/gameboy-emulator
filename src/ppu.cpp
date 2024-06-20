#include "ppu.h"

PPU::PPU() 
{
    // initialize renderer
}

uint8_t PPU::read(uint16_t address)
{
    return vram_[address - 0x8000];
}

void PPU::write(uint16_t address, uint8_t value)
{
    vram_[address - 0x8000] = value;
}
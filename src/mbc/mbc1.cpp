#include "./mbc/mbc1.h"
#include <cstdint>

uint8_t MBC1::read(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3fff) {
        // this is the fixed bank area (ROM bank X0)
    }
    else if (address >= 0x4000 && address <= 0x7fff) {
        // this is the switchable bank area (ROM bank 01 - 7f)
    }
    else if (address >= 0xa000 && address <= 0xbfff) {
        // RAM bank 00-03
    }

    return cartridge_data_[address]; 
}

void MBC1::write(uint16_t address, uint8_t value)
{
    // TODO: change the MBC registers
}
#ifndef MBC1_H
#define MBC1_H

#include "mbc.h"
#include <vector>
#include <array>

class MBC1 : public MBC 
{
    public:
        MBC1(std::vector<uint8_t> cartridge) : MBC(cartridge) {}; // MBC type 0x1
        uint8_t read(uint16_t address) override; // Read the byte at the address in the cartridge based on the behaviour of MBC1
        void write(uint16_t address, uint8_t value) override; // write to the MBC registers

};

#endif
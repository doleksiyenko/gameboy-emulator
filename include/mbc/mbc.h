#ifndef MBC_H
#define MBC_H

#include <cstdint>
#include <unistd.h>
#include <vector>
#include <iostream>

class MBC
{
    public:
        // constructor for all MBCs saves cartridge data for MBC reference
        MBC(std::vector<uint8_t> cartridge) : cartridge_data_(cartridge) {};

        // given the address, read from different banks / external RAM depending on the MBC. Base class returns invalid read
        virtual uint8_t read(uint16_t address) { return 0xff; }; 
    protected:
        std::vector<uint8_t> cartridge_data_;
};

#endif
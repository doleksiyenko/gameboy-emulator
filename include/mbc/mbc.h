#ifndef MBC_H
#define MBC_H

#include <cstdint>
#include <unistd.h>
#include <vector>

class MBC
{
    public:
        MBC(std::vector<uint8_t> cartridge);
        uint8_t read(uint16_t address); // given the address, read from different banks / external RAM depending on the MBC
    protected:
        std::vector<uint8_t> cartridge_data_;
};

#endif
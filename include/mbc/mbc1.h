#ifndef MBC1_H
#define MBC1_H

#include "mbc.h"

class MBC1 : public MBC 
{
    public:
        uint8_t read(uint16_t address); // overriden function. Read the byte at the address in the cartridge based on the behaviour of MBC1
    private:
};

#endif
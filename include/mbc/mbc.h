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
        virtual void write(uint16_t address, uint8_t value) {};  // write to different MBC registers

    protected:
        std::vector<uint8_t> cartridge_data_;

        // possible registers required by MBCs. All registers default to 0x00 on power up
        bool ram_enable_ = false; // RAM enable needs to be activated first by writing $A to memory
        uint8_t rom_bank_number_ = 0x0; 
        uint8_t ram_bank_number_ = 0x0; 
        uint8_t banking_mode_ = 0x0;
};

#endif
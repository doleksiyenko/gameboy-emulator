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
        MBC(std::vector<uint8_t> cartridge) :
         cartridge_data_(cartridge), rom_size_code(cartridge[0x0148]), external_ram_size_code(cartridge[0x0149]) {};

        // given the address, read from different banks / external RAM depending on the MBC. Base class returns invalid read
        virtual uint8_t read(uint16_t address) { return 0xff; }; 
        virtual void write(uint16_t address, uint8_t value) {};  // write to different MBC registers

        uint8_t get_external_ram_size_code() { return external_ram_size_code; };
        uint8_t get_rom_size_code() { return rom_size_code; };

    protected:
        // cartridge metadata
        uint8_t external_ram_size_code;
        uint8_t rom_size_code;
        std::vector<uint8_t> cartridge_data_;

        // possible registers required by MBCs. All registers default to 0x00 on power up
        bool ram_enable_ = false; // RAM enable needs to be activated first by writing $A to memory
        uint8_t rom_bank_number_ = 0x0; 
        uint8_t ram_bank_reg_ = 0x0;
        uint8_t banking_mode_ = 0x0;


};

#endif
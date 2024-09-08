#ifndef MBC3_H
#define MBC3_H

#include "mbc.h"
#include <_types/_uint8_t.h>
#include <vector>
#include <array>

class MBC3 : public MBC 
{
    public:
        MBC3(std::vector<uint8_t> cartridge) : MBC(cartridge) { external_ram = std::vector<uint8_t>(ram_code_to_size_.at(external_ram_size_code), 0); }; // MBC type 0x1
        uint8_t read(uint16_t address) override; // Read the byte at the address in the cartridge based on the behaviour of MBC1
        void write(uint16_t address, uint8_t value) override; // write to the MBC registers
    private:
        void latch_clock();
        std::vector<uint8_t> external_ram;
    private:
        bool rtc_rw_enable_ = false;
        uint8_t latch_clock_data_ = 0;
        uint8_t rtc_s = 0;
        uint8_t rtc_m = 0;
        uint8_t rtc_h = 0;
        uint8_t rtc_dl = 0; 
        uint8_t rtc_dh = 0;
};

#endif